// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include "Common.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
class ShaderPass;
class SwapChain;
namespace detail {
class RenderingQueue;
class IRenderPass;
class GraphicsCommandList;
}

/*
 * グラフィクスデバイスへの描画呼出しを発行するためのクラスです。
 */
class LN_API GraphicsContext
    : public Object {
public:
    /** 同時に設定できる RenderTarget の最大数です。 */
    static const int MaxMultiRenderTargets = detail::MaxMultiRenderTargets;

    /** 同時に設定できる 頂点ストリーム の最大数です。 */
    static const int MaxVertexStreams = detail::MaxVertexStreams;

public:
    /** BlendState を設定します。 */
    void setBlendState(const BlendStateDesc& value);

    /** BlendState を取得します。 */
    const BlendStateDesc& blendState() const { return m_staging.blendState; }

    /** RasterizerState を設定します。 */
    void setRasterizerState(const RasterizerStateDesc& value);

    /** RasterizerState を取得します。 */
    const RasterizerStateDesc& rasterizerState() const { return m_staging.rasterizerState; }

    /** DepthStencilState を設定します。 */
    void setDepthStencilState(const DepthStencilStateDesc& value);

    /** DepthStencilState を取得します。 */
    const DepthStencilStateDesc& depthStencilState() const { return m_staging.depthStencilState; }

    ///** RenderTarget を設定します。index 0 に設定した場合、Viewport と Scissor 領域は新しい RenderTarget のサイズに合わせて再設定されます。 */
    // void setRenderTarget(int index, RenderTargetTexture* value);

    ///** RenderTarget を取得します。 */
    // RenderTargetTexture* renderTarget(int index) const;

    ///** DepthBuffer を設定します。 */
    // void setDepthBuffer(DepthBuffer* value);

    ///** DepthBuffer を取得します。 */
    // DepthBuffer* depthBuffer() const;

    /** ビューポートの矩形を設定します。 */
    void setViewportRect(const Rect& value);

    /** ビューポートの矩形を取得します。 */
    const Rect& viewportRect() const { return m_staging.viewportRect; }

    /** シザー領域の矩形を設定します。 */
    void setScissorRect(const Rect& value);

    /** シザー領域の矩形を取得します。 */
    const Rect& scissorRect() const { return m_staging.scissorRect; }

    /** VertexLayout を設定します。 */
    void setVertexLayout(VertexLayout* value);

    /** VertexLayout を取得します。 */
    VertexLayout* vertexLayout() const;

    /** 描画プリミティブの種類を設定します。 */
    void setPrimitiveTopology(PrimitiveTopology value);

    /** IndexBuffer を取得します。 */
    PrimitiveTopology primitiveTopology() const { return m_staging.topology; }

    /** VertexBuffer を設定します。 */
    void setVertexBuffer(int streamIndex, VertexBuffer* value);

    /** VertexBuffer を取得します。 */
    VertexBuffer* vertexBuffer(int streamIndex) const;

    /** IndexBuffer を設定します。 */
    void setIndexBuffer(IndexBuffer* value);

    /** IndexBuffer を取得します。 */
    IndexBuffer* indexBuffer() const;

    /** ShaderPass を設定します。 */
    void setShaderPass(ShaderPass* value);

    /** ShaderPass を取得します。 */
    ShaderPass* shaderPass() const;

    ///** ShaderPass を設定します。 */
    void setShaderDescriptor(detail::ShaderSecondaryDescriptor* value);

    ///** ShaderPass を取得します。 */
    // ShaderDefaultDescriptor* shaderDescriptor() const;

    /** デフォルト設定を復元します。 */
    void resetState();

    ///** RenderPass を設定します。Viewport と Scissor 領域は新しい RenderTarget のサイズに合わせて再設定されます。 */
    // void setRenderPass(RenderPass* value);

    /** RenderPass を開始します。 */
    void beginRenderPass(RenderPass* value);

    /** RenderPass を終了します。 */
    void endRenderPass();

    /** RenderPass を取得します。 */
    RenderPass* renderPass() const;

    void dispatch(int groupCountX, int groupCountY, int groupCountZ);

    /**
     * レンダーターゲット、深度バッファ、ステンシルバッファをクリアします。
     *
     * 複数のレンダーターゲットが設定されている場合、先頭 (index 0) のターゲットのみクリアされます。
     * また、ViewportRect と ScissorRect の設定は適用されません。
     */
    void clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);

    /**
     * プリミティブを描画します。
     */
    void drawPrimitive(int startVertex, int primitiveCount);

    /**
     * インデックス付きのプリミティブを描画します。
     */
    void drawPrimitiveIndexed(int startIndex, int primitiveCount, int instanceCount = 0, int vertexOffset = 0);

    void drawExtension(INativeGraphicsExtension* extension);

    ln::detail::ShaderSecondaryDescriptor* allocateShaderDescriptor(ShaderPass* shaderPass);

    void interruptCurrentRenderPassFromResolveRHI();

    // TODO: intenral
    const Ref<detail::GraphicsCommandList>& commandList() const { return m_commandList; }

