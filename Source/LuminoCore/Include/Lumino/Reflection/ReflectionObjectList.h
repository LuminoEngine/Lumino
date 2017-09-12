
#pragma once
#include "../Base/Collection.h"
#include "ReflectionObject.h"

LN_NAMESPACE_BEGIN
namespace tr
{

/**
	@brief		
*/
template<typename T>
class ReflectionObjectList
	: public RefObject
	, public Collection<T>
{
public:
	typedef typename Collection<T>::value_type	value_type;

public:
	ReflectionObjectList()
	{}

	virtual ~ReflectionObjectList()
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

private:
};

} // namespace tr

LN_NAMESPACE_END

