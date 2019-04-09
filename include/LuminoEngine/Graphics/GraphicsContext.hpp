#pragma once

#include "Common.hpp"
#include "GeometryStructs.hpp"
#include "ColorStructs.hpp"
#include "RenderState.hpp"

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
	class GraphicsManager;
	class IGraphicsDevice;
}

class LN_API GraphicsContext
	: public Object
{
public:
	void resetState();
	void setBlendState(const BlendStateDesc& value);
	void setRasterizerState(const RasterizerStateDesc& value);
	void setDepthStencilState(const DepthStencilStateDesc& value);
	void setColorBuffer(int index, RenderTargetTexture* value);	// Viewport, Scissor はクリア TODO: やっぱり ColorBuffer は驚く
    RenderTargetTexture* colorBuffer(int index) const;
	void setDepthBuffer(DepthBuffer* value);
    DepthBuffer* depthBuffer() const;
	void setViewportRect(const Rect& value);
	void setScissorRect(const Rect& value);
	void setVertexDeclaration(VertexLayout* value);
	void setVertexBuffer(int streamIndex, VertexBuffer* value);
	void setIndexBuffer(IndexBuffer* value);
	void setShaderPass(ShaderPass* value);
	void setPrimitiveTopology(PrimitiveTopology value);

	void clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);
	void drawPrimitive(int startVertex, int primitiveCount);
	void drawPrimitiveIndexed(int startIndex, int primitiveCount);
	void present(SwapChain* swapChain);
    //void flush();

	// TODO: internal
    void beginCommandRecodingIfNeeded();
    void endCommandRecodingIfNeeded();
    void flushCommandRecoding(RenderTargetTexture* affectRendreTarget);
	// IGraphicsDevice の clear, draw 系の機能を呼び出したい場合はこの戻り値を使うこと。
	// GraphicsContext は変更中のステートをキャッシュするが、それを確実に IGraphicsDevice へ送信した状態にする。
	detail::IGraphicsDevice* commitState();

LN_CONSTRUCT_ACCESS:
	GraphicsContext();
	virtual ~GraphicsContext();
	void init(detail::IGraphicsDevice* device);

LN_INTERNAL_ACCESS:
	virtual void onDispose(bool explicitDisposing) override;

private:

	detail::GraphicsManager* m_manager;
	detail::IGraphicsDevice* m_device;

    // TODO: 途中
    enum ModifiedFlags
    {
        ModifiedFlags_None = 0,
        ModifiedFlags_VertexBuffers = 1 << 2,
        ModifiedFlags_IndexBuffer = 1 << 3,
        ModifiedFlags_ShaderPass = 1 << 4,
        ModifiedFlags_All = 0xFFFFFFFF,
    };

	struct State
	{
		BlendStateDesc blendState;
		RasterizerStateDesc rasterizerState;
		DepthStencilStateDesc depthStencilState;
		std::array<Ref<RenderTargetTexture>, 4> renderTargets;
		Ref<DepthBuffer> depthBuffer;
		Rect viewportRect;
		Rect scissorRect;
		Ref<VertexLayout> vertexDeclaration;
		std::array<Ref<VertexBuffer>, 4> vertexBuffers;
		Ref<IndexBuffer> indexBuffer;
		Ref<Shader> shader;		// shaderPass owner, for keep reference.
		ShaderPass* shaderPass;
		PrimitiveTopology topology;

		void reset();
	};

	State m_staging;
	State m_lastCommit;
    uint32_t m_modifiedFlags;
    bool m_recordingBegan;
};

} // namespace ln
