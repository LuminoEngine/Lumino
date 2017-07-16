
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

	Ref<RenderTargetTexture> requestRenderTarget(const SizeI& size, TextureFormat format, int mipLevel);
	void gcRenderTargets();

private:
	typedef std::vector<RenderTargetTexture*>	RenderTargetList;

	GraphicsManager*								m_manager;
	std::unordered_map<uint64_t, RenderTargetList>	m_renderTargetMap;
};

} // namespace detail
LN_NAMESPACE_END
