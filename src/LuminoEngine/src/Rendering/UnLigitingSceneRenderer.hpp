#pragma once
#include "SceneRenderer.hpp"

namespace ln {
namespace detail {

class UnLigitingSceneRendererPass
	: public SceneRendererPass
{
public:
	UnLigitingSceneRendererPass();

	void init(RenderingManager* manager);



	virtual void onBeginPass(SceneRenderer* sceneRenderer, GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
	virtual RenderPass* renderPass() const;

	virtual ShaderTechnique* selectShaderTechnique(
		const ShaderTechniqueRequestClasses& requester,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;


private:
	Ref<Shader> m_defaultShader;
	Ref<RenderPass> m_renderPass;
};

class UnLigitingSceneRenderer
	: public SceneRenderer
{
public:
	void init(RenderingManager* manager);

	SceneRendererPass* mainRenderPass() const override;

	virtual void onSetAdditionalShaderPassVariables(ShaderTechnique* technique) override;

	const Ref<UnLigitingSceneRendererPass>& rendererPass() const { return m_rendererPass; }

	RenderTargetTexture* lightOcclusionMap = nullptr;

private:
	Ref<UnLigitingSceneRendererPass> m_rendererPass;
};

} // namespace detail
} // namespace ln

