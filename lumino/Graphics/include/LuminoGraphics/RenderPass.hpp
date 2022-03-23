// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"
#include "GraphicsCommandBuffer.hpp"

namespace ln {
class GraphicsCommandList;
class RenderTargetTexture;
class DepthBuffer;
namespace detail {
class IRenderPass;
}

/**  */
// Note: GraphicsCommandList に set 済みの RenderPass に対して後から setRenderTarget() しても描画結果は正しくなる。
// ただし変更が発生するたびに IRenderPass の共有のためキャッシュを検索しに行くので、若干のオーバーヘッドが発生する。
// 動的に変化させる必要が無ければ事前作成して、set～ はしないようにしておくとパフォーマンスが良くなる。
class RenderPass
    : public Object
	, public IGraphicsResource
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

	/** RenderPass 開始時にクリアする要素を設定します。(Default: None) */
	void setClearFlags(ClearFlags value);

	/** RenderPass 開始時にレンダーターゲットをクリアする場合に使用する色を設定します。(Default: (0, 0, 0, 0)) */
	void setClearColor(const Color& value);

	/** RenderPass 開始時に深度バッファをクリアする場合に使用する Z 値を設定します。(Default: 1.0) */
	void setClearDepth(float value);

	/** RenderPass 開始時にステンシルバッファをクリアする場合に使用するステンシル値を設定します。(Default: 0x00) */
	void setClearStencil(uint8_t value);

	void setClearValues(ClearFlags flags, const Color& color, float depth, uint8_t stencil);

protected:
    void onDispose(bool explicitDisposing) override;
	void onManagerFinalizing() override { dispose(); }
	void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	RenderPass();
    virtual ~RenderPass();

    /** @copydoc create() */
    void init();
	void init(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);

private:
    detail::IRenderPass* resolveRHIObject(GraphicsCommandList* context, bool* outModified);
    detail::IRenderPass* resolveRHIObjectNoClear(GraphicsCommandList* context, bool* outModified);
	void releaseRHI();

	detail::GraphicsManager* m_manager;
    Ref<detail::IRenderPass> m_rhiObject;
    Ref<detail::IRenderPass> m_rhiObjectNoClear;
    std::array<Ref<RenderTargetTexture>, GraphicsCommandList::MaxMultiRenderTargets> m_renderTargets;
	Ref<DepthBuffer> m_depthBuffer;
	ClearFlags m_clearFlags;
	Color m_clearColor;
	float m_clearDepth;
	uint8_t m_clearStencil;
	bool m_dirty;
	bool m_active;

    friend class detail::GraphicsResourceInternal;
    friend class GraphicsCommandList;
};

//namespace detail {
//class GraphicsManager;
//
//class RenderPassPool
//{
//public:
//	struct FindKey
//	{
//		std::array<RenderTargetTexture*, MaxMultiRenderTargets> renderTargets = {};
//		DepthBuffer* depthBuffer = nullptr;
//		ClearFlags clearFlags = ClearFlags::All;
//		Color clearColor = Color(0, 0, 0, 0);
//		float clearDepth = 1.0f;
//		uint8_t clearStencil = 0x00;
//	};
//
//	RenderPassPool(GraphicsManager* manager);
//	RenderPass* findOrCreate(const FindKey& key);
//	static uint64_t computeHash(const FindKey& key);
//
//private:
//	struct Entry
//	{
//		int referenceCount = 0;
//		Ref<IRenderPass> value;
//	};
//
//	GraphicsManager* m_manager;
//	std::unordered_map<uint64_t, Ref<RenderPass>> m_hashMap;
//};
//
//} // namespace detail
} // namespace ln
