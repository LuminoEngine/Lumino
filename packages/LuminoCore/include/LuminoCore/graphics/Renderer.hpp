#pragma once
#include <memory>
#include <array>
#include <unordered_map>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/graphics/rhi/Rhi.hpp>
#include <LuminoCore/graphics/Camera.hpp>
#include <LuminoCore/graphics/Material.hpp>
#include <LuminoCore/graphics/Mesh.hpp>
#include <LuminoCore/graphics/Transform.hpp>
#include <LuminoCore/graphics/DynamicUniformAllocator.hpp>

namespace ln {

class GraphicsContext;

/**
 * 中級マルチパスレンダラー。
 *
 * オブジェクトごとの動的UBO割り当て、パイプラインキャッシュ、および
 * RenderPassエンコーディングを管理します。クライアントは、サブクラス化することなく、
 * カスタムマルチパスレンダリング戦略（Gバッファ、クラスタ化ライティング、ポストエフェクトなど）を実装できます。
 *
 * C-APIフレンドリーな設計：すべての状態は呼び出しごとに明示的に渡されます。
 *
 * 内部で使用される4セットBindGroup規約：
 * - セット0：視点データ（カメラ行列）— beginRenderPass(camera) で自動設定、または setPassBindGroup(0) で上書き可
 * - セット1：シーン環境データ（ライティング等）— setPassBindGroup(1) を介してクライアントから提供されます
 * - セット2：マテリアルごとのデータ— Material::materialBindGroup() から取得されます
 * - セット3：オブジェクトごとのデータ（動的UBO）— Rendererによって内部的に管理されます
 *
 * Usage:
 * @code
 *   renderer->beginFrame();
 *
 *   // 3Dパス: カメラを渡すと set=0 (ViewUBO) が自動設定される
 *   renderer->beginRenderPass(colorTarget, depthTarget, camera, clearColor);
 *   renderer->setPassBindGroup(1, sceneBindGroup);  // ライティング等
 *   for (auto& obj : objects) {
 *       renderer->drawMesh(obj.mesh.get(), obj.transform);
 *   }
 *   renderer->endRenderPass();
 *
 *   // ポストエフェクトパス: カメラ不要
 *   renderer->beginRenderPass(screenTarget, nullptr, Color::black());
 *   renderer->drawScreenRect(postMaterial.get());
 *   renderer->endRenderPass();
 *
 *   renderer->endFrame();
 * @endcode
 */
class Renderer : public Object {
public:

    // ---- Layout accessors (for external BindGroup creation) ----

    /** Shared PipelineLayout (4 sets: view, scene, material, object). */
    rhi::PipelineLayout* pipelineLayout() const { return m_pipelineLayout.get(); }

    /** BindGroupLayout for set=0 (camera/view data). */
    rhi::BindGroupLayout* viewBindGroupLayout() const { return m_viewBindGroupLayout.get(); }

    /**
     * BindGroupLayout for set=1 (scene environment data: lighting, etc.).
     * Use this to create BindGroups for setPassBindGroup(1, ...).
     */
    rhi::BindGroupLayout* sceneBindGroupLayout() const { return m_sceneBindGroupLayout.get(); }

    /** BindGroupLayout for set=3 (per-object dynamic UBO). */
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

    /** Current in-flight frame slot (0 or 1). Valid after beginFrame(). */
    u32 currentFrameSlot() const { return m_currentFrameSlot; }

    // ---- Pass lifecycle ----

    /**
     * Begin a render pass targeting the given attachments.
     * Camera data is uploaded to the internal View UBO and set=0 is bound automatically.
     * @param colorTarget  Color attachment (required).
     * @param depthTarget  Depth attachment (nullptr to skip depth, e.g. for post-effects).
     * @param camera       Camera to upload to set=0 View UBO.
     * @param clearColor   Clear color applied to the color attachment.
     */
    void beginRenderPass(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Camera& camera,
        const Color& clearColor = Color{0, 0, 0, 1});

    /**
     * Begin a render pass without a camera (e.g. post-processing passes).
     * set=0 is not automatically bound; use setPassBindGroup(0, ...) if needed.
     */
    void beginRenderPass(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Color& clearColor = Color{0, 0, 0, 1});

    /** End the current render pass. */
    void endRenderPass();

    /**
     * Begin an overlay render pass without clearing the color target (LoadOp::Load).
     * Intended for HUD / debug overlays drawn on top of an already-rendered scene.
     * No camera is bound; positions should be in NDC.
     */
    void beginOverlayRenderPass(rhi::TextureView* colorTarget);

