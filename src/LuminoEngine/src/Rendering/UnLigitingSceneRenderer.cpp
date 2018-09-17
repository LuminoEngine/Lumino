
#include "Internal.hpp"
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

void UnLigitingSceneRendererPass::initialize(RenderingManager* manager)
{
	m_defaultShader = manager->builtinShader(BuiltinShader::Sprite);
}

ShaderTechnique* UnLigitingSceneRendererPass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	Shader* requestedShader,
	ShadingModel requestedShadingModel)
{
	ShaderTechnique* tech = nullptr;
	if (requestedShader) {
		ShaderTechniqueClass key = {
			ShaderTechniqueClass_Ligiting::Forward,
			ShaderTechniqueClass_Phase::Geometry,
			requestedMeshProcess,
			ShaderTechniqueClass_ShadingModel::UnLighting,	// requestedShadingModel が同指定されていても、Pass 優先
		};
		tech = requestedShader->findTechniqueByClass(key);
	}

	if (tech) {
		return tech;
	}
	else {
		return m_defaultShader->techniques().front();
	}
}

//==============================================================================
// UnLigitingSceneRenderer

void UnLigitingSceneRenderer::initialize(RenderingManager* manager)
{
	auto pass = makeRef<UnLigitingSceneRendererPass>();
	pass->initialize(manager);
	addPass(pass);
}

} // namespace detail
} // namespace ln

