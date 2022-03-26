
#include "Internal.hpp"
#include <LuminoGraphics/RHI/RenderPass.hpp>
#include "RenderingManager.hpp"
#include "UnLigitingSceneRenderer.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UnLigitingSceneRendererPass

UnLigitingSceneRendererPass::UnLigitingSceneRendererPass()
	: m_defaultShader(nullptr)
{
}

void UnLigitingSceneRendererPass::init(RenderingManager* manager, bool forPostEffect)
{
	if (forPostEffect)
		m_defaultShader = manager->builtinShader(BuiltinShader::CopyScreen);
	else
		m_defaultShader = manager->builtinShader(BuiltinShader::Sprite);
	m_renderPass = makeObject<RenderPass>();
}

void UnLigitingSceneRendererPass::onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) {
	m_renderPass->setRenderTarget(0, renderTarget);
	m_renderPass->setDepthBuffer(depthBuffer);
	const auto& info = clearInfo();
	m_renderPass->setClearValues(info.flags, info.color, info.depth, info.stencil);
}

RenderPass* UnLigitingSceneRendererPass::renderPass() const
{
	return m_renderPass;
}

ShaderTechnique* UnLigitingSceneRendererPass::selectShaderTechnique(
	const ShaderTechniqueRequestClasses& requester,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	ShaderTechnique* tech = nullptr;
	if (requestedShader) {
		kokage::ShaderTechniqueClass key = {
            false,
			kokage::ShaderTechniqueClass_Phase::Forward,
			requester.meshProcess,
			kokage::ShaderTechniqueClass_ShadingModel::Unlit,	// requestedShadingModel が同指定されていても、Pass 優先
			requester.drawMode,
			kokage::ShaderTechniqueClass_Normal::Default,
			kokage::ShaderTechniqueClass_Roughness::Default,
		};
		tech = ShaderInternal::findTechniqueByClass(requestedShader, key);
	}

	if (tech) {
		return tech;
	}
	else {
		return m_defaultShader->techniques()->front();
	}
}

#if 0
//==============================================================================
// UnLigitingSceneRenderer

void UnLigitingSceneRenderer::init(RenderingManager* manager, bool forPostEffect)
{
	SceneRenderer::init();
	m_rendererPass = makeRef<UnLigitingSceneRendererPass>();
	m_rendererPass->init(manager, forPostEffect);
	addPass(m_rendererPass);
}

SceneRendererPass* UnLigitingSceneRenderer::mainRenderPass() const
{
	return m_rendererPass;
}

void UnLigitingSceneRenderer::onSetAdditionalShaderPassVariables(ShaderTechnique* technique)
{
	ShaderParameter2* v;

	// TODO: Test
	v = technique->shader()->findParameter(u"_LensflareOcclusionMap");
	if (v) {
		v->setTexture(lightOcclusionMap);
	}
}
#endif

} // namespace detail
} // namespace ln

