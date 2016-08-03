/*
	Manager に持たせて、1つの Cache をたくさんのビューが共有すると、GC のタイミングが難しくなる。
	なので、少し無駄はでるけど Viewport 単位で持つ。
	そうしておけば、Viewport::Render() の最後で GC すれば良くなる。


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

//------------------------------------------------------------------------------
RenderTargetTextureCache::RenderTargetTextureCache(GraphicsManager* manager)
	: m_manager(manager)
{
}

//------------------------------------------------------------------------------
RenderTargetTextureCache::~RenderTargetTextureCache()
{
}

//------------------------------------------------------------------------------
RefPtr<RenderTarget> RenderTargetTextureCache::RequestRenderTarget(const SizeI& size, TextureFormat format, int mipLevel)
{
	LN_NOTIMPLEMENTED();

	// 検索キーを作る
	uint64_t w = (uint64_t)size.width;
	uint64_t h = (uint64_t)size.height;
	uint64_t f = (uint64_t)format;
	uint64_t m = (uint64_t)mipLevel;
	uint64_t key = m << 40 | f << 32 | h << 16 | w;
	
	// 使えるレンダーターゲットを探す
	RenderTarget* renderTarget = nullptr;
	auto itr = m_renderTargetMap.find(key);
	if (itr != m_renderTargetMap.end())
	{
		for (RenderTarget* rt : itr->second)
		{
			if (rt->GetReferenceCount() == 1)	// Cache からしか参照されていない？
			{
				renderTarget = rt;
				break;
			}
		}
	}

	// 見つからなかったら新しく作って map に追加する
	if (renderTarget == nullptr)
	{
		auto rt = RefPtr<RenderTarget>::MakeRef();
		rt->CreateImpl(m_manager, size, mipLevel, format);
		renderTarget = rt.DetachMove();

		m_renderTargetMap[key].push_back(renderTarget);
	}

	// 現在のフレームで使用されたことをマークする
	renderTarget->m_usedCacheOnFrame = true;

	return RefPtr<RenderTarget>(renderTarget, true);
}

//------------------------------------------------------------------------------
void RenderTargetTextureCache::GCRenderTargets()
{
	LN_NOTIMPLEMENTED();
}

} // namespace detail
LN_NAMESPACE_END
