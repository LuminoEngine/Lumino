#pragma once
#include "WebGL2Common.hpp"
#include <vector>

namespace ln::rhi::webgl2 {

class WebGL2Device;
class WebGL2BindGroup;
class WebGL2CommandBuffer;
class WebGL2RenderPipeline;

/** 同時にバインドできるディスクリプタセットの数。リフレクション由来のセットは 0..3 程度。 */
static const uint32_t kMaxBindGroupSets = 8;
/** 同時にバインドできる頂点バッファのスロット数。 */
static const uint32_t kMaxVertexBufferSlots = 4;

// ------ 記録するコマンド ------------------------------------------------------------------------------------------------------

enum class GLCmdType : uint8_t {
    BeginRenderPass,
    EndRenderPass,
    SetPipeline,
    SetVertexBuffer,
    SetIndexBuffer,
    SetBindGroup,
    SetViewport,
    SetScissorRect,
    SetStencilReference,
    Draw,
    DrawIndexed,
};

struct GLCommand {
    GLCmdType type;
    union {
        struct { uint32_t passIndex; } pass;
        struct { RenderPipeline* pipeline; } setPipeline;
        struct { uint32_t slot; Buffer* buffer; uint64_t offset; } setVertexBuffer;
        struct { Buffer* buffer; IndexFormat format; uint64_t offset; } setIndexBuffer;
        struct { uint32_t setIndex; BindGroup* group; uint32_t offsetPos; uint32_t offsetCount; } setBindGroup;
        struct { float x, y, w, h, minDepth, maxDepth; } viewport;
        struct { uint32_t x, y, w, h; } scissor;
        struct { uint32_t reference; } stencilRef;
        struct { uint32_t vertexCount, instanceCount, firstVertex, firstInstance; } draw;
        struct { uint32_t indexCount, instanceCount, firstIndex, firstInstance; int32_t baseVertex; } drawIndexed;
    };
};

/**
 * beginRenderPass に渡されたアタッチメントの控え。
 *
 * RenderPassDesc::depthStencilAttachment は呼び出し側のスタック上にあることがあるため、
 * ポインタではなく値でコピーしておく。
 */
struct RecordedRenderPass {
    SmallVector<ColorAttachment, kMaxMultiRenderTargets> colorAttachments;
    DepthStencilAttachment depthStencil;
    bool hasDepthStencil;
    RenderPassLayoutDesc layoutDesc;
};

// ------ WebGL2RenderPass ------------------------------------------------------------------------------------------------------

/**
 * コマンドを記録するだけのレンダーパス。
 *
 * GL は即時実行の API だが、Rhi.hpp の writeBuffer の順序契約
 * (「同一フレーム内の draw はそのフレームの最後の書き込み内容を読む」)を満たすため、
 * ここでは発行せず CommandBuffer::submit() でまとめて再生する。
 */
class WebGL2RenderPass final : public RenderPass {
public:
    void init(WebGL2CommandBuffer* owner, const RenderPassLayoutDesc& layoutDesc);

    const RenderPassLayoutDesc& layoutDesc() const override { return m_layoutDesc; }
    void setPipeline(RenderPipeline* pipeline) override;
    void setVertexBuffer(uint32_t slot, Buffer* buffer, uint64_t offset) override;
    void setIndexBuffer(Buffer* buffer, IndexFormat format, uint64_t offset) override;
    void setBindGroup(uint32_t index, BindGroup* group) override;
    void setBindGroup(uint32_t index, BindGroup* group,
                      const uint32_t* dynamicOffsets, uint32_t dynamicOffsetCount) override;
    void setViewport(float x, float y, float w, float h, float minDepth, float maxDepth) override;
    void setScissorRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;
    void setStencilReference(uint32_t reference) override;
    void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
    void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                     int32_t baseVertex, uint32_t firstInstance) override;
    void end() override;

private:
    WebGL2CommandBuffer* m_owner = nullptr;
    RenderPassLayoutDesc m_layoutDesc;
};

// ------ WebGL2CommandBuffer ---------------------------------------------------------------------------------------------------

class WebGL2CommandBuffer final : public CommandBuffer {
public:
    VoidResult init(WebGL2Device* device);

    RenderPass* beginRenderPass(const RenderPassDesc& desc) override;
    void submit() override;

    /** WebGL2RenderPass から呼ばれる記録用のヘルパー。 */
    void record(const GLCommand& cmd) { m_commands.push_back(cmd); }
    /** 動的オフセットの列を控え、先頭位置を返す。 */
    uint32_t recordDynamicOffsets(const uint32_t* offsets, uint32_t count);

protected:
    void finalize() override;

private:
    /** 再生中に持ち回すバインド状態。 */
    struct ReplayState {
        const WebGL2RenderPipeline* pipeline;
        WebGL2BindGroup* groups[kMaxBindGroupSets];
        uint32_t dynamicOffsetPos[kMaxBindGroupSets];
        uint32_t dynamicOffsetCount[kMaxBindGroupSets];
        Buffer* vertexBuffers[kMaxVertexBufferSlots];
        uint64_t vertexOffsets[kMaxVertexBufferSlots];
        Buffer* indexBuffer;
        IndexFormat indexFormat;
        uint64_t indexOffset;
        uint32_t stencilReference;
        /** 現在 glEnableVertexAttribArray してあるロケーションのビットマスク。 */
        uint32_t enabledAttribs;
    };

    void replay();
    void beginPass(const RecordedRenderPass& pass);
    void endPass(const RecordedRenderPass& pass);
    void applyDrawState(ReplayState& state);

    WebGL2Device* m_device = nullptr;
    std::vector<GLCommand> m_commands;
    std::vector<RecordedRenderPass> m_passes;
    std::vector<uint32_t> m_dynamicOffsets;
    Ref<WebGL2RenderPass> m_currentRenderPass;

    /** レンダーパスで使い回す FBO。アタッチメントはパスごとに張り替える。 */
    GLuint m_framebuffer = 0;
    /** 直前に FBO へ張ったアタッチメントの GL 名。変化したときだけ完全性を検査する。 */
    std::vector<GLuint> m_lastAttachments;
    /** 直前のパスのカラーアタッチメント数。減ったぶんを FBO から外すために使う。 */
    size_t m_lastColorCount = 0;
};

} // namespace ln::rhi::webgl2
