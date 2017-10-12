
#include "../Internal.h"
#include <Lumino/Rendering/Rendering.h>
#include "../Graphics/GraphicsManager.h"
#include "ClusteredShadingSceneRenderer.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// LightClusters
//==============================================================================
LightClusters::LightClusters()
	: m_clustersData()
	, m_clustersAddCount()
	, m_clustersTexture(nullptr)
	, m_lightInofs()
	, m_lightInfoTexture(nullptr)
	, m_view()
	, m_proj()
	, m_nearClip(0)
	, m_farClip(0)
{
}

void LightClusters::init()
{
	m_clustersTexture = tr::Texture3D::create(ClusterWidth, ClusterHeight, ClusterDepth);
	m_lightInfoTexture = Texture2D::create(sizeof(LightInfo) / sizeof(Vector4), MaxLights, TextureFormat::R32G32B32A32_Float, false);
	m_lightInofs.reserve(MaxLights);
}

void LightClusters::beginMakeClusters(const Matrix& view, const Matrix& proj, const Vector3& cameraPos, float nearClip, float farClip)
{
	m_view = view;
	m_proj = proj;
	m_nearClip = nearClip;
	m_farClip = farClip;

	// TODO: 全部 0 クリアする Texture3D::clear() があっても良いかも
	//for (int y = 0; y < ClusterHeight; y++)
	//{
	//	for (int x = 0; x < ClusterWidth; x++)
	//	{
	//		for (int z = 0; z < ClusterDepth; z++)
	//		{
	//			m_clustersTexture->setPixel32(x, y, z, Color32(0, 0, 0, 0));
	//		}
	//	}
	//}

	m_clustersAddCount.clear();
	m_clustersData.clear();
	m_clustersAddCount.resize(ClusterWidth * ClusterHeight * ClusterDepth);
	m_clustersData.resize(ClusterWidth * ClusterHeight * ClusterDepth);

	m_lightInofs.clear();
	m_lightInofs.add(LightInfo{});	// dummy
}

void LightClusters::addPointLight(const Vector3& pos, float range, const Color& color)
{
	addClusterSpherical(pos, range);

	LightInfo info;
	info.posAndRange = Vector4(pos, range);
	info.spotDirection = Vector4::Zero;
	info.spotAngle = Vector4::Zero;
	info.color = color;
	m_lightInofs.add(info);
}

void LightClusters::addSpotLight(const Vector3& pos, float range, const Vector3& direction, float outerRadius, float innerRadius, const Color& color)
{
	// TODO: とりあえず球と同じ方法でクラスタ化。最悪パターンの想定なので、一般的にはかなり無駄が多い。
	addClusterSpherical(pos, range);

	LightInfo info;
	info.posAndRange = Vector4(pos, range);
	info.spotDirection = Vector4(direction, 0);
	info.spotAngle = Vector4(cos(outerRadius), 1.0f / cos(innerRadius), 0, 0);
	info.color = color;
	m_lightInofs.add(info);
}

void LightClusters::endMakeClusters()
{
	// m_clustersData -> m_clustersTexture
	for (int y = 0; y < ClusterHeight; y++)
	{
		for (int x = 0; x < ClusterWidth; x++)
		{
			for (int z = 0; z < ClusterDepth; z++)
			{
				m_clustersTexture->setPixel32(x, y, z, m_clustersData[((ClusterWidth * ClusterHeight * z) + (ClusterWidth * y) + x)]);
			}
		}
	}

	m_lightInfoTexture->setMappedData(&m_lightInofs[0]);
}

void LightClusters::addClusterSpherical(const Vector3& pos, float range)
{
	float lightRadius = range;
	float lightId = m_lightInofs.getCount() + 1;
	float flipRange = m_farClip - m_nearClip;

	// カメラから見た位置。奥が Z+。一番手前は 0.0
	Vector4 cp = Vector4::transform(Vector4(pos, 1.0f), m_view);

	// ビュー空間での、AABB の前面と後面
	float zn = cp.z - lightRadius;
	float zf = cp.z + lightRadius;

	Vector3 viewPoints[4] =
	{
		Vector3::transformCoord(Vector3(cp.x - lightRadius, cp.y - lightRadius, zf), m_proj),	// 左上 奥
		Vector3::transformCoord(Vector3(cp.x + lightRadius, cp.y + lightRadius, zf), m_proj),	// 右下 奥
		Vector3::transformCoord(Vector3(cp.x - lightRadius, cp.y - lightRadius, zn), m_proj),	// 左上 手前
		Vector3::transformCoord(Vector3(cp.x + lightRadius, cp.y + lightRadius, zn), m_proj),	// 右下 手前
	};

	Vector3 vpMin, vpMax;
	vpMin.x = std::min(viewPoints[0].x, viewPoints[2].x);
	vpMin.y = std::min(viewPoints[0].y, viewPoints[2].y);
	vpMax.x = std::max(viewPoints[1].x, viewPoints[3].x);
	vpMax.y = std::max(viewPoints[1].y, viewPoints[3].y);

	// Viewport 空間での、AABB の前面と後面
	float vpZn = (zn - m_nearClip) / flipRange;
	float vpZf = (zf - m_nearClip) / flipRange;

	// 効率悪そうだけど、クラスタ情報の書き込むみ先を探すには全クラスタ数のインデックスベースで for しなければならない。
	// bios で指数関数的なクラスタ分割を行うので、座標からクラスタの範囲を求めてループするのはちょっと複雑。

	// index -> viewport
	float xs = 2.0f / ClusterWidth;
	float ys = 2.0f / ClusterHeight;
	float zs = 1.0f / ClusterDepth;

	float biasBase = 0.9;

	for (int y = 0; y < ClusterHeight; y++)
	{
		float cb = (ys * y) - 1.0f;
		float ct = (ys * (y + 1)) - 1.0f;
		if ((vpMax.y > cb && vpMin.y < ct))
		{
			for (int x = 0; x < ClusterWidth; x++)
			{
				float cl = (xs * x) - 1.0f;
				float cr = (xs * (x + 1)) - 1.0f;
				if ((vpMax.x > cl && vpMin.x < cr))
				{
					for (int z = 0; z < ClusterDepth; z++)
					{
						// 0.0..1.0
						float cn = zs * z;
						float cf = zs * (z + 1);

						float czn = bias(biasBase, cn);
						float czf = bias(biasBase, cf);
						if (vpZf > czn && vpZn < czf)
						{
							addClusterData(x, y, z, lightId);
						}
					}
				}
			}
		}
	}
}

