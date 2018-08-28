#pragma once

#include "Common.hpp"
#include "GeometryStructs.hpp"
#include "ColorStructs.hpp"
#include "RenderState.hpp"

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
	class GraphicsManager;
	class IGraphicsDeviceContext;
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
	void initialize(detail::PlatformWindow* window, const SizeI& backbufferSize);

LN_INTERNAL_ACCESS:
	detail::ISwapChain* resolveRHIObject() const;

private:
	Ref<detail::ISwapChain> m_rhiObject;
	Ref<RenderTargetTexture> m_colorBuffer;
	Ref<DepthBuffer> m_depthBuffer;
};

class LN_API GraphicsContext
	: public Object
{
public:

	void setBlendState(const BlendStateDesc& value);
	void setRasterizerState(const RasterizerStateDesc& value);
	void setDepthStencilState(const DepthStencilStateDesc& value);
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
	void commitStatus();

	detail::GraphicsManager* m_manager;
	detail::IGraphicsDeviceContext* m_device;

	struct State
	{
		BlendStateDesc blendState;
		RasterizerStateDesc rasterizerState;
		DepthStencilStateDesc depthStencilState;
		std::array<Ref<RenderTargetTexture>, 4> renderTargets;
		Ref<DepthBuffer> depthBuffer;
		Ref<VertexDeclaration> vertexDeclaration;
		std::array<Ref<VertexBuffer>, 4> vertexBuffers;
		Ref<IndexBuffer> indexBuffer;
		Ref<Shader> shader;		// shaderPass owner, for keep reference.
		ShaderPass* shaderPass;

		void reset();
	};

	State m_staging;
	State m_current;
};

} // namespace ln
