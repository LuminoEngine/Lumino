#pragma once
#include <memory>
#include <array>
#include <unordered_map>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoCore/Graphics/Camera.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/Transform.hpp>
#include <LuminoCore/Graphics/DynamicUniformAllocator.hpp>
#include <LuminoCore/Graphics/Batch.hpp>

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
 * 内部で使用されるBindGroup規約（セットインデックスはシェーダリフレクションから決定）：
 * - $Material セット：マテリアルごとのデータ- Material から取得されます
 * - View セット：視点データ（カメラ行列）- beginRenderPass(camera) で自動設定
 * - Scene セット：シーン環境データ（ライティング等）- setPassBindGroup() を介してクライアントから提供
 * - Object セット：オブジェクトごとのデータ（動的UBO）- Rendererによって内部的に管理
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
     * @param presentTarget  If non-null, a barrier is recorded to transition
     *                       the image from COLOR_ATTACHMENT_OPTIMAL to PRESENT_SRC_KHR.
     */
    void endFrame();

    /** Current in-flight frame slot. Valid after beginFrame(). */
    uint32_t currentFrameSlot() const { return m_currentFrameSlot; }

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

    /**
     * Begin a render pass with a fully specified RenderPassDesc.
     * Camera data is uploaded to the internal View UBO and set=0 is bound automatically.
     * @param rpDesc  RHI render pass descriptor.
     * @param camera  Camera to upload to set=0 View UBO.
     */
    void beginRenderPass(const rhi::RenderPassDesc& rpDesc, const Camera& camera);

    /**
     * Begin a render pass with a fully specified RenderPassDesc, without a camera.
     * set=0 is not automatically bound; use setPassBindGroup(0, ...) if needed.
     * @param rpDesc  RHI render pass descriptor.
     */
    void beginRenderPass(const rhi::RenderPassDesc& rpDesc);

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
     * Changes take effect lazily - they are flushed to the GPU after the
     * next setPipeline call (inside drawMesh / drawScreenRect).
     *
     * Typical use: setPassBindGroup(0, viewBindGroup) once per pass.
     *
     * @param setIndex        Descriptor set index (0-3).
     * @param bindGroup       BindGroup to bind. Pass nullptr to unbind.
     * @param dynamicOffset   Dynamic offset for UBO bindings (0 if none).
     * @param dynamicOffsetCount Number of dynamic offsets (0 or 1).
     */
    void setPassBindGroup(uint32_t setIndex, rhi::BindGroup* bindGroup,
                          uint32_t dynamicOffset = 0, uint32_t dynamicOffsetCount = 0);

    // ---- Statistics ----

    /** Draw call count for the current frame. Reset each beginFrame(). */
    uint32_t drawCallCount() const { return m_drawCallCount; }

    // ---- Drawing (batched) ----

    /**
     * メッシュ描画コマンドを内部コマンドバッファに蓄積します。
     * 蓄積されたコマンドは endRenderPass() 時に自動的にソート→バッチ化→描画されます。
     * @param mesh      メッシュ
     * @param transform ワールドトランスフォーム
     * @param zIndex    ソート優先度 (デフォルト 0)
     */
    void drawMesh(Mesh* mesh, const Transform& transform, int32_t zIndex = 0);

    /**
     * スプライト描画コマンドを内部コマンドバッファに蓄積します。
     * 蓄積されたコマンドは endRenderPass() 時に自動的にソート→バッチ化→描画されます。
     */
    void drawSprite(Material* material, int32_t zIndex,
                    const Vector3& pos, const Vector2& size,
                    const Vector2& uvOffset, const Vector2& uvSize,
                    const Color& color, float rotation = 0.0f);

    // ---- Drawing (immediate) ----

    /**
     * メッシュを即座に描画します (バッチ化なし)。
     * レンダーパス内で即時に GPU コマンドを発行します。
     */
    Result<void> drawMeshImmediate(Mesh* mesh, const Transform& transform);

    /**
     * すべてのサブメッシュに明示的なマテリアルを使用してメッシュを即座に描画します。
     * 主にポストエフェクト用の特殊用途です。
     */
    Result<void> drawMeshImmediate(Mesh* mesh, const Transform& transform, Material* material);

    /**
     * Draw a single submesh with an explicit material and transform.
     * Used by BatchProcessor for per-submesh drawing.
     */
    Result<void> drawSingleSubMesh(Mesh* mesh, uint32_t submeshIndex,
                                   Material* material, const Transform& transform);

    /**
     * Draw a fullscreen rectangle using the given material.
     * Intended for post-processing passes. The quad covers NDC [-1,1]x[-1,1]
     * with UV (0,0) at top-left and (1,1) at bottom-right.
     */
    Result<void> drawScreenRect(Material* material);

    // ---- Stencil Mask ----

    /**
     * ステンシルマスクをプッシュします。
     * maskMesh をステンシルバッファにのみ描画し（カラー書き込みなし）、
     * 以降の drawMesh はマスク領域内のみ描画されます。
     * ネスト可能（内部でステンシル参照値をインクリメント）。
     * @param mesh       マスク形状のメッシュ（アルファマスクテクスチャ付き）
     * @param transform  メッシュのトランスフォーム
     * @param material   マスク描画に使うマテリアル（テクスチャ参照のため）
     */
    Result<void> pushStencilMask(Mesh* mesh, const Transform& transform, Material* material);

    /**
     * 直前の pushStencilMask に対応するマスクを解除します。
     * 内部でマスクメッシュを再描画してステンシル値をデクリメントします。
     */
    Result<void> popStencilMask();

