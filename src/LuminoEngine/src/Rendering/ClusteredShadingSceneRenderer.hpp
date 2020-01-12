#pragma once
#include "SceneRenderer.hpp"
#include "LightClusters.hpp"

namespace ln {
namespace detail {
class ClusteredShadingSceneRenderer;

#if 1

//struct FogParams
//{
//	Color	color;
//	float	density = 0.0f;
//};

class DepthPrepass
	: public SceneRendererPass
{
public:
	DepthPrepass();
	virtual ~DepthPrepass();
	void init();


	virtual void onBeginRender(SceneRenderer* sceneRenderer) override;
	virtual void onEndRender(SceneRenderer* sceneRenderer) override;

	virtual void onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
	//virtual void onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer) override;
	virtual RenderPass* renderPass() const;

	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;

	//virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;
	//virtual void onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView) override;

public:	// TODO:
	Ref<Shader>					m_defaultShader;
	Ref<RenderTargetTexture>	m_depthMap;
	Ref<DepthBuffer>	m_depthBuffer;
	Ref<RenderPass> m_renderPass;
};

class ClusteredShadingGeometryRenderingPass
	: public SceneRendererPass
{
public:
	ClusteredShadingGeometryRenderingPass();
	virtual ~ClusteredShadingGeometryRenderingPass();
	void init(ClusteredShadingSceneRenderer* ownerRenderer);

	virtual void onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
	//virtual void onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer) override;
	virtual RenderPass* renderPass() const;

	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;

	//virtual Shader* getDefaultShader() const override;
	//virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;

	//virtual void onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView) override;

private:
	ClusteredShadingSceneRenderer* m_ownerRenderer;
	Ref<Shader> m_defaultShader;
	ShaderTechnique* m_defaultShaderTechnique;
	//Ref<Shader>					m_unLightingShader;
	//ShaderTechnique*			m_unLightingShaderTechnique;
	Ref<RenderPass> m_renderPass;
};

class ShadowCasterPass
	: public SceneRendererPass
{
public:
	//CameraInfo	view;

	ShadowCasterPass();
	virtual ~ShadowCasterPass();
	void init();

	//virtual Shader* getDefaultShader() const override;

	virtual void onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
	//virtual void onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer) override;
	virtual RenderPass* renderPass() const;

	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;


	//virtual void overrideCameraInfo(detail::CameraInfo* cameraInfo) override;

protected:
	//virtual ShaderPass* selectShaderPass(Shader* shader) override;

public:	// TODO:
	Ref<Shader>		m_defaultShader;
	Ref<RenderTargetTexture>	m_shadowMap;
	Ref<DepthBuffer>	m_depthBuffer;
	Ref<RenderPass> m_renderPass;
};

class ClusteredShadingSceneRenderer
	: public SceneRenderer
{
public:
	ClusteredShadingSceneRenderer();
	virtual ~ClusteredShadingSceneRenderer();
	void init(RenderingManager* manager);
	//void setSceneGlobalRenderSettings(const SceneGlobalRenderSettings& settings) { m_renderSettings = settings; }
	//void setFogParams(const FogParams& params) { m_fogParams = params; }
	DepthPrepass* getDepthPrepass() const { return m_depthPrepass; }
	const LightClusters& lightClusters() const { return m_lightClusters; }

protected:
	//virtual void onBeginRender() override;
	//virtual void onEndRender() override;

	virtual void collect(const detail::CameraInfo& cameraInfo) override;
	virtual void onCollectLight(const DynamicLightInfo& light) override;
	virtual void onSetAdditionalShaderPassVariables(Shader* shader) override;

private:
	LightClusters				m_lightClusters;
	//SceneGlobalRenderSettings	m_renderSettings;
	//FogParams					m_fogParams;
	Ref<DepthPrepass>			m_depthPrepass;
};
#endif

} // namespace detail
} // namespace ln

