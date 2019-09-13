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
}

/** スワップチェーンのクラスです。 */
class LN_API SwapChain
    : public GraphicsResource
{
public:
    /** バックバッファを取得します。(返されるインスタンスはフレームごとに異なります。このインスタンスを保持しないでください) */
    RenderTargetTexture* currentBackbuffer() const;

	GraphicsContext* beginFrame();
	void endFrame();

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    SwapChain();
    virtual ~SwapChain();
    void init(detail::PlatformWindow* window, const SizeI& backbufferSize);

private:
	const Ref<detail::ICommandList>& currentCommandList() const { return m_commandLists[m_imageIndex]; }
    void resizeBackbuffer(int width, int height);
	void resetRHIBackbuffers();
    void present(GraphicsContext* context);
    detail::ISwapChain* resolveRHIObject(GraphicsContext* context, bool* outModified) const;
    int imageIndex() const { return m_imageIndex; }

    Ref<detail::ISwapChain> m_rhiObject;
	std::vector<Ref<RenderTargetTexture>> m_backbuffers;
	std::vector<Ref<DepthBuffer>> m_depthBuffers;
	std::vector<Ref<RenderPass>> m_renderPasses;
	std::vector<Ref<detail::ICommandList>> m_commandLists;
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
    static const Ref<detail::ICommandList>& currentCommandList(SwapChain* swapChain) { return swapChain->currentCommandList(); }
    static void resizeBackbuffer(SwapChain* swapChain, int width, int height) { swapChain->resizeBackbuffer(width, height); }
    static void present(SwapChain* swapChain, GraphicsContext* context) { swapChain->present(context); }
    static int imageIndex(SwapChain* swapChain) { return swapChain->imageIndex(); }
};

} // namespace detail
} // namespace ln
