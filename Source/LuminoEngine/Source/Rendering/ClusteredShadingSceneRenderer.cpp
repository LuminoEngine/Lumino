
#include "../Internal.h"
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Rendering/RenderView.h>
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
	m_lightInofs.reserve(MaxLights);
	m_lightInfoTexture = Texture2D::create(sizeof(LightInfo) / sizeof(Vector4), MaxLights, TextureFormat::R32G32B32A32_Float, false);
	m_globalLightInofs.reserve(MaxLights);
	m_globalLightInfoTexture = Texture2D::create(sizeof(GlobalLightInfo) / sizeof(Vector4), MaxLights, TextureFormat::R32G32B32A32_Float, false);
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

	m_globalLightInofs.clear();
}

void LightClusters::addPointLight(const Vector3& pos, float range, float attenuation, const Color& color)
{
	addClusterSpherical(pos, range);

	LightInfo info;
	//info.posAndRange = Vector4(pos, range);
	info.posAndRange = Vector4(Vector3::transform(pos, m_view).GetXYZ(), range);
	info.directionAndAtt = Vector4(0, 0, 0, attenuation);
	info.spotAngle = Vector4::Zero;
	info.color = color;
	m_lightInofs.add(info);
}

// TODO: Vector3 クラスへ
static Vector3 transformDirection(const Vector3& vec, const Matrix& mat)
{
	return Vector3(
		(((vec.x * mat.m11) + (vec.y * mat.m21)) + (vec.z * mat.m31)),
		(((vec.x * mat.m12) + (vec.y * mat.m22)) + (vec.z * mat.m32)),
		(((vec.x * mat.m13) + (vec.y * mat.m23)) + (vec.z * mat.m33)));

}

void LightClusters::addSpotLight(const Vector3& pos, float range, float attenuation, const Color& color, const Vector3& direction, float cone, float penumbra)
{
	// TODO: とりあえず球と同じ方法でクラスタ化。最悪パターンの想定なので不足は無いが、かなり無駄が多い。
	addClusterSpherical(pos, range);

	LightInfo info;
	info.posAndRange = Vector4(Vector3::transform(pos, m_view).GetXYZ(), range);
	info.directionAndAtt = Vector4(transformDirection(-direction, m_view), attenuation);
	info.spotAngle = Vector4(cos(cone), cos(cone * (1.0 - penumbra)), 0, 0);
	info.color = color;
	m_lightInofs.add(info);
}

void LightClusters::addDirectionalLight(const Vector3& dir, const Color& color)
{
	GlobalLightInfo info;
	info.color = color;
	info.directionAndType = Vector4(dir, 1.0f + FLT_EPSILON);
	m_globalLightInofs.add(info);
}

void LightClusters::addAmbientLight(const Color& color)
{
	GlobalLightInfo info;
	info.color = color;
	info.directionAndType = Vector4(0.0f, 0.0f, 0.0f, 2.0f + FLT_EPSILON);
	m_globalLightInofs.add(info);
}
void LightClusters::addHemisphereLight(const Color& skyColor, const Color& groundColor)
{
	GlobalLightInfo info;
	info.color = skyColor;
	info.groundColor = groundColor;
	info.directionAndType = Vector4(0.0f, 0.0f, 0.0f, 3.0f + FLT_EPSILON);
	m_globalLightInofs.add(info);
}

