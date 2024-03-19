
#include "Internal.hpp"
#include <LuminoEngine/GPU/SamplerState.hpp>
#include <LuminoEngine/GPU/RenderPass.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawCommand.hpp>
//#include "../Graphics/RenderTargetTextureCache.hpp"
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderingPipeline/StandardRenderingPipeline.hpp>
#include "ClusteredShadingSceneRenderer.hpp"
#include "RenderStage.hpp"
#include "RenderElement.hpp"
#include "RLIs/RLIMaterial.hpp"

namespace ln {

namespace detail {

#if 1

//==============================================================================
// ForwardGBufferPrepass

ForwardGBufferPrepass::ForwardGBufferPrepass() {
}

ForwardGBufferPrepass::~ForwardGBufferPrepass() {
}

void ForwardGBufferPrepass::init() {
    SceneRendererPass::init();
    m_defaultShader = manager()->builtinShader(BuiltinShader::ForwardGBufferPrepass);

    if (Debug) {

        //m_depthMap = RenderTargetTexture::create(640, 480, TextureFormat::RGBA32F);
        //m_depthMap->setSamplerState(m_samplerState);
        //m_normalMap = RenderTargetTexture::create(640, 480, TextureFormat::RGBA32F);
        //m_normalMap->setSamplerState(m_samplerState);
        //m_materialMap = RenderTargetTexture::create(640, 480, TextureFormat::RGBA32F);
        //m_materialMap->setSamplerState(m_samplerState);
    }
    m_renderPass = makeObject_deprecated<RenderPass>();

    m_internalSceneRenderPass = makeURef<kanata::SceneRenderPass>(manager(), m_defaultShader, kokage::ShaderTechniqueClass_Phase::ForwardGBufferPrepass);
    m_internalSceneRenderPass->overrideCommand = [](kanata::DrawCommand* cmd) {
        // このパスではブレンドを禁止する。
        // 例えば法線の向きをブレンドしてしまうと描画がおかしくなる。
        // また G-Buffer で R32S を使っているものがあるが、これはそもそも Blend が禁止されている。 (Vulkan の Validation にひっかかる)
        cmd->pipelineState.blendState.independentBlendEnable = false;
        cmd->pipelineState.blendState.renderTargets[0].blendEnable = false;
    };

}

void ForwardGBufferPrepass::onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) {
    const auto* renderingPipeline = static_cast<SceneRenderingPipeline*>(sceneRenderer->renderingPipeline());
    auto size = renderingPipeline->renderingFrameBufferSize();
    m_depthBuffer = DepthBuffer::getTemporary(size.width, size.height);

    m_renderPass->setRenderTarget(0, renderingPipeline->viweNormalAndDepthBuffer());
    m_renderPass->setRenderTarget(1, renderingPipeline->viweDepthBuffer());
    m_renderPass->setRenderTarget(2, renderingPipeline->materialBuffer());
    m_renderPass->setRenderTarget(3, renderingPipeline->objectIdBuffer());
    m_renderPass->setDepthBuffer(m_depthBuffer);
    m_renderPass->setClearValues(ClearFlags::Depth, Color::Gray, 1.0f, 0);
}

void ForwardGBufferPrepass::onEndRender(SceneRenderer* sceneRenderer) {
    //const auto* renderingPipeline = sceneRenderer->renderingPipeline();

    if (!Debug) {
        //RenderTargetTexture::releaseTemporary(m_depthMap);
        //m_depthMap = nullptr;
        //RenderTargetTexture::releaseTemporary(m_normalMap);
        //m_normalMap = nullptr;
        //RenderTargetTexture::releaseTemporary(m_materialMap);
        //m_materialMap = nullptr;
    }
    DepthBuffer::releaseTemporary(m_depthBuffer);
    m_depthBuffer = nullptr;
}

//void ForwardGBufferPrepass::onBeginPass(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
//{
//}

//void ForwardGBufferPrepass::onBeginPass(GraphicsCommandList* context, FrameBuffer* frameBuffer)
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

RenderPass* ForwardGBufferPrepass::renderPass() const {
    return m_renderPass;
}

void ForwardGBufferPrepass::overrideFinalMaterial(RLIMaterial* material) {
    // このパスではブレンドを禁止する。
    // 例えば法線の向きをブレンドしてしまうと描画がおかしくなる。
    material->blendMode = BlendMode::Normal;
}

ShaderTechnique* ForwardGBufferPrepass::selectShaderTechnique(
    const ShaderTechniqueRequestClasses& requester,
    Shader* requestedShader,
    ShadingModel requestedShadingModel) {
    const Shader* actualShader = (requestedShader) ? requestedShader : m_defaultShader.get();

    kokage::ShaderTechniqueClass classSet;
    classSet.defaultTechnique = false;
    classSet.phase = kokage::ShaderTechniqueClass_Phase::ForwardGBufferPrepass;
    classSet.meshProcess = requester.meshProcess;
    classSet.drawMode = requester.drawMode;
    classSet.shadingModel = kokage::ShaderTechniqueClass_ShadingModel::Default;
    classSet.normalClass = requester.normal;       // Normal を出力したいので考慮する
    classSet.roughnessClass = requester.roughness; // Roughness を出力したいので考慮する
    ShaderTechnique* technique = ShaderInternal::findTechniqueByClass(actualShader, classSet);
    if (technique)
        return technique;
    else
        return m_defaultShader->techniques()->front();
}

//==============================================================================
// LightOcclusionPass
#if 0
LightOcclusionPass::LightOcclusionPass()
{
}

void LightOcclusionPass::init()
{
	SceneRendererPass::init();
	m_blackShader = detail::EngineDomain::renderingManager()->builtinShader(BuiltinShader::BlackShader);
	m_blackShaderTechnique = m_blackShader->findTechnique(_TT("Default");
	m_renderPass = makeObject_deprecated<RenderPass>();
}

void LightOcclusionPass::onBeginRender(SceneRenderer* sceneRenderer)
{
	auto size = sceneRenderer->renderingPipeline()->renderingFrameBufferSize();
	acquireBuffers(size.width, size.height);
	m_depthBuffer = DepthBuffer::getTemporary(size.width, size.height);
}

void LightOcclusionPass::onEndRender(SceneRenderer* sceneRenderer)
{
	//RenderTargetTexture::releaseTemporary(m_lensflareOcclusionMap);
	DepthBuffer::releaseTemporary(m_depthBuffer);
	//m_lensflareOcclusionMap = nullptr;
	m_depthBuffer = nullptr;
}

void LightOcclusionPass::onBeginPass(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	m_renderPass->setRenderTarget(0, m_lensflareOcclusionMap);
	m_renderPass->setDepthBuffer(m_depthBuffer);
	m_renderPass->setClearValues(ClearFlags::All, Color::Black, 1.0f, 0);
	//m_renderPass->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
}

RenderPass* LightOcclusionPass::renderPass() const
{
	return m_renderPass;
}

bool LightOcclusionPass::filterElement(RenderDrawElement* element) const
{
	//return true;
	//return SceneRendererPass::filterElement(element);
	if (element->flags().hasFlag(RenderDrawElementTypeFlags::BackgroundSky)) return false;

	return (element->flags() & (
		RenderDrawElementTypeFlags::Opaque |
		RenderDrawElementTypeFlags::Transparent |
		RenderDrawElementTypeFlags::LightDisc)) != RenderDrawElementTypeFlags::None;
}

ShaderTechnique* LightOcclusionPass::selectShaderTechnique(
	const ShaderTechniqueRequestClasses& requester,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	if (!requestedShader)
		return m_blackShaderTechnique;

	ShaderTechniqueClass classSet;
	classSet.defaultTechnique = false;
	classSet.phase = ShaderTechniqueClass_Phase::LightDisc;
	classSet.meshProcess = requester.meshProcess;
	classSet.shadingModel = tlanslateShadingModel(requestedShadingModel);
	classSet.drawMode = requester.drawMode;
	classSet.normalClass = ShaderTechniqueClass_Normal::Default;
	classSet.roughnessClass = ShaderTechniqueClass_Roughness::Default;
	ShaderTechnique* technique = ShaderHelper::findTechniqueByClass(requestedShader, classSet);
	if (technique)
		return technique;
	else
		return m_blackShaderTechnique;
}

void LightOcclusionPass::acquireBuffers(int width, int height)
{
	// TODO: getTemporary() 使った方がいいかも？
	// ただ、SceneRenderer をまたいでポストエフェクトで使いたいので、get/releaseのスコープを Pipeline単位にしたりする必要がある。

	if (!m_lensflareOcclusionMap || (m_lensflareOcclusionMap->width() != width || m_lensflareOcclusionMap->height() != height)) {
		m_lensflareOcclusionMap = makeObject_deprecated< RenderTargetTexture>(width, height, TextureFormat::RGBA8, false);
	}

	//if (!m_depthBuffer || (m_depthBuffer->width() != width || m_depthBuffer->height() != height)) {
	//	m_depthBuffer = DepthBuffer::getTemporary(width, height);
	//}
}
#endif

//==============================================================================
// ClusteredShadingGeometryRenderingPass

static const String ClusteredShadingGeometryRenderingPass_TechniqueName = _T("Forward_Geometry");
//static const String ClusteredShadingGeometryRenderingPass_PassName = _T("Geometry");

ClusteredShadingGeometryRenderingPass::ClusteredShadingGeometryRenderingPass() {
}

ClusteredShadingGeometryRenderingPass::~ClusteredShadingGeometryRenderingPass() {
}

void ClusteredShadingGeometryRenderingPass::init(ClusteredShadingSceneRenderer* ownerRenderer) {
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
    //#include "Resource/Unlit.fx.h"
    //		};
    //		static const size_t size = LN_ARRAY_SIZE_OF(data);
    //		m_unLightingShader = Shader::create((const char*)data, size, nullptr, ShaderCodeType::RawHLSL);
    //	}
    //	m_unLightingShaderTechnique = m_unLightingShader->getTechniques()[0];

    m_renderPass = makeObject_deprecated<RenderPass>();
    m_internalSceneRenderPass = makeURef<kanata::SceneRenderPass>(manager(), m_defaultShader, kokage::ShaderTechniqueClass_Phase::Forward);
}

void ClusteredShadingGeometryRenderingPass::onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) {
    m_renderPass->setRenderTarget(0, renderTarget);
    m_renderPass->setDepthBuffer(depthBuffer);
    //m_renderPass->setClearValues(ClearFlags::None, Color::Transparency, 1.0f, 0);
    const auto& info = clearInfo();
    m_renderPass->setClearValues(info.flags, info.color, info.depth, info.stencil);
}

//void ClusteredShadingGeometryRenderingPass::onBeginPass(GraphicsCommandList* context, FrameBuffer* frameBuffer)
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

RenderPass* ClusteredShadingGeometryRenderingPass::renderPass() const {
    return m_renderPass;
}

//bool ClusteredShadingGeometryRenderingPass::filterElement(RenderDrawElement* element) const
//{
//	return element->elementType == RenderDrawElementType::LightDisc;
//}

ShaderTechnique* ClusteredShadingGeometryRenderingPass::selectShaderTechnique(
    const ShaderTechniqueRequestClasses& requester,
    Shader* requestedShader,
    ShadingModel requestedShadingModel) {
    Shader* shader = requestedShader;
    if (!shader) shader = m_defaultShader;

    // ライトがひとつもない場合はライティングなしを選択
    if (!m_ownerRenderer->lightClusters().hasLight()) {
        requestedShadingModel = ShadingModel::Unlit;
        // TODO: わざわざ Unlit テクニック用意しないとならないので面倒というか忘れやすい
    }

    kokage::ShaderTechniqueClass classSet;
    classSet.defaultTechnique = false;
    classSet.phase = kokage::ShaderTechniqueClass_Phase::Forward;
    classSet.meshProcess = requester.meshProcess;
    classSet.shadingModel = tlanslateShadingModel(requestedShadingModel);
    classSet.drawMode = requester.drawMode;
    classSet.normalClass = requester.normal;
    ShaderTechnique* technique = ShaderInternal::findTechniqueByClass(shader, classSet);
    if (technique)
        return technique;
    else
        return m_defaultShaderTechnique;
}

//void ClusteredShadingGeometryRenderingPass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
//{
//
//	//if (stageData.shadingModel == ShadingModel::Unlit)
//	//{
//	//	classSet.shadingModel = ShaderTechniqueClass_ShadingModel::Unlit;
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
ShadowCasterPass::ShadowCasterPass() {
}

ShadowCasterPass::~ShadowCasterPass() {
}

RenderTargetTexture* g_m_shadowMap = nullptr;
void ShadowCasterPass::init() {
    SceneRendererPass::init();

    m_defaultShader = manager()->builtinShader(BuiltinShader::ShadowCaster);

    m_renderPass = makeObject_deprecated<RenderPass>();

    m_internalSceneRenderPass = makeURef<kanata::SceneRenderPass>(manager(), m_defaultShader, kokage::ShaderTechniqueClass_Phase::ShadowCaster);
}

//Shader* ShadowCasterPass::getDefaultShader() const
//{
//	return m_defaultShader;
//}

void ShadowCasterPass::onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) {
    const auto* renderingPipeline = static_cast<SceneRenderingPipeline*>(sceneRenderer->renderingPipeline());
    //RenderTargetTexture* shadowMap =

    g_m_shadowMap = renderingPipeline->shadowMap();

    m_renderPass->setRenderTarget(0, renderingPipeline->shadowMap());
    m_renderPass->setDepthBuffer(renderingPipeline->shadowMapDepthBuffer());
    m_renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0);

    if (auto param = m_defaultShader->findParameter(_TT("ln_ViewProjection_Light0"))) {
        param->setMatrix(sceneRenderer->mainRenderViewInfo().mainLightViewProjection);
    }

    //sceneRenderer->m_mainCameraInfo.mainLightShadowMapPixelSize =
}

