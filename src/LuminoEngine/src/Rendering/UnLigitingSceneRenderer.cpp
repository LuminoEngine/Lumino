
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

void UnLigitingSceneRendererPass::init(RenderingManager* manager)
{
	m_defaultShader = manager->builtinShader(BuiltinShader::Sprite);
	m_renderPass = makeObject<RenderPass>();
}

void UnLigitingSceneRendererPass::onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	m_renderPass->setRenderTarget(0, renderTarget);
	m_renderPass->setDepthBuffer(depthBuffer);
	m_renderPass->setClearValues(ClearFlags::None, Color::Transparency, 1.0f, 0);
}

RenderPass* UnLigitingSceneRendererPass::renderPass() const
{
	return m_renderPass;
}

ShaderTechnique* UnLigitingSceneRendererPass::selectShaderTechnique(
	ShaderTechniqueClass_MeshProcess requestedMeshProcess,
	ShaderTechniqueClass_DrawMode drawMode,
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
			ShaderTechniqueClass_ShadingModel::Unlit,	// requestedShadingModel が同指定されていても、Pass 優先
			drawMode,
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
	SceneRenderer::init();
	auto pass = makeRef<UnLigitingSceneRendererPass>();
	pass->init(manager);
	addPass(pass);
}

void UnLigitingSceneRenderer::onSetAdditionalShaderPassVariables(Shader* shader)
{
	ShaderParameter2* v;

	// TODO: Test
	v = shader->findParameter(u"_LensflareOcclusionMap");
	if (v) {
		v->setTexture(lightOcclusionMap);
	}
}

} // namespace detail
} // namespace ln

