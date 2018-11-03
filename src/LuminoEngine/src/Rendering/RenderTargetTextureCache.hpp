
#pragma once
#include <unordered_map>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/DepthBuffer.hpp>

namespace ln {
namespace detail {

// キーとオブジェクトの対応表を持つ。
// また、オブジェクトが 60 フレーム再リクエストされなかった場合は解放する。
class TemporaryGraphicsResourceObjectCacheBase
	: public RefObject
{
public:
	void gcObjects();

protected:
	TemporaryGraphicsResourceObjectCacheBase();
	virtual ~TemporaryGraphicsResourceObjectCacheBase();
	GraphicsResource* findBase(uint64_t key);
	void insertBase(uint64_t key, GraphicsResource* obj);
	void releaseBase(uint64_t key, GraphicsResource* obj);

private:
	struct Entry
	{
		int							refCount;// こが 0 であれば再利用できる
		int							lifeFrames;	
		Ref<GraphicsResource>	resourceObject;
	};
	typedef std::vector<Entry>	RenderTargetList;

	std::unordered_map<uint64_t, RenderTargetList>	m_renderTargetMap;
};

class RenderTargetTextureCacheManager
	: public TemporaryGraphicsResourceObjectCacheBase
{
public:
	RenderTargetTextureCacheManager();
	virtual ~RenderTargetTextureCacheManager();

	RenderTargetTexture* requestObject(const SizeI& size, TextureFormat format, bool mipmap);
	void releaseObject(RenderTargetTexture* rt);

private:
	uint16_t makeKey(const SizeI& size, TextureFormat format, bool mipmap);

	GraphicsManager*	m_manager;
};

class DepthBufferCacheManager
	: public TemporaryGraphicsResourceObjectCacheBase
{
public:
	DepthBufferCacheManager();
	virtual ~DepthBufferCacheManager();

	DepthBuffer* requestObject(const SizeI& size);
	void releaseObject(DepthBuffer* rt);

private:
	uint16_t makeKey(const SizeI& size);

	GraphicsManager*	m_manager;
};

// RenderTarget, DepthBuffer の Cache の release 忘れを防ぐためのクラス。
// (防ぐというより、ユーザーに使わせるときに「release しなくてもいいよ」仕様にするために使う)
// FrameBufferCache のインスタンスは RenderView ごとに持っておく。
// CacheManager は RenderingManager が持って置き、FrameBufferCache はGCのタイミングをとる。
// beginRenderSection/endRenderSection は RenderView など、あるシーンの描画の起点で呼び出す。
// これは RenderView のネストを考慮して GC するための仕組み。レベル 0 で end するとき、GC を走らせる。
// Editor モードだとどんなタイミングで描画が走るのか予測が難しいが、起点は絶対にあるはずなので、そこで GC をねらう。
class FrameBufferCache
	: public RefObject
{
public:

	FrameBufferCache(RenderTargetTextureCacheManager* renderTargetCache, DepthBufferCacheManager* depthBufferCache);
	virtual ~FrameBufferCache();

	// request したら release で解放する。
	// release しなくても endRenderSection() で解放されるが、都度 release しておくとメモリ効率が良くなる。
	RenderTargetTexture* requestRenderTargetTexture(const SizeI& size, TextureFormat format, bool mipmap);
	DepthBuffer* requestDepthBuffer(const SizeI& size);
	void release(RenderTargetTexture* renderTarget);
	void release(DepthBuffer* depthBuffer);

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
	RenderTargetTextureCacheManager* m_renderTargetCache;
	DepthBufferCacheManager* m_depthBufferCache;
	List<RenderTargetTexture*> m_aliveRenderTargets;
	List<DepthBuffer*> m_aliveDepthBuffer;
	int	m_sectionLevel;
};

} // namespace detail
} // namespace ln
