
#pragma once
#include "../Base/Uuid.h"

LN_NAMESPACE_BEGIN

template<typename TValue>
void serialize(Archive2& ar, Ref<TValue>& value)
{
	if (value.isNull())
	{
		value = newObject<TValue>();
	}

	value->serialize(ar);
}

template<typename TValue>
void serialize(Archive2& ar, List<TValue>& value)
{
	int size = 0;
	ar.makeArrayTag(&size);

	if (ar.isLoading())
	{
		value.resize(size);
	}

	for (TValue& v : value)
	{
		ar.process(v);
	}
}

inline void serialize(Archive2& ar, Uuid& value)
{
	if (ar.isSaving())
	{
		String str = value.toString();
		ar.makeStringTag(&str);
	}
	else
	{
		String str;
		ar.makeStringTag(&str);
		value = Uuid(str);
	}
}

LN_NAMESPACE_END
