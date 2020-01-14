
#include "Internal.hpp"
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "../Graphics/RenderTargetTextureCache.hpp"
#include "RenderingManager.hpp"
#include "ClusteredShadingSceneRenderer.hpp"
#include "RenderingPipeline.hpp"
#include "RenderStage.hpp"

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

void DepthPrepass::init()
{
	SceneRendererPass::init();
	m_defaultShader = manager()->builtinShader(BuiltinShader::DepthPrepass);
}

void DepthPrepass::onBeginRender(SceneRenderer* sceneRenderer)
{
	auto size = sceneRenderer->renderingPipeline()->renderingFrameBufferSize();
	m_depthMap = RenderTargetTexture::getTemporary(size.width, size.height, TextureFormat::RGBA8, false);
	m_depthBuffer = DepthBuffer::getTemporary(size.width, size.height);
	m_renderPass = makeObject<RenderPass>();
}

void DepthPrepass::onEndRender(SceneRenderer* sceneRenderer)
{
	RenderTargetTexture::releaseTemporary(m_depthMap);
	DepthBuffer::releaseTemporary(m_depthBuffer);
	m_depthMap = nullptr;
	m_depthBuffer = nullptr;
}

void DepthPrepass::onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	m_renderPass->setRenderTarget(0, m_depthMap);
	m_renderPass->setDepthBuffer(m_depthBuffer);
	m_renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0);
}

//void DepthPrepass::onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer)
//{
//	frameBuffer->renderTarget[0] = m_depthMap;
//	frameBuffer->depthBuffer = m_depthBuffer;
//	m_renderPass->setRenderTarget(0, m_depthMap);
//	m_renderPass->setDepthBuffer(m_depthBuffer);
//	//context->setRenderTarget(0, m_depthMap);
//	//context->setDepthBuffer(m_depthBuffer);
//	//context->setRenderPass(m_renderPass);
//	//context->clear(ClearFlags::All, Color::Transparency, 1.0f, 0);	// TODO: renderPassに統合していいと思う
//	m_renderPass->setRenderTarget(0, m_depthMap);
//	m_renderPass->setDepthBuffer(m_depthBuffer);
//	m_renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0);
//}

RenderPass* DepthPrepass::renderPass() const
{
	return m_renderPass;
}

ShaderTechnique* DepthPrepass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	// force default
	return m_defaultShader->techniques()->front();
}

//==============================================================================
// LightOcclusionPass

LightOcclusionPass::LightOcclusionPass()
{
}

void LightOcclusionPass::init()
{
	SceneRendererPass::init();
	m_blackShader = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/BlackShader.fx");
	m_blackShaderTechnique = m_blackShader->findTechnique(u"Default");
	m_renderPass = makeObject<RenderPass>();
}

void LightOcclusionPass::onBeginRender(SceneRenderer* sceneRenderer)
{
	auto size = sceneRenderer->renderingPipeline()->renderingFrameBufferSize();
	m_lensflareOcclusionMap = RenderTargetTexture::getTemporary(size.width, size.height, TextureFormat::RGBA8, false);
	m_depthBuffer = DepthBuffer::getTemporary(size.width, size.height);
}

void LightOcclusionPass::onEndRender(SceneRenderer* sceneRenderer)
{
	RenderTargetTexture::releaseTemporary(m_lensflareOcclusionMap);
	DepthBuffer::releaseTemporary(m_depthBuffer);
	m_lensflareOcclusionMap = nullptr;
	m_depthBuffer = nullptr;
}

void LightOcclusionPass::onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	m_renderPass->setRenderTarget(0, m_lensflareOcclusionMap);
	m_renderPass->setDepthBuffer(m_depthBuffer);
	m_renderPass->setClearValues(ClearFlags::All, Color::Black, 1.0f, 0);
	//m_renderPass->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
	printf("  LightOcclusionPass::m_renderPass(%p)\n", m_renderPass.get());
}

RenderPass* LightOcclusionPass::renderPass() const
{
	return m_renderPass;
}

bool LightOcclusionPass::filterElement(RenderDrawElement* element) const
{
	return element->elementType == RenderDrawElementType::Geometry || element->elementType == RenderDrawElementType::LightDisc;
}

