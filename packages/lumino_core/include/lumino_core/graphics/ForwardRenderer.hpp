#pragma once
#include <vector>
#include <lumino_base/Result.hpp>
#include <lumino_base/RefCounted.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>
#include <lumino_core/graphics/Camera.hpp>
#include <lumino_core/graphics/Material.hpp>
#include <lumino_core/graphics/Mesh.hpp>
#include <lumino_core/graphics/MeshLoader.hpp>
#include <lumino_core/graphics/Transform.hpp>
#include <lumino_core/graphics/GraphicsContext.hpp>

namespace ln {

/** Lighting parameters for the forward renderer. */
struct DirectionalLight {
    Vector3 direction = Vector3{0.0f, -1.0f, 0.5f};
    Color color = Color::white();
    Color ambient = Color{0.15f, 0.15f, 0.15f, 1.0f};
};

/**
 * A simple single-pass forward renderer.
 * Manages per-view and per-object UBOs, and drives the RHI command encoding.
 */
class ForwardRenderer : public RefCounted {
public:
    static Result<Ref<ForwardRenderer>> create(
        rhi::Device* device,
        rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8Unorm,
        rhi::TextureFormat depthFormat = rhi::TextureFormat::Depth32Float);

    /** Create from a GraphicsContext (uses its device and formats). */
    static Result<Ref<ForwardRenderer>> create(GraphicsContext* ctx);

    /** Shared PipelineLayout (3 sets: view, material, object). */
    rhi::PipelineLayout* pipelineLayout() const { return pipelineLayout_.get(); }

    /** Color format used by this renderer. */
    rhi::TextureFormat colorFormat() const { return colorFormat_; }

    /** Depth format used by this renderer. */
    rhi::TextureFormat depthFormat() const { return depthFormat_; }

    /** Set the directional light for the scene. */
    void setLight(const DirectionalLight& light) { light_ = light; }

    /**
     * Render a frame.
     * @param colorTarget  The color texture view to render into (from SwapChain).
     * @param depthTarget  The depth texture view to use. Must be created externally.
     * @param camera       The camera for this frame.
     * @param objects      The objects to render.
     * @param clearColor   Background clear color.
     */
    Result<void> renderFrame(
        rhi::Device* device,
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Camera& camera,
        const std::vector<RenderObject>& objects,
        const Color& clearColor = Color{0.1f, 0.1f, 0.1f, 1.0f});

private:
    rhi::TextureFormat colorFormat_;
    rhi::TextureFormat depthFormat_;

    // Shared layouts
    Ref<rhi::BindGroupLayout> viewBindGroupLayout_;
    Ref<rhi::BindGroupLayout> objectBindGroupLayout_;
    Ref<rhi::PipelineLayout> pipelineLayout_;

    // Per-view resources
    Ref<rhi::Buffer> viewUBO_;
    Ref<rhi::BindGroup> viewBindGroup_;

    // Reflected UBO sizes
    u64 viewUBOSize_ = 0;
    u64 objectUBOSize_ = 0;

    // Per-object resources (resized dynamically)
    std::vector<Ref<rhi::Buffer>> objectUBOs_;
    std::vector<Ref<rhi::BindGroup>> objectBindGroups_;

    DirectionalLight light_;

    Result<void> ensureObjectResources(rhi::Device* device, size_t count);
};

} // namespace ln
