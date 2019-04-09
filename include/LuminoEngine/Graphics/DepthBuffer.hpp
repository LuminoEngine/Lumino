
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class IDepthBuffer; }

/** 深度バッファのクラスです。 */
class DepthBuffer
	: public GraphicsResource
{
public:
	/** 幅を取得します。(ピクセル単位) */
	int width() const { return m_size.width; }

	/** 高さを取得します。 (ピクセル単位) */
	int height() const { return m_size.height; }

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	DepthBuffer();
	virtual ~DepthBuffer();
	void init(int width, int height);

private:
	detail::IDepthBuffer* resolveRHIObject();

	Ref<detail::IDepthBuffer> m_rhiObject;
	SizeI m_size;

    friend class detail::GraphicsResourceHelper;
};

} // namespace ln
