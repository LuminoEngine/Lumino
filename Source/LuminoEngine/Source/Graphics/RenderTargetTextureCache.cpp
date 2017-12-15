/*
	Manager に持たせて、1つの Cache をたくさんのビューが共有すると、GC のタイミングが難しくなる。
	なので、少し無駄はでるけど Viewport 単位で持つ。
	そうしておけば、Viewport::render() の最後で GC すれば良くなる。


	キャッシュの容量と解放タイミングについて
		あるフレームで使われなかったら即解放、だと少し効率が悪いかもしれない。
		例えば頻繁に Enable⇔Disable が切り替えられるポストエフェクトとか。

		このキャッシュの主な用途はポストエフェクトであり、Viewport と同じサイズが
		ダウンキャストに使われる。
*/
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include "RenderTargetTextureCache.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// TemporaryGraphicsResourceObjectCacheBase
//==============================================================================
TemporaryGraphicsResourceObjectCacheBase::TemporaryGraphicsResourceObjectCacheBase()
	: m_renderTargetMap()
{
}

TemporaryGraphicsResourceObjectCacheBase::~TemporaryGraphicsResourceObjectCacheBase()
{
}

void TemporaryGraphicsResourceObjectCacheBase::gcRenderTargets()
{
	for (auto itr1 = m_renderTargetMap.begin(); itr1 != m_renderTargetMap.end();)
	{
		auto& pair = *itr1;
		for (auto itr = pair.second.begin(); itr != pair.second.end();)
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
			else
			{
				// force release (leak objects)
				itr->refCount = 0;
				itr->lifeFrames = 60;
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

GraphicsResourceObject* TemporaryGraphicsResourceObjectCacheBase::findBase(uint64_t key)
{
	auto itr = m_renderTargetMap.find(key);
	if (itr != m_renderTargetMap.end())
	{
		for (Entry& e : itr->second)
		{
			if (e.refCount == 0)
			{
				e.refCount++;
				return e.resourceObject;
			}
		}
	}
	return nullptr;
}

void TemporaryGraphicsResourceObjectCacheBase::insertBase(uint64_t key, GraphicsResourceObject* obj)
{
	Entry e;
	e.resourceObject = obj;
	e.refCount = 1;
	e.lifeFrames = 0;
	m_renderTargetMap[key].push_back(e);
}

void TemporaryGraphicsResourceObjectCacheBase::releaseBase(uint64_t key, GraphicsResourceObject* obj)
{
	auto itr = m_renderTargetMap.find(key);
	if (itr != m_renderTargetMap.end())
	{
		for (Entry& e : itr->second)
		{
			if (e.resourceObject == obj)
			{
				e.refCount--;
				if (e.refCount == 0)
				{
					e.lifeFrames = 60;
				}
				return;
			}
		}
	}

	LN_UNREACHABLE();
}

//==============================================================================
// RenderTargetTextureCache
//==============================================================================
RenderTargetTextureCache::RenderTargetTextureCache(GraphicsManager* manager)
	: m_manager(manager)
{
}

RenderTargetTextureCache::~RenderTargetTextureCache()
{
}

RenderTargetTexture* RenderTargetTextureCache::requestObject(const SizeI& size, TextureFormat format, int mipLevel)
{
	uint64_t key = makeKey(size, format, mipLevel);
	
	GraphicsResourceObject* obj = findBase(key);
	if (obj) return static_cast<RenderTargetTexture*>(obj);

	auto t = Ref<RenderTargetTexture>::makeRef();
	t->createImpl(m_manager, size, mipLevel, format);
	insertBase(key, t);
	return t;
}

void RenderTargetTextureCache::releaseObject(RenderTargetTexture* rt)
{
	if (LN_REQUIRE(rt)) return;

	uint64_t key = makeKey(rt->getSize(), rt->getFormat(), rt->m_mipLevels);
	releaseBase(key, rt);
}

uint16_t RenderTargetTextureCache::makeKey(const SizeI& size, TextureFormat format, int mipLevel)
{
	uint64_t w = (uint64_t)size.width;
	uint64_t h = (uint64_t)size.height;
	uint64_t f = (uint64_t)format;
	uint64_t m = (uint64_t)mipLevel;
	return m << 40 | f << 32 | h << 16 | w;
}

//==============================================================================
// DepthBufferCache
//==============================================================================
DepthBufferCache::DepthBufferCache(GraphicsManager* manager)
	: m_manager(manager)
{
}

DepthBufferCache::~DepthBufferCache()
{
}

DepthBuffer* DepthBufferCache::requestObject(const SizeI& size, TextureFormat format)
{
	uint64_t key = makeKey(size, format);

	GraphicsResourceObject* obj = findBase(key);
	if (obj) return static_cast<DepthBuffer*>(obj);

	auto t = Ref<DepthBuffer>::makeRef();
	t->createImpl(m_manager, size, format);
	insertBase(key, t);
	return t;
}

void DepthBufferCache::releaseObject(DepthBuffer* rt)
{
	if (LN_REQUIRE(rt)) return;

	uint64_t key = makeKey(rt->getSize(), rt->getFormat());
	releaseBase(key, rt);
}

uint16_t DepthBufferCache::makeKey(const SizeI& size, TextureFormat format)
{
	uint64_t w = (uint64_t)size.width;
	uint64_t h = (uint64_t)size.height;
	uint64_t f = (uint64_t)format;
	return f << 32 | h << 16 | w;
}

//==============================================================================
// FrameBufferCache
//==============================================================================
FrameBufferCache::FrameBufferCache(GraphicsManager* manager)
	: renderTargetCache(manager)
	, depthBufferCache(manager)
{
}

FrameBufferCache::~FrameBufferCache()
{
}

void FrameBufferCache::beginRenderSection()
{
	m_sectionLevel++;
}

void FrameBufferCache::endRenderSection()
{
	m_sectionLevel--;
	if (m_sectionLevel == 0)
	{
		renderTargetCache.gcRenderTargets();
		depthBufferCache.gcRenderTargets();
	}
}

} // namespace detail
LN_NAMESPACE_END
