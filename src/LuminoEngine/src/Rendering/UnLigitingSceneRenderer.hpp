#pragma once
#include "SceneRenderer.hpp"

namespace ln {
namespace detail {

class UnLigitingSceneRendererPass
	: public SceneRendererPass
{
public:
	UnLigitingSceneRendererPass();

	void init(RenderingManager* manager, bool forPostEffect);



	virtual void onBeginRender(SceneRenderer* sceneRenderer, GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
	virtual RenderPass* renderPass() const;

	virtual ShaderTechnique* selectShaderTechnique(
		const ShaderTechniqueRequestClasses& requester,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;


private:
	Ref<Shader> m_defaultShader;
	Ref<RenderPass> m_renderPass;
};

#if 0
class UnLigitingSceneRenderer
	: public SceneRenderer
{
public:
	void init(RenderingManager* manager, bool forPostEffect = false);

	SceneRendererPass* mainRenderPass() const override;

	virtual void onSetAdditionalShaderPassVariables(ShaderTechnique* technique) override;

	const Ref<UnLigitingSceneRendererPass>& rendererPass() const { return m_rendererPass; }

	RenderTargetTexture* lightOcclusionMap = nullptr;

private:
	Ref<UnLigitingSceneRendererPass> m_rendererPass;
};
#endif

} // namespace detail
} // namespace ln

