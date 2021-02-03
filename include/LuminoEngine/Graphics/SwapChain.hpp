// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"
#include "GeometryStructs.hpp"
#include "GraphicsResource.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
class ShaderPass;

namespace detail {
class GraphicsManager;
class PlatformWindow;
class ISwapChain;
class SwapChainInternal;
class SingleFrameUniformBufferAllocator;


}

/** スワップチェーンのクラスです。 */
// Note: present は公開しないで行ってみる。
// Metal の present は CommandBuffer の一部だったりする。
// また Vulkan は Swap ごとに Semaphore が必要になるが、Test_Graphics_LowLevelRendering でやってたみたいに
// present せずにオフスクリーン的にレンダリングしてしまうと、直前の CommandBuffer の submit 時の通知先セマフォと同じものを使ってしまうので、検証レイヤーが怒る。
// 実際のところ Swapchaing の backbuffer をオフスクリーン的に使うべきではないし、
// 普通に使う場合は endFrame() の直後に present() するだけなので、endFrame() で一緒にやってしまう。これで簡潔さと誤用防止、移植性upを狙ってみる。
class LN_API SwapChain
    : public Object
    , public IGraphicsResource
{
public:
    Size backbufferSize() const;

    /** バックバッファを取得します。(返されるインスタンスはフレームごとに異なります。このインスタンスを保持しないでください) */
    RenderTargetTexture* currentBackbuffer() const;

	GraphicsContext* beginFrame2();	// 続いで currentRenderPass() で取得したパスの begin が必要。※begin 前に data-translate したりしたいので分けている。
	RenderPass* currentRenderPass() const;

	void endFrame();

protected:
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    SwapChain();
    virtual ~SwapChain();
    void init(detail::PlatformWindow* window, const SizeI& backbufferSize);

private:
	const Ref<detail::GraphicsCommandList>& currentCommandList() const { return m_commandLists[m_imageIndex]; }
    void resizeBackbuffer(int width, int height);
	void resetRHIBackbuffers();
    void present(GraphicsContext* context);
    detail::ISwapChain* resolveRHIObject(GraphicsContext* context, bool* outModified) const;
    int imageIndex() const { return m_imageIndex; }

    detail::GraphicsManager* m_manager;
    Ref<detail::ISwapChain> m_rhiObject;
	std::vector<Ref<RenderTargetTexture>> m_backbuffers;
	std::vector<Ref<DepthBuffer>> m_depthBuffers;
	std::vector<Ref<RenderPass>> m_renderPasses;
	std::vector<Ref<detail::GraphicsCommandList>> m_commandLists;
	Ref<GraphicsContext> m_graphicsContext;
    int m_imageIndex;

    friend class detail::GraphicsResourceInternal;
    friend class detail::SwapChainInternal;
};

namespace detail {

class SwapChainInternal
{
public:
    static void setBackendBufferSize(SwapChain* swapChain, int width, int height);
    static void setOpenGLBackendFBO(SwapChain* swapChain, uint32_t id);
    static const Ref<detail::GraphicsCommandList>& currentCommandList(SwapChain* swapChain) { return swapChain->currentCommandList(); }
    static void resizeBackbuffer(SwapChain* swapChain, int width, int height) { swapChain->resizeBackbuffer(width, height); }
    //static void present(SwapChain* swapChain, GraphicsContext* context) { swapChain->present(context); }
    static int imageIndex(SwapChain* swapChain) { return swapChain->imageIndex(); }
    static int swapBufferCount(SwapChain* swapChain) { return static_cast<int>(swapChain->m_backbuffers.size()); }
};

} // namespace detail
} // namespace ln
