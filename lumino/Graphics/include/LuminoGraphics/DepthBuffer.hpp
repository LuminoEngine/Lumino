// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"

namespace ln {

/** 深度バッファのクラスです。 */
class DepthBuffer
    : public Object
    , public IGraphicsResource
{
public:
    /**
     * 深度バッファを作成します。
     * @param[in]   width   : 幅 (px 単位)
     * @param[in]   height  : 高さ (px 単位)
     */
    static Ref<DepthBuffer> create(int width, int height);

    /** 一時的な DepthBuffer を取得します。（内容の保証はありません。使用する前に必ずクリアしてください） */
    static Ref<DepthBuffer> getTemporary(int width, int height);

    /** getTemporary で取得した一時的な DepthBuffer を解放します。 */
    static void releaseTemporary(DepthBuffer* depthBuffer);

    /** 幅を取得します。(ピクセル単位) */
    int width() const { return m_size.width; }

    /** 高さを取得します。 (ピクセル単位) */
    int height() const { return m_size.height; }

    bool m_cleared = false;

protected:
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	DepthBuffer();
    virtual ~DepthBuffer();

    /** @copydoc create(int, int) */
    void init(int width, int height);

private:
    detail::RHIResource* resolveRHIObject(GraphicsCommandList* context, bool* outModified);

    detail::GraphicsManager* m_manager;
    Ref<detail::RHIResource> m_rhiObject;
    SizeI m_size;

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln
