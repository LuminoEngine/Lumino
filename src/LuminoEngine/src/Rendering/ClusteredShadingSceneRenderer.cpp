
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "RenderingManager.hpp"
#include "ClusteredShadingSceneRenderer.hpp"
#include "RenderTargetTextureCache.hpp"
#include "RenderingPipeline.hpp"

namespace ln {
namespace detail {

#if 1

//==============================================================================
// DepthPrepass

DepthPrepass::DepthPrepass()
{
}

DepthPrepass::~DepthPrepass()
{
}

void DepthPrepass::initialize()
{
	SceneRendererPass::initialize();
	m_defaultShader = manager()->builtinShader(BuiltinShader::DepthPrepass);
}

void DepthPrepass::onBeginRender(SceneRenderer* sceneRenderer)
{
	m_depthMap = manager()->frameBufferCache()->requestRenderTargetTexture(sceneRenderer->renderingPipeline()->renderingFrameBufferSize(), TextureFormat::RGBA32, false);
	m_depthBuffer = manager()->frameBufferCache()->requestDepthBuffer(sceneRenderer->renderingPipeline()->renderingFrameBufferSize());
}

void DepthPrepass::onEndRender(SceneRenderer* sceneRenderer)
{
    manager()->frameBufferCache()->release(m_depthMap);
    manager()->frameBufferCache()->release(m_depthBuffer);
	m_depthMap = nullptr;
	m_depthBuffer = nullptr;
}

void DepthPrepass::onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer)
{
	frameBuffer->renderTarget[0] = m_depthMap;
	frameBuffer->depthBuffer = m_depthBuffer;
	context->setColorBuffer(0, m_depthMap);
	context->setDepthBuffer(m_depthBuffer);
	context->clear(ClearFlags::All, Color::Transparency, 1.0f, 0);
}

ShaderTechnique* DepthPrepass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	// force default
	return m_defaultShader->techniques().front();
}

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
		m_defaultShader = manager()->builtinShader(BuiltinShader::ClusteredShadingDefault);
	}
	
	// TODO: getPass 引数型
	m_defaultShaderTechnique = m_defaultShader->findTechnique(ClusteredShadingGeometryRenderingPass_TechniqueName);

//	{
//		static const byte_t data[] =
//		{
//#include "Resource/UnLighting.fx.h"
//		};
//		static const size_t size = LN_ARRAY_SIZE_OF(data);
//		m_unLightingShader = Shader::create((const char*)data, size, nullptr, ShaderCodeType::RawHLSL);
//	}
//	m_unLightingShaderTechnique = m_unLightingShader->getTechniques()[0];
}

ShaderTechnique* ClusteredShadingGeometryRenderingPass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	Shader* shader = requestedShader;
	if (!shader) shader = m_defaultShader;

	ShaderTechniqueClass classSet;
	classSet.ligiting = ShaderTechniqueClass_Ligiting::Forward;
	classSet.phase = ShaderTechniqueClass_Phase::Geometry;
	classSet.meshProcess = requestedMeshProcess;
	classSet.shadingModel = tlanslateShadingModel(requestedShadingModel);
	ShaderTechnique* technique = shader->findTechniqueByClass(classSet);
	if (technique)
		return technique;
	else
		return m_defaultShaderTechnique;
}

//void ClusteredShadingGeometryRenderingPass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
//{
//
//	//if (stageData.shadingModel == ShadingModel::UnLighting)
//	//{
//	//	classSet.shadingModel = ShaderTechniqueClass_ShadingModel::UnLighting;
//	//	outPolicy->shaderTechnique = stageData.shader->findTechniqueByClass(classSet);
//	//	if (!outPolicy->shaderTechnique)
//	//	{
//	//		outPolicy->shaderTechnique = m_unLightingShaderTechnique;
//	//	}
//	//}
//	//else
//	//{
//	//	classSet.shadingModel = ShaderTechniqueClass_ShadingModel::Default;
//	//	outPolicy->shaderTechnique = stageData.shader->findTechniqueByClass(classSet);
//	//	if (!outPolicy->shaderTechnique)
//	//	{
//	//		outPolicy->shaderTechnique = m_defaultShaderTechnique;
//	//	}
//	//}
//
//	outPolicy->shader = outPolicy->shaderTechnique->getOwnerShader();
//	outPolicy->visible = true;
//
//}

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
// ShadowCasterPass
//==============================================================================
ShadowCasterPass::ShadowCasterPass() 
{
}

ShadowCasterPass::~ShadowCasterPass()
{
}

//RenderTargetTexture* g_m_shadowMap  = nullptr;
void ShadowCasterPass::initialize()
{
	SceneRendererPass::initialize();

	m_defaultShader = manager()->builtinShader(BuiltinShader::ShadowCaster);

	m_shadowMap = newObject<RenderTargetTexture>(1024, 1024, TextureFormat::R32G32B32A32Float, false);
	m_depthBuffer = newObject<DepthBuffer>(1024, 1024);

	//g_m_shadowMap = m_shadowMap;
}

