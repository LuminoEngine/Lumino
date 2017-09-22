
#pragma once
#include <unordered_map>

LN_NAMESPACE_BEGIN
namespace detail
{

// 
class RenderTargetTextureCache
	: public RefObject
{
public:
	RenderTargetTextureCache(GraphicsManager* manager);
	virtual ~RenderTargetTextureCache();

	RenderTargetTexture* request(const SizeI& size, TextureFormat format, int mipLevel);
	void release(RenderTargetTexture* rt);

	void gcRenderTargets();

private:
	uint16_t makeKey(const SizeI& size, TextureFormat format, int mipLevel);

	struct Entry
	{
		int							refCount;
		int							lifeFrames;
		Ref<RenderTargetTexture>	rendertarget;
	};
	typedef std::vector<Entry>	RenderTargetList;

	GraphicsManager*								m_manager;
	std::unordered_map<uint64_t, RenderTargetList>	m_renderTargetMap;
};

} // namespace detail
LN_NAMESPACE_END
