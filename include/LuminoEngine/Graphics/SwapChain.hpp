#pragma once

#include "Common.hpp"
#include "GeometryStructs.hpp"

namespace ln {
class VertexDeclaration;
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

class LN_API SwapChain
	: public Object
{
public:

	RenderTargetTexture* colorBuffer() const;
	DepthBuffer* depthBuffer() const;

	virtual void dispose() override;

	void wait();

LN_CONSTRUCT_ACCESS:
	SwapChain();
	virtual ~SwapChain();
	void init(detail::PlatformWindow* window, const SizeI& backbufferSize);

LN_INTERNAL_ACCESS:
	detail::ISwapChain* resolveRHIObject() const;

private:
	Ref<detail::ISwapChain> m_rhiObject;
	Ref<RenderTargetTexture> m_colorBuffer;
	Ref<DepthBuffer> m_depthBuffer;
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
