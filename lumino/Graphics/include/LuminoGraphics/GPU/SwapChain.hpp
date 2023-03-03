// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
class ShaderPass;
class PlatformWindow;

namespace detail {
class GraphicsManager;
class ISwapChain;
class SwapChainInternal;
class SingleFrameUniformBufferAllocator;
}

/** スワップチェーンのクラスです。 */
class LN_API SwapChain
    : public Object
    , public IGraphicsResource {
public:
    Size backbufferSize() const;

    GraphicsCommandList* currentCommandList2() const;

    /** バックバッファを取得します。(返されるインスタンスはフレームごとに異なります。このインスタンスを保持しないでください) */
    RenderTargetTexture* currentBackbuffer() const;

    RenderPass* currentRenderPass() const;

    void present();

protected:
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    SwapChain();
    virtual ~SwapChain();
    void init(PlatformWindow* window);

private:
    void resizeBackbuffer(int width, int height);
    void resetRHIBackbuffers();
    void nextFrame();
    void presentInternal();
    detail::ISwapChain* resolveRHIObject(GraphicsCommandList* context, bool* outModified) const;
    int imageIndex() const { return m_imageIndex; }

    detail::GraphicsManager* m_manager;
    URef<detail::ISwapChain> m_rhiObject;
    std::vector<Ref<RenderTargetTexture>> m_backbuffers;
    std::vector<Ref<DepthBuffer>> m_depthBuffers;
    std::vector<Ref<RenderPass>> m_renderPasses;
    std::vector<Ref<GraphicsCommandList>> m_commandLists;
    int m_imageIndex;

    friend class detail::GraphicsResourceInternal;
    friend class detail::SwapChainInternal;
};

namespace detail {

class SwapChainInternal {
public:
    static void setBackendBufferSize(SwapChain* swapChain, int width, int height);
    static void setOpenGLBackendFBO(SwapChain* swapChain, uint32_t id);
    static void resizeBackbuffer(SwapChain* swapChain, int width, int height) { swapChain->resizeBackbuffer(width, height); }
    static int imageIndex(SwapChain* swapChain) { return swapChain->imageIndex(); }
    static int swapBufferCount(SwapChain* swapChain) { return static_cast<int>(swapChain->m_backbuffers.size()); }
};

} // namespace detail
} // namespace ln