ShaderTechnique* LightOcclusionPass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	if (!requestedShader)
		return m_blackShaderTechnique;

	ShaderTechniqueClass classSet;
	classSet.defaultTechnique = false;
	classSet.ligiting = ShaderTechniqueClass_Ligiting::LightDisc;
	classSet.phase = ShaderTechniqueClass_Phase::Geometry;
	classSet.meshProcess = requestedMeshProcess;
	classSet.shadingModel = tlanslateShadingModel(requestedShadingModel);
	ShaderTechnique* technique = ShaderHelper::findTechniqueByClass(requestedShader, classSet);
	if (technique)
		return technique;
	else
		return m_blackShaderTechnique;
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

void ClusteredShadingGeometryRenderingPass::init(ClusteredShadingSceneRenderer* ownerRenderer)
{
	SceneRendererPass::init();
	m_ownerRenderer = ownerRenderer;

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

	m_renderPass = makeObject<RenderPass>();
}

void ClusteredShadingGeometryRenderingPass::onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	m_renderPass->setRenderTarget(0, renderTarget);
	m_renderPass->setDepthBuffer(depthBuffer);
	m_renderPass->setClearValues(ClearFlags::None, Color::Transparency, 1.0f, 0);
}

//void ClusteredShadingGeometryRenderingPass::onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer)
//{
//	frameBuffer->renderTarget[0] = m_depthMap;
//	frameBuffer->depthBuffer = m_depthBuffer;
//	m_renderPass->setRenderTarget(0, m_depthMap);
//	m_renderPass->setDepthBuffer(m_depthBuffer);
//	//context->setRenderTarget(0, m_depthMap);
//	//context->setDepthBuffer(m_depthBuffer);
//	//context->setRenderPass(m_renderPass);
//	//context->clear(ClearFlags::All, Color::Transparency, 1.0f, 0);	// TODO: renderPassに統合していいと思う
//	m_renderPass->setRenderTarget(0, m_depthMap);
//	m_renderPass->setDepthBuffer(m_depthBuffer);
//	m_renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0);
//}

RenderPass* ClusteredShadingGeometryRenderingPass::renderPass() const
{
	return m_renderPass;
}

//bool ClusteredShadingGeometryRenderingPass::filterElement(RenderDrawElement* element) const
//{
//	return element->elementType == RenderDrawElementType::LightDisc;
//}

ShaderTechnique* ClusteredShadingGeometryRenderingPass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	Shader* shader = requestedShader;
	if (!shader) shader = m_defaultShader;

	// ライトがひとつもない場合はライティングなしを選択
	if (!m_ownerRenderer->lightClusters().hasLight()) {
		requestedShadingModel = ShadingModel::UnLighting;
        // TODO: わざわざ UnLighting テクニック用意しないとならないので面倒というか忘れやすい
	}

	ShaderTechniqueClass classSet;
    classSet.defaultTechnique = false;
	classSet.ligiting = ShaderTechniqueClass_Ligiting::Forward;
	classSet.phase = ShaderTechniqueClass_Phase::Geometry;
	classSet.meshProcess = requestedMeshProcess;
	classSet.shadingModel = tlanslateShadingModel(requestedShadingModel);
    ShaderTechnique* technique = ShaderHelper::findTechniqueByClass(shader, classSet);
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
void ShadowCasterPass::init()
{
	SceneRendererPass::init();

	m_defaultShader = manager()->builtinShader(BuiltinShader::ShadowCaster);

	m_shadowMap = makeObject<RenderTargetTexture>(1024, 1024, TextureFormat::RGBA32F, false);
	m_depthBuffer = makeObject<DepthBuffer>(1024, 1024);
	m_renderPass = makeObject<RenderPass>();

	//g_m_shadowMap = m_shadowMap;
}

//Shader* ShadowCasterPass::getDefaultShader() const
//{
//	return m_defaultShader;
//}

void ShadowCasterPass::onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	m_renderPass->setRenderTarget(0, m_shadowMap);
	m_renderPass->setDepthBuffer(m_depthBuffer);
	m_renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0);
}

