
#pragma once
#include "Variant.h"

namespace Lumino
{

#if 0
/**
	@brief		
*/
template<class TObject>
class CoreObjectList
	: public CoreObject
{
public:
	typedef typename Array<TObject*>::iterator			iterator;
	typedef typename Array<TObject*>::const_iterator	const_iterator;
	typedef typename Array<TObject*>::reference			reference;
	typedef typename Array<TObject*>::const_reference	const_reference;

public:
	CoreObjectList() {}
	virtual ~CoreObjectList() { Clear(); }

public:

	/// 要素数を取得する
	int GetCount() const { return m_list.GetCount(); }

	/// 指定インデックスに要素を格納する
	void SetAtBase(int index, TObject* item)
	{
		if (OnItemAdding(item))
		{
			m_list.SetAt(index, item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// 指定インデックスの要素を取得する
	TObject* GetAtBase(int index) const
	{
		return m_list.GetAt(index);
	}

	/// 要素を末尾に追加する
	void Add(TObject* item)
	{
		if (OnItemAdding(item))
		{
			m_list.Add(item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// 全ての要素を削除する
	void Clear()
	{
		LN_FOREACH(TObject* item, m_list) {
			OnItemRemoved(item);	// TODO: erase しながらひとつずつ呼ぶべきかも
			LN_SAFE_RELEASE(item);
		}
		m_list.Clear();
	}

	/// 指定したインデックスの位置に要素を挿入する
	void Insert(int index, TObject* item)
	{
		if (OnItemAdding(item))
		{
			m_list.Insert(index, item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// item と一致する最初の要素を削除する
	void Remove(TObject* item)
	{
		bool b = m_list.Remove(item);
		if (b) {
			OnItemRemoved(item);
			LN_SAFE_RELEASE(item);
		}
	}

	/// 指定したインデックスの要素を削除する
	void RemoveAt(int index)
	{
		TObject* item = m_list.GetAt(index);
		m_list.RemoveAt(index);
		OnItemRemoved(item);
		LN_SAFE_RELEASE(item);
	}

	/// 配列のインデックスとして有効な整数値であるかを確認する
	bool CheckValidIndex(int index) const { return m_list.CheckValidIndex(index); }

public:
	// STL interface
	iterator		begin()			{ return m_list.begin(); }
	const_iterator	begin() const	{ return m_list.begin(); }
	iterator		end()			{ return m_list.end(); }
	const_iterator	end() const		{ return m_list.end(); }

protected:
	virtual bool OnItemAdding(TObject* item) { return true; }
	virtual void OnItemAdded(TObject* item) {}
	virtual void OnItemRemoved(TObject* item) {}

protected:
	Array<TObject*>	m_list;
}; 
#endif

} // namespace Lumino
