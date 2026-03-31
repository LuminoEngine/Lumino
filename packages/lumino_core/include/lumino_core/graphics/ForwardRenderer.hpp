#pragma once
#include <memory>
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
#include <lumino_core/graphics/DynamicUniformAllocator.hpp>

namespace ln {

/** Lighting parameters for the forward renderer. */
struct DirectionalLight {
    Vector3 direction = Vector3{0.0f, -1.0f, 0.5f};
    Color color = Color::white();
    Color ambient = Color{0.15f, 0.15f, 0.15f, 1.0f};
};

/**
 * A flexible multi-pass renderer.
 *
 * Manages per-object dynamic UBO allocation and pipeline caching,
 * while exposing a mid-level API for clients to implement custom
 * multi-pass rendering (e.g. G-Buffer, clustered lighting, post-effects).
 *
 * The API is designed to be C-API friendly (no inheritance required).
 *
 * Usage:
 * @code
 *   renderer->beginFrame();
 *
 *   // Pass 1: Scene
 *   renderer->beginRenderPass(colorTarget, depthTarget, clearColor);
 *   renderer->setPassBindGroup(0, viewBindGroup);
 *   for (auto& obj : objects) {
 *       renderer->drawMesh(obj.mesh.get(), obj.transform);
 *   }
 *   renderer->endRenderPass();
 *
 *   // Pass 2: Post-effect
 *   renderer->beginRenderPass(screenTarget, nullptr, Color::black());
 *   renderer->setPassBindGroup(0, postEffectBindGroup);
 *   renderer->drawScreenRect(postEffectMaterial.get());
 *   renderer->endRenderPass();
 *
 *   renderer->endFrame();
 * @endcode
 */
class ForwardRenderer : public RefObject {
public:
    static Result<Ref<ForwardRenderer>> create(
        rhi::Device* device,
        rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8Unorm,
        rhi::TextureFormat depthFormat = rhi::TextureFormat::Depth32Float);

    /** Create from a GraphicsContext (uses its device and formats). */
    static Result<Ref<ForwardRenderer>> create(GraphicsContext* ctx);

    // ---- Layout accessors (for client BindGroup creation) ----

    /** Shared PipelineLayout (3 sets: view, material, object). */
    rhi::PipelineLayout* pipelineLayout() const { return m_pipelineLayout.get(); }

    /** BindGroupLayout for set=0 (view/scene data). */
    rhi::BindGroupLayout* viewBindGroupLayout() const { return m_viewBindGroupLayout.get(); }

    /** BindGroupLayout for set=2 (per-object dynamic UBO). */
    rhi::BindGroupLayout* objectBindGroupLayout() const { return m_objectBindGroupLayout.get(); }

    /** Color format used by this renderer. */
    rhi::TextureFormat colorFormat() const { return m_colorFormat; }

    /** Depth format used by this renderer. */
    rhi::TextureFormat depthFormat() const { return m_depthFormat; }

    // ---- High-level convenience (existing API) ----

    /** Set the directional light for the scene. */
    void setLight(const DirectionalLight& light) { m_light = light; }

    /**
     * Render a single-pass frame (convenience wrapper over the multi-pass API).
     * Internally calls beginFrame/beginRenderPass/drawMesh/endRenderPass/endFrame.
     */
    Result<void> renderFrame(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Camera& camera,
        const std::vector<RenderObject>& objects,
        const Color& clearColor = Color{0.1f, 0.1f, 0.1f, 1.0f});

    // ---- Multi-pass API ----

    /**
     * Begin a new frame. Resets per-frame allocators and acquires a command buffer.
     * Must be called before any beginRenderPass().
     */
    void beginFrame();

    /**
     * End the current frame and submit commands to the GPU.
     * Must be called after the last endRenderPass().
     */
    void endFrame();

    /**
     * Begin a render pass with the given targets.
     * @param colorTarget  Color attachment (required).
     * @param depthTarget  Depth attachment (nullptr to skip depth).
     * @param clearColor   Clear color for the color attachment.
     */
    void beginRenderPass(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Color& clearColor = Color{0, 0, 0, 1});

    /** End the current render pass. */
    void endRenderPass();

    /**
     * Bind a BindGroup to the given set index for the current render pass.
     * Typically used for set=0 (view/scene data) which varies per pass.
     * @param setIndex   Descriptor set index (0, 1, or 2).
     * @param bindGroup  The BindGroup to bind.
     */
    void setPassBindGroup(u32 setIndex, rhi::BindGroup* bindGroup);

    /**
     * Draw a mesh with the given transform.
     * Internally handles per-object UBO allocation, material resolution, and pipeline selection.
     * @param mesh       The mesh to draw.
     * @param transform  World transform for this draw call.
     */
    Result<void> drawMesh(Mesh* mesh, const Transform& transform);

    /**
     * Draw a mesh with an explicit material override.
     * All submeshes will use the provided material instead of the mesh's own materials.
     * @param mesh       The mesh to draw.
     * @param transform  World transform for this draw call.
     * @param material   Material to use for all submeshes.
     */
    Result<void> drawMesh(Mesh* mesh, const Transform& transform, Material* material);

    /**
     * Draw a fullscreen rectangle using the given material.
     * Useful for post-processing effects.
     * @param material  Material (shader + parameters) for the screen rect.
     */
    Result<void> drawScreenRect(Material* material);

private:
    rhi::TextureFormat m_colorFormat;
    rhi::TextureFormat m_depthFormat;

    // Shared layouts
    Ref<rhi::BindGroupLayout> m_viewBindGroupLayout;
    Ref<rhi::BindGroupLayout> m_objectBindGroupLayout;
    Ref<rhi::PipelineLayout> m_pipelineLayout;

    // Per-view resources (used by renderFrame convenience method)
    Ref<rhi::Buffer> m_viewUBO;
    Ref<rhi::BindGroup> m_viewBindGroup;

    // Reflected UBO sizes
    u64 m_viewUBOSize = 0;
    u64 m_objectUBOSize = 0;

    // Dynamic UBO allocator for per-object data.
    std::unique_ptr<DynamicUniformAllocator> m_objectAllocator;

    // Fullscreen quad mesh (lazily created)
    Ref<Mesh> m_screenRectMesh;

    DirectionalLight m_light;

    GraphicsContext* m_ctx = nullptr;
    u32 m_frameCounter = 0;

    // Per-frame state
    rhi::CommandBuffer* m_currentCmd = nullptr;
    rhi::RenderPassEncoder* m_currentPass = nullptr;

    // Deferred per-pass bind groups (set via setPassBindGroup, flushed in drawSubmesh after setPipeline).
    static constexpr u32 kMaxBindGroupSets = 4;
    rhi::BindGroup* m_passBindGroups[kMaxBindGroupSets] = {};
    bool m_passBindGroupDirty[kMaxBindGroupSets] = {};

    void flushPassBindGroups();
    Result<void> drawSubmesh(Mesh* mesh, Material* mat, const Transform& transform, const SubMesh& sub);
    Result<Mesh*> getScreenRectMesh();
};

} // namespace ln