private:
    friend class GraphicsContext;

    static Result<Ref<Renderer>> create(GraphicsContext* ctx);

    Renderer() = default;

    rhi::TextureFormat m_colorFormat = {};
    rhi::TextureFormat m_depthFormat = {};
    // Reference PipelineLayout (from a builtin ShaderPass, used for dynamic UBO allocators)
    rhi::PipelineLayout* m_referencePipelineLayout = nullptr;
    int16_t m_viewSetIndex = -1;
    int16_t m_sceneSetIndex = -1;
    int16_t m_objectSetIndex = -1;
    uint64_t m_objectUBOSize = 0;

    // Per-frame view UBO allocator (camera data) - double-buffered via DynamicUniformAllocator
    std::unique_ptr<DynamicUniformAllocator> m_viewAllocator;

    // Per-frame dynamic UBO allocator for scene-level data (lighting etc.)
    std::unique_ptr<DynamicUniformAllocator> m_sceneAllocator;

    // Per-frame dynamic UBO allocator for per-object data
    std::unique_ptr<DynamicUniformAllocator> m_objectAllocator;

    // Fullscreen quad mesh (lazily created on first drawScreenRect call)
    Ref<Mesh> m_screenRectMesh;

    // Context (non-owning; provides device + pipelineCache)
    GraphicsContext* m_ctx = nullptr;

    uint32_t m_frameCounter = 0;
    uint32_t m_currentFrameSlot = 0;
    uint32_t m_framesInFlight = 2;
    uint32_t m_drawCallCount = 0;

    // Per-frame command encoding state (valid between beginFrame / endFrame)
    rhi::CommandBuffer*     m_currentCmd  = nullptr;
    rhi::RenderPass* m_currentPass = nullptr;
    rhi::TextureView*       m_currentColorTarget = nullptr;

    // Deferred per-pass bind groups (set via setPassBindGroup, flushed after setPipeline)
    static constexpr uint32_t kMaxBindGroupSets = 4;
    rhi::BindGroup* m_passBindGroups[kMaxBindGroupSets] = {};
    uint32_t             m_passBindGroupDynamicOffsets[kMaxBindGroupSets] = {};
    uint32_t             m_passBindGroupDynamicOffsetCounts[kMaxBindGroupSets] = {};
    bool            m_passBindGroupDirty[kMaxBindGroupSets] = {};

    // Flush all dirty pass BindGroups to the current RenderPass.
    // Called inside drawSubmesh after setPipeline.
    void flushPassBindGroups();

    Result<void> drawSubmesh(Mesh* mesh, Material* mat, const Transform& transform, const SubMesh& sub);
    Result<Mesh*> getScreenRectMesh();

    // ---- Stencil mask internal helpers ----

    /** Draw a mesh for the purpose of stencil write only (no color output). */
    Result<void> drawStencilMaskMesh(Mesh* mesh, const Transform& transform, Material* material,
                                     rhi::CompareFunction compare, uint32_t stencilRef, rhi::StencilOp passOp);

    struct StencilMaskEntry {
        Mesh* mesh;
        Transform transform;
        Material* material;
    };
    std::vector<StencilMaskEntry> m_stencilMaskStack;
    uint32_t m_stencilRef = 0;

    // ---- Batch rendering (internal) ----

    DrawCommandBuffer m_commandBuffer;
    std::unique_ptr<BatchProcessor> m_batchProcessor;

    /** Flush batched commands and clear the command buffer. */
    Result<void> flushBatch();

    // ---- Per-material BindGroup cache (Renderer-owned) ----

    /** Cached GPU resources for a single Material, double-buffered per in-flight frame. */
    struct CachedMaterialBind {
        uint64_t paramVersion = 0;
        // Per-binding texture tracking (binding index -> last texture pointer)
        std::unordered_map<uint32_t, rhi::Texture*> lastTextures;
        std::unordered_map<uint32_t, Ref<rhi::TextureView>> textureViews;
        Ref<rhi::Sampler>     sampler;
        std::vector<Ref<rhi::Buffer>>    paramBuffers;
        std::vector<Ref<rhi::BindGroup>> bindGroups;
        std::vector<bool> dirty;
    };

    std::unordered_map<Material*, CachedMaterialBind> m_materialCache;

    /** Get or create the BindGroup for the given material and current frame slot. */
    Result<rhi::BindGroup*> getOrCreateMaterialBindGroup(Material* mat);

};

} // namespace ln
