#pragma once
#include <unordered_map>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/GPU/DepthBuffer.hpp>

namespace ln {
namespace detail {

// キーとオブジェクトの対応表を持つ。
// また、オブジェクトが 60 フレーム再リクエストされなかった場合は解放する。
class TemporaryGraphicsResourceObjectCacheBase
	: public RefObject
{
public:
	static const int HoldFrameCount = 60;

	void gcObjects();
	int32_t count() const { return m_renderTargetMap.size(); }

protected:
	TemporaryGraphicsResourceObjectCacheBase();
	virtual ~TemporaryGraphicsResourceObjectCacheBase();
	Object* findBase(uint64_t key);
	void insertBase(uint64_t key, Object* obj);
	void releaseBase(uint64_t key, Object* obj);

private:
	struct Entry
	{
		int							refCount;// こが 0 であれば再利用できる
		int							lifeFrames;	
		Ref<Object>	resourceObject;
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
	uint64_t makeKey(const SizeI& size, TextureFormat format, bool mipmap);

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
	uint64_t makeKey(const SizeI& size);

	GraphicsManager*	m_manager;
};

// RenderTarget, DepthBuffer の Cache の release 忘れを防ぐためのクラス。
// (防ぐというより、ユーザーに使わせるときに「release しなくてもいいよ」仕様にするために使う)
// CacheManager は RenderingManager が持って置き、FrameBufferCache はGCのタイミングをとる。
// beginRenderSection/endRenderSection は RenderView など、あるシーンの描画の起点で呼び出す。
// これは RenderView のネストを考慮して GC するための仕組み。レベル 0 で end するとき、GC を走らせる。
// Editor モードだとどんなタイミングで描画が走るのか予測が難しいが、起点は絶対にあるので、そこで GC をねらう。
class FrameBufferCache
	: public RefObject
{
public:

	FrameBufferCache(RenderTargetTextureCacheManager* renderTargetCache, DepthBufferCacheManager* depthBufferCache);
	virtual ~FrameBufferCache();

	// request したら release で解放する。
	// release しなくても endRenderSection() で解放されるが、都度 release しておくとメモリ効率が良くなる。
	Ref<RenderTargetTexture> requestRenderTargetTexture2(const SizeI& size, TextureFormat format, bool mipmap);
	Ref<DepthBuffer> requestDepthBuffer2(const SizeI& size);
	void release(RenderTargetTexture* renderTarget);
	void release(DepthBuffer* depthBuffer);

	//void beginRenderSection();
	//void endRenderSection();
	void gcObjects();

	//class ScopedSection
	//{
	//public:
	//	FrameBufferCache* m_cache;
	//	ScopedSection(FrameBufferCache* cache) : m_cache(cache) { m_cache->beginRenderSection(); }
	//	~ScopedSection() { m_cache->endRenderSection(); }
	//};

	int aliveRenderTargetCount() const { return m_aliveRenderTargets.size(); }
	int aliveDepthBufferCount() const { return m_aliveDepthBuffer.size(); }

private:
	RenderTargetTextureCacheManager* m_renderTargetCache;
	DepthBufferCacheManager* m_depthBufferCache;
	List<Ref<RenderTargetTexture>> m_aliveRenderTargets;
	List<Ref<DepthBuffer>> m_aliveDepthBuffer;
	int	m_sectionLevel;
};

} // namespace detail
} // namespace ln
