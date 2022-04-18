#pragma once
#include <LuminoEngine/Rendering/Kanata/KSceneRenderPass.hpp>
#include "SceneRenderer.hpp"
#include "LightClusters.hpp"

namespace ln {
namespace detail {
class ClusteredShadingSceneRenderer;

#if 1

// ClusteredShadingSceneRenderer 用の G-Buffer 生成パス。
// 一般的な Deferred Rendering の G-Buffer と比べて、色情報は出力しない。
// 出力は以下の通り。
// - SV_TARGET0: xyz=Normal (View-Space. 視点方向を向いているものは Z+), z=Depth
// http://rarihoma.xvs.jp/wiki/#!UE4/GBuffer%E3%81%AB%E6%9B%B8%E3%81%8D%E8%BE%BC%E3%81%BE%E3%82%8C%E3%82%8B%E5%86%85%E5%AE%B9/i.md
class ForwardGBufferPrepass
    : public SceneRendererPass {
public:
    static const int Debug = 1;
    ForwardGBufferPrepass();
    virtual ~ForwardGBufferPrepass();
    void init();

    virtual void onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
    virtual void onEndRender(SceneRenderer* sceneRenderer) override;

    //virtual void onBeginPass(GraphicsCommandList* context, FrameBuffer* frameBuffer) override;
    virtual RenderPass* renderPass() const;

    virtual void overrideFinalMaterial(RLIMaterial* material) override;

    virtual ShaderTechnique* selectShaderTechnique(
        const ShaderTechniqueRequestClasses& requester,
        Shader* requestedShader,
        ShadingModel requestedShadingModel) override;

    //virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;
    //virtual void onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView) override;
    
    kanata::SceneRenderPass* internalSceneRenderPass() const override { return m_internalSceneRenderPass; }

public: // TODO:
    Ref<Shader> m_defaultShader;
    //Ref<RenderTargetTexture>	m_depthMap;
    //Ref<RenderTargetTexture>	m_normalMap;
    //Ref<RenderTargetTexture> m_materialMap;	// x, y, z:roughness,
    Ref<DepthBuffer> m_depthBuffer;
    Ref<RenderPass> m_renderPass;
    URef<kanata::SceneRenderPass> m_internalSceneRenderPass;
};

#if 0
class LightOcclusionPass
	: public SceneRendererPass
{
public:
	LightOcclusionPass();
	void init();

	const Ref<RenderTargetTexture>& lightOcclusionMap() const { return m_lensflareOcclusionMap; }

	virtual void onBeginRender(SceneRenderer* sceneRenderer) override;
	virtual void onEndRender(SceneRenderer* sceneRenderer) override;
	virtual void onBeginPass(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
	virtual RenderPass* renderPass() const;
	virtual bool filterElement(RenderDrawElement* element) const;
	virtual ShaderTechnique* selectShaderTechnique(
		const ShaderTechniqueRequestClasses& requester,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) override;

public:
	void acquireBuffers(int width, int height);

	Ref<Shader> m_blackShader;
	Ref<ShaderTechnique> m_blackShaderTechnique;
	Ref<RenderTargetTexture> m_lensflareOcclusionMap;
	Ref<DepthBuffer> m_depthBuffer;
	Ref<RenderPass> m_renderPass;
};
#endif

class ClusteredShadingGeometryRenderingPass
    : public SceneRendererPass {
public:
    ClusteredShadingGeometryRenderingPass();
    virtual ~ClusteredShadingGeometryRenderingPass();
    void init(ClusteredShadingSceneRenderer* ownerRenderer);

    virtual void onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
    //virtual void onBeginPass(GraphicsCommandList* context, FrameBuffer* frameBuffer) override;
    virtual RenderPass* renderPass() const;

    //virtual bool filterElement(RenderDrawElement* element) const override;

    virtual ShaderTechnique* selectShaderTechnique(
        const ShaderTechniqueRequestClasses& requester,
        Shader* requestedShader,
        ShadingModel requestedShadingModel) override;

    //virtual Shader* getDefaultShader() const override;
    //virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;

    //virtual void onBeginPass(DefaultStatus* defaultStatus, RenderView* renderView) override;
    
    kanata::SceneRenderPass* internalSceneRenderPass() const override { return m_internalSceneRenderPass; }

private:
    ClusteredShadingSceneRenderer* m_ownerRenderer;
    Ref<Shader> m_defaultShader;
    ShaderTechnique* m_defaultShaderTechnique;
    //Ref<Shader>					m_unLightingShader;
    //ShaderTechnique*			m_unLightingShaderTechnique;
    Ref<RenderPass> m_renderPass;
    URef<kanata::SceneRenderPass> m_internalSceneRenderPass;
};

class ShadowCasterPass
    : public SceneRendererPass {
public:
    //CameraInfo	view;

    ShadowCasterPass();
    virtual ~ShadowCasterPass();
    void init();

    //virtual Shader* getDefaultShader() const override;

    virtual void onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
    //virtual void onBeginPass(GraphicsCommandList* context, FrameBuffer* frameBuffer) override;
    virtual RenderPass* renderPass() const;

    virtual ShaderTechnique* selectShaderTechnique(
        const ShaderTechniqueRequestClasses& requester,
        Shader* requestedShader,
        ShadingModel requestedShadingModel) override;

    kanata::SceneRenderPass* internalSceneRenderPass() const override { return m_internalSceneRenderPass; }

    //virtual void overrideCameraInfo(detail::CameraInfo* cameraInfo) override;

protected:
    //virtual ShaderPass* selectShaderPass(Shader* shader) override;

public: // TODO:
    Ref<Shader> m_defaultShader;
    Ref<RenderPass> m_renderPass;
    URef<kanata::SceneRenderPass> m_internalSceneRenderPass;
};

class ClusteredShadingSceneRenderer
    : public SceneRenderer {
public:
    ClusteredShadingSceneRenderer();
    virtual ~ClusteredShadingSceneRenderer();
    void init(RenderingManager* manager);
    SceneRendererPass* mainRenderPass() const override;
    //void setSceneGlobalRenderSettings(const SceneGlobalRenderSettings& settings) { m_renderSettings = settings; }
    //void setFogParams(const FogParams& params) { m_fogParams = params; }
    ShadowCasterPass* shadowCasterPass() const { return m_shadowCasterPass; }
    ForwardGBufferPrepass* gbufferPass() const { return m_depthPrepass; }
    ClusteredShadingGeometryRenderingPass* geometryPass() const { return m_geometryPass; }
    //const Ref<LightOcclusionPass>& lightOcclusionPass() const { return m_lightOcclusionPass; }

    const LightClusters& lightClusters() const { return m_lightClusters; }

protected:
    //virtual void onBeginRender() override;
    //virtual void onEndRender() override;

    virtual void collect(RenderingPipeline* renderingPipeline, const detail::CameraInfo& cameraInfo, RenderPart targetPhase) override;
    virtual void onCollectLight(const DynamicLightInfo& light) override;
    virtual void onSetAdditionalShaderPassVariables(ShaderSecondaryDescriptor* descriptor, ShaderTechnique* technique) override;
    virtual void onSetAdditionalShaderPassVariables2(ShaderDescriptor* descriptor, ShaderPass* shaderPass) override;

private:
    LightClusters m_lightClusters;
    //SceneGlobalRenderSettings	m_renderSettings;
    //FogParams					m_fogParams;
    Ref<ShadowCasterPass> m_shadowCasterPass;
    Ref<ForwardGBufferPrepass> m_depthPrepass;
    //Ref<LightOcclusionPass> m_lightOcclusionPass;
    Ref<ClusteredShadingGeometryRenderingPass> m_geometryPass;
};
#endif

} // namespace detail
} // namespace ln
