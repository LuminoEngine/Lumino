#pragma once

#include "Common.hpp"
#include "GeometryStructs.hpp"
#include "ColorStructs.hpp"

namespace ln {
class VertexDeclaration;
class VertexBuffer;
class IndexBuffer;
class RenderTargetTexture;
class DepthBuffer;
class ShaderPass;

namespace detail {
	class PlatformWindow;
	class GraphicsManager;
	class IGraphicsDeviceContext;
	class ISwapChain;
}


class LN_API SwapChain
	: public Object
{
public:
	virtual void dispose();

	RenderTargetTexture* colorBuffer() const;

LN_CONSTRUCT_ACCESS:
	SwapChain();
	virtual ~SwapChain();
	void initialize(detail::PlatformWindow* window, const SizeI& backbufferSize);

LN_INTERNAL_ACCESS:
	detail::ISwapChain* resolveRHIObject() const;

private:
	Ref<detail::ISwapChain> m_rhiObject;
	Ref<RenderTargetTexture> m_colorBuffer;
};

class LN_API GraphicsContext
	: public Object
{
public:




	void setColorBuffer(int index, RenderTargetTexture* value);
	void setDepthBuffer(DepthBuffer* value);
	void setVertexDeclaration(VertexDeclaration* value);
	void setVertexBuffer(int streamIndex, VertexBuffer* value);
	void setIndexBuffer(IndexBuffer* value);
	void setShaderPass(ShaderPass* value);

	void clear(ClearFlags flags, const Color& color, float z, uint8_t stencil);
	void drawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);
	void drawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);
	void present(SwapChain* swapChain);


LN_CONSTRUCT_ACCESS:
	GraphicsContext();
	virtual ~GraphicsContext();
	void initialize(detail::IGraphicsDeviceContext* device);

LN_INTERNAL_ACCESS:
	virtual void dispose();

private:
	detail::GraphicsManager* m_manager;
	detail::IGraphicsDeviceContext* m_device;
};

} // namespace ln
