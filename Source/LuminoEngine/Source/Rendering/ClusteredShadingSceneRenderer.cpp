
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
	m_cameraPos = cameraPos;
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
	// TODO: とりあえず球と同じ方法でクラスタ化。最悪パターンの想定なので不足は無いが、かなり無駄が多い。
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

static float b3(float v1, float v2, float v3, float t)
{
	return (1.0f - t)*(1.0f - t)*v1 + 2.0f * (1.0f - t)*t*v2 + t*t*v3;
}

static float b3_2(float v1, float v2, float v3, float t)
{
	return t*t*v3;
}
static float b3_r(float v1, float v2, float v3, float t)
{
	//return t*t*v3;
	//return v3 / ( t * t);
	return sqrt(t);
}

static float bias(float b, float x) { return pow(x, log(b) / log(0.5)); }
//static float bias(float b, float x) { return x; }

static float bias_x1(float x) { return b3(0.0, 0.0, 1.0, x); }
static float bias_x1_r(float x) { return b3_r(0.0, 0.0, 1.0, x); }
static float bias_x2(float x) { return b3(0.0, 1.0, 1.0, x); }


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

	// Viewport 空間での、AABB の前面と後面 (0.0..1.0)
	float vpZn = (zn - m_nearClip) / flipRange;
	float vpZf = (zf - m_nearClip) / flipRange;

	// 効率悪そうだけど、クラスタ情報の書き込むみ先を探すには全クラスタ数のインデックスベースで for しなければならない。
	// bios で指数関数的なクラスタ分割を行うので、座標からクラスタの範囲を求めてループするのはちょっと複雑。

	// index -> viewport
	float xs = 2.0f / ClusterWidth;
	float ys = 2.0f / ClusterHeight;
	float zs = 1.0f / ClusterDepth;

	float biasBase = 0.1;

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

						float czn = bias_x1(cn);
						float czf = bias_x1(cf);
						//printf("%f\n", czn);
						if (vpZf > czn && vpZn < czf)
						{
							addClusterData(x, y, z, lightId);
						}
					}
				}
			}
		}
	}

	//float aa1 = bias_x1(0.8);
	//float aa2 = bias_x1(0.2);
	//float aa3 = bias_x2(0.8);
	//float aa4 = bias_x2(0.2);
	//float bb1 = aa2 + aa3;
	//float bb2 = aa1 + aa4;
	//float cc1 = bias_x1_r(aa1);
	////float cc2 = b3_2(0,0,1, aa1);

	//for (float z = 0; z < 1.0f; z += 0.01)
	//{
	//	float cz = bias_x1_r(z); //z;// bias(biasBase, z);//1.0 - bias(1.0 - biasBase, 1.0 - z);
	//	int i_cz = /*ClusterDepth - */(int)/*trunc*/(cz * ClusterDepth);
	//	printf("%f (%f): %d\n", z, cz, i_cz);
	//}

	//for (int z = 0; z < ClusterDepth; z++)
	//{
	//	// index -> viewport.z
	//	float cn = zs * z;
	//	float cf = zs * (z + 1);

	//	// viewport.z > cluster range
	//	float czn = bias_x1(cn);
	//	float czf = bias_x1(cf);
	//	//printf("%d: %f:%f r:%f\n", z, cn, czn, bias_x2(czn));
	//	printf("%d: %f .. %f\n", z, czn, czf);
	//	//printf("%d: %f:%f .. %f:%f\n", z, cn, czn, cf, czf);
	//}
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
	//else
	//{
	//	printf("%d\n", clustersAddCount);
	//}
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


	m_lightClusters.addPointLight(Vector3(0, 0, 0), 2, Color::White);
	m_lightClusters.addPointLight(Vector3(5, 0, 5), 2, Color::Red);
	m_lightClusters.addPointLight(Vector3(-5, 0, 5), 3, Color::Blue);
	m_lightClusters.addPointLight(Vector3(5, 0, -5), 1, Color::Green);
	m_lightClusters.addPointLight(Vector3(-5, 0, -5), 1, Color::Yellow);

	m_lightClusters.addPointLight(Vector3(7, 0, 0), 5, Color::Magenta);
	m_lightClusters.addPointLight(Vector3(-7, 0, 0), 1, Color::Cyan);
	m_lightClusters.addPointLight(Vector3(0, 0, 7), 1, Color::AliceBlue);
	m_lightClusters.addPointLight(Vector3(0, 0, -7), 2, Color::BlueViolet);


	m_lightClusters.endMakeClusters();
}

void ClusteredShadingSceneRenderer::onCollectLight(DynamicLightInfo* light)
{
	SceneRenderer::onCollectLight(light);
}

void ClusteredShadingSceneRenderer::onShaderPassChainging(ShaderPass* pass)
{
	SceneRenderer::onShaderPassChainging(pass);

	Shader* shader = pass->getOwnerShader();

	ShaderVariable* v;

	v = shader->findVariable(_T("ln_pointLightInfoTexture"));
	if (v) v->setTexture(m_lightClusters.getLightInfoTexture());

	v = shader->findVariable(_T("ln_clustersTexture"));
	if (v) v->setTexture(m_lightClusters.getClustersVolumeTexture());

	v = shader->findVariable(_T("ln_nearClip"));
	if (v) v->setFloat(m_lightClusters.m_nearClip);

	v = shader->findVariable(_T("ln_farClip"));
	if (v) v->setFloat(m_lightClusters.m_farClip);

	v = shader->findVariable(_T("ln_cameraPos"));
	if (v) v->setVector(Vector4(m_lightClusters.m_cameraPos, 0));
}

} // namespace detail
LN_NAMESPACE_END

