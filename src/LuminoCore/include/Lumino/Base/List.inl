
namespace ln {

//==============================================================================
// List
//==============================================================================
//------------------------------------------------------------------------------
template<typename T>
List<T>::List()
	: m_data()
{
}
template<typename T>
List<T>::List(const List& ary)
	: m_data(ary.m_data)
{
}

template<typename T>
List<T>::List(List&& ary)
	: m_data(std::move(ary.m_data))
{
}

template<typename T>
List<T>::List(std::initializer_list<T> list)
	: m_data(list)
{
}

template <typename T> template <class TIter>
List<T>::List(TIter begin, TIter end)
	: m_data(begin, end)
{
}

//------------------------------------------------------------------------------
template<typename T>
List<T>::~List()
{
}


////------------------------------------------------------------------------------
//template<typename T>
//void List<T>::checkDetachShared()
//{
//	if (m_data->isShared())
//	{
//		auto* newData = LN_NEW ArrayData(m_data);	// copy
//		m_data->release();
//		m_data = newData;
//	}
//}

////------------------------------------------------------------------------------
//template<typename T>
//void List<T>::checkOutOfRange(int index) const
//{
//	LN_REQUIRE(!isOutOfRange(index));
//}
//
////==============================================================================
//// List::ArrayData
////==============================================================================
////------------------------------------------------------------------------------
//template<typename T>
//void List<T>::ArrayData::release()
//{
//	--m_refCount;
//	if (m_refCount <= 0)
//	{
//		if (this != getSharedEmpty()) {
//			delete this;
//		}
//	}
//}
//
////------------------------------------------------------------------------------
//template<typename T>
//typename List<T>::ArrayData* List<T>::ArrayData::getSharedEmpty()
//{
//	static ArrayData sharedEmpty(100);	// 共有オブジェクトは delete しないように、適当に参照カウントを増やしておく
//	return &sharedEmpty;
//}

} // namespace ln
