
LN_NAMESPACE_BEGIN

//==============================================================================
// List
//==============================================================================
//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
List<T, TAllocator>::List()
	: m_data(NULL)
{
	m_data = ArrayData::getSharedEmpty();
	m_data->addRef();
}
template<typename T, typename TAllocator>
List<T, TAllocator>::List(const List& ary)
{
	m_data = ary.m_data;
	m_data->addRef();
}
template<typename T, typename TAllocator>
List<T, TAllocator>::List(List&& ary)
	: m_data(ary.m_data)
{
	ary.m_data = NULL;
}
template<typename T, typename TAllocator>
List<T, TAllocator>::List(std::initializer_list<T> list)
{
	m_data = LN_NEW ArrayData(1);
	m_data->m_vector.assign(list.begin(), list.end());
}

template <typename T, typename TAllocator> template <class TIter>
List<T, TAllocator>::List(TIter begin, TIter end)
{
	m_data = LN_NEW ArrayData(1);
	m_data->m_vector.template assign<TIter>(begin, end);
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
List<T, TAllocator>::~List()
{
	if (m_data != NULL) {
		m_data->release();
	}
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
List<T, TAllocator>& List<T, TAllocator>::operator=(const List& ary)
{
	m_data->release();
	m_data = ary.m_data;
	m_data->addRef();
	return *this;
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
T& List<T, TAllocator>::operator[] (int index)
{
	checkOutOfRange(index);
	checkDetachShared();
	return m_data->m_vector[index];
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
const T& List<T, TAllocator>::operator[] (int index) const
{
	checkOutOfRange(index);
	return m_data->m_vector[index];
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
void List<T, TAllocator>::checkDetachShared()
{
	if (m_data->isShared())
	{
		auto* newData = LN_NEW ArrayData(m_data->m_vector);	// copy
		m_data->release();
		m_data = newData;
	}
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
void List<T, TAllocator>::checkOutOfRange(int index) const
{
	LN_THROW(!isOutOfRange(index), OutOfRangeException);
}

//==============================================================================
// List::ArrayData
//==============================================================================
//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
void List<T, TAllocator>::ArrayData::release()
{
	--m_refCount;
	if (m_refCount <= 0)
	{
		if (this != getSharedEmpty()) {
			delete this;
		}
	}
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
typename List<T, TAllocator>::ArrayData* List<T, TAllocator>::ArrayData::getSharedEmpty()
{
	static ArrayData sharedEmpty(100);	// 共有オブジェクトは delete しないように、適当に参照カウントを増やしておく
	return &sharedEmpty;
}

LN_NAMESPACE_END
