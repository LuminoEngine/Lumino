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
namespace detail
{

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

RenderTargetTexture* RenderTargetTextureCache::request(const SizeI& size, TextureFormat format, int mipLevel)
{
	uint64_t key = makeKey(size, format, mipLevel);
	
	// 使えるレンダーターゲットを探す
	auto itr = m_renderTargetMap.find(key);
	if (itr != m_renderTargetMap.end())
	{
		for (Entry& e : itr->second)
		{
			if (e.refCount == 0)
			{
				e.refCount++;
				return e.rendertarget;	// 見つかった
			}
		}
	}

	// 見つからなかったら新しく作って map に追加する
	Entry e;
	e.rendertarget = Ref<RenderTargetTexture>::makeRef();
	e.rendertarget->createImpl(m_manager, size, mipLevel, format);
	e.refCount = 1;
	e.lifeFrames = 0;
	m_renderTargetMap[key].push_back(e);
	return e.rendertarget;
}

void RenderTargetTextureCache::release(RenderTargetTexture* rt)
{
	if (LN_REQUIRE(rt)) return;

	uint64_t key = makeKey(rt->getSize(), rt->getFormat(), rt->m_mipLevels);
	auto itr = m_renderTargetMap.find(key);
	if (itr != m_renderTargetMap.end())
	{
		for (Entry& e : itr->second)
		{
			if (e.rendertarget == rt)
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

void RenderTargetTextureCache::gcRenderTargets()
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

uint16_t RenderTargetTextureCache::makeKey(const SizeI& size, TextureFormat format, int mipLevel)
{
	uint64_t w = (uint64_t)size.width;
	uint64_t h = (uint64_t)size.height;
	uint64_t f = (uint64_t)format;
	uint64_t m = (uint64_t)mipLevel;
	return m << 40 | f << 32 | h << 16 | w;
}

} // namespace detail
LN_NAMESPACE_END
