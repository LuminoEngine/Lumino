
#pragma once
#include <Lumino/Base/Array.h>

namespace Lumino
{

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
	typedef Array<TRefObj>	ObjectArray;

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
			if ((*itr)->GetRefCount() == 1)
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

} // namespace Lumino
