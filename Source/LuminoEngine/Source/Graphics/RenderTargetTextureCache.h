
#pragma once
#include <unordered_map>

LN_NAMESPACE_BEGIN
namespace detail {

class TemporaryGraphicsResourceObjectCacheBase
	: public RefObject
{
public:
	void gcRenderTargets();

protected:
	TemporaryGraphicsResourceObjectCacheBase();
	virtual ~TemporaryGraphicsResourceObjectCacheBase();
	GraphicsResourceObject* findBase(uint64_t key);
	void insertBase(uint64_t key, GraphicsResourceObject* obj);
	void releaseBase(uint64_t key, GraphicsResourceObject* obj);

private:
	struct Entry
	{
		int							refCount;
		int							lifeFrames;
		Ref<GraphicsResourceObject>	resourceObject;
	};
	typedef std::vector<Entry>	RenderTargetList;

	std::unordered_map<uint64_t, RenderTargetList>	m_renderTargetMap;
};

class RenderTargetTextureCache
	: public TemporaryGraphicsResourceObjectCacheBase
{
public:
	RenderTargetTextureCache(GraphicsManager* manager);
	virtual ~RenderTargetTextureCache();

	RenderTargetTexture* requestObject(const SizeI& size, TextureFormat format, int mipLevel);
	void releaseObject(RenderTargetTexture* rt);

private:
	uint16_t makeKey(const SizeI& size, TextureFormat format, int mipLevel);

	GraphicsManager*	m_manager;
};

class DepthBufferCache
	: public TemporaryGraphicsResourceObjectCacheBase
{
public:
	DepthBufferCache(GraphicsManager* manager);
	virtual ~DepthBufferCache();

	DepthBuffer* requestObject(const SizeI& size, TextureFormat format);
	void releaseObject(DepthBuffer* rt);

private:
	uint16_t makeKey(const SizeI& size, TextureFormat format);

	GraphicsManager*	m_manager;
};

class FrameBufferCache
	: public RefObject
{
public:
	RenderTargetTextureCache renderTargetCache;
	DepthBufferCache depthBufferCache;

	FrameBufferCache(GraphicsManager* manager);
	virtual ~FrameBufferCache();
	void beginRenderSection();
	void endRenderSection();

	class ScopedSection
	{
	public:
		FrameBufferCache* m_cache;
		ScopedSection(FrameBufferCache* cache) : m_cache(cache) { m_cache->beginRenderSection(); }
		~ScopedSection() { m_cache->endRenderSection(); }
	};

private:
	int	m_sectionLevel;
};

} // namespace detail
LN_NAMESPACE_END
