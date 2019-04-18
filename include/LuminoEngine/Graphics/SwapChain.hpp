#pragma once

#include "Common.hpp"
#include "GeometryStructs.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
class ShaderPass;

namespace detail {
	class PlatformWindow;
	class ISwapChain;
}

/** スワップチェーンのクラスです。 */
class LN_API SwapChain
	: public Object
{
public:
	/** バックバッファを取得します。 */
	RenderTargetTexture* backbuffer() const;

	virtual void onDispose(bool explicitDisposing) override;

	//void wait();

	void resizeBackbuffer(int width, int height);

LN_CONSTRUCT_ACCESS:
	SwapChain();
	virtual ~SwapChain();
	void init(detail::PlatformWindow* window, const SizeI& backbufferSize);

LN_INTERNAL_ACCESS:
	detail::ISwapChain* resolveRHIObject() const;
    void onPostPresent();
    int imageIndex() const { return m_imageIndex; }

private:
	Ref<detail::ISwapChain> m_rhiObject;
	Ref<RenderTargetTexture> m_backbuffer;
	//Ref<DepthBuffer> m_depthBuffer;
    int m_imageIndex;
};

namespace detail {

class SwapChainHelper
{
public:
    static void setBackendBufferSize(SwapChain* swapChain, int width, int height);
    static void setOpenGLBackendFBO(SwapChain* swapChain, uint32_t id);
};

} // namespace detail
} // namespace ln
