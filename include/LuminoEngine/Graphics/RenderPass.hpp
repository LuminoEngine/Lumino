// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"
#include "GraphicsContext.hpp"

namespace ln {
class RenderTargetTexture;
class DepthBuffer;
namespace detail {
class IRenderPass;
}

/**  */
class RenderPass
    : public GraphicsResource
{
public:
	/** RenderTarget を設定します。 */
	void setRenderTarget(int index, RenderTargetTexture* value);

	/** RenderTarget を取得します。 */
	RenderTargetTexture* renderTarget(int index) const;

	/** DepthBuffer を設定します。 */
	void setDepthBuffer(DepthBuffer* value);

	/** DepthBuffer を取得します。 */
	DepthBuffer* depthBuffer() const;

	/** RenderPass 開始時にクリアする要素を設定します。(Default: All) */
	void setClearFlags(ClearFlags value);

	/** RenderPass 開始時にレンダーターゲットをクリアする場合に使用する色を設定します。(Default: (0, 0, 0, 0)) */
	void setClearColor(const Color& value);

	/** RenderPass 開始時に深度バッファをクリアする場合に使用する Z 値を設定します。(Default: 1.0) */
	void setClearDepth(float value);

	/** RenderPass 開始時にステンシルバッファをクリアする場合に使用するステンシル値を設定します。(Default: 0x00) */
	void setClearStencil(uint8_t value);

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	RenderPass();
    virtual ~RenderPass();

    /** @copydoc create() */
    void init();

private:
    detail::IRenderPass* resolveRHIObject(GraphicsContext* context, bool* outModified);
	void releaseRHI();

    Ref<detail::IRenderPass> m_rhiObject;
	std::array<Ref<RenderTargetTexture>, GraphicsContext::MaxMultiRenderTargets> m_renderTargets;
	Ref<DepthBuffer> m_depthBuffer;
	ClearFlags m_clearFlags;
	Color m_clearColor;
	float m_clearDepth;
	uint8_t m_clearStencil;
	bool m_dirty;

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln
