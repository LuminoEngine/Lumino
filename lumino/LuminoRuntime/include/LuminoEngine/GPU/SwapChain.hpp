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
class LN_API GraphicsContext
    : public Object
    , public IGraphicsResource {
    /*
    NOTE: 移行作業中。
    SwapChain と、それをターゲットとする CommandList の管理を行う。
    将来的には、 RHI リソースの管理も行いたい。

    Backend ごとに派生クラスを作って、様々な使い方に対応できるようにしておくのがいいだろう。
    Skia のような API でも良いかと思ったが、 Unity への組み込みなどで
    ゲーム側から VulkanCommandList をもらってラップして使うようなケースでは、
    Skia のような API だと使いにくいかもしれない。

    
    ## Vulkan の External-CommandList を使う場合のイメージ

    ```
    auto context = GraphicsContext::createFromExternalVulkan();
    auto renderer = RenderingContext::create(context);

    context.setCommandList(vulkanCommandList[frame]);
    renderer.drawRectangle(...);
}
    // vulkanCommandList[frame] を Queue に送るのはユーザーコードで。
    ```


    ## API 変更の動機
    Lyric プロジェクトのエディタで、複数の canvas への描画を行うためです。

    */

public:
    Size backbufferSize() const;

    GraphicsCommandList* currentCommandList2() const;

    /** バックバッファを取得します。(返されるインスタンスはフレームごとに異なります。このインスタンスを保持しないでください) */
    RenderTargetTexture* currentBackbuffer() const;

    RenderPass* currentRenderPass() const;

    void present();

public: // TODO: internal
    detail::RHIGraphicsResourceRegistry* rhiResourceRegistry() const { return m_rhiResourceRegistry; }

protected:
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    GraphicsContext();
    virtual ~GraphicsContext();
    void init(PlatformWindow* window);

private:
    void resizeBackbuffer(int width, int height);
    void resetRHIBackbuffers();
    void nextFrame();
    void presentInternal();
    detail::ISwapChain* resolveRHIObject(GraphicsCommandList* context, bool* outModified) const;
    int imageIndex() const { return m_imageIndex; }

    detail::GraphicsManager* m_manager;
    Ref<detail::ISwapChain> m_rhiObject;
    URef<detail::RHIGraphicsResourceRegistry> m_rhiResourceRegistry;
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
    static void setBackendBufferSize(GraphicsContext* swapChain, int width, int height);
    static void setOpenGLBackendFBO(GraphicsContext* swapChain, uint32_t id);
    static void resizeBackbuffer(GraphicsContext* swapChain, int width, int height) { swapChain->resizeBackbuffer(width, height); }
    static int imageIndex(GraphicsContext* swapChain) { return swapChain->imageIndex(); }
    static int swapBufferCount(GraphicsContext* swapChain) { return static_cast<int>(swapChain->m_backbuffers.size()); }
};

} // namespace detail
} // namespace ln
