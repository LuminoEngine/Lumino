#pragma once
#include <memory>
#include <lumino_base/Result.hpp>
#include <lumino_base/RefObject.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>
#include <lumino_core/graphics/Material.hpp>
#include <lumino_core/graphics/Mesh.hpp>
#include <lumino_core/graphics/Transform.hpp>
#include <lumino_core/graphics/DynamicUniformAllocator.hpp>

namespace ln {

class GraphicsContext;

/**
 * Mid-level multi-pass renderer.
 *
 * Manages per-object dynamic UBO allocation, pipeline caching, and
 * RenderPass encoding. Clients can implement custom multi-pass rendering
 * strategies (G-Buffer, clustered lighting, post-effects, etc.) without
 * subclassing.
 *
 * Designed to be C-API friendly: all state is passed explicitly per call.
 *
 * The 3-set BindGroup convention used internally:
 *   - Set 0: per-pass data (view, scene) — supplied by the client via setPassBindGroup()
 *   - Set 1: per-material data            — taken from Material::materialBindGroup()
 *   - Set 2: per-object data (dynamic UBO) — managed internally by Renderer
 *
 * Usage:
 * @code
 *   renderer->beginFrame();
 *
 *   renderer->beginRenderPass(colorTarget, depthTarget, clearColor);
 *   renderer->setPassBindGroup(0, viewBindGroup);
 *   for (auto& obj : objects) {
 *       renderer->drawMesh(obj.mesh.get(), obj.transform);
 *   }
 *   renderer->endRenderPass();
 *
 *   renderer->beginRenderPass(screenTarget, nullptr, Color::black());
 *   renderer->setPassBindGroup(0, postBindGroup);
 *   renderer->drawScreenRect(postMaterial.get());
 *   renderer->endRenderPass();
 *
 *   renderer->endFrame();
 * @endcode
 */
class Renderer : public RefObject {
public:
    /** Create from a GraphicsContext (uses its device, formats, and pipeline cache). */
    static Result<Ref<Renderer>> create(GraphicsContext* ctx);

    /** Create with an explicit device and formats. ctx may be nullptr if using beginFrame(cmd) explicitly. */
    static Result<Ref<Renderer>> create(
        rhi::Device* device,
        rhi::TextureFormat colorFormat,
        rhi::TextureFormat depthFormat);

    // ---- Layout accessors (for external BindGroup creation) ----

    /** Shared PipelineLayout (3 sets: view, material, object). */
    rhi::PipelineLayout* pipelineLayout() const { return m_pipelineLayout.get(); }

    /**
     * BindGroupLayout for set=0 (view/scene data).
     * Use this to create BindGroups for setPassBindGroup(0, ...).
     */
    rhi::BindGroupLayout* viewBindGroupLayout() const { return m_viewBindGroupLayout.get(); }

    /** BindGroupLayout for set=2 (per-object dynamic UBO). */
    rhi::BindGroupLayout* objectBindGroupLayout() const { return m_objectBindGroupLayout.get(); }

    /** Color format this renderer was created with. */
    rhi::TextureFormat colorFormat() const { return m_colorFormat; }

    /** Depth format this renderer was created with. */
    rhi::TextureFormat depthFormat() const { return m_depthFormat; }

    // ---- Frame lifecycle ----

    /**
     * Begin a new frame. Resets per-frame allocators and acquires a command buffer.
     * Must be called before any beginRenderPass().
     */
    void beginFrame();

    /**
     * End the current frame and submit all recorded commands to the GPU.
     * Must be called after the last endRenderPass().
     */
    void endFrame();

    // ---- Pass lifecycle ----

    /**
     * Begin a render pass targeting the given attachments.
     * @param colorTarget  Color attachment (required).
     * @param depthTarget  Depth attachment (nullptr to skip depth, e.g. for post-effects).
     * @param clearColor   Clear color applied to the color attachment.
     */
    void beginRenderPass(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Color& clearColor = Color{0, 0, 0, 1});

    /** End the current render pass. */
    void endRenderPass();

    /**
     * Bind a BindGroup to the given descriptor set for the current pass.
     * Changes take effect lazily — they are flushed to the GPU after the
     * next setPipeline call (inside drawMesh / drawScreenRect).
     *
     * Typical use: setPassBindGroup(0, viewBindGroup) once per pass.
     *
     * @param setIndex   Descriptor set index (0, 1, or 2).
     * @param bindGroup  BindGroup to bind. Pass nullptr to unbind.
     */
    void setPassBindGroup(u32 setIndex, rhi::BindGroup* bindGroup);

    // ---- Drawing ----

    /**
     * Draw a mesh with the given world transform.
     * Per-submesh: resolves the material from the mesh, selects or creates a
     * cached pipeline, allocates object UBO, and records the draw call.
     */
    Result<void> drawMesh(Mesh* mesh, const Transform& transform);

    /**
     * Draw a mesh using an explicit material for all submeshes.
     * The mesh's own materials are ignored.
     */
    Result<void> drawMesh(Mesh* mesh, const Transform& transform, Material* material);

    /**
     * Draw a fullscreen rectangle using the given material.
     * Intended for post-processing passes. The quad covers NDC [-1,1]x[-1,1]
     * with UV (0,0) at top-left and (1,1) at bottom-right.
     */
    Result<void> drawScreenRect(Material* material);

private:
    Renderer() = default;

    rhi::TextureFormat m_colorFormat = {};
    rhi::TextureFormat m_depthFormat = {};

    // Layouts
    Ref<rhi::BindGroupLayout> m_viewBindGroupLayout;
    Ref<rhi::BindGroupLayout> m_objectBindGroupLayout;
    Ref<rhi::PipelineLayout>  m_pipelineLayout;

    // Object UBO size (from shader reflection)
    u64 m_objectUBOSize = 0;

    // Per-frame dynamic UBO allocator for per-object data
    std::unique_ptr<DynamicUniformAllocator> m_objectAllocator;

    // Fullscreen quad mesh (lazily created on first drawScreenRect call)
    Ref<Mesh> m_screenRectMesh;

    // Context (non-owning; provides device + pipelineCache)
    GraphicsContext* m_ctx = nullptr;

    u32 m_frameCounter = 0;

    // Per-frame command encoding state (valid between beginFrame / endFrame)
    rhi::CommandBuffer*     m_currentCmd  = nullptr;
    rhi::RenderPassEncoder* m_currentPass = nullptr;

    // Deferred per-pass bind groups (set via setPassBindGroup, flushed after setPipeline)
    static constexpr u32 kMaxBindGroupSets = 4;
    rhi::BindGroup* m_passBindGroups[kMaxBindGroupSets] = {};
    bool            m_passBindGroupDirty[kMaxBindGroupSets] = {};

    // Flush all dirty pass BindGroups to the current RenderPassEncoder.
    // Called inside drawSubmesh after setPipeline.
    void flushPassBindGroups();

    Result<void> drawSubmesh(Mesh* mesh, Material* mat, const Transform& transform, const SubMesh& sub);
    Result<Mesh*> getScreenRectMesh();
};

} // namespace ln
