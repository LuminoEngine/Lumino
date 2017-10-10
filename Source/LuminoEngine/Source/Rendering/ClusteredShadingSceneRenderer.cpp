
#include "../Internal.h"
#include "../Graphics/GraphicsManager.h"
#include "ClusteredShadingSceneRenderer.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// LightClusters
//==============================================================================
LightClusters::LightClusters()
{
}

void LightClusters::init()
{
}

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

	//m_defaultShader = GraphicsManager::getInstance()->getBuiltinShader(BuiltinShader::LegacyDiffuse);

	m_defaultShader = Shader::create(_T("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Rendering/Resource/ClusteredShadingDefault.fx"), ShaderCodeType::RawIR);
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

void ClusteredShadingSceneRenderer::collect()
{

}

void ClusteredShadingSceneRenderer::onCollectLight(DynamicLightInfo* light)
{
	SceneRenderer::onCollectLight(light);
}

void ClusteredShadingSceneRenderer::onShaderPassChainging(ShaderPass* pass)
{
	return SceneRenderer::onShaderPassChainging(pass);
}

} // namespace detail
LN_NAMESPACE_END

