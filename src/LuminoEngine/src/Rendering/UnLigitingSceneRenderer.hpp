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

	virtual void onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
	virtual RenderPass* renderPass() const;

	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
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

private:
};

} // namespace detail
} // namespace ln

