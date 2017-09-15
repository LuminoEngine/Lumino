
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
		clear();
	}

public:

	/** コレクションが空であるかを確認します。*/
	bool isEmpty() const
	{
		return m_array.isEmpty();
	}

	/** 格納されている要素の数を取得します。*/
	int getCount() const
	{
		return m_array.getCount();
	}

	/** 末尾に要素を追加します。*/
	//void Add(value_type& item)
	//{
	//	InsertItem(GetCount(), item);
	//}

	/** 末尾に要素を追加します。*/
	void add(const value_type& item)
	{
		insertItem(getCount(), item);
	}

	/** 指定したインデックスの位置に要素を挿入します。*/
	void insert(int index, const value_type& item)
	{
		insertItem(index, item);
	}

	/** 指定したインデックスの位置に要素を挿入します。*/
	//void Insert(int index, const T& item)
	//{
	//	InsertItemConst(index, item);
	//}

	/** item に一致する最初の要素を削除します。(正常に削除された場合は true を返す。要素が見つからなければ false を返す)*/
	bool remove(const T& item)
	{
		int index = m_array.indexOf(item);
		if (index < 0) { return false; }
		removeItem(index);
		return true;
	}

	/** 指定したインデックスにある要素を削除します。*/
	void removeAt(int index)
	{
		removeItem(index);
	}

	/** 全ての要素を削除します。*/
	void clear()
	{
		clearItems();
	}

	/** 配列用のメモリを指定したサイズで確保します。*/
	void reserve(int size) { m_array.reserve(size); }


	/** 配列の要素数を変更します。*/
	void resize(int count)
	{
		if (LN_REQUIRE(count >= 0)) return;

		int d = getCount() - count;
		if (d > 0)
		{
			for (int i = getCount() - 1; i >= count; --i)
			{
				removeItem(i);
			}
		}
		m_array.resize(count);
	}

	/** 指定した要素がこの配列内に存在するかどうかを判断します。*/
	bool contains(const T& item) const
	{
		return m_array.contains(item);
	}

	/** 指定したインデックスがこの配列の境界の範囲外かを確認します。*/
	bool isOutOfRange(int index) const
	{
		return m_array.isOutOfRange(index);
	}

	/**
		@brief		要素を指定した位置から検索し、最初に見つかったインデックスを返します。
		@param[in]	item		: 検索する要素
		@param[in]	startIndex	: 検索を開始するインデックス (省略した場合は先頭から)
		@return		検索した要素が最初に現れた位置。見つからなかった場合は -1。
	*/
	int indexOf(const T& item, int startIndex = 0) const
	{
		return m_array.indexOf(item, startIndex);
	}

	/** 指定したインデックスに要素を設定します。*/
	void getAt(int index, const T& item) { setItem(index, item); }

	/** 指定したインデックスにある要素への参照を取得します。*/
	reference getAt(int index) { return m_array.getAt(index); }

	/** 指定したインデックスにある要素への参照を取得します。*/
	const_reference getAt(int index) const { return m_array.getAt(index); }

	/** 先頭要素の参照を返します。*/
	reference getFront() { return m_array.getFront(); }

	/** 先頭要素の参照を返します。*/
	const_reference getFront() const { return m_array.getFront(); }

	/** 終端要素の参照を返します。*/
	reference getLast() { return m_array.getLast(); }

	/** 終端要素の参照を返します。*/
	const_reference getLast() const { return m_array.getLast(); }

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
	virtual void insertItem(int index, const value_type& item)
	{
		m_array.insert(index, item);
	}

	/** 指定したインデックスの位置に要素を挿入します。*/
	//virtual void InsertItemConst(int index, value_type& item)
	//{
	//	m_array.Insert(index, item);
	//}

	/** 全ての要素を削除します。*/
	virtual void clearItems()
	{
		m_array.clear();
	}

	/** 指定したインデックスにある要素を削除します。*/
	virtual void removeItem(int index)
	{
		m_array.removeAt(index);
	}

	/** 指定したインデックス位置にある要素を置き換えます。*/
	virtual void setItem(int index, const value_type& item)
	{
		m_array.getAt(index, item);
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
