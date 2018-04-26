
#pragma once
#include "../IO/Path.hpp"
#include "../Base/Uuid.hpp"

namespace ln {

template<typename TValue>
void serialize(Archive& ar, Ref<TValue>& value)
{
	if (ar.isLoading())
	{
		if (!value)
		{
			value = makeRef<TValue>();
		}
		value->serialize(ar);
	}
	else
	{
		if (value)
		{
			value->serialize(ar);
		}
	}
}

template<typename TValue>
void serialize(Archive& ar, List<TValue>& value)
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

inline void serialize(Archive& ar, Path& value)
{
	if (ar.isSaving())
	{
		String str = value.str();
		ar.makeStringTag(&str);
	}
	else
	{
		String str;
		ar.makeStringTag(&str);
		value = Path(str);
	}
}

inline void serialize(Archive& ar, Uuid& value)
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

} // namespace ln
