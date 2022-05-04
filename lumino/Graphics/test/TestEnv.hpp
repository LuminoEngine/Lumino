#pragma once
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoGraphics/Rendering/FeatureRenderer/SpriteRenderer.hpp>
#include <LuminoGraphics/Rendering/FeatureRenderer/MeshRenderer.hpp>
#include <LuminoGraphics/Mesh/MeshModel.hpp>
#include <LuminoGraphics/Mesh/AnimationController.hpp>
#include <LuminoGraphics/Mesh/MeshModeEntity.hpp>
#include <LuminoGraphics/Testing/GraphicsTestHelper.hpp>

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
