
#pragma once
#include <Lumino/Base/List.h>
#include <Lumino/Threading/Mutex.h>

LN_NAMESPACE_BEGIN

/**
	@brief		複数のスレッドからの要素の追加・削除を補助する RefObject の配列です。
	@details	要素の追加・削除は AddObject() と RemoveObject() で行います。
				追加または削除されても直ちに配列本体を更新するわけではありません。
				配列本体が別のスレッドでイテレート中であることに備え、追加または削除待ちリストに追加されます。
				Commit() が呼ばれた時点で待ちリストを使用し、配列本体を更新します。
*/
template<typename TRefObj>
class MultiThreadingRefObjectList
{
public:
	typedef List<TRefObj>	ObjectArray;

private:
	ObjectArray	m_objectArray;		///< メインのリスト
	ObjectArray	m_registerList;		///< メインのリストへの追加待ちリスト
	ObjectArray	m_unregisterList;	///< メインのリストからの削除待ちリスト

public:
	MultiThreadingRefObjectList() {}
	~MultiThreadingRefObjectList() { Clear(); }

public:

	/// 追加
	void AddObject(TRefObj obj)
	{
		assert(obj);
		assert(std::find(m_registerList.begin(), m_registerList.end(), obj) == m_registerList.end());	// 既に追加要求されている
		assert(std::find(m_objectArray.begin(), m_objectArray.end(), obj) == m_objectArray.end());		// 現在削除待ちである
		m_registerList.Add(obj);
		obj->AddRef();
	}

	/// 削除
	void RemoveObject(TRefObj obj)
	{
		assert(obj);

		// 登録リストに入ってたらこの時点で削除してしまう
		typename ObjectArray::iterator itr = std::find(m_registerList.begin(), m_registerList.end(), obj);
		if (itr != m_registerList.end())
		{
			m_registerList.erase(itr);
			return;
		}

		assert(std::find(m_unregisterList.begin(), m_unregisterList.end(), obj) == m_unregisterList.end());
		assert(std::find(m_objectArray.begin(), m_objectArray.end(), obj) != m_objectArray.end());
		m_unregisterList.Add(obj);
		obj->AddRef();
	}

	/// 同期
	void Commit()
	{
		typename ObjectArray::iterator itr, end;

		// 追加
		if (!m_registerList.IsEmpty())
		{
			itr = m_registerList.begin();
			end = m_registerList.end();
			for (; itr != end; ++itr)
			{
				m_objectArray.Add(*itr);
				// 参照カウントは m_registerList から外す分と m_objectArray に
				// 追加する分で ±0 なので操作はしない
			}
			m_registerList.Clear();
		}

		// 削除
		if (!m_unregisterList.IsEmpty())
		{
			itr = m_unregisterList.begin();
			end = m_unregisterList.end();
			for (; itr != end; ++itr)
			{
				typename ObjectArray::iterator pos = std::find(m_objectArray.begin(), m_objectArray.end(), (*itr));
				if (pos != m_objectArray.end())
				{
					(*pos)->Release();	// m_unregisterList から外す分
					(*pos)->Release();	// m_registerList から外す分
					m_objectArray.erase(pos);
				}
			}
			m_unregisterList.Clear();
		}
	}

	/// このリストからしか参照されていないオブジェクト (参照カウントが 1 であるオブジェクト) を取り除きます。
	void CollectGC()
	{
		typename ObjectArray::iterator itr = m_objectArray.begin();
		typename ObjectArray::iterator end = m_objectArray.end();
		for (; itr != end;)
		{
			if ((*itr)->GetReferenceCount() == 1)
			{
				(*itr)->Release();
				itr = m_objectArray.erase(itr);
				end = m_objectArray.end();
			}
			else {
				++itr;
			}
		}
	}

	/// 追加予約中配列取得
	ObjectArray& GetRegistOrderObjectArray() { return m_registerList; }

	/// 削除予約中配列取得
	ObjectArray& GetUnegistOrderObjectArray() { return m_unregisterList; }

	/// 配列取得
	ObjectArray& GetObjectArray() { return m_objectArray; }

	/// すべてクリア (終了処理用。Clear() を呼ぶスレッド以外は停止していること)
	void Clear()
	{
		typename ObjectArray::iterator itr, end;

		itr = m_registerList.begin();
		end = m_registerList.end();
		for (; itr != end; ++itr) (*itr)->Release();
		m_registerList.Clear();

		itr = m_unregisterList.begin();
		end = m_unregisterList.end();
		for (; itr != end; ++itr) (*itr)->Release();
		m_unregisterList.Clear();

		itr = m_objectArray.begin();
		end = m_objectArray.end();
		for (; itr != end; ++itr) (*itr)->Release();
		m_objectArray.Clear();
	}
};







// メインスレッドで追加し、更新スレッドで全オブジェクトを更新するような場合に使用する
template<typename TRefObj>
class MultiThreadingInFrameGCList
{
public:
	typedef List<TRefObj*>	ObjectArray;

private:
	ObjectArray	m_objectArray;		///< メインのリスト
	ObjectArray	m_registerList;		///< メインのリストへの追加待ちリスト
	ObjectArray	m_unregisterList;	///< メインのリストからの削除待ちリスト (参照カウントは増やさない)
	ObjectArray	m_removingList;		// メインスレッドで Release して欲しいものリスト
	Mutex		m_mutex;

public:
	MultiThreadingInFrameGCList() {}
	~MultiThreadingInFrameGCList() { Clear(); }

public:

