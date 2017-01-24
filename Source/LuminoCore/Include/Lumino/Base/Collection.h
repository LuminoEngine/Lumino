
#pragma once
#include "List.h"

LN_NAMESPACE_BEGIN

template<typename T>
struct CollectionTraits
{
	typedef T& DirectReference;
	typedef const T& ConstDirectReference;
};

template<typename T>
struct CollectionTraits<T*>
{
	typedef T* DirectReference;
	typedef const T* ConstDirectReference;
};

/**
	@brief		コレクションの基本クラス
	@details	Collection は、継承して独自のコレクションを定義することを想定した Array クラスのラッパーです。
*/
template<typename T, typename TAllocator = detail::StlAllocator<T> >
class Collection
{
	LN_DISALLOW_COPY_AND_ASSIGN(Collection);
public:
	typedef List<T>							internal_array;
	typedef typename internal_array::value_type			value_type;
	typedef typename internal_array::difference_type	difference_type;
	typedef typename internal_array::pointer			pointer;
	typedef typename internal_array::const_pointer		const_pointer;
	//typedef typename internal_array::iterator			iterator;
	//typedef typename internal_array::const_iterator		const_iterator;
	typedef typename CollectionTraits<T>::DirectReference			reference;
	typedef typename CollectionTraits<T>::ConstDirectReference	const_reference;
	class iterator;
	class const_iterator;

public:

	/** 空のコレクションを初期化します。*/
	Collection() {}

	/** デストラクタ */
	virtual ~Collection()
	{
		Clear();
	}

public:

	/** コレクションが空であるかを確認します。*/
	bool IsEmpty() const
	{
		return m_array.IsEmpty();
	}

	/** 格納されている要素の数を取得します。*/
	int GetCount() const
	{
		return m_array.GetCount();
	}

	/** 末尾に要素を追加します。*/
	//void Add(value_type& item)
	//{
	//	InsertItem(GetCount(), item);
	//}

	/** 末尾に要素を追加します。*/
	void Add(const value_type& item)
	{
		InsertItem(GetCount(), item);
	}

	/** 指定したインデックスの位置に要素を挿入します。*/
	void Insert(int index, const value_type& item)
	{
		InsertItem(index, item);
	}

	/** 指定したインデックスの位置に要素を挿入します。*/
	//void Insert(int index, const T& item)
	//{
	//	InsertItemConst(index, item);
	//}

	/** item に一致する最初の要素を削除します。(正常に削除された場合は true を返す。要素が見つからなければ false を返す)*/
	bool Remove(const T& item)
	{
		int index = m_array.IndexOf(item);
		if (index < 0) { return false; }
		RemoveItem(index);
		return true;
	}

	/** 指定したインデックスにある要素を削除します。*/
	void RemoveAt(int index)
	{
		RemoveItem(index);
	}

	/** 全ての要素を削除します。*/
	void Clear()
	{
		ClearItems();
	}

	/** 配列用のメモリを指定したサイズで確保します。*/
	void Reserve(int size) { m_array.Reserve(size); }


	/** 配列の要素数を変更します。*/
	void Resize(int count)
	{
		LN_CHECK_ARG(count >= 0);

		int d = GetCount() - count;
		if (d > 0)
		{
			for (int i = GetCount() - 1; i >= count; --i)
			{
				RemoveItem(i);
			}
		}
		m_array.Resize(count);
	}

	/** 指定した要素がこの配列内に存在するかどうかを判断します。*/
	bool Contains(const T& item) const
	{
		return m_array.Contains(item);
	}

	/** 指定したインデックスがこの配列の境界の範囲外かを確認します。*/
	bool IsOutOfRange(int index) const
	{
		return m_array.IsOutOfRange(index);
	}

	/**
		@brief		要素を指定した位置から検索し、最初に見つかったインデックスを返します。
		@param[in]	item		: 検索する要素
		@param[in]	startIndex	: 検索を開始するインデックス (省略した場合は先頭から)
		@return		検索した要素が最初に現れた位置。見つからなかった場合は -1。
	*/
	int IndexOf(const T& item, int startIndex = 0) const
	{
		return m_array.IndexOf(item, startIndex);
	}

	/** 指定したインデックスに要素を設定します。*/
	void SetAt(int index, const T& item) { SetItem(index, item); }

	/** 指定したインデックスにある要素への参照を取得します。*/
	reference GetAt(int index) { return m_array.GetAt(index); }

	/** 指定したインデックスにある要素への参照を取得します。*/
	const_reference GetAt(int index) const { return m_array.GetAt(index); }

	/** 先頭要素の参照を返します。*/
	reference GetFront() { return m_array.GetFront(); }

	/** 先頭要素の参照を返します。*/
	const_reference GetFront() const { return m_array.GetFront(); }

