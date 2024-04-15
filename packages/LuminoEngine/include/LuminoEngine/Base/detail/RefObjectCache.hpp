
#pragma once
#include <list>

namespace ln {
namespace detail {

// RefObject のキャッシュ管理。
// RefObject を作りたいときは、まず先に findObject を呼び出してキャッシュ探す。なければ呼び出し側で作って registerObject()。
// RefObject は普通に makeRef で作ってよい。また、作った直後は registerObject() で登録しておく。
// 定期的に collectUnreferenceObjects() を呼び出すことで、aliveList からしか参照されていない Object を freeList に移動する。
// ※frameUpdate で呼ばなくても、オブジェクトを new する時でも構わない。
// RefObject を release するとき、その直前で releaseObject() に指定して呼び出しておくと、明示的に freeList に入れることができる。
// 最大オブジェクト数とメモリ量は free オブジェクトの条件。alive は影響しない。
// 最大オブジェクト数とメモリ量は or 条件。どちらかが満たされたら古いオブジェクトを削除する。0 の場合は無視。
template<class TKey, class TObject>
class ObjectCache
{
public:
	ObjectCache()
		: m_maxCacheObjectCount(0)
		, m_maxCacheMemory(0)
		, m_aliveList()
		, m_freeList()
		, m_freeObjectMemory(0)
	{}

	void init(uint32_t maxCacheObjectCount = 64, uint32_t maxCacheMemory = 0)
	{
		dispose();
		m_maxCacheObjectCount = maxCacheObjectCount;
		m_maxCacheMemory = maxCacheMemory;
	}

	void dispose()
	{
		m_aliveList.clear();
		m_freeList.clear();
		m_aliveListItr = m_aliveList.begin();
		m_maxCacheObjectCount = 0;
		m_maxCacheMemory = 0;
		m_freeObjectMemory = 0;
	}

	Ref<TObject> findObject(const TKey& key)
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

				m_aliveListItr = m_aliveList.end();

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
			m_aliveListItr = m_aliveList.end();
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

			m_aliveListItr = m_aliveList.end();

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

	void collectUnreferenceObjects(bool all)
	{
		auto stepCollect = [this]() {
			if (RefObjectHelper::getReferenceCount(m_aliveListItr->obj) == 1)	// m_aliveList からのみ参照されている
			{
				if (m_aliveListItr->memorySize > m_maxCacheMemory) {
					// キャッシュに入らない大きなオブジェクトはここで削除
					m_aliveListItr = m_aliveList.erase(m_aliveListItr);
				}
				else {
					m_freeList.push_back(*m_aliveListItr);
					m_freeObjectMemory += m_aliveListItr->memorySize;
					m_aliveListItr = m_aliveList.erase(m_aliveListItr);
					collectOldObject();
				}
			}
			else {
				++m_aliveListItr;
			}
		};

		if (all) {
			m_aliveListItr = m_aliveList.begin();

			for (; m_aliveListItr != m_aliveList.end();)
			{
				stepCollect();
				//if (RefObjectHelper::getReferenceCount(itr->obj) == 1)	// m_aliveList からのみ参照されている
				//{
				//	if (itr->memorySize > m_maxCacheMemory) {
				//		// キャッシュに入らない大きなオブジェクトはここで削除
				//		itr = m_aliveList.erase(itr);
				//	}
				//	else {
				//		m_freeList.push_back(*itr);
				//		m_freeObjectMemory += itr->memorySize;
				//		itr = m_aliveList.erase(itr);
				//		collectOldObject();
				//	}
				//}
				//else {
				//	++itr;
				//}
			}
		}
		else {
			if (m_aliveListItr == m_aliveList.end()) {
				// restart
				m_aliveListItr = m_aliveList.begin();
			}

			if (m_aliveListItr != m_aliveList.end()) {
				stepCollect();
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
			int count = static_cast<int>(m_freeList.size() - m_maxCacheObjectCount);
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

	typedef typename std::list<Entry> EntryList;
	typedef typename std::list<Entry>::iterator EntryListIterator;

	uint32_t m_maxCacheObjectCount;
	uint32_t m_maxCacheMemory;
	EntryList m_aliveList;
	EntryList m_freeList;    // front:oldest, back:newest
	EntryListIterator m_aliveListItr;
	size_t m_freeObjectMemory;
};

} // namespace detail
} // namespace ln

