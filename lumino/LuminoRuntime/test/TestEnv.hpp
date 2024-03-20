#pragma once
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/SpriteRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/MeshRenderer.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include <LuminoEngine/Mesh/MeshModeEntity.hpp>
#include <LuminoEngine/Testing/GraphicsTestHelper.hpp>

class TestRenderVew : public RenderView {
public:
    void resetView(const Vector3& pos = Vector3(0, 0, 5), const Vector3& lookAt = Vector3::Zero);
    RenderingContext* begin(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget) { return RenderView::beginRenderPipeline(commandList, renderTarget); }
    void end() { RenderView::endRenderPipeline(); }

protected:
    void onUpdateViewPoint(RenderViewPoint* viewPoint, RenderTargetTexture* renderTarget) override;
    void onRender(GraphicsCommandList* graphicsContext, RenderingContext* renderingContext, RenderTargetTexture* renderTarget) override;

private:
    Vector3 m_pos;
    Vector3 m_lookAt;
};

class TestEnv {
public:
    static void setup();
    static void teardown();
    static void initializeRendering();

    static bool isCI;
    static Ref<SwapChain> swapChain;
    static Ref<FlatRenderingPipeline> flatRenderingPipeline;
    static Ref<TestRenderVew> renderView;
};

#define RESET_VIEW() TestEnv::renderView->resetView()
