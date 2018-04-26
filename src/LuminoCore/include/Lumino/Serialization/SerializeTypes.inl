
#pragma once
#include "../IO/Path.hpp"
#include "../Base/Uuid.hpp"
#include "../Base/Variant.hpp"

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

inline void serialize(Archive& ar, Variant& value)
{
	ArchiveNodeType type;
	ar.makeVariantTag(&type);

	if (ar.isSaving())
	{
		switch (value.type())
		{
		case VariantType::Null:
		{
			LN_NOTIMPLEMENTED();
			break;
		}
		case VariantType::Bool:
		{
			auto v = value.get<bool>();
			ar.process(v);
			break;
		}
		case VariantType::Char:
		{
			LN_NOTIMPLEMENTED();
			break;
		}
		case VariantType::Int8:
		{
			auto v = value.get<int8_t>();
			ar.process(v);
			break;
		}
		case VariantType::Int16:
		{
			auto v = value.get<int16_t>();
			ar.process(v);
			break;
		}
		case VariantType::Int32:
		{
			auto v = value.get<int32_t>();
			ar.process(v);
			break;
		}
		case VariantType::Int64:
		{
			auto v = value.get<int64_t>();
			ar.process(v);
			break;
		}
		case VariantType::UInt8:
		{
			auto v = value.get<uint8_t>();
			ar.process(v);
			break;
		}
		case VariantType::UInt16:
		{
			auto v = value.get<uint16_t>();
			ar.process(v);
			break;
		}
		case VariantType::UInt32:
		{
			auto v = value.get<uint32_t>();
			ar.process(v);
			break;
		}
		case VariantType::UInt64:
		{
			auto v = value.get<uint64_t>();
			ar.process(v);
			break;
		}
		case VariantType::Float:
		{
			auto v = value.get<float>();
			ar.process(v);
			break;
		}
		case VariantType::Double:
		{
			auto v = value.get<double>();
			ar.process(v);
			break;
		}
		case VariantType::String:
		{
			auto v = value.get<String>();
			ar.process(v);
			break;
		}
		default:
			LN_UNREACHABLE();
			break;
		}
	}
	else
	{
		switch (type)
		{
		case ln::ArchiveNodeType::Null:
		{
			value.clear();
			break;
		}
		case ln::ArchiveNodeType::Bool:
		{
			bool v;
			ar.process(v);
			value = v;
			break;
		}
		case ln::ArchiveNodeType::Int64:
		{
			int64_t v;
			ar.process(v);
			value = v;
			break;
		}
		case ln::ArchiveNodeType::Double:
		{
			double v;
			ar.process(v);
			value = v;
			break;
		}
		case ln::ArchiveNodeType::String:
		{
			String v;
			ar.process(v);
			value = v;
			break;
		}
		case ln::ArchiveNodeType::Object:
		{
			LN_NOTIMPLEMENTED();
			break;
		}
		case ln::ArchiveNodeType::Array:
		{
			LN_NOTIMPLEMENTED();
			break;
		}
		default:
			LN_UNREACHABLE();
			break;
		}
	}
}

} // namespace ln