    /**
     * Bind a BindGroup to the given descriptor set for the current pass.
     * Changes take effect lazily — they are flushed to the GPU after the
     * next setPipeline call (inside drawMesh / drawScreenRect).
     *
     * Typical use: setPassBindGroup(0, viewBindGroup) once per pass.
     *
     * @param setIndex        Descriptor set index (0–3).
     * @param bindGroup       BindGroup to bind. Pass nullptr to unbind.
     * @param dynamicOffset   Dynamic offset for UBO bindings (0 if none).
     * @param dynamicOffsetCount Number of dynamic offsets (0 or 1).
     */
    void setPassBindGroup(u32 setIndex, rhi::BindGroup* bindGroup,
                          u32 dynamicOffset = 0, u32 dynamicOffsetCount = 0);

    // ---- Statistics ----

    /** Draw call count for the current frame. Reset each beginFrame(). */
    u32 drawCallCount() const { return m_drawCallCount; }

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
    friend class GraphicsContext;

    static Result<Ref<Renderer>> create(GraphicsContext* ctx);

    Renderer() = default;

    rhi::TextureFormat m_colorFormat = {};
    rhi::TextureFormat m_depthFormat = {};

    // Layouts
    Ref<rhi::BindGroupLayout> m_viewBindGroupLayout;
    Ref<rhi::BindGroupLayout> m_sceneBindGroupLayout;
    Ref<rhi::BindGroupLayout> m_objectBindGroupLayout;
    Ref<rhi::PipelineLayout>  m_pipelineLayout;

    // Per-frame view UBO allocator (camera data, set=0) — double-buffered via DynamicUniformAllocator
    std::unique_ptr<DynamicUniformAllocator> m_viewAllocator;

    // Object UBO size (from shader reflection)
    u64 m_objectUBOSize = 0;

    // Per-frame dynamic UBO allocator for per-object data
    std::unique_ptr<DynamicUniformAllocator> m_objectAllocator;

    // Fullscreen quad mesh (lazily created on first drawScreenRect call)
    Ref<Mesh> m_screenRectMesh;

    // Context (non-owning; provides device + pipelineCache)
    GraphicsContext* m_ctx = nullptr;

    u32 m_frameCounter = 0;
    u32 m_currentFrameSlot = 0;
    u32 m_drawCallCount = 0;

    // Per-frame command encoding state (valid between beginFrame / endFrame)
    rhi::CommandBuffer*     m_currentCmd  = nullptr;
    rhi::RenderPassEncoder* m_currentPass = nullptr;

    // Deferred per-pass bind groups (set via setPassBindGroup, flushed after setPipeline)
    static constexpr u32 kMaxBindGroupSets = 4;
    rhi::BindGroup* m_passBindGroups[kMaxBindGroupSets] = {};
    u32             m_passBindGroupDynamicOffsets[kMaxBindGroupSets] = {};
    u32             m_passBindGroupDynamicOffsetCounts[kMaxBindGroupSets] = {};
    bool            m_passBindGroupDirty[kMaxBindGroupSets] = {};

    // Flush all dirty pass BindGroups to the current RenderPassEncoder.
    // Called inside drawSubmesh after setPipeline.
    void flushPassBindGroups();

    Result<void> drawSubmesh(Mesh* mesh, Material* mat, const Transform& transform, const SubMesh& sub);
    Result<Mesh*> getScreenRectMesh();

    // ---- Per-material BindGroup cache (Renderer-owned) ----

    static constexpr u32 kMaxFramesInFlight = 2;

    /** Cached GPU resources for a single Material, double-buffered per in-flight frame. */
    struct CachedMaterialBind {
        uint64_t paramVersion = 0;
        Ref<rhi::TextureView> textureView;
        Ref<rhi::Sampler>     sampler;
        std::array<Ref<rhi::Buffer>,    kMaxFramesInFlight> paramBuffers;
        std::array<Ref<rhi::BindGroup>, kMaxFramesInFlight> bindGroups;
        std::array<bool, kMaxFramesInFlight> dirty = {true, true};
    };

    std::unordered_map<Material*, CachedMaterialBind> m_materialCache;

    /** Get or create the BindGroup for the given material and current frame slot. */
    Result<rhi::BindGroup*> getOrCreateMaterialBindGroup(Material* mat);
};

} // namespace ln