void LightClusters::endMakeClusters()
{
	// Global lights
	{
		// 最大未満であれば番兵を入れることで末尾を検出できるようにする
		if (m_globalLightInofs.getCount() < MaxLights)
		{
			m_globalLightInofs.add(GlobalLightInfo{});
		}

		//m_globalLightInfoTexture->setMappedData(&m_globalLightInofs[0]);
		//m_globalLightInfoTexture->resolveDeviceObject();
	}

	// Local lights
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

	// ビュー空間での、AABB の前面と後面 (clip 内に clamp しないと、特に near より手前の時に次の viewPoints を求めるときの符号が反転してしまう)
	float zn = Math::clamp(cp.z - lightRadius, m_nearClip, m_farClip);
	float zf = Math::clamp(cp.z + lightRadius, m_nearClip, m_farClip);

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
				float cr = (xs * x) - 1.0f;
				float cl = (xs * (x + 1)) - 1.0f;
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

static const String ClusteredShadingGeometryRenderingPass_TechniqueName = _T("Forward_Geometry");
//static const String ClusteredShadingGeometryRenderingPass_PassName = _T("Geometry");

ClusteredShadingGeometryRenderingPass::ClusteredShadingGeometryRenderingPass()
{
}

ClusteredShadingGeometryRenderingPass::~ClusteredShadingGeometryRenderingPass()
{
}

void ClusteredShadingGeometryRenderingPass::initialize()
{
	RenderingPass2::initialize();

	{
		static const byte_t data[] =
		{
#include "Resource/ClusteredShadingDefault.fx.h"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		m_defaultShader = Shader::create((const char*)data, size, ShaderCodeType::RawHLSL);
	}
	
	// TODO: getPass 引数型
	m_defaultShaderTechnique = m_defaultShader->findTechnique(ClusteredShadingGeometryRenderingPass_TechniqueName);




	m_unLightingShader = Shader::create(_T("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Rendering/Resource/UnLighting.fx"), ShaderCodeType::RawHLSL);
	m_unLightingShaderTechnique = m_unLightingShader->getTechniques()[0];
}

void ClusteredShadingGeometryRenderingPass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
{
	// TODO: ユーザーシェーダから UnLit 取れればそれを使う
	if (stageData.shadingModel == ShadingModel::UnLighting)
	{
		outPolicy->shaderTechnique = m_unLightingShaderTechnique;
	}
	else
	{
		outPolicy->shaderTechnique = selectShaderTechniqueHelper(
			stageData.shader,
			ClusteredShadingGeometryRenderingPass_TechniqueName,
			//ClusteredShadingGeometryRenderingPass_PassName,
			m_defaultShaderTechnique);
	}

	outPolicy->shader = outPolicy->shaderTechnique->getOwnerShader();
	outPolicy->visible = true;

}

RenderTargetTexture* g_m_normalRenderTarget = nullptr;
void ClusteredShadingGeometryRenderingPass::onBeginPass(DefaultStatus* defaultStatus)
{
	//g_m_normalRenderTarget = m_normalRenderTarget;
	//defaultStatus->defaultRenderTarget[1] = m_normalRenderTarget;
}

//ShaderPass* ClusteredShadingGeometryRenderingPass::selectShaderPass(Shader* shader)
//{
//	//ShaderPass* pass = nullptr;
//	//ShaderTechnique* tech = shader->findTechnique(_T("ClusteredForward"));
//	//if (tech)
//	//{
//	//	pass = tech->getPass(_T("Geometry"));
//	//	if (pass)
//	//	{
//	//		return pass;
//	//	}
//	//}
//
//	return RenderingPass2::selectShaderPass(shader);
//}




//==============================================================================
// DepthPrepass
//==============================================================================
DepthPrepass::DepthPrepass()
{
}

DepthPrepass::~DepthPrepass()
{
}

void DepthPrepass::initialize()
{
	{
		static const byte_t data[] =
		{
#include "Resource/DepthPrepass.fx.h"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		m_defaultShader = Shader::create((const char*)data, size, ShaderCodeType::RawIR);
	}

	m_depthMap = Ref<RenderTargetTexture>::makeRef();
	m_depthMap->createImpl(GraphicsManager::getInstance(), SizeI(640, 480), 1, TextureFormat::R32G32B32A32_Float);
}

void DepthPrepass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
{
	// TODO: とりあえずデフォルト強制
	outPolicy->shader = m_defaultShader;
	outPolicy->shaderTechnique = m_defaultShader->getTechniques()[0];

	// とありあえず全部可
	outPolicy->visible = true;
}

void DepthPrepass::onBeginPass(DefaultStatus* defaultStatus)
{
	defaultStatus->defaultRenderTarget[0] = m_depthMap;
}

//==============================================================================
// ShadowCasterPass
//==============================================================================
ShadowCasterPass::ShadowCasterPass() 
{
}

ShadowCasterPass::~ShadowCasterPass()
{
}

RenderTargetTexture* g_m_shadowMap  = nullptr;
void ShadowCasterPass::initialize()
{
	RenderingPass2::initialize();

	{
		static const byte_t data[] =
		{
#include "Resource/ShadowCaster.fx.h"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		m_defaultShader = Shader::create((const char*)data, size, ShaderCodeType::RawIR);
	}

	m_shadowMap = Ref<RenderTargetTexture>::makeRef();
	m_shadowMap->createImpl(GraphicsManager::getInstance(), SizeI(1024, 1024), 1, TextureFormat::R32G32B32A32_Float);

	g_m_shadowMap = m_shadowMap;
}

//Shader* ShadowCasterPass::getDefaultShader() const
//{
//	return m_defaultShader;
//}

void ShadowCasterPass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
{
	// TODO: とりあえずデフォルト強制
	outPolicy->shader = m_defaultShader;
	outPolicy->shaderTechnique = m_defaultShader->getTechniques()[0];

	// とありあえず全部可
	outPolicy->visible = true;
}

void ShadowCasterPass::onBeginPass(DefaultStatus* defaultStatus)
{
	defaultStatus->defaultRenderTarget[0] = m_shadowMap;
}

void ShadowCasterPass::overrideCameraInfo(detail::CameraInfo* cameraInfo)
{
	*cameraInfo = view;
}

//ShaderPass* ShadowCasterPass::selectShaderPass(Shader* shader)
//{
//	return RenderingPass2::selectShaderPass(shader);
//}

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

	m_depthPrepass = newObject<DepthPrepass>();
	//addPass(m_depthPrepass);

	// pass "Geometry"
	addPass(newObject<ClusteredShadingGeometryRenderingPass>());

	m_lightClusters.init();


	// TODO: Test
	//m_renderSettings.ambientColor = Color(1, 0, 0, 1);

}

void ClusteredShadingSceneRenderer::prepare()
{
	SceneRenderer::prepare();
}

void ClusteredShadingSceneRenderer::collect(RenderingPass2* pass, const detail::CameraInfo& cameraInfo)
{
	m_lightClusters.beginMakeClusters(cameraInfo.viewMatrix, cameraInfo.projMatrix, cameraInfo.viewPosition, cameraInfo.nearClip, cameraInfo.farClip);

	SceneRenderer::collect(pass, cameraInfo);

	m_lightClusters.endMakeClusters();
}

void ClusteredShadingSceneRenderer::onCollectLight(DynamicLightInfo* light)
{
	if (LN_REQUIRE(light)) return;

	const CameraInfo& view = getRenderView()->m_cameraInfo;

	Color color = light->m_diffuse;
	color *= light->m_intensity;

	switch (light->m_type)
	{
	case LightType::Ambient:
		m_lightClusters.addAmbientLight(color);
		break;
	case LightType::Hemisphere:
		m_lightClusters.addHemisphereLight(color, light->m_groundColor * light->m_intensity);
		break;
	case LightType::Directional:
		m_lightClusters.addDirectionalLight(transformDirection(-light->m_direction, view.viewMatrix), color);
		break;
	case LightType::Point:
		m_lightClusters.addPointLight(light->m_position, light->m_range, light->m_attenuation, color);
		break;
	case LightType::Spot:
		//m_lightClusters.addSpotLight(Vector3::transform(light->m_position, view.viewMatrix).GetXYZ(), light->m_range, light->m_attenuation, transformDirection(-light->m_direction, view.viewMatrix), light->m_spotAngle, Math::lerp(light->m_spotAngle, 0, light->m_spotPenumbra), light->m_diffuse);
		m_lightClusters.addSpotLight(light->m_position, light->m_range, light->m_attenuation, color, light->m_direction, light->m_spotAngle, light->m_spotPenumbra);
		break;
	default:
		LN_UNREACHABLE();
		break;
	}

	SceneRenderer::onCollectLight(light);
}

void ClusteredShadingSceneRenderer::onShaderPassChainging(ShaderPass* pass)
{
	SceneRenderer::onShaderPassChainging(pass);

	Shader* shader = pass->getOwnerShader();

	ShaderVariable* v;
	
	v = shader->findVariable(_T("ln_GlobalLightInfoTexture"));
	printf("lt:");
	if (v) v->setTexture(m_lightClusters.getGlobalLightInfoTexture());

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


	//v = shader->findVariable(_T("ln_AmbientColor"));
	//if (v) v->setVector(Vector4(m_renderSettings.ambientColor));	// TODO: Color 直接渡しできるようにしてもいいと思う

	//v = shader->findVariable(_T("ln_AmbientSkyColor"));
	//if (v) v->setVector(Vector4(m_renderSettings.ambientSkyColor));

	//v = shader->findVariable(_T("ln_AmbientGroundColor"));
	//if (v) v->setVector(Vector4(m_renderSettings.ambientGroundColor));

	v = shader->findVariable(_T("ln_FogParams"));
	if (v) v->setVector(Vector4(m_fogParams.color.rgb() * m_fogParams.color.a, m_fogParams.density));
}

} // namespace detail
LN_NAMESPACE_END

