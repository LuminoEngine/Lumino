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

	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;

private:
	Ref<Shader> m_defaultShader;
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