	/// 追加
	void AddObject(TRefObj* obj)
	{
		assert(obj);
		assert(std::find(m_registerList.begin(), m_registerList.end(), obj) == m_registerList.end());	// 既に追加要求されている
		assert(std::find(m_objectArray.begin(), m_objectArray.end(), obj) == m_objectArray.end());		// 現在削除待ちである

		MutexScopedLock lock(m_mutex);
		// 追加リストに入れる
		m_registerList.Add(obj);
		tr::ReflectionHelper::AddRefInternal(obj);
	}

	/// 削除
	void RemoveObject(TRefObj* obj)
	{
		assert(obj);

		MutexScopedLock lock(m_mutex);

		// 登録リストに入ってたらこの時点で削除してしまう
		typename ObjectArray::iterator itr = std::find(m_registerList.begin(), m_registerList.end(), obj);
		if (itr != m_registerList.end())
		{
			m_registerList.erase(itr);
			return;
		}

		assert(std::find(m_unregisterList.begin(), m_unregisterList.end(), obj) == m_unregisterList.end());
		assert(std::find(m_objectArray.begin(), m_objectArray.end(), obj) != m_objectArray.end());

		// 削除してほしいリストに入れる
		m_unregisterList.Add(obj);
		//tr::ReflectionHelper::AddRefInternal(obj);
	}

	/// 同期
	// 更新処理を呼ぶ前に実行する。更新スレッド用
	void Commit(std::function<void(TRefObj* obj)> added, std::function<void(TRefObj* obj)> removed)
	{
		typename ObjectArray::iterator itr, end;

		MutexScopedLock lock(m_mutex);

		// 追加
		if (!m_registerList.IsEmpty())
		{
			itr = m_registerList.begin();
			end = m_registerList.end();
			for (; itr != end; ++itr)
			{
				m_objectArray.Add(*itr);
				// 参照カウントは m_registerList から外す分と m_objectArray に
				// 追加する分で ±0 なので操作はしない
			}
			m_registerList.Clear();
		}

		// 削除
		if (!m_unregisterList.IsEmpty())
		{
			itr = m_unregisterList.begin();
			end = m_unregisterList.end();
			for (; itr != end; ++itr)
			{
				typename ObjectArray::iterator pos = std::find(m_objectArray.begin(), m_objectArray.end(), (*itr));
				if (pos != m_objectArray.end())
				{
					//tr::ReflectionHelper::ReleaseInternal(*pos);	// m_unregisterList から外す分
					// 参照カウントはもうひとつ m_objectArray の分があるが、これはすぐ m_removingList に移すのでデクリメントしない
					m_objectArray.erase(pos);
					m_removingList.Add(*pos);
				}
			}
			m_unregisterList.Clear();
		}

		// Check GC
		{
			itr = m_objectArray.begin();
			end = m_objectArray.end();
			for (; itr != end;)
			{
				// このリストからしか削除されていないものを削除して欲しいオブジェクトリストへ移す
				if ((*itr)->GetReferenceCount() == 0 && tr::ReflectionHelper::GetInternalReferenceCount(*itr) == 1)
				{
					m_removingList.Add(*itr);
					itr = m_objectArray.erase(itr);
					end = m_objectArray.end();
				}
				else
				{
					++itr;
				}
			}
		}
	}

	/// このリストからしか参照されていないオブジェクトを取り除きます。
	// (メインスレッドから呼び出す用)
	void CollectGC()
	{
		MutexScopedLock lock(m_mutex);

		for (TRefObj* obj : m_removingList)
		{
			tr::ReflectionHelper::ReleaseInternal(obj);
		}
		m_removingList.Clear();

		//typename ObjectArray::iterator itr = m_objectArray.begin();
		//typename ObjectArray::iterator end = m_objectArray.end();
		//for (; itr != end;)
		//{
		//	if ((*itr)->GetReferenceCount() == 0)
		//	{
		//		tr::ReflectionHelper::ReleaseInternal(*itr);
		//		itr = m_objectArray.erase(itr);
		//		end = m_objectArray.end();
		//	}
		//	else {
		//		++itr;
		//	}
		//}
	}

	///// 追加予約中配列取得
	//ObjectArray& GetRegistOrderObjectArray() { return m_registerList; }

	///// 削除予約中配列取得
	//ObjectArray& GetUnegistOrderObjectArray() { return m_unregisterList; }

	/// 配列取得
	// (メインスレッドで呼び出してはならない)
	ObjectArray& GetObjectArray() { return m_objectArray; }

	/// すべてクリア (終了処理用。Clear() を呼ぶスレッド以外は停止していること)
	void Clear()
	{
		typename ObjectArray::iterator itr, end;
		MutexScopedLock lock(m_mutex);

		itr = m_registerList.begin();
		end = m_registerList.end();
		for (; itr != end; ++itr) tr::ReflectionHelper::ReleaseInternal(*itr);
		m_registerList.Clear();
		

		itr = m_unregisterList.begin();
		end = m_unregisterList.end();
		for (; itr != end; ++itr) tr::ReflectionHelper::ReleaseInternal(*itr);
		m_unregisterList.Clear();

		itr = m_objectArray.begin();
		end = m_objectArray.end();
		for (; itr != end; ++itr) tr::ReflectionHelper::ReleaseInternal(*itr);
		m_objectArray.Clear();
	}
};
//
//class IAutoChildManager
//{
//LN_PROTECTED_INTERNAL_ACCESS:
//	virtual void 
//};

namespace detail {

template<class TManager, class TObject>
inline void SafeAutoAddChild(TManager* manager, TObject* obj)
{
	if (manager != nullptr)
	{
		manager->AutoAddChild(obj);
	}
}

} // namespace detail

LN_NAMESPACE_END
