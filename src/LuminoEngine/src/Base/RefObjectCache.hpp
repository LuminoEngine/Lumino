#pragma once

namespace ln {
namespace detail {

// RefObject のキャッシュ管理。
// RefObject を作りたいときは、まず先に findObject を呼び出してキャッシュ探す。なければ呼び出し側で作って registerObject()。
// RefObject は普通に makeRef で作ってよい。また、作った直後は registerObject() で登録しておく。
// RefObject を release するとき、その直前で releaseObject() に渡しておく。
// 最大オブジェクト数とメモリ量は free オブジェクトの条件。alive は影響しない。
// 最大オブジェクト数とメモリ量は or 条件。どちらかが満たされたら古いオブジェクトを削除する。0 の場合は無視。
template<class TKey, class TObject>
class ObjectCache
{
public:
	ObjectCache(uint32_t maxCacheObjectCount = 64, uint32_t maxCacheMemory = 0)
		: m_maxCacheObjectCount(maxCacheObjectCount)
		, m_maxCacheMemory(maxCacheMemory)
		, m_aliveList()
		, m_freeList()
		, m_freeObjectMemory(0)
	{}

	void dispose()
	{
		m_aliveList.clear();
		m_freeList.clear();
		m_maxCacheObjectCount = 0;
		m_maxCacheMemory = 0;
		m_freeObjectMemory = 0;
	}

	TObject* findObject(const TKey& key)
	{
		if (LN_REQUIRE(!isDisposed())) return nullptr;

		for (auto itr = m_aliveList.begin(); itr != m_aliveList.end(); ++itr) {
			if (itr->key == key) {
				return itr->obj;
			}
		}

		for (auto itr = m_freeList.begin(); itr != m_freeList.end(); ++itr) {
			if (itr->key == key) {
				m_freeObjectMemory -= itr->memorySize;
				m_aliveList.splice(m_aliveList.end(), std::move(m_freeList), itr);	// move to tail

				return m_aliveList.back().obj;
			}
		}

		return nullptr;
	}

	void registerObject(const TKey& key, TObject* obj, uint32_t memorySize = 0)
	{
		if (LN_REQUIRE(!isDisposed())) return;

		if (obj) {
			m_aliveList.push_back({ key, obj, memorySize });
		}
	}

	void releaseObject(TObject* obj)
	{
		if (LN_REQUIRE(!isDisposed())) return;

		if (obj) {
			auto itr = m_aliveList.begin();
			for (; itr != m_aliveList.end(); ++itr) {
				if (itr->obj == obj) {
					break;
				}
			}
			if (LN_REQUIRE(itr != m_aliveList.end())) return;   // not contained m_aliveList

			if (itr->memorySize > m_maxCacheMemory) {
				// キャッシュに入らない大きなオブジェクトはここで削除
				m_aliveList.erase(itr);
			}
			else {
				m_freeList.splice(m_freeList.end(), std::move(m_aliveList), itr);
				m_freeObjectMemory += itr->memorySize;

				collectOldObject();
			}
		}
	}

	std::vector<TObject*> aliveObjects() const
	{
		std::vector<TObject*> result;
		for (auto& e : m_aliveList) result.push_back(e.obj);
		return result;
	}

	std::vector<TObject*> freeObjects() const
	{
		std::vector<TObject*> result;
		for (auto& e : m_freeList) result.push_back(e.obj);
		return result;
	}

private:
	bool isDisposed() const
	{
		return m_maxCacheObjectCount == 0 && m_maxCacheMemory == 0;
	}

	void collectOldObject()
	{
		if (m_maxCacheObjectCount > 0)
		{
			int count = m_freeList.size() - m_maxCacheObjectCount;
			if (count > 0) {
				for (int i = 0; i < count; i++) {
					m_freeList.pop_front();
				}
			}
		}

		if (m_maxCacheMemory > 0)
		{
			while (!m_freeList.empty() && m_freeObjectMemory > m_maxCacheMemory)
			{
				m_freeObjectMemory -= m_freeList.front().memorySize;
				m_freeList.pop_front();
			}
		}
	}

	struct Entry
	{
		TKey key;
		Ref<TObject> obj;
		uint32_t memorySize;
	};

	uint32_t m_maxCacheObjectCount;
	uint32_t m_maxCacheMemory;
	std::list<Entry> m_aliveList;
	std::list<Entry> m_freeList;    // front:oldest, back:newest
	size_t m_freeObjectMemory;
};

} // namespace detail
} // namespace ln

