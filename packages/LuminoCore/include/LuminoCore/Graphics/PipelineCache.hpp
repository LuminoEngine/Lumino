#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

namespace ln {

class ShaderPass;

/**
 * RenderPipelineの検索または作成に使用するキー。
 * マテリアル由来のフィールドとレンダリングコンテキストのフィールドが組み合わされます。
 */
struct PipelineCacheKey {
    // Material 由来
    ShaderPass* shaderPass = nullptr;
    rhi::CullMode cullMode = rhi::CullMode::Back;
    rhi::BlendState blendState;     // blendState.enabled==false の場合、他のフィールドは無視
    bool depthTestEnabled = true;
    bool depthWriteEnabled = true;
    bool stencilTestEnabled = false;
    rhi::StencilFaceState stencilFront;
    rhi::StencilFaceState stencilBack;
    uint32_t stencilReadMask = 0xFF;
    uint32_t stencilWriteMask = 0xFF;
    bool colorWriteEnabled = true;
    // 描画コンテキスト由来
    rhi::PrimitiveTopology topology = rhi::PrimitiveTopology::TriangleList;
    rhi::RenderPass* renderPass = nullptr;

    bool operator==(const PipelineCacheKey& o) const;
};

struct PipelineCacheKeyHash {
    size_t operator()(const PipelineCacheKey& key) const;
};

/**
 * ハッシュベースのRenderPipelineキャッシュ。
 * いわゆる POC (Pipeline Object Cache)
 */
class PipelineCache {
public:
    explicit PipelineCache(rhi::Device* device);
    ~PipelineCache();

    /** 指定したキーに一致する RenderPipeline を検索します (無ければ作成します)。 */
    Result<rhi::RenderPipeline*> getOrCreate(const PipelineCacheKey& key);

    /** キャッシュ済みのパイプラインをすべて破棄します。 */
    void clear();

    /** キャッシュ済みのパイプライン数。 */
    size_t size() const { return m_cache.size(); }

private:
    /** オブジェクト破棄時にそのオブジェクトを参照するエントリをキャッシュから削除します。 */
    void evictByObject(rhi::RHIObject* obj);

    /** オブジェクトがまだ未登録であればコールバックを登録します。 */
    void trackObject(rhi::RHIObject* obj);

    rhi::Device* m_device;
    std::unordered_map<PipelineCacheKey, Ref<rhi::RenderPipeline>, PipelineCacheKeyHash> m_cache;
    std::unordered_set<rhi::RHIObject*> m_trackedObjects;

    /**
     * PipelineCache の生存フラグ。
     * PipelineCache が先に破棄されたときに、RHIObject 側のコールバックが
     * ダングリングポインタを触らないようにするために使います。
     */
    std::shared_ptr<bool> m_alive;
};

} // namespace ln
