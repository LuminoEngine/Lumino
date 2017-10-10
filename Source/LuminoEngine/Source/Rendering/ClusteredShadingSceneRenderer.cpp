
#include "../Internal.h"
#include "ClusteredShadingSceneRenderer.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// ClusteredShadingGeometryRenderingPass
//==============================================================================
ClusteredShadingGeometryRenderingPass::ClusteredShadingGeometryRenderingPass()
{
}

ClusteredShadingGeometryRenderingPass::~ClusteredShadingGeometryRenderingPass()
{
}

void ClusteredShadingGeometryRenderingPass::initialize()
{
	RenderingPass2::initialize();

	m_defaultShader = manager->getBuiltinShader(BuiltinShader::Sprite);
}

Shader* ClusteredShadingGeometryRenderingPass::getDefaultShader() const
{
	return m_defaultShader;
}

ShaderPass* ClusteredShadingGeometryRenderingPass::selectShaderPass(Shader* shader)
{
	//ShaderPass* pass = nullptr;
	//ShaderTechnique* tech = shader->findTechnique(_T("ClusteredForward"));
	//if (tech)
	//{
	//	pass = tech->getPass(_T("Geometry"));
	//	if (pass)
	//	{
	//		return pass;
	//	}
	//}

	return RenderingPass2::selectShaderPass(shader);
}

//==============================================================================
// ClusteredShadingSceneRenderer
//==============================================================================

ClusteredShadingSceneRenderer::ClusteredShadingSceneRenderer()
{
}

ClusteredShadingSceneRenderer::~ClusteredShadingSceneRenderer()
{
}

void ClusteredShadingSceneRenderer::initialize(GraphicsManager* manager)
{
	SceneRenderer::initialize(manager);

	// pass "Geometry"
	addPass(newObject<ClusteredShadingGeometryRenderingPass>());
}

void ClusteredShadingSceneRenderer::prepare()
{
	SceneRenderer::prepare();
}

void ClusteredShadingSceneRenderer::onShaderPassChainging(ShaderPass* pass)
{
	return SceneRenderer::onShaderPassChainging(pass);
}

} // namespace detail
LN_NAMESPACE_END

