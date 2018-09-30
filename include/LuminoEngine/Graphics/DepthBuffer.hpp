
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class IDepthBuffer; }

class DepthBuffer
	: public GraphicsResource
{
public:

LN_CONSTRUCT_ACCESS:
	DepthBuffer();
	virtual ~DepthBuffer();
	void initialize(int width, int height);
	virtual void dispose() override;
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

LN_INTERNAL_ACCESS:
	detail::IDepthBuffer* resolveRHIObject();

private:
	Ref<detail::IDepthBuffer> m_rhiObject;
};

} // namespace ln
