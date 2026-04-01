#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/graphics/rhi/Rhi.hpp>

namespace ln {

/**
 * RenderPipelineの検索または作成に使用するキー。
 * マテリアル由来のフィールドとレンダリングコンテキストのフィールドが組み合わされます。
 */
struct PipelineCacheKey {
    // Material 由来
    rhi::ShaderModule* vertexShader = nullptr;
    rhi::ShaderModule* fragmentShader = nullptr;
    std::string vertexEntry;
    std::string fragmentEntry;
    rhi::CullMode cullMode = rhi::CullMode::Back;
    bool blendEnabled = false;
    rhi::BlendState blendState;     // blendEnabled==false の場合は無視
    bool depthTestEnabled = true;
    bool depthWriteEnabled = true;
    // 描画コンテキスト由来
    rhi::PipelineLayout* pipelineLayout = nullptr;
    rhi::PrimitiveTopology topology = rhi::PrimitiveTopology::TriangleList;
    rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8Unorm;
    rhi::TextureFormat depthStencilFormat = rhi::TextureFormat::Depth32Float;
    u32 sampleCount = 1;

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

    /** Find or create a RenderPipeline matching the given key. */
    Result<rhi::RenderPipeline*> getOrCreate(const PipelineCacheKey& key);

    /** Destroy all cached pipelines. */
    void clear();

    /** Number of cached pipelines. */
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
