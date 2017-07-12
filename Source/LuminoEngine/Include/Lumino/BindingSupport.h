
#pragma once
#include <Lumino/Base/List.h>

LN_NAMESPACE_BEGIN
#if 1	// TODO: Scene モジュールを修正したら削除する予定

/**
	@brief	
	@note	ジェネリックの無い言語向けには RefObjectList という型で公開したい。
			
*/
class RefObjectList
	: public RefObject
{
public:
	RefObjectList() {}
	virtual ~RefObjectList() { clear(); }

public:

	/// 要素数を取得する
	int getCount() const { return m_list.getCount(); }

	/// 指定インデックスに要素を格納する
	void SetAtBase(int index, RefObject* item)
	{
		//if (OnItemAdding(item))
		{
			m_list.getAt(index, item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// 指定インデックスの要素を取得する
	RefObject* GetAtBase(int index) const
	{
		return m_list.getAt(index);
	}

	/// 要素を末尾に追加する
	void add(RefObject* item)
	{
		//if (OnItemAdding(item))
		{
			m_list.add(item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// 全ての要素を削除する
	void clear()
	{
		for (RefObject* item : m_list) {
			OnItemRemoved(item);	// TODO: erase しながらひとつずつ呼ぶべきかも
			LN_SAFE_RELEASE(item);
		}
		m_list.clear();
	}

	/// 指定したインデックスの位置に要素を挿入する
	void insert(int index, RefObject* item)
	{
		//if (OnItemAdding(item))
		{
			m_list.insert(index, item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// item と一致する最初の要素を削除する
	void remove(RefObject* item)
	{
		bool b = m_list.remove(item);
		if (b) {
			OnItemRemoved(item);
			LN_SAFE_RELEASE(item);
		}
	}

	/// 指定したインデックスの要素を削除する
	void removeAt(int index)
	{
		RefObject* item = m_list.getAt(index);
		m_list.removeAt(index);
		OnItemRemoved(item);
		LN_SAFE_RELEASE(item);
	}

protected:
	//virtual bool OnItemAdding(RefObject* item) { return true; }
	virtual void OnItemAdded(RefObject* item) {}
	virtual void OnItemRemoved(RefObject* item) {}

protected:
	List<RefObject*>	m_list;
};

#define LN_REF_OBJECT_LIST_DECL(itemType) \
	void getAt(int index, itemType* item); \
	itemType* getAt(int index) const;

#define LN_REF_OBJECT_LIST_IMPL(listType, itemType) \
	void listType::getAt(int index, itemType* item) { RefObjectList::SetAtBase(index, item); } \
	itemType* listType::getAt(int index) const { return static_cast<itemType*>(RefObjectList::GetAtBase(index)); }


template<class TRefObj>
class RefObjectListBase
	: public RefObjectList
{
public:
	class const_iterator
	{
	public:
		typedef List<RefObject*>			internal_list;
		typedef TRefObj*						value_type;
		typedef internal_list::difference_type	difference_type;
		typedef const value_type*				pointer;
		typedef const value_type				reference;

	public:
		const_iterator() : m_internalItr() {}
		const_iterator(const const_iterator& obj) : m_internalItr(obj.m_internalItr) {}
		const_iterator& operator = (const const_iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }

		reference operator*() const		{ return *m_internalItr; }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		const_iterator& operator++()	{ ++m_internalItr; return (*this); }
		const_iterator operator++(int)	{ const_iterator tmp = *this; ++(*this); return tmp; }
		const_iterator& operator--()	{ --m_internalItr; return (*this); }
		const_iterator operator--(int)	{ const_iterator tmp = *this; --(*this); return tmp; }
		const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return m_internalItr[offset]; }
//		bool operator==(const const_iterator& right) const	{ return m_internalItr.operator==(right.m_internalItr); }
//		bool operator!=(const const_iterator& right) const	{ return m_internalItr.operator!=(right.m_internalItr); }
		bool operator<(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class RefObjectListBase;
		const_iterator(const internal_list::const_iterator& itr) : m_internalItr(itr) {}

		internal_list::const_iterator m_internalItr;
	};

	class iterator
	{
	public:
		typedef List<RefObject*>			internal_list;
		typedef TRefObj*						value_type;
		typedef internal_list::difference_type	difference_type;
		typedef value_type*						pointer;
		typedef value_type						reference;

	public:
		iterator() : m_internalItr() {}
		iterator(const iterator& obj) : m_internalItr(obj.m_internalItr) {}
		iterator& operator = (const iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }

		reference operator*() const		{ return static_cast<reference>(*m_internalItr); }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		iterator& operator++()			{ ++m_internalItr; return (*this); }
		iterator operator++(int)		{ iterator tmp = *this; ++(*this); return tmp; }
		iterator& operator--()			{ --m_internalItr; return (*this); }
		iterator operator--(int)		{ iterator tmp = *this; --(*this); return tmp; }
		const_iterator& operator+=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		const_iterator operator+(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		const_iterator& operator-=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		const_iterator operator-(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		difference_type operator-(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return static_cast<reference>(m_internalItr[offset]); }
		//bool operator==(const iterator& right) const		{ return m_internalItr.operator==(right.m_internalItr); }
		//bool operator!=(const iterator& right) const		{ return m_internalItr.operator!=(right.m_internalItr); }
		bool operator<(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class RefObjectListBase;
		iterator(const internal_list::iterator& itr) : m_internalItr(itr) {}

		internal_list::iterator m_internalItr;
	};

public:
	RefObjectListBase() {}
	virtual ~RefObjectListBase() {}

	void getAt(int index, TRefObj* item) { RefObjectList::SetAtBase(index, item); }
	TRefObj* getAt(int index) const { return static_cast<TRefObj*>(RefObjectList::GetAtBase(index)); }

	iterator		begin()			{ return iterator(m_list.begin()); }
	const_iterator	begin() const	{ return const_iterator(m_list.begin()); }
	iterator		end()			{ return iterator(m_list.end()); }
	const_iterator	end() const		{ return const_iterator(m_list.end()); }
};
#endif


class ListObject	// TODO: detail
	: public Object
{
	LN_OBJECT();
protected:
	virtual ~ListObject() = default;

LN_PROTECTED_INTERNAL_ACCESS:	// TODO: friend のほうがいいかな
	virtual int GetCount_Object() = 0;
	virtual void SetAt_Object(int index, Object* item) = 0;
	virtual Object* GetAt_Object(int index) = 0;
	virtual void Add_Object(Object* item) = 0;
	virtual void Clear_Object() = 0;
	virtual void Insert_Object(int index, Object* item) = 0;
	virtual bool Remove_Object(Object* item) = 0;
	virtual void RemoveAt_Object(int index) = 0;
};

template<typename T>
class PrimitiveListObject
	: public ListObject
	, public Collection<T>
{
public:
	PrimitiveListObject() = default;
	virtual ~PrimitiveListObject() = default;
};


template<typename T>
class ObjectList
	: public PrimitiveListObject<T>
{
public:
	typedef typename Collection<T>::value_type	value_type;

public:
	ObjectList()
	{}

	virtual ~ObjectList()
	{
		Collection<T>::clear();
	}

protected:
	virtual void insertItem(int index, const value_type& item) override
	{
		Collection<T>::insertItem(index, item);
		LN_SAFE_ADDREF(item);
	}
	virtual void clearItems() override
	{
		for (auto* item : *this) {
			LN_SAFE_RELEASE(item);
		}
		Collection<T>::clearItems();
	}
	virtual void removeItem(int index) override
	{
		if (Collection<T>::getAt(index) != nullptr) {
			Collection<T>::getAt(index)->release();
		}
		Collection<T>::removeItem(index);
	}
	virtual void setItem(int index, const value_type& item) override
	{
		LN_SAFE_ADDREF(item);
		if (Collection<T>::getAt(index) != nullptr) {
			Collection<T>::getAt(index)->release();
		}
		Collection<T>::setItem(index, item);
	}

	// IObjectListObject interface
	virtual int GetCount_Object() override { return Collection<T>::getCount(); }
	virtual void SetAt_Object(int index, Object* item) override { Collection<T>::getAt(index, static_cast<T>(item)); }
	virtual Object* GetAt_Object(int index) override { return Collection<T>::getAt(index); }
	virtual void Add_Object(Object* item) override { Collection<T>::add(static_cast<T>(item)); }
	virtual void Clear_Object() override { Collection<T>::clear(); }
	virtual void Insert_Object(int index, Object* item) override { auto t = static_cast<T>(item); Collection<T>::insert(index, t); }
	virtual bool Remove_Object(Object* item) override { return Collection<T>::remove(static_cast<T>(item)); }
	virtual void RemoveAt_Object(int index) override { Collection<T>::removeAt(index); }

private:
};

LN_NAMESPACE_END
