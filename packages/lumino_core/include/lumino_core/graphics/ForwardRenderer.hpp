#pragma once
#include <vector>
#include <lumino_base/Result.hpp>
#include <lumino_base/RefObject.hpp>
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
class ForwardRenderer : public RefObject {
public:
    static Result<Ref<ForwardRenderer>> create(
        rhi::Device* device,
        rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8Unorm,
        rhi::TextureFormat depthFormat = rhi::TextureFormat::Depth32Float);

    /** Create from a GraphicsContext (uses its device and formats). */
    static Result<Ref<ForwardRenderer>> create(GraphicsContext* ctx);

    /** Shared PipelineLayout (3 sets: view, material, object). */
    rhi::PipelineLayout* pipelineLayout() const { return m_pipelineLayout.get(); }

    /** Color format used by this renderer. */
    rhi::TextureFormat colorFormat() const { return m_colorFormat; }

    /** Depth format used by this renderer. */
    rhi::TextureFormat depthFormat() const { return m_depthFormat; }

    /** Set the directional light for the scene. */
    void setLight(const DirectionalLight& light) { m_light = light; }

    /**
     * Render a frame.
     * @param pipelineCache Pipeline cache for looking up / creating RenderPipelines.
     * @param colorTarget  The color texture view to render into (from SwapChain).
     * @param depthTarget  The depth texture view to use. Must be created externally.
     * @param camera       The camera for this frame.
     * @param objects      The objects to render.
     * @param clearColor   Background clear color.
     */
    Result<void> renderFrame(
        rhi::Device* device,
        PipelineCache* pipelineCache,
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Camera& camera,
        const std::vector<RenderObject>& objects,
        const Color& clearColor = Color{0.1f, 0.1f, 0.1f, 1.0f});

private:
    rhi::TextureFormat m_colorFormat;
    rhi::TextureFormat m_depthFormat;

    // Shared layouts
    Ref<rhi::BindGroupLayout> m_viewBindGroupLayout;
    Ref<rhi::BindGroupLayout> m_objectBindGroupLayout;
    Ref<rhi::PipelineLayout> m_pipelineLayout;

    // Per-view resources
    Ref<rhi::Buffer> m_viewUBO;
    Ref<rhi::BindGroup> m_viewBindGroup;

    // Reflected UBO sizes
    u64 m_viewUBOSize = 0;
    u64 m_objectUBOSize = 0;

    // Per-object resources (resized dynamically)
    std::vector<Ref<rhi::Buffer>> m_objectUBOs;
    std::vector<Ref<rhi::BindGroup>> m_objectBindGroups;

    DirectionalLight m_light;

    Result<void> ensureObjectResources(rhi::Device* device, size_t count);
};

} // namespace ln