//void ShadowCasterPass::onBeginPass(GraphicsCommandList* context, FrameBuffer* frameBuffer)
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
RenderPass* ShadowCasterPass::renderPass() const {
    return m_renderPass;
}

ShaderTechnique* ShadowCasterPass::selectShaderTechnique(
    const ShaderTechniqueRequestClasses& requester,
    Shader* requestedShader,
    ShadingModel requestedShadingModel) {
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

ClusteredShadingSceneRenderer::ClusteredShadingSceneRenderer() {
}

ClusteredShadingSceneRenderer::~ClusteredShadingSceneRenderer() {
}

void ClusteredShadingSceneRenderer::init(RenderingManager* manager) {
    SceneRenderer::init();

    m_shadowCasterPass = makeObject_deprecated<ShadowCasterPass>();
    //addPass(shadowPass);

    m_depthPrepass = makeObject_deprecated<ForwardGBufferPrepass>();
    //addPass(m_depthPrepass);

    //m_lightOcclusionPass = makeObject_deprecated<LightOcclusionPass>();
    //addPass(m_lightOcclusionPass);

    // pass "Geometry"
    m_geometryPass = makeObject_deprecated<ClusteredShadingGeometryRenderingPass>(this);
    //addPass(m_geometryPass);

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

SceneRendererPass* ClusteredShadingSceneRenderer::mainRenderPass() const {
    return m_geometryPass;
}

void ClusteredShadingSceneRenderer::collect(RenderingPipeline* renderingPipeline, const detail::CameraInfo& cameraInfo, RenderPart targetPhase) {
    m_lightClusters.beginMakeClusters(cameraInfo.viewMatrix, cameraInfo.projMatrix, cameraInfo.viewPosition, cameraInfo.nearClip, cameraInfo.farClip);

    SceneRenderer::collect(renderingPipeline, cameraInfo, targetPhase);

    m_lightClusters.endMakeClusters();
}

// TODO: Vector3 クラスへ
static Vector3 transformDirection(const Vector3& vec, const Matrix& mat) {
    return Vector3(
        (((vec.x * mat.m11) + (vec.y * mat.m21)) + (vec.z * mat.m31)),
        (((vec.x * mat.m12) + (vec.y * mat.m22)) + (vec.z * mat.m32)),
        (((vec.x * mat.m13) + (vec.y * mat.m23)) + (vec.z * mat.m33)));
}

void ClusteredShadingSceneRenderer::onCollectLight(const DynamicLightInfo& light) {
    const CameraInfo& view = mainRenderViewInfo().cameraInfo;

    Color color = light.m_color;
    color *= light.m_intensity;

    switch (light.m_type) {
        case LightType::Ambient:
            m_lightClusters.addAmbientLight(color);
            break;
        case LightType::Hemisphere:
            m_lightClusters.addHemisphereLight(color, light.m_color2 * light.m_intensity);
            break;
        case LightType::Directional: {
            //auto a1 = transformDirection(-light.m_direction, view.viewMatrix);
            //auto a2 = -transformDirection(light.m_direction, view.viewMatrix);
            //m_lightClusters.addDirectionalLight(transformDirection(-light.m_direction, view.viewMatrix), color);
            m_lightClusters.addDirectionalLight(light.m_direction, color);
            break;
        }
        case LightType::Point:
            m_lightClusters.addPointLight(light.m_position.xyz(), light.m_range, light.m_attenuation, color);
            break;
        case LightType::Spot:
            m_lightClusters.addSpotLight(light.m_position.xyz(), light.m_range, light.m_attenuation, color, light.m_direction, light.m_spotAngle, light.m_spotPenumbra);
            break;
        default:
            LN_UNREACHABLE();
            break;
    }
}

void ClusteredShadingSceneRenderer::onSetAdditionalShaderPassVariables2(ShaderDescriptor* descriptor, ShaderPass* shaderPass) {
    // TODO:
    // 毎回 findParameter していたのをテーブル対応にしたことで 50us → 1us 以下にできた。
    // ただ、もう少し最適化の余地はある。以下のパラメータはシーン全体でひとつなので、
    // 今 onSetAdditionalShaderPassVariables は DrawElement ごとに呼び出されているが、
    // 事前に描画で使うシェーダを集めておいて Scene 単位はまとめて設定する。

    Shader* shader = shaderPass->shader();
    const auto* ssm = shaderPass->semanticsManager();

    ShaderParameter2* v;

    const auto* params = sceneGlobalParams();

    ClusteredShadingRendererInfo info = {
        m_lightClusters.getGlobalLightInfoTexture(),
        m_lightClusters.getLightInfoTexture(),
        m_lightClusters.getClustersVolumeTexture(),
        (mainLightInfo() ? mainLightInfo()->m_direction : Vector3(0, -1, 0)),
        (params ? Vector4(params->startDistance, params->lowerHeight, params->upperHeight, params->heightFogDensity) : Vector4::Zero),
        (params ? Vector4(params->fogColor.rgb() * params->fogColor.a, params->fogDensity) : Vector4::Zero),
        m_lightClusters.m_nearClip,
        m_lightClusters.m_farClip,
    };

    ssm->updateClusteredShadingVariables(descriptor, info);

#if 0
    //// TODO: Test
    //v = shader->findParameter(_TT("_LensflareOcclusionMap");
    //if (v) {
    //	v->setTexture(m_lightOcclusionPass->m_lensflareOcclusionMap);
    //}

    // TODO: Test
    if (auto param = shader->findParameter(_TT("ln_ViewProjection_Light0"))) {
        if (const auto* mainLight = mainLightInfo()) {

            //const auto pos = Vector3(10, 10, 10);
            //const auto view = Matrix::makeLookAtLH(
            //	pos,
            //	Vector3::Zero,
            //	Vector3::UnitY);
            //const auto proj = Matrix::makePerspectiveFovLH(
            //	Math::PI / 2.0f,
            //	1024.0 / 1024.0,	// TODO: LightMapSize
            //	0.5f, 100.0f);	// TODO: clip range

            //param->setMatrix(Matrix::multiply(view, proj))
            param->setMatrix(mainRenderViewInfo().mainLightViewProjection);
        }
    }
#endif
}

#endif

} // namespace detail
} // namespace ln
