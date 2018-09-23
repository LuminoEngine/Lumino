#pragma once
#include "SceneRenderer.hpp"
#include "LightClusters.hpp"

namespace ln {
namespace detail {

#if 1

struct FogParams
{
	Color	color;
	float	density = 0.0f;
};

class DepthPrepass
	: public SceneRendererPass
{
public:
	DepthPrepass();
	virtual ~DepthPrepass();
	void initialize();


	virtual void onBeginRender(RenderView* renderView) override;
	virtual void onEndRender(RenderView* renderView) override;

	virtual void onBeginPass(RenderView* renderView, GraphicsContext* context, FrameBuffer* frameBuffer) override;

	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;

	//virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;
	//virtual void onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView) override;

public:	// TODO:
	Ref<Shader>					m_defaultShader;
	RenderTargetTexture*	m_depthMap;
	DepthBuffer*	m_depthBuffer;
};

class ClusteredShadingGeometryRenderingPass
	: public SceneRendererPass
{
public:
	ClusteredShadingGeometryRenderingPass();
	virtual ~ClusteredShadingGeometryRenderingPass();
	void initialize();

	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;

	//virtual Shader* getDefaultShader() const override;
	//virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;

	//virtual void onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView) override;

private:
	Ref<Shader>					m_defaultShader;
	ShaderTechnique*			m_defaultShaderTechnique;
	//Ref<Shader>					m_unLightingShader;
	//ShaderTechnique*			m_unLightingShaderTechnique;
};

class ShadowCasterPass
	: public SceneRendererPass
{
public:
	//CameraInfo	view;

	ShadowCasterPass();
	virtual ~ShadowCasterPass();
	void initialize();

	//virtual Shader* getDefaultShader() const override;

	virtual void onBeginPass(RenderView* renderView, GraphicsContext* context, FrameBuffer* frameBuffer) override;

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
};

class ClusteredShadingSceneRenderer
	: public SceneRenderer
{
public:
	ClusteredShadingSceneRenderer();
	virtual ~ClusteredShadingSceneRenderer();
	void initialize(RenderingManager* manager);
	//void setSceneGlobalRenderSettings(const SceneGlobalRenderSettings& settings) { m_renderSettings = settings; }
	void setFogParams(const FogParams& params) { m_fogParams = params; }
	DepthPrepass* getDepthPrepass() const { return m_depthPrepass; }

protected:
	//virtual void onBeginRender() override;
	//virtual void onEndRender() override;

	virtual void collect(const detail::CameraInfo& cameraInfo) override;
	virtual void onCollectLight(const DynamicLightInfo& light) override;
	virtual void onSetAdditionalShaderPassVariables(Shader* shader) override;

private:
	LightClusters				m_lightClusters;
	//SceneGlobalRenderSettings	m_renderSettings;
	FogParams					m_fogParams;
	Ref<DepthPrepass>			m_depthPrepass;
};
#endif

} // namespace detail
} // namespace ln

