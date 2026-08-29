#pragma once
#include <memory>
#include <array>
#include <unordered_map>
#include <unordered_set>
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
 * 使用例:
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

    /** このレンダラーの作成時に指定されたカラーフォーマット。 */
    rhi::TextureFormat colorFormat() const { return m_colorFormat; }

    /** このレンダラーの作成時に指定されたデプスフォーマット。 */
    rhi::TextureFormat depthFormat() const { return m_depthFormat; }

    // ---- フレームのライフサイクル ----

    /**
     * 新しいフレームを開始します。フレームごとのアロケータをリセットし、コマンドバッファを取得します。
     * beginRenderPass() より前に呼び出す必要があります。
     */
    void beginFrame();

    /**
     * 現在のフレームを終了し、記録したすべてのコマンドを GPU へ送信します。
     * 最後の endRenderPass() の後に呼び出す必要があります。
     * @param presentTarget  非 null の場合、イメージを COLOR_ATTACHMENT_OPTIMAL から
     *                       PRESENT_SRC_KHR へ遷移させるバリアを記録します。
     */
    void endFrame();

    /** 現在のインフライトフレームスロット。beginFrame() の後に有効です。 */
    uint32_t currentFrameSlot() const { return m_currentFrameSlot; }

    /** マテリアル BindGroup キャッシュのエントリ数。リークの検出に使います。 */
    size_t materialCacheSize() const { return m_materialCache.size(); }

    // ---- パスのライフサイクル ----

    /**
     * 指定したアタッチメントを対象とするレンダーパスを開始します。
     * カメラデータは内部の View UBO にアップロードされ、set=0 が自動的にバインドされます。
     * @param colorTarget  カラーアタッチメント (必須)。
     * @param depthTarget  デプスアタッチメント (ポストエフェクトなどでデプスを使わない場合は nullptr)。
     * @param camera       set=0 の View UBO にアップロードするカメラ。
     * @param clearColor   カラーアタッチメントに適用するクリアカラー。
     */
    void beginRenderPass(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Camera& camera,
        const Color& clearColor = Color{0, 0, 0, 1},
        SortMode sortMode = SortMode::Stable);

    /**
     * カメラ無しでレンダーパスを開始します (ポストプロセスパスなど)。
     * set=0 は自動的にはバインドされません。必要なら setPassBindGroup(0, ...) を使ってください。
     */
    void beginRenderPass(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Color& clearColor = Color{0, 0, 0, 1});

    /**
     * 完全に指定された RenderPassDesc でレンダーパスを開始します。
     * カメラデータは内部の View UBO にアップロードされ、set=0 が自動的にバインドされます。
     * @param rpDesc          RHI のレンダーパス記述子。
     * @param camera          set=0 の View UBO にアップロードするカメラ。
     * @param shaderPassName  描画時にマテリアルから優先的に選択する ShaderPass 名。
     *                        空文字列の場合 "Forward" が使用されます。
     *                        マテリアルがこの名前のパスを持たない場合、その描画はスキップされます。
     */
    void beginRenderPass(const rhi::RenderPassDesc& rpDesc, const Camera& camera,
                         const std::string& shaderPassName = {},
                         SortMode sortMode = SortMode::Stable);

    /**
     * 完全に指定された RenderPassDesc で、カメラ無しのレンダーパスを開始します。
     * set=0 は自動的にはバインドされません。必要なら setPassBindGroup(0, ...) を使ってください。
     * @param rpDesc          RHI のレンダーパス記述子。
     * @param shaderPassName  描画時にマテリアルから優先的に選択する ShaderPass 名 (空 = "Forward")。
     */
    void beginRenderPass(const rhi::RenderPassDesc& rpDesc,
                         const std::string& shaderPassName = {});

    /** 現在のレンダーパスを終了します。 */
    void endRenderPass();

    /**
     * カラーターゲットをクリアせずに (LoadOp::Load) オーバーレイ用のレンダーパスを開始します。
     * 描画済みのシーンの上に重ねる HUD やデバッグ表示を想定しています。
     * カメラはバインドされないため、位置は NDC で指定してください。
     */
    void beginOverlayRenderPass(rhi::TextureView* colorTarget);

    /**
     * 現在のパスに対して、指定したディスクリプタセットに BindGroup をバインドします。
     * 変更は遅延して反映されます。次の setPipeline 呼び出し
     * (drawMesh / drawScreenRect の内部) の後に GPU へフラッシュされます。
     *
     * 典型的な使い方: パスごとに一度 setPassBindGroup(0, viewBindGroup) を呼びます。
     *
     * @param setIndex        ディスクリプタセットのインデックス (0-3)。
     * @param bindGroup       バインドする BindGroup。nullptr を渡すとバインドを解除します。
     * @param dynamicOffset   UBO バインディングの動的オフセット (無ければ 0)。
     * @param dynamicOffsetCount 動的オフセットの数 (0 または 1)。
     */
    void setPassBindGroup(uint32_t setIndex, rhi::BindGroup* bindGroup,
                          uint32_t dynamicOffset = 0, uint32_t dynamicOffsetCount = 0);

    // ---- 統計 ----

    /** 現在のフレームのドローコール数。beginFrame() ごとにリセットされます。 */
    uint32_t drawCallCount() const { return m_drawCallCount; }

    /**
     * 現在の (もしくは直前の) レンダーパスのカメラが 2D モードかどうか。
     * BatchProcessor がスプライト頂点レイアウトを 2D 用に切り替えるために参照します。
     */
    bool isCurrentCamera2D() const { return m_currentCamera2D; }

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
                    const Matrix4x4& transform, const Vector2& offset,
                    const Vector2& size, const Vector2& pivot,
                    const Vector2& uvOffset, const Vector2& uvSize,
                    const Color& color);

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
     * 明示的なマテリアルとトランスフォームで単一のサブメッシュを描画します。
     * BatchProcessor がサブメッシュ単位の描画に使用します。
     */
    Result<void> drawSingleSubMesh(Mesh* mesh, uint32_t submeshIndex,
                                   Material* material, const Transform& transform);

    /**
     * 指定したマテリアルで全画面の矩形を描画します。
     * ポストプロセスパスを想定しています。矩形は NDC [-1,1]x[-1,1] を覆い、
     * UV は左上が (0,0)、右下が (1,1) です。
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
    ~Renderer() override;

    rhi::TextureFormat m_colorFormat = {};
    rhi::TextureFormat m_depthFormat = {};
    // 参照用 PipelineLayout (組み込み ShaderPass 由来。動的 UBO アロケータに使用)
    rhi::PipelineLayout* m_referencePipelineLayout = nullptr;
    int16_t m_viewSetIndex = -1;
    int16_t m_sceneSetIndex = -1;
    int16_t m_objectSetIndex = -1;
    uint64_t m_objectUBOSize = 0;

    // フレームごとの View UBO アロケータ (カメラデータ) - DynamicUniformAllocator でダブルバッファリング
    std::unique_ptr<DynamicUniformAllocator> m_viewAllocator;

    // シーン単位のデータ (ライティング等) 用の、フレームごとの動的 UBO アロケータ
    std::unique_ptr<DynamicUniformAllocator> m_sceneAllocator;

    // オブジェクト単位のデータ用の、フレームごとの動的 UBO アロケータ
    std::unique_ptr<DynamicUniformAllocator> m_objectAllocator;

    // 全画面クアッドのメッシュ (最初の drawScreenRect 呼び出し時に遅延生成)
    Ref<Mesh> m_screenRectMesh;

    // コンテキスト (非所有。device と pipelineCache を提供する)
    GraphicsContext* m_ctx = nullptr;

    uint32_t m_frameCounter = 0;
    uint32_t m_currentFrameSlot = 0;
    uint32_t m_framesInFlight = 2;
    uint32_t m_drawCallCount = 0;

    // フレームごとのコマンドエンコード状態 (beginFrame / endFrame の間で有効)
    rhi::CommandBuffer*     m_currentCmd  = nullptr;
    rhi::RenderPass* m_currentPass = nullptr;
    rhi::TextureView*       m_currentColorTarget = nullptr;

    // 現在のレンダーパスでマテリアルを描画する際に優先するシェーダパス名。
    // このパスを持たないマテリアルの描画はスキップされる。
    // 既定は "Forward"。
    std::string m_currentShaderPassName = "Forward";

    // 遅延反映されるパス単位の BindGroup (setPassBindGroup で設定し、setPipeline の後にフラッシュ)
    static constexpr uint32_t kMaxBindGroupSets = 4;
    rhi::BindGroup* m_passBindGroups[kMaxBindGroupSets] = {};
    uint32_t             m_passBindGroupDynamicOffsets[kMaxBindGroupSets] = {};
    uint32_t             m_passBindGroupDynamicOffsetCounts[kMaxBindGroupSets] = {};
    bool            m_passBindGroupDirty[kMaxBindGroupSets] = {};

    // ダーティなパス BindGroup をすべて現在の RenderPass へフラッシュする。
    // drawSubmesh 内で setPipeline の後に呼ばれる。
    void flushPassBindGroups();

    Result<void> drawSubmesh(Mesh* mesh, Material* mat, const Transform& transform, const SubMesh& sub);
    Result<Mesh*> getScreenRectMesh();

    // ---- ステンシルマスクの内部ヘルパー ----

    /** ステンシル書き込みのためだけにメッシュを描画する (カラー出力なし)。 */
    Result<void> drawStencilMaskMesh(Mesh* mesh, const Transform& transform, Material* material,
                                     rhi::CompareFunction compare, uint32_t stencilRef, rhi::StencilOp passOp);

    struct StencilMaskEntry {
        Mesh* mesh;
        Transform transform;
        Material* material;
    };
    std::vector<StencilMaskEntry> m_stencilMaskStack;
    uint32_t m_stencilRef = 0;

    // ---- バッチレンダリング (内部) ----

    DrawCommandBuffer m_commandBuffer;
    std::unique_ptr<BatchProcessor> m_batchProcessor;

    /** beginRenderPass(camera, ...) が Camera::is2D() に基づいて設定する。 */
    bool m_currentCamera2D = false;

    /** 現在のパスのビュー行列。深度ソート (SortMode::FrontToBack/BackToFront) で使用。
     *  カメラ無しパスでは単位行列。 */
    Matrix4x4 m_currentViewMatrix = Matrix4x4::identity();

    /** 現在のパスの二次ソート方法。beginRenderPass(camera, ...) で指定。既定は Stable。 */
    SortMode m_currentSortMode = SortMode::Stable;

    /** 蓄積したコマンドをフラッシュし、コマンドバッファをクリアする。 */
    Result<void> flushBatch();

    // ---- マテリアル単位の BindGroup キャッシュ (Renderer が所有) ----

    /** 単一の Material に対するキャッシュ済み GPU リソース。インフライトフレームごとにダブルバッファリングする。 */
    struct CachedMaterialBind {
        // Material::bindingVersion() の追随値。ズレたらテクスチャ/サンプラーを解決し直す。
        uint64_t bindingVersion = 0;
        // バインディングごとのテクスチャ追跡 (binding index -> 最後のテクスチャポインタ)
        std::unordered_map<uint32_t, rhi::Texture*> lastTextures;
        std::unordered_map<uint32_t, Ref<rhi::TextureView>> textureViews;
        // Sampler バインディングごとのサンプラー (binding index -> Sampler)。
        // 名前付きテクスチャ単位でフィルタ/アドレッシングを変えられるよう binding 単位で保持する。
        // 実体は m_samplerPool で設定が同じもの同士が共有される。
        std::unordered_map<uint32_t, Ref<rhi::Sampler>> samplers;
        std::vector<Ref<rhi::Buffer>>    paramBuffers;
        // 空 (nullptr) のスロットは「BindGroup を作り直す必要がある」印。
        std::vector<Ref<rhi::BindGroup>> bindGroups;
        // スロットごとの、UBO へ書き込み済みの Material::paramVersion()。
        std::vector<uint64_t> writtenParamVersion;
    };

    /** 複合キー: Material の BindGroup は、Material のパラメータと
     *  ShaderPass の PipelineLayout / マテリアルセットレイアウトの両方に依存する。 */
    struct MaterialBindKey {
        Material*   mat;
        ShaderPass* pass;
        bool operator==(const MaterialBindKey& o) const {
            return mat == o.mat && pass == o.pass;
        }
    };
    struct MaterialBindKeyHash {
        size_t operator()(const MaterialBindKey& k) const {
            return std::hash<Material*>()(k.mat) ^
                   (std::hash<ShaderPass*>()(k.pass) << 1);
        }
    };
    std::unordered_map<MaterialBindKey, CachedMaterialBind, MaterialBindKeyHash> m_materialCache;

    /**
     * 破棄コールバックを登録済みの Material の集合 (二重登録を避けるため)。
     *
     * ShaderPass 側を追跡しないのは、キーの pass が必ず mat->findPass() 由来であり、
     * Material が m_shaderPasses で Ref を握っているためです。つまり ShaderPass は
     * それを参照する Material より先に死にません。
     */
    std::unordered_set<Material*> m_trackedMaterials;

    /**
     * Renderer の生存フラグ。Renderer が先に破棄されたときに、Material 側に
     * 残ったコールバックがダングリングポインタを触らないようにするために使います。
     */
    std::shared_ptr<bool> m_alive = std::make_shared<bool>(true);

    /** (material, pass) と現在のフレームスロットに対応する BindGroup を取得 (無ければ作成)。 */
    Result<rhi::BindGroup*> getOrCreateMaterialBindGroup(Material* mat, ShaderPass* pass);

    /** mat を参照する m_materialCache のエントリをすべて削除します。 */
    void evictMaterialCache(Material* mat);

    // ---- サンプラープール (Renderer が所有) ----
    //
    // サンプラーは設定の組み合わせが少なく、マテリアル間で共有できる。
    // SamplerState をパックしたキーで引き当て、同じ設定なら GPU オブジェクトを共有する。
    std::unordered_map<uint32_t, Ref<rhi::Sampler>> m_samplerPool;

    /** SamplerState に対応する Sampler を取得 (無ければ作成してプールへ登録)。 */
    Result<rhi::Sampler*> getOrCreateSampler(const SamplerState& state);

};

} // namespace ln
