
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

void UnLigitingSceneRendererPass::init(RenderingManager* manager)
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
            false,
			ShaderTechniqueClass_Ligiting::Forward,
			ShaderTechniqueClass_Phase::Geometry,
			requestedMeshProcess,
			ShaderTechniqueClass_ShadingModel::UnLighting,	// requestedShadingModel が同指定されていても、Pass 優先
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

//==============================================================================
// UnLigitingSceneRenderer

void UnLigitingSceneRenderer::init(RenderingManager* manager)
{
	auto pass = makeRef<UnLigitingSceneRendererPass>();
	pass->init(manager);
	addPass(pass);
}

} // namespace detail
} // namespace ln