//Shader* ShadowCasterPass::getDefaultShader() const
//{
//	return m_defaultShader;
//}


void ShadowCasterPass::onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer)
{
	frameBuffer->renderTarget[0] = m_shadowMap;
	frameBuffer->depthBuffer = m_depthBuffer;
	context->setColorBuffer(0, m_shadowMap);
	context->setDepthBuffer(m_depthBuffer);
	context->clear(ClearFlags::All, Color::Transparency, 1.0f, 0);
}

ShaderTechnique* ShadowCasterPass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	// force default
	return m_defaultShader->techniques().front();
}

//void ShadowCasterPass::overrideCameraInfo(detail::CameraInfo* cameraInfo)
//{
//	*cameraInfo = view;
//}

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

void ClusteredShadingSceneRenderer::initialize(RenderingManager* manager)
{
	SceneRenderer::initialize();

	m_depthPrepass = newObject<DepthPrepass>();
	//addPass(m_depthPrepass);

	// pass "Geometry"
	addPass(newObject<ClusteredShadingGeometryRenderingPass>());

	m_lightClusters.init();


	// TODO: Test
	//m_renderSettings.ambientColor = Color(1, 0, 0, 1);

}

//void ClusteredShadingSceneRenderer::onBeginRender()
//{
//
//}
//
//void ClusteredShadingSceneRenderer::onEndRender()
//{
//
//}


void ClusteredShadingSceneRenderer::collect(const detail::CameraInfo& cameraInfo)
{
	m_lightClusters.beginMakeClusters(cameraInfo.viewMatrix, cameraInfo.projMatrix, cameraInfo.viewPosition, cameraInfo.nearClip, cameraInfo.farClip);

	SceneRenderer::collect(cameraInfo);

	m_lightClusters.endMakeClusters();
}

// TODO: Vector3 クラスへ
static Vector3 transformDirection(const Vector3& vec, const Matrix& mat)
{
	return Vector3(
		(((vec.x * mat.m11) + (vec.y * mat.m21)) + (vec.z * mat.m31)),
		(((vec.x * mat.m12) + (vec.y * mat.m22)) + (vec.z * mat.m32)),
		(((vec.x * mat.m13) + (vec.y * mat.m23)) + (vec.z * mat.m33)));

}

void ClusteredShadingSceneRenderer::onCollectLight(const DynamicLightInfo& light)
{
	const CameraInfo& view = mainCameraInfo();

	Color color = light.m_color;
	color *= light.m_intensity;

	switch (light.m_type)
	{
	case LightType::Ambient:
		m_lightClusters.addAmbientLight(color);
		break;
	case LightType::Hemisphere:
		m_lightClusters.addHemisphereLight(color, light.m_color2 * light.m_intensity);
		break;
	case LightType::Directional:
		m_lightClusters.addDirectionalLight(transformDirection(-light.m_direction, view.viewMatrix), color);
		break;
	case LightType::Point:
		m_lightClusters.addPointLight(light.m_position, light.m_range, light.m_attenuation, color);
		break;
	case LightType::Spot:
		m_lightClusters.addSpotLight(light.m_position, light.m_range, light.m_attenuation, color, light.m_direction, light.m_spotAngle, light.m_spotPenumbra);
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

void ClusteredShadingSceneRenderer::onSetAdditionalShaderPassVariables(Shader* shader)
{
	ShaderParameter* v;
	
	v = shader->findParameter(_T("ln_GlobalLightInfoTexture"));
	if (v) v->setTexture(m_lightClusters.getGlobalLightInfoTexture());

	v = shader->findParameter(_T("ln_pointLightInfoTexture"));
	if (v) v->setTexture(m_lightClusters.getLightInfoTexture());

	v = shader->findParameter(_T("ln_clustersTexture"));
	if (v) v->setTexture(m_lightClusters.getClustersVolumeTexture());

	v = shader->findParameter(_T("ln_nearClip"));
	if (v) v->setFloat(m_lightClusters.m_nearClip);

	v = shader->findParameter(_T("ln_farClip"));
	if (v) v->setFloat(m_lightClusters.m_farClip);

	v = shader->findParameter(_T("ln_cameraPos"));
	if (v) v->setVector(Vector4(m_lightClusters.m_cameraPos, 0));


	//v = shader->findParameter(_T("ln_AmbientColor"));
	//if (v) v->setVector(Vector4(m_renderSettings.ambientColor));	// TODO: Color 直接渡しできるようにしてもいいと思う
	
	//v = shader->findParameter(_T("ln_AmbientSkyColor"));
	//if (v) v->setVector(Vector4(m_renderSettings.ambientSkyColor));

	//v = shader->findParameter(_T("ln_AmbientGroundColor"));
	//if (v) v->setVector(Vector4(m_renderSettings.ambientGroundColor));

	v = shader->findParameter(_T("ln_FogParams"));
	if (v) v->setVector(Vector4(m_fogParams.color.rgb() * m_fogParams.color.a, m_fogParams.density));
}

#endif

} // namespace detail
} // namespace ln