protected:
    virtual void onDispose(bool explicitDisposing) override;

private:
    LN_INTERNAL_NEW_OBJECT;
    GraphicsContext();
    virtual ~GraphicsContext();
    void init(/*RenderingType renderingType*/);
    void resetCommandList(detail::GraphicsCommandList* commandList);

    //RenderingType renderingType() const { return m_renderingType; }
    //detail::RenderingCommandList* renderingCommandList();
    void beginCommandRecoding();
    void endCommandRecoding();
    //void flushCommandRecoding(RenderTargetTexture* affectRendreTarget);
    // void closeRenderPass();
    detail::ICommandList* commitState();
    // void submitCommandList();

    enum DirtyFlags {
        DirtyFlags_None = 0,
        DirtyFlags_BlendState = 1 << 1,
        DirtyFlags_RasterizerState = 1 << 2,
        DirtyFlags_DepthStencilState = 1 << 3,
        DirtyFlags_RegionRects = 1 << 4,
        // DirtyFlags_Framebuffer = 1 << 5,
        DirtyFlags_PipelinePrimitiveState = 1 << 6,
        DirtyFlags_PrimitiveBuffers = 1 << 7,
        DirtyFlags_ShaderPass = 1 << 8,
        DirtyFlags_RenderPass = 1 << 9,
        DirtyFlags_ShaderDescriptor = 1 << 10,
        DirtyFlags_All = 0xFFFFFFFF,
    };

    struct State {
        BlendStateDesc blendState;
        RasterizerStateDesc rasterizerState;
        DepthStencilStateDesc depthStencilState;
        Rect viewportRect;
        Rect scissorRect;
        Ref<VertexLayout> VertexLayout;
        std::array<Ref<VertexBuffer>, detail::MaxVertexStreams> vertexBuffers;
        Ref<IndexBuffer> indexBuffer;
        Ref<Shader> shader; // shaderPass owner, for keep reference.
        ShaderPass* shaderPass;
        // Ref<ShaderDefaultDescriptor> shaderDescriptor;
        detail::ShaderSecondaryDescriptor* shaderDescriptor;
        PrimitiveTopology topology;

        void reset();
    };

    enum class ScopeState {
        Idle,
        RenderPassOutside,
        BeginRenderPassRequired, // 次の Commit で実際に RenderPass を開始したい
        RenderPassInterrupted,   // 次の Commit で実際に RenderPass を開始したい
        Active,
    };

    bool checkRenderPassInside() const;

    detail::GraphicsManager* m_manager;
    Ref<detail::GraphicsCommandList> m_commandList;
    detail::ICommandList* m_rhiCommandList;
    //Ref<detail::RenderingCommandList> m_recordingCommandList;
    //Ref<detail::RenderingCommandList> m_executingCommandList;
    //RenderingType m_renderingType;
    State m_staging;
    //State m_lastCommit;
    Ref<RenderPass> m_currentRenderPass;
    Ref<detail::IRenderPass> m_currentRHIRenderPass;
    uint32_t m_dirtyFlags;
    ScopeState m_scopeState;
    //bool m_recordingBegan;

    friend class detail::GraphicsContextInternal;
    friend class detail::RenderingQueue;
};

namespace detail {
class GraphicsContextInternal {
public:
    static void resetCommandList(GraphicsContext* self, detail::GraphicsCommandList* commandLsit) { self->resetCommandList(commandLsit); }
    //static RenderingType getRenderingType(GraphicsContext* self) { return self->renderingType(); }
     //static detail::RenderingCommandList* getRenderingCommandList(GraphicsContext* self) { return self->renderingCommandList(); }
    static void beginCommandRecoding(GraphicsContext* self) { self->beginCommandRecoding(); }
    static void endCommandRecoding(GraphicsContext* self) { self->endCommandRecoding(); }
    //static void flushCommandRecoding(GraphicsContext* self, RenderTargetTexture* affectRendreTarget) { self->flushCommandRecoding(affectRendreTarget); }
    //static detail::ICommandList* getCommandListForTransfer(GraphicsContext* self) { return self->m_rhiCommandList; }
    //static ICommandList* commitState(GraphicsContext* self) { return self->commitState(); }
    //static void enterRenderState(GraphicsContext* self) { self->enterRenderState(); }
    //static void leaveRenderState(GraphicsContext* self) { self->leaveRenderState(); }
    //static const Ref<RenderPass>& currentRenderPass(GraphicsContext* self) { return self->m_currentRenderPass; }
};
}
} // namespace ln
