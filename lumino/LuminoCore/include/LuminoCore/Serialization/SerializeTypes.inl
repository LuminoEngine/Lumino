#pragma once
#include "../IO/Path.hpp"
#include "../Base/Array.hpp"
#include "../Base/Uuid.hpp"
#include "../Base/Optional.hpp"

namespace ln {

//template<typename TValue>
template<
	typename TValue,
	typename std::enable_if<!detail::is_lumino_engine_object<TValue>::value, std::nullptr_t>::type = nullptr>
void serialize(Archive& ar, Ref<TValue>& value)
{
    bool isNull = (value == nullptr);
	ar.makeSmartPtrTag(&isNull);

    if (ar.isSaving()) {
        if (!isNull) {
            ar.process(*value.get());
        }
    }
    else {
        if (!isNull) {
            value = makeRef<TValue>();
            ar.process(*value.get());
        }
        else {
            value = nullptr;
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

template<typename TValue>
void serialize(Archive& ar, Array<TValue>& value) {
    int size = 0;
    ar.makeArrayTag(&size);

    if (ar.isLoading()) {
        value.resize(size);
    }

    for (TValue& v : value) {
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

template<typename TValue>
void serialize(Archive& ar, Optional<TValue>& value)
{
	bool hasValue = value.hasValue();
	ar.makeOptionalTag(&hasValue);

	if (ar.isSaving()) {
		if (hasValue) {
			ar.process(value.value());
		}
	}
	else {
		if (hasValue) {
			TValue v;
			ar.process(v);
			value = v;
		}
        else {
            value.reset();
        }
	}
}

template<typename TKey, typename TValue>
void serialize(Archive& ar, std::unordered_map<TKey, TValue>& value)
{
	int size = static_cast<int>(value.size());
	ar.makeMapTag(&size);
	if (ar.isSaving()) {
		for (auto& p : value) {
			ar.makeMapItem(p.first, p.second);
		}
	}
	else {
		value.clear();
		auto hint = value.begin();
		for (int i = 0; i < size; i++) {
			TKey k;
			TValue v;
			ar.makeMapItem(k, v);
			hint = value.emplace_hint(hint, std::move(k), std::move(v));
		}
	}
}

} // namespace ln
