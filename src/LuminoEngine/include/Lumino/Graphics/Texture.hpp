
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class ITexture; }

class Bitmap2D;


class RenderTargetTexture
	: public GraphicsResource
{
public:
	Ref<Bitmap2D> readData();

	virtual void dispose() override;

LN_CONSTRUCT_ACCESS:
	RenderTargetTexture();
	virtual ~RenderTargetTexture();
	void initialize(int width, int height, TextureFormat requestFormat, bool mipmap);
	void initialize(detail::ITexture* ref);

LN_INTERNAL_ACCESS:
	detail::ITexture* resolveRHIObject();
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
	Ref<detail::ITexture> m_rhiObject;
	//SizeI m_size;
	//TextureFormat m_requestFormat;
	//bool m_mipmap;
};

} // namespace ln
