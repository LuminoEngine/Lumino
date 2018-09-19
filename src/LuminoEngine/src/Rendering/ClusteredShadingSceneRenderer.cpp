
#include "Internal.hpp"
#include "ClusteredShadingSceneRenderer.hpp"

namespace ln {
namespace detail {

#if 0
	
//==============================================================================
// ClusteredShadingGeometryRenderingPass

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
	SceneRendererPass::initialize();

	{
		static const byte_t data[] =
		{
#include "Resource/ClusteredShadingDefault.fx.h"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		m_defaultShader = Shader::create((const char*)data, size, nullptr, ShaderCodeType::RawHLSL);
	}
	
	// TODO: getPass 引数型
	m_defaultShaderTechnique = m_defaultShader->findTechnique(ClusteredShadingGeometryRenderingPass_TechniqueName);

	{
		static const byte_t data[] =
		{
#include "Resource/UnLighting.fx.h"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		m_unLightingShader = Shader::create((const char*)data, size, nullptr, ShaderCodeType::RawHLSL);
	}
	m_unLightingShaderTechnique = m_unLightingShader->getTechniques()[0];
}

void ClusteredShadingGeometryRenderingPass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
{
	ShaderTechniqueClassSet classSet;
	classSet.ligiting = ShaderTechniqueClass_Ligiting::Forward;
	classSet.phase = ShaderTechniqueClass_Phase::Geometry;
	classSet.meshProcess = element->vertexProcessing;

	outPolicy->shader = (stageData.shader) ? stageData.shader : m_defaultShader;

	classSet.shadingModel = stageData.shadingModel;
	outPolicy->shaderTechnique = outPolicy->shader->findTechniqueByClass(classSet);
	if (!outPolicy->shaderTechnique)
	{
		outPolicy->shaderTechnique = m_defaultShaderTechnique;
	}

	//if (stageData.shadingModel == ShadingModel::UnLighting)
	//{
	//	classSet.shadingModel = ShaderTechniqueClass_ShadingModel::UnLighting;
	//	outPolicy->shaderTechnique = stageData.shader->findTechniqueByClass(classSet);
	//	if (!outPolicy->shaderTechnique)
	//	{
	//		outPolicy->shaderTechnique = m_unLightingShaderTechnique;
	//	}
	//}
	//else
	//{
	//	classSet.shadingModel = ShaderTechniqueClass_ShadingModel::Default;
	//	outPolicy->shaderTechnique = stageData.shader->findTechniqueByClass(classSet);
	//	if (!outPolicy->shaderTechnique)
	//	{
	//		outPolicy->shaderTechnique = m_defaultShaderTechnique;
	//	}
	//}

	outPolicy->shader = outPolicy->shaderTechnique->getOwnerShader();
	outPolicy->visible = true;

}

//RenderTargetTexture* g_m_normalRenderTarget = nullptr;
//void ClusteredShadingGeometryRenderingPass::onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView)
//{
//	//g_m_normalRenderTarget = m_normalRenderTarget;
//	//defaultStatus->defaultRenderTarget[1] = m_normalRenderTarget;
//}

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
		m_defaultShader = Shader::create((const char*)data, size, nullptr, ShaderCodeType::RawIR);
	}
}

void DepthPrepass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
{
	// TODO: とりあえずデフォルト強制
	outPolicy->shader = m_defaultShader;
	outPolicy->shaderTechnique = m_defaultShader->getTechniques()[0];

	// とありあえず全部可
	outPolicy->visible = true;
}

void DepthPrepass::onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView)
{
	auto viewSize = SizeI::fromFloatSize(renderView->getViewSize());
	if (!m_depthMap || m_depthMap->getSize() != viewSize)
	{
		m_depthMap = Ref<RenderTargetTexture>::makeRef();
		m_depthMap->createImpl(GraphicsManager::getInstance(), viewSize, 1, TextureFormat::R32G32B32A32_Float);
	}

	defaultStatus->defaultRenderTarget[0] = m_depthMap;
}

#if 0
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
		m_defaultShader = Shader::create((const char*)data, size, nullptr, ShaderCodeType::RawIR);
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

void ShadowCasterPass::onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView)
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
#endif

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
	addPass(m_depthPrepass);

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
#endif

} // namespace detail
} // namespace ln

