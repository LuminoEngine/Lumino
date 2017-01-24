
LN_NAMESPACE_BEGIN

//==============================================================================
// List
//==============================================================================
//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
List<T, TAllocator>::List()
	: m_data(NULL)
{
	m_data = ArrayData::GetSharedEmpty();
	m_data->AddRef();
}
template<typename T, typename TAllocator>
List<T, TAllocator>::List(const List& ary)
{
	m_data = ary.m_data;
	m_data->AddRef();
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
	m_data->m_vector.assign<TIter>(begin, end);
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
List<T, TAllocator>::~List()
{
	if (m_data != NULL) {
		m_data->Release();
	}
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
List<T, TAllocator>& List<T, TAllocator>::operator=(const List& ary)
{
	m_data->Release();
	m_data = ary.m_data;
	m_data->AddRef();
	return *this;
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
T& List<T, TAllocator>::operator[] (int index)
{
	CheckOutOfRange(index);
	CheckDetachShared();
	return m_data->m_vector[index];
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
const T& List<T, TAllocator>::operator[] (int index) const
{
	CheckOutOfRange(index);
	return m_data->m_vector[index];
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
void List<T, TAllocator>::CheckDetachShared()
{
	if (m_data->IsShared())
	{
		auto* newData = LN_NEW ArrayData(m_data->m_vector);	// copy
		m_data->Release();
		m_data = newData;
	}
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
void List<T, TAllocator>::CheckOutOfRange(int index) const
{
	LN_THROW(!IsOutOfRange(index), OutOfRangeException);
}

//==============================================================================
// List::ArrayData
//==============================================================================
//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
void List<T, TAllocator>::ArrayData::Release()
{
	--m_refCount;
	if (m_refCount <= 0)
	{
		if (this != GetSharedEmpty()) {
			delete this;
		}
	}
}

//------------------------------------------------------------------------------
template<typename T, typename TAllocator>
typename List<T, TAllocator>::ArrayData* List<T, TAllocator>::ArrayData::GetSharedEmpty()
{
	static ArrayData sharedEmpty(100);	// 共有オブジェクトは delete しないように、適当に参照カウントを増やしておく
	return &sharedEmpty;
}

LN_NAMESPACE_END