	/** 終端要素の参照を返します。*/
	reference GetLast() { return m_array.GetLast(); }

	/** 終端要素の参照を返します。*/
	const_reference GetLast() const { return m_array.GetLast(); }

	/** 指定したインデックスにある要素への参照を取得します。*/
	//reference operator[] (int index)
	//{
	//	return m_array[index];
	//}

	/** 指定したインデックスにある要素への参照を取得します。*/
	const_reference operator[] (int index) const
	{
		return m_array[index];
	}

	//----------------------------------------------------------------------------
	/** @name STL interface */
	/** @{ */

	iterator		begin()			{ return iterator(m_array.begin()); }
	const_iterator	begin() const	{ return const_iterator(m_array.begin()); }
	iterator		end()			{ return iterator(m_array.end()); }
	const_iterator	end() const		{ return const_iterator(m_array.end()); }

	const_iterator	cbegin() const	{ return const_iterator(m_array.begin()); }
	const_iterator	cend() const	{ return const_iterator(m_array.end()); }

	/** @} */

public:
	class const_iterator
		: public std::iterator<std::random_access_iterator_tag, T>
	{
	public:
		const_iterator() : m_internalItr() {}
		const_iterator(const const_iterator& obj) : m_internalItr(obj.m_internalItr) {}
		const_iterator& operator = (const const_iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		const_iterator& operator += (difference_type offset) { m_internalItr.operator+=(offset); return (*this); }

		bool operator==(const const_iterator& right) const	{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const const_iterator& right) const	{ return m_internalItr != right.m_internalItr; }
		bool operator<(const const_iterator& right) const	{ return m_internalItr < right.m_internalItr; }

		const_reference operator*() const	{ return *m_internalItr; }
		const_pointer operator->() const	{ return m_internalItr.operator->(); }

		const_iterator& operator++()		{ ++m_internalItr; return (*this); }
		const_iterator operator++(int)		{ const_iterator tmp = *this; ++(*this); return tmp; }

		const_iterator& operator--() { --m_internalItr; return (*this); }
		const_iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

		difference_type operator-(const const_iterator& right) const { return (m_internalItr - right.m_internalItr); }
		const_iterator operator+(difference_type offset) const { return const_iterator(m_internalItr + offset); }

	private:
		friend class Collection;
		const_iterator(const typename internal_array::const_iterator& itr) : m_internalItr(itr) {}
		typename internal_array::const_iterator m_internalItr;
	};

	class iterator
		: public std::iterator<std::random_access_iterator_tag, T>
	{
	public:
		iterator() : m_internalItr() {}
		iterator(const iterator& obj) : m_internalItr(obj.m_internalItr) {}
		iterator& operator = (const iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		iterator& operator += (difference_type offset) { m_internalItr.operator+=(offset); return (*this); }

		bool operator==(const iterator& right) const		{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const iterator& right) const		{ return m_internalItr != right.m_internalItr; }
		bool operator<(const iterator& right) const			{ return m_internalItr < right.m_internalItr; }

		reference operator*()	{ return *m_internalItr; }
		pointer operator->()			{ return m_internalItr.operator->(); }

		iterator& operator++()				{ ++m_internalItr; return (*this); }
		iterator operator++(int)			{ iterator tmp = *this; ++(*this); return tmp; }

		iterator& operator--() { --m_internalItr; return (*this); }
		iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

		difference_type operator-(const iterator& right) const { return (m_internalItr - right.m_internalItr); }
		iterator operator+(difference_type offset) const { return iterator(m_internalItr + offset); }

	private:
		friend class Collection;
		iterator(const typename internal_array::iterator& itr) : m_internalItr(itr) {}
		typename internal_array::iterator m_internalItr;
	};

protected:

	/** 指定したインデックスの位置に要素を挿入します。*/
	virtual void InsertItem(int index, const value_type& item)
	{
		m_array.Insert(index, item);
	}

	/** 指定したインデックスの位置に要素を挿入します。*/
	//virtual void InsertItemConst(int index, value_type& item)
	//{
	//	m_array.Insert(index, item);
	//}

	/** 全ての要素を削除します。*/
	virtual void ClearItems()
	{
		m_array.Clear();
	}

	/** 指定したインデックスにある要素を削除します。*/
	virtual void RemoveItem(int index)
	{
		m_array.RemoveAt(index);
	}

	/** 指定したインデックス位置にある要素を置き換えます。*/
	virtual void SetItem(int index, const value_type& item)
	{
		m_array.SetAt(index, item);
	}

	/** 指定したインデックス位置にある要素を置き換えます。*/
	//virtual void SetItemConst(int index, const value_type& item)
	//{
	//	m_array.SetAt(index, item);
	//}

private:
	List<T>	m_array;
};

LN_NAMESPACE_END
