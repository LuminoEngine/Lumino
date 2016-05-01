
#pragma once
#include <unordered_map>

LN_NAMESPACE_BEGIN
namespace detail
{


class RenderTargetTextureCache
	: public Object
{
public:
	RenderTargetTextureCache(GraphicsManager* manager);
	virtual ~RenderTargetTextureCache();

	RefPtr<RenderTarget> RequestRenderTarget(const Size& size, TextureFormat format, int mipLevel);
	void GCRenderTargets();

private:
	typedef std::vector<RenderTarget*>	RenderTargetList;

	GraphicsManager*								m_manager;
	std::unordered_map<uint64_t, RenderTargetList>	m_renderTargetMap;
};

} // namespace detail
LN_NAMESPACE_END
