
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class IDepthBuffer; }

class DepthBuffer
	: public GraphicsResource
{
public:
	/** 幅を取得します。(ピクセル単位) */
	int width() const { return m_size.width; }

	/** 高さを取得します。 (ピクセル単位) */
	int height() const { return m_size.height; }

LN_CONSTRUCT_ACCESS:
	DepthBuffer();
	virtual ~DepthBuffer();
	void init(int width, int height);
	virtual void dispose() override;
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

LN_INTERNAL_ACCESS:
	detail::IDepthBuffer* resolveRHIObject();

private:
	Ref<detail::IDepthBuffer> m_rhiObject;
	SizeI m_size;
};

} // namespace ln
