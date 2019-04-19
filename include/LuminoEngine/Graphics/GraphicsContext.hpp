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

/*
 * グラフィクスデバイスへの描画呼出しを発行するためのクラスです。
 */
class LN_API GraphicsContext
	: public Object
{
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

	/** RenderTarget を設定します。index 0 に設定した場合、Viewport と Scissor 領域は新しい RenderTarget のサイズに合わせて再設定されます。 */
	void setRenderTarget(int index, RenderTargetTexture* value);

	/** RenderTarget を取得します。 */
	RenderTargetTexture* renderTarget(int index) const;

	/** DepthBuffer を設定します。 */
	void setDepthBuffer(DepthBuffer* value);

	/** DepthBuffer を取得します。 */
    DepthBuffer* depthBuffer() const;

	/** ビューポートの矩形を設定します。 */
	void setViewportRect(const Rect& value);

	/** ビューポートの矩形を取得します。 */
	const Rect& viewportRect() const { return m_staging.viewportRect; }

	/** シザー領域の矩形を設定します。 */
	void setScissorRect(const Rect& value);

	/** ビューポートの矩形を取得します。 */
	const Rect& scissorRect() const { return m_staging.scissorRect; }

	void setVertexLayout(VertexLayout* value);
	void setVertexBuffer(int streamIndex, VertexBuffer* value);
	void setIndexBuffer(IndexBuffer* value);
	void setShaderPass(ShaderPass* value);
	void setPrimitiveTopology(PrimitiveTopology value);

	/** デフォルト設定を復元します。 */
	void resetState();

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
	detail::IGraphicsContext* commitState();

LN_CONSTRUCT_ACCESS:
	GraphicsContext();
	virtual ~GraphicsContext();
	void init(detail::IGraphicsContext* context);

LN_INTERNAL_ACCESS:
	virtual void onDispose(bool explicitDisposing) override;

private:

	detail::GraphicsManager* m_manager;
	detail::IGraphicsContext* m_context;

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
		Ref<VertexLayout> VertexLayout;
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
