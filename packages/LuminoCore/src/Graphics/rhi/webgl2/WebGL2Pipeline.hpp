#pragma once
#include "WebGL2Common.hpp"
#include <LuminoBase/SmallVector.hpp>
#include <vector>

namespace ln::rhi::webgl2 {

// ------ WebGL2BindGroup -------------------------------------------------------------------------------------------------------

// GL にはバインドグループに相当するオブジェクトが無いため、エントリを控えておき、
// 描画の再生時にパイプラインが要求するバインディングを引く。
class WebGL2BindGroup final : public BindGroup {
public:
    void init(const BindGroupEntry* entries, size_t entryCount);

    /** binding 番号でエントリを引く。見つからなければ nullptr。 */
    const BindGroupEntry* find(uint32_t binding) const;

private:
    SmallVector<BindGroupEntry, kMaxBindGroupEntries> m_entries;
};

// ------ WebGL2PipelineLayout --------------------------------------------------------------------------------------------------

class WebGL2PipelineLayout final : public PipelineLayout {
public:
    void init(const PipelineLayoutDesc& desc);

    Result<Ref<BindGroup>> createBindGroup(
        uint32_t setIndex, const BindGroupEntry* entries, size_t entryCount) override;

    const std::vector<BindGroupLayoutDesc>& setLayouts() const { return m_setLayouts; }
    const std::vector<CombinedSamplerBinding>& combinedSamplers() const { return m_combinedSamplers; }

private:
    std::vector<BindGroupLayoutDesc> m_setLayouts;
    std::vector<CombinedSamplerBinding> m_combinedSamplers;
};

// ------ WebGL2RenderPipeline --------------------------------------------------------------------------------------------------

/** リンク済みプログラムのユニフォームブロック 1 つ分の結び付け情報。 */
struct GLUniformBlockBinding {
    uint32_t set;
    uint32_t binding;
    uint32_t bindingPoint;  ///< glBindBufferRange に渡すインデックス
    int32_t dynamicIndex;   ///< セット内で何番目の動的オフセットか。動的でなければ -1
};

class WebGL2RenderPipeline final : public RenderPipeline {
public:
    VoidResult init(const RenderPipelineDesc& desc);

    GLuint program() const { return m_program; }
    GLenum topology() const { return m_topology; }
    const std::vector<VertexBufferLayout>& vertexBuffers() const { return m_vertexBuffers; }
    const std::vector<GLUniformBlockBinding>& uniformBlocks() const { return m_uniformBlocks; }
    const std::vector<CombinedSamplerBinding>& combinedSamplers() const { return m_combinedSamplers; }

    /** ラスタライザ / ブレンド / 深度ステンシルのステートを現在の GL コンテキストへ適用する。 */
    void applyFixedFunctionState() const;
    /** ステンシル参照値だけを更新する (setStencilReference と setPipeline の両方から呼ばれる)。 */
    void applyStencilReference(uint32_t reference) const;

    const std::string& debugName() const { return m_debugName; }

protected:
    void finalize() override;

private:
    GLuint m_program = 0;
    GLenum m_topology = GL_TRIANGLES;
    std::vector<VertexBufferLayout> m_vertexBuffers;
    std::vector<GLUniformBlockBinding> m_uniformBlocks;
    // レイアウトへの参照は持たず値をコピーする。パイプラインがレイアウトを生かし続けると
    // PipelineCache の破棄コールバックによる追い出しが効かなくなるため。
    std::vector<CombinedSamplerBinding> m_combinedSamplers;
    CullMode m_cullMode = CullMode::None;
    FrontFace m_frontFace = FrontFace::CCW;
    BlendState m_blend;
    DepthStencilState m_depthStencil;
    std::string m_debugName;
};

} // namespace ln::rhi::webgl2
