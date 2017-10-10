
#pragma once
#include <Lumino/Rendering/SceneRenderer.h>

LN_NAMESPACE_BEGIN
namespace detail {

class LightClusters
{
public:
};

class ClusteredShadingGeometryRenderingPass
	: public RenderingPass2
{
public:
	ClusteredShadingGeometryRenderingPass();
	virtual ~ClusteredShadingGeometryRenderingPass();
	void initialize();

	virtual Shader* getDefaultShader() const override;

protected:
	virtual ShaderPass* selectShaderPass(Shader* shader) override;

private:
	Ref<Shader>		m_defaultShader;
};

class ClusteredShadingSceneRenderer
	: public SceneRenderer
{
public:
	ClusteredShadingSceneRenderer();
	virtual ~ClusteredShadingSceneRenderer();
	void initialize(GraphicsManager* manager);

protected:
	virtual void prepare() override;
	virtual void onShaderPassChainging(ShaderPass* pass) override;
};

} // namespace detail
LN_NAMESPACE_END