void LightClusters::addClusterData(int x, int y, int z, int lightId)
{
	int clustersAddCountIndex =	((ClusterWidth * ClusterHeight * z) + (ClusterWidth * y) + x);
	int clustersAddCount = m_clustersAddCount[clustersAddCountIndex];
	if (clustersAddCount < 4)
	{
		Color32& color = m_clustersData[clustersAddCountIndex];
		switch (clustersAddCount)
		{
		case 0:
			color.r = lightId;
			break;
		case 1:
			color.g = lightId;
			break;
		case 2:
			color.b = lightId;
			break;
		case 3:
			color.a = lightId;
			break;
		default:
			LN_UNREACHABLE();
			break;
		}

		m_clustersAddCount[clustersAddCountIndex]++;
	}
}

//==============================================================================
// ClusteredShadingGeometryRenderingPass
//==============================================================================
ClusteredShadingGeometryRenderingPass::ClusteredShadingGeometryRenderingPass()
{
}

ClusteredShadingGeometryRenderingPass::~ClusteredShadingGeometryRenderingPass()
{
}

void ClusteredShadingGeometryRenderingPass::initialize()
{
	RenderingPass2::initialize();

	//m_defaultShader = GraphicsManager::getInstance()->getBuiltinShader(BuiltinShader::LegacyDiffuse);

	m_defaultShader = Shader::create(_T("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Rendering/Resource/ClusteredShadingDefault.fx"), ShaderCodeType::RawIR);
}

Shader* ClusteredShadingGeometryRenderingPass::getDefaultShader() const
{
	return m_defaultShader;
}

ShaderPass* ClusteredShadingGeometryRenderingPass::selectShaderPass(Shader* shader)
{
	//ShaderPass* pass = nullptr;
	//ShaderTechnique* tech = shader->findTechnique(_T("ClusteredForward"));
	//if (tech)
	//{
	//	pass = tech->getPass(_T("Geometry"));
	//	if (pass)
	//	{
	//		return pass;
	//	}
	//}

	return RenderingPass2::selectShaderPass(shader);
}

//==============================================================================
// ClusteredShadingSceneRenderer
//==============================================================================

ClusteredShadingSceneRenderer::ClusteredShadingSceneRenderer()
{
}

ClusteredShadingSceneRenderer::~ClusteredShadingSceneRenderer()
{
}

void ClusteredShadingSceneRenderer::initialize(GraphicsManager* manager)
{
	SceneRenderer::initialize(manager);

	m_lightClusters.init();

	// pass "Geometry"
	addPass(newObject<ClusteredShadingGeometryRenderingPass>());
}

void ClusteredShadingSceneRenderer::prepare()
{
	SceneRenderer::prepare();
}

void ClusteredShadingSceneRenderer::collect()
{
	const CameraInfo& view = getRenderView()->m_cameraInfo;
	m_lightClusters.beginMakeClusters(view.viewMatrix, view.projMatrix, view.viewPosition, view.nearClip, view.farClip);
	SceneRenderer::collect();


	//lc.addPointLight(Vector3(0, 0, 0), 10, Color::White);
	m_lightClusters.addPointLight(Vector3(5, 0, 5), 2, Color::Red);
	m_lightClusters.addPointLight(Vector3(-5, 0, 5), 3, Color::Blue);
	m_lightClusters.addPointLight(Vector3(5, 0, -5), 4, Color::Green);
	m_lightClusters.addPointLight(Vector3(-5, 0, -5), 5, Color::Yellow);

	m_lightClusters.addPointLight(Vector3(7, 0, 0), 10, Color::Magenta);
	m_lightClusters.addPointLight(Vector3(-7, 0, 0), 4, Color::Cyan);
	m_lightClusters.addPointLight(Vector3(0, 0, 7), 10, Color::AliceBlue);
	m_lightClusters.addPointLight(Vector3(0, 0, -7), 2, Color::BlueViolet);


	m_lightClusters.endMakeClusters();
}

void ClusteredShadingSceneRenderer::onCollectLight(DynamicLightInfo* light)
{
	SceneRenderer::onCollectLight(light);
}

void ClusteredShadingSceneRenderer::onShaderPassChainging(ShaderPass* pass)
{
	return SceneRenderer::onShaderPassChainging(pass);
}

} // namespace detail
LN_NAMESPACE_END