//void ShadowCasterPass::onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer)
//{
//	frameBuffer->renderTarget[0] = m_shadowMap;
//	frameBuffer->depthBuffer = m_depthBuffer;
//	//context->setRenderTarget(0, m_shadowMap);
//	//context->setDepthBuffer(m_depthBuffer);
//	//m_renderPass->setRenderTarget(0, m_shadowMap);
//	//m_renderPass->setDepthBuffer(m_depthBuffer);
//	//context->setRenderPass(m_renderPass);
//	//context->clear(ClearFlags::All, Color::Transparency, 1.0f, 0);
//	m_renderPass->setRenderTarget(0, m_shadowMap);
//	m_renderPass->setDepthBuffer(m_depthBuffer);
//	m_renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0);
//}
//
RenderPass* ShadowCasterPass::renderPass() const
{
	return m_renderPass;
}

ShaderTechnique* ShadowCasterPass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	// force default
	return m_defaultShader->techniques()->front();
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

void ClusteredShadingSceneRenderer::init(RenderingManager* manager)
{
	SceneRenderer::init();

	m_depthPrepass = makeObject<DepthPrepass>();
	//addPass(m_depthPrepass);


	m_lightOcclusionPass = makeObject<LightOcclusionPass>();
	addPass(m_lightOcclusionPass);

	// pass "Geometry"
    auto geometryPass = makeObject<ClusteredShadingGeometryRenderingPass>(this);
	addPass(geometryPass);

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
    detail::ShaderSemanticsManager* ssm = detail::ShaderHelper::semanticsManager(shader);

    // TODO: 
    // 毎回 findParameter していたのをテーブル対応にしたことで 50us → 1us 以下にできた。
    // ただ、もう少し最適化の余地はある。以下のパラメータはシーン全体でひとつなので、
    // 今 onSetAdditionalShaderPassVariables は DrawElement ごとに呼び出されているが、
    // 事前に描画で使うシェーダを集めておいて Scene 単位はまとめて設定する。

#if 1
    v = ssm->getParameterBySemantics(BuiltinSemantics::GlobalLightInfoTexture);
    if (v) v->setTexture(m_lightClusters.getGlobalLightInfoTexture());

    v = ssm->getParameterBySemantics(BuiltinSemantics::LocalLightInfoTexture);
    if (v) v->setTexture(m_lightClusters.getLightInfoTexture());

    v = ssm->getParameterBySemantics(BuiltinSemantics::LightClustersTexture);
    if (v) v->setTexture(m_lightClusters.getClustersVolumeTexture());

    v = ssm->getParameterBySemantics(BuiltinSemantics::NearClip2);
    if (v) v->setFloat(m_lightClusters.m_nearClip);

    v = ssm->getParameterBySemantics(BuiltinSemantics::FarClip2);
    if (v) v->setFloat(m_lightClusters.m_farClip);

    v = ssm->getParameterBySemantics(BuiltinSemantics::CameraPosition2);
    if (v) v->setVector(Vector4(m_lightClusters.m_cameraPos, 0));

	if (const auto* params = sceneGlobalParams()) {
		v = ssm->getParameterBySemantics(BuiltinSemantics::FogColorAndDensity);
		if (v) v->setVector(Vector4(params->fogColor.rgb() * params->fogColor.a, params->fogDensity));
		
		v = ssm->getParameterBySemantics(BuiltinSemantics::FogParams);
		if (v) v->setVector(Vector4(params->startDistance, params->lowerHeight, params->upperHeight, params->heightFogDensity));
	
	

		// TODO: Test
		v = shader->findParameter(u"ln_MainLightDirection");
		if (v) v->setVector(Vector4(mainLightInfo()->m_direction, 0.0f));
	}

	// TODO: Test
	v = shader->findParameter(u"_LensflareOcclusionMap");
	if (v) {
		v->setTexture(m_lightOcclusionPass->m_lensflareOcclusionMap);
	}
	

#else
    //static Shader* lastShader = nullptr;
    //if (lastShader == shader) return;
    //lastShader = shader;
	
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
#endif
}

#endif

} // namespace detail
} // namespace ln

