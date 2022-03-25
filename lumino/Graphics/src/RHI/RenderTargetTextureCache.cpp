
#include "Internal.hpp"
#include "RenderTargetTextureCache.hpp"

namespace ln {
namespace detail {

//==============================================================================
// TemporaryGraphicsResourceObjectCacheBase

TemporaryGraphicsResourceObjectCacheBase::TemporaryGraphicsResourceObjectCacheBase()
	: m_renderTargetMap()
{
}

TemporaryGraphicsResourceObjectCacheBase::~TemporaryGraphicsResourceObjectCacheBase()
{
}

void TemporaryGraphicsResourceObjectCacheBase::gcObjects()
{
	for (auto itr1 = m_renderTargetMap.begin(); itr1 != m_renderTargetMap.end();)
	{
		auto& pair = *itr1;
		for (auto itr = pair.second.begin(); itr != pair.second.end();)
		{
			if (itr->refCount == 0)
			{
				if (itr->lifeFrames > 0)
				{
					itr->lifeFrames--;

					if (itr->lifeFrames <= 0)
					{
						itr = pair.second.erase(itr);
						continue;
					}
				}
			}

			++itr;
		}

		// remove empty key
		if (pair.second.empty())
		{
			itr1 = m_renderTargetMap.erase(itr1);
		}
		else
		{
			++itr1;
		}
	}
}

Object* TemporaryGraphicsResourceObjectCacheBase::findBase(uint64_t key)
{
	auto itr = m_renderTargetMap.find(key);
	if (itr != m_renderTargetMap.end())
	{
		for (Entry& e : itr->second)
		{
			if (e.refCount == 0)
			{
                // TODO: ここで lifeFrames=0にしなくていいんだっけ？

				e.refCount++;
				return e.resourceObject;
			}
		}
	}
	return nullptr;
}

void TemporaryGraphicsResourceObjectCacheBase::insertBase(uint64_t key, Object* obj)
{
	Entry e;
	e.resourceObject = obj;
	e.refCount = 1;
	e.lifeFrames = 0;
	m_renderTargetMap[key].push_back(e);
}

void TemporaryGraphicsResourceObjectCacheBase::releaseBase(uint64_t key, Object* obj)
{
	auto itr = m_renderTargetMap.find(key);
	if (itr != m_renderTargetMap.end())
	{
		for (Entry& e : itr->second)
		{
			if (e.resourceObject == obj)
			{
				e.refCount--;
				assert(e.refCount >= 0);
				if (e.refCount == 0)
				{
					e.lifeFrames = HoldFrameCount;
				}
				return;
			}
		}
	}

	LN_UNREACHABLE();
}

//==============================================================================
// RenderTargetTextureCacheManager

RenderTargetTextureCacheManager::RenderTargetTextureCacheManager()
{
}

RenderTargetTextureCacheManager::~RenderTargetTextureCacheManager()
{
}

RenderTargetTexture* RenderTargetTextureCacheManager::requestObject(const SizeI& size, TextureFormat format, bool mipmap)
{
	uint64_t key = makeKey(size, format, mipmap);
	
	Object* obj = findBase(key);
	if (obj) return static_cast<RenderTargetTexture*>(obj);

	auto t = makeObject<RenderTargetTexture>(size.width, size.height, format, mipmap, false);
	insertBase(key, t);
	return t;
}

void RenderTargetTextureCacheManager::releaseObject(RenderTargetTexture* rt)
{
	if (LN_REQUIRE(rt)) return;

	uint64_t key = makeKey(SizeI(rt->width(), rt->height()), rt->format(), rt->mipmap());
	releaseBase(key, rt);
}

uint64_t RenderTargetTextureCacheManager::makeKey(const SizeI& size, TextureFormat format, bool mipmap)
{
	uint64_t w = (uint64_t)size.width;
	uint64_t h = (uint64_t)size.height;
	uint64_t f = (uint64_t)format;
	uint64_t m = (mipmap) ? 1 : 0;
	return (m << 40) | (f << 32) | (h << 16) | (w);
}

//==============================================================================
// DepthBufferCacheManager

DepthBufferCacheManager::DepthBufferCacheManager()
{
}

DepthBufferCacheManager::~DepthBufferCacheManager()
{
}

DepthBuffer* DepthBufferCacheManager::requestObject(const SizeI& size)
{
	uint64_t key = makeKey(size);

	Object* obj = findBase(key);
	if (obj) return static_cast<DepthBuffer*>(obj);

	//auto t = makeRef<DepthBuffer>();
	//t->createImpl(m_manager, size, format);
	auto t = makeObject<DepthBuffer>(size.width, size.height);
	insertBase(key, t);
	return t;
}

void DepthBufferCacheManager::releaseObject(DepthBuffer* rt)
{
	if (LN_REQUIRE(rt)) return;

	uint64_t key = makeKey(SizeI(rt->width(), rt->height()));
	releaseBase(key, rt);
}

uint64_t DepthBufferCacheManager::makeKey(const SizeI& size)
{
	uint64_t w = (uint64_t)size.width;
	uint64_t h = (uint64_t)size.height;
	return h << 16 | w;
}

//==============================================================================
// FrameBufferCache

FrameBufferCache::FrameBufferCache(RenderTargetTextureCacheManager* renderTargetCache, DepthBufferCacheManager* depthBufferCache)
	: m_renderTargetCache(renderTargetCache)
	, m_depthBufferCache(depthBufferCache)
{
	m_aliveRenderTargets.reserve(32);
	m_aliveDepthBuffer.reserve(32);
}

FrameBufferCache::~FrameBufferCache()
{
}

Ref<RenderTargetTexture> FrameBufferCache::requestRenderTargetTexture2(const SizeI& size, TextureFormat format, bool mipmap)
{
	Ref<RenderTargetTexture> obj = m_renderTargetCache->requestObject(size, format, mipmap);
	m_aliveRenderTargets.add(obj);
	return obj;
}

Ref<DepthBuffer> FrameBufferCache::requestDepthBuffer2(const SizeI& size)
{
	Ref<DepthBuffer> obj = m_depthBufferCache->requestObject(size);
	m_aliveDepthBuffer.add(obj);
	return obj;
}

void FrameBufferCache::release(RenderTargetTexture* renderTarget)
{
	if (m_aliveRenderTargets.remove(renderTarget)) {
		m_renderTargetCache->releaseObject(renderTarget);
	}
}

void FrameBufferCache::release(DepthBuffer* depthBuffer)
{
	if (m_aliveDepthBuffer.remove(depthBuffer)) {
		m_depthBufferCache->releaseObject(depthBuffer);
	}
}

void FrameBufferCache::gcObjects()
{
	for (auto itr = m_aliveRenderTargets.begin(); itr != m_aliveRenderTargets.end();) {
		if (RefObjectHelper::getReferenceCount((*itr)) == 2) {	// iterate 中のリストと、manger のリストからのみ参照されている
			m_renderTargetCache->releaseObject((*itr));
			itr = m_aliveRenderTargets.erase(itr);
		}
		else {
			++itr;
		}
	}

	for (auto itr = m_aliveDepthBuffer.begin(); itr != m_aliveDepthBuffer.end();) {
		if (RefObjectHelper::getReferenceCount((*itr)) == 2) {	// iterate 中のリストと、manger のリストからのみ参照されている
			m_depthBufferCache->releaseObject((*itr));
			itr = m_aliveDepthBuffer.erase(itr);
		}
		else {
			++itr;
		}
	}

	m_renderTargetCache->gcObjects();
	m_depthBufferCache->gcObjects();
}

//void FrameBufferCache::beginRenderSection()
//{
//	m_sectionLevel++;
//}
//
//void FrameBufferCache::endRenderSection()
//{
//	m_sectionLevel--;
//	if (m_sectionLevel == 0)
//	{
//		for (RenderTargetTexture* obj : m_aliveRenderTargets) {
//			m_renderTargetCache->releaseObject(obj);
//		}
//		for (DepthBuffer* obj : m_aliveDepthBuffer) {
//			m_depthBufferCache->releaseObject(obj);
//		}
//		m_aliveRenderTargets.clear();
//		m_aliveDepthBuffer.clear();
//
//		m_renderTargetCache->gcObjects();
//		m_depthBufferCache->gcObjects();
//	}
//}

} // namespace detail
} // namespace ln
