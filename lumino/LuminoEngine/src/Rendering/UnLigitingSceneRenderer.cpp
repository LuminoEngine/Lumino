﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/RenderPass.hpp>
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

void UnLigitingSceneRendererPass::onBeginRender(SceneRenderer* sceneRenderer, GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
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
		ShaderTechniqueClass key = {
            false,
			ShaderTechniqueClass_Phase::Forward,
			requester.meshProcess,
			ShaderTechniqueClass_ShadingModel::Unlit,	// requestedShadingModel が同指定されていても、Pass 優先
			requester.drawMode,
			ShaderTechniqueClass_Normal::Default,
			ShaderTechniqueClass_Roughness::Default,
		};
		tech = ShaderHelper::findTechniqueByClass(requestedShader, key);
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

