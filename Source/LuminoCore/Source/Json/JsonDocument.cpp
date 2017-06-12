
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/IO/StringReader.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/IO/StreamWriter.h>
#include <Lumino/IO/StringWriter.h>
#include <Lumino/Json/JsonHandler.h>
#include <Lumino/Json/JsonReader.h>
#include <Lumino/Json/JsonWriter.h>
#include <Lumino/Json/JsonDocument.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// JsonDocument
//==============================================================================

//------------------------------------------------------------------------------
void JsonDocument::Parse(const String& text)
{
	StringReader textReader(text);
	Parse(&textReader);
}
void JsonDocument::Parse(const TCHAR* text, int len)
{
	if (LN_CHECK_ARG(text != nullptr)) return;

	StringReader textReader(String(text, (len < 0) ? (int)StringTraits::tcslen(text) : len));
	Parse(&textReader);
}

//------------------------------------------------------------------------------
void JsonDocument::Parse(TextReader* textReader)
{
	JsonDOMHandler handler(this);
	JsonReader reader(&handler);
	reader.Parse(textReader);
	handler.Build();
}



//==============================================================================
// JsonHelper
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
bool JsonHelper::IsValueType(JsonToken type)
{
	return
		type == JsonToken::Null ||
		type == JsonToken::Boolean ||
		type == JsonToken::Int32 ||
		type == JsonToken::Int64 ||
		type == JsonToken::Float ||
		type == JsonToken::Double ||
		type == JsonToken::String;
}

//------------------------------------------------------------------------------
bool JsonHelper::IsValueType(JsonValueType type)
{
	return
		type == JsonValueType::Null ||
		type == JsonValueType::Bool ||
		type == JsonValueType::Int32 ||
		type == JsonValueType::Int64 ||
		type == JsonValueType::Float ||
		type == JsonValueType::Double ||
		type == JsonValueType::String;
}

//------------------------------------------------------------------------------
JsonParseResult JsonHelper::LoadElement(JsonDocument2* doc, JsonReader2* reader, JsonElement2** outElement)
{
	if (LN_CHECK_ARG(doc != nullptr)) return JsonParseResult::Error;
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;
	if (LN_CHECK_ARG(outElement != nullptr)) return JsonParseResult::Error;

	JsonToken type = reader->GetTokenType();
	if (type == JsonToken::StartObject)
	{
		auto* value = doc->NewElement<JsonObject2>();
		JsonParseResult result = value->load(reader);
		if (result != JsonParseResult::Success) return result;
		*outElement = value;
	}
	else if (type == JsonToken::StartArray)
	{
		auto* value = doc->NewElement<JsonArray2>();
		JsonParseResult result = value->load(reader);
		if (result != JsonParseResult::Success) return result;
		*outElement = value;
	}
	else if (detail::JsonHelper::IsValueType(type))
	{
		auto* value = doc->NewElement<JsonValue2>();
		JsonParseResult result = value->load(reader);
		if (result != JsonParseResult::Success) return result;
		*outElement = value;
	}
	else
	{
		if (LN_CHECK(0, InvalidFormatException)) return JsonParseResult::Error;
	}
	return JsonParseResult::Success;
}

} // namespace detail

//==============================================================================
// JsonElement2
//==============================================================================
//------------------------------------------------------------------------------
JsonElement2::JsonElement2(JsonDocument2* owner)
	: m_ownerDoc(owner)
	, m_type(JsonValueType::Null)
{
}

//------------------------------------------------------------------------------
JsonElement2::~JsonElement2()
{
}

//------------------------------------------------------------------------------
SerializationValueType JsonElement2::getSerializationValueType() const
{
	switch (getType())
	{
	case JsonValueType::Null: return SerializationValueType::Null;
	case JsonValueType::Bool: return SerializationValueType::Bool;
	case JsonValueType::Int32: return SerializationValueType::Int32;
	case JsonValueType::Int64: return SerializationValueType::Int64;
	case JsonValueType::Float: return SerializationValueType::Float;
	case JsonValueType::Double: return SerializationValueType::Double;
	case JsonValueType::String: return SerializationValueType::String;
	default:
		LN_UNREACHABLE();
		return SerializationValueType::Null;
	}
}

//==============================================================================
// JsonObject2
//==============================================================================
//------------------------------------------------------------------------------
JsonValue2::JsonValue2(JsonDocument2* ownerDoc)
	: JsonElement2(ownerDoc)
{
}

//------------------------------------------------------------------------------
JsonValue2::~JsonValue2()
{
	CheckRelease();
}

//------------------------------------------------------------------------------
void JsonValue2::SetNull()
{
	CheckRelease();
	SetType(JsonValueType::Null);
}

//------------------------------------------------------------------------------
void JsonValue2::SetBool(bool value)
{
	CheckRelease();
	SetType(JsonValueType::Bool);
	m_bool = value;
}

//------------------------------------------------------------------------------
void JsonValue2::SetInt32(int32_t value)
{
	CheckRelease();
	SetType(JsonValueType::Int32);
	m_int32 = value;
}

//------------------------------------------------------------------------------
void JsonValue2::SetInt64(int64_t value)
{
	CheckRelease();
	SetType(JsonValueType::Int64);
	m_int64 = value;
}

//------------------------------------------------------------------------------
void JsonValue2::SetFloat(float value)
{
	CheckRelease();
	SetType(JsonValueType::Float);
	m_float = value;
}

//------------------------------------------------------------------------------
void JsonValue2::SetDouble(double value)
{
	CheckRelease();
	SetType(JsonValueType::Double);
	m_double = value;
}

//------------------------------------------------------------------------------
void JsonValue2::setString(const StringRef& value)
{
	CheckRelease();
	SetType(JsonValueType::String);
	m_stringCore = LN_NEW ln::detail::GenericStringCore<TCHAR>();
	m_stringCore->assign(value.getBegin(), value.getLength());
}

//------------------------------------------------------------------------------
bool JsonValue2::isNull() const
{
	return getType() == JsonValueType::Null;
}

//------------------------------------------------------------------------------
bool JsonValue2::GetBool() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Bool)) return false;
	return m_bool;
}

//------------------------------------------------------------------------------
int32_t JsonValue2::GetInt32() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Int32)) return 0;
	return m_int32;
}

//------------------------------------------------------------------------------
int64_t JsonValue2::GetInt64() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Int64)) return 0;
	return m_int64;
}

//------------------------------------------------------------------------------
float JsonValue2::GetFloat() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Float)) return 0;
	return m_float;
}

//------------------------------------------------------------------------------
double JsonValue2::GetDouble() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Double)) return 0;
	return m_double;
}

//------------------------------------------------------------------------------
String JsonValue2::getString() const
{
	String str;
	ln::detail::StringHelper::attachStringCore(&str, m_stringCore);
	return str;
}

//------------------------------------------------------------------------------
void JsonValue2::CheckRelease()
{
	if (getType() == JsonValueType::String)
	{
		m_stringCore->release();
	}
	SetType(JsonValueType::Null);
}

//------------------------------------------------------------------------------
void JsonValue2::OnSave(JsonWriter* writer)
{
	if (LN_CHECK_ARG(writer != nullptr)) return;
	switch (getType())
	{
		case JsonValueType::Null:
			writer->WriteNull();
			break;
		case JsonValueType::Bool:
			writer->WriteBool(m_bool);
			break;
		case JsonValueType::Int32:
			writer->WriteInt32(m_int32);
			break;
		case JsonValueType::Int64:
			writer->WriteInt64(m_int64);
			break;
		case JsonValueType::Float:
			writer->WriteFloat(m_float);
			break;
		case JsonValueType::Double:
			writer->WriteDouble(m_double);
			break;
		case JsonValueType::String:
			writer->WriteString(m_stringCore->c_str(), m_stringCore->length());
			break;
		default:
			LN_UNREACHABLE();
			break;
	}
}

//------------------------------------------------------------------------------
JsonParseResult JsonValue2::OnLoad(JsonReader2* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;

	switch (reader->GetTokenType())
	{
		case JsonToken::Int32:
			SetInt32(reader->GetInt32Value());
			break;
		case JsonToken::Int64:
			SetInt64(reader->GetInt64Value());
			break;
		case JsonToken::Float:
			SetFloat(reader->GetFloatValue());
			break;
		case JsonToken::Double:
			SetDouble(reader->GetDoubleValue());
			break;
		case JsonToken::Null:
			SetNull();
			break;
		case JsonToken::Boolean:
			SetBool(reader->GetBoolValue());
			break;
		case JsonToken::String:
			setString(reader->getValue());
			break;
		default:
			LN_UNREACHABLE();
			break;
	}
	return JsonParseResult::Success;
}

//==============================================================================
// JsonObject2
//==============================================================================

//------------------------------------------------------------------------------
JsonArray2::JsonArray2(JsonDocument2* ownerDoc)
	: JsonElement2(ownerDoc)
{
	SetType(JsonValueType::Array);
}

//------------------------------------------------------------------------------
JsonArray2::~JsonArray2()
{
}

//------------------------------------------------------------------------------
void JsonArray2::AddNull()
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetNull();
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddBool(bool value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetBool(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddInt32(int32_t value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetInt64(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddInt64(int64_t value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetInt64(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddFloat(float value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetFloat(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddDouble(double value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetDouble(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddString(const StringRef& value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->setString(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
JsonArray2* JsonArray2::AddArray()
{
	auto ptr = GetOwnerDocument()->NewElement<JsonArray2>();
	m_itemList.add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
JsonObject2* JsonArray2::addObject()
{
	auto ptr = GetOwnerDocument()->NewElement<JsonObject2>();
	m_itemList.add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
void JsonArray2::OnSave(JsonWriter* writer)
{
	if (LN_CHECK_ARG(writer != nullptr)) return;
	writer->WriteStartArray();
	for (JsonElement2* item : m_itemList)
	{
		item->save(writer);
	}
	writer->WriteEndArray();
}

//------------------------------------------------------------------------------
JsonParseResult JsonArray2::OnLoad(JsonReader2* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;

	// この時点で reader は StartArray('[') を指している

	while (reader->read())
	{
		if (reader->GetTokenType() == JsonToken::EndArray) return JsonParseResult::Success;	// end scope

		// member value
		JsonElement2* element;
		JsonParseResult result = detail::JsonHelper::LoadElement(GetOwnerDocument(), reader, &element);
		if (result != JsonParseResult::Success) return result;
		m_itemList.add(element);
	}

	return JsonParseResult::Success;
}

//------------------------------------------------------------------------------
void JsonArray2::addSerializeItemValue(SerializationValueType type, const void* value)
{
	switch (type)
	{
	case SerializationValueType::Null: AddNull(); break;
	case SerializationValueType::Bool: AddBool(*static_cast<const bool*>(value)); break;
	case SerializationValueType::Int8: AddInt32(*static_cast<const int8_t*>(value)); break;
	case SerializationValueType::Int16: AddInt32(*static_cast<const int16_t*>(value)); break;
	case SerializationValueType::Int32: AddInt32(*static_cast<const int32_t*>(value)); break;
	case SerializationValueType::Int64: AddInt64(*static_cast<const int64_t*>(value)); break;
	case SerializationValueType::UInt8: AddInt32(*static_cast<const uint8_t*>(value)); break;
	case SerializationValueType::UInt16: AddInt32(*static_cast<const uint16_t*>(value)); break;
	case SerializationValueType::UInt32: AddInt32(*static_cast<const uint32_t*>(value)); break;
	case SerializationValueType::UInt64: AddInt64(*static_cast<const uint64_t*>(value)); break;
	case SerializationValueType::Float: AddFloat(*static_cast<const float*>(value)); break;
	case SerializationValueType::Double: AddDouble(*static_cast<const double*>(value)); break;
	case SerializationValueType::String: AddString(*static_cast<const String*>(value)); break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

//------------------------------------------------------------------------------
ISerializeElement* JsonArray2::addSerializeItemNewArray()
{
	return AddArray();
}

//------------------------------------------------------------------------------
ISerializeElement* JsonArray2::addSerializeItemNewObject()
{
	return addObject();
}

//==============================================================================
// JsonObject2
//==============================================================================

//------------------------------------------------------------------------------
JsonObject2::JsonObject2(JsonDocument2* ownerDoc)
	: JsonElement2(ownerDoc)
{
	SetType(JsonValueType::Object);
}

//------------------------------------------------------------------------------
JsonObject2::~JsonObject2()
{
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberNull(const StringRef& name)
{
	getValue(name)->SetNull();
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberBool(const StringRef& name, bool value)
{
	getValue(name)->SetBool(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberInt32(const StringRef& name, int32_t value)
{
	getValue(name)->SetInt32(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberInt64(const StringRef& name, int64_t value)
{
	getValue(name)->SetInt64(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberFloat(const StringRef& name, float value)
{
	getValue(name)->SetFloat(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberDouble(const StringRef& name, double value)
{
	getValue(name)->SetDouble(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberString(const StringRef& name, const StringRef& value)
{
	getValue(name)->setString(value);
}

//------------------------------------------------------------------------------
JsonArray2* JsonObject2::AddMemberArray(const StringRef& name)
{
	Member* m = m_memberList.find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || m->value->getType() != JsonValueType::Array)
	{
		auto* ptr = GetOwnerDocument()->NewElement<JsonArray2>();
		m_memberList.add({ name, ptr });
		return ptr;
	}
	return static_cast<JsonArray2*>(m->value);
}

//------------------------------------------------------------------------------
JsonObject2* JsonObject2::AddMemberObject(const StringRef& name)
{
	Member* m = m_memberList.find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || m->value->getType() != JsonValueType::Object)
	{
		auto* ptr = GetOwnerDocument()->NewElement<JsonObject2>();
		m_memberList.add({ name, ptr });
		return ptr;
	}
	return static_cast<JsonObject2*>(m->value);
}

//------------------------------------------------------------------------------
JsonElement2* JsonObject2::find(const StringRef& name) const
{
	Member* m = m_memberList.find([name](const Member& m) { return m.name == name; });
	if (m == nullptr) return nullptr;
	return m->value;
}

//------------------------------------------------------------------------------
void JsonObject2::OnSave(JsonWriter* writer)
{
	if (LN_CHECK_ARG(writer != nullptr)) return;

	writer->WriteStartObject();

	for (auto& m : m_memberList)
	{
		writer->WritePropertyName(m.name.c_str(), m.name.getLength());
		m.value->save(writer);
	}

	writer->WriteEndObject();
}

//------------------------------------------------------------------------------
JsonParseResult JsonObject2::OnLoad(JsonReader2* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;

	// この時点で reader は StartObject('{') を指している

	while (reader->read())
	{
		if (reader->GetTokenType() == JsonToken::EndObject) return JsonParseResult::Success;	// end scope

		// member name
		if (reader->GetTokenType() != JsonToken::PropertyName) return JsonParseResult::Error;
		String name = reader->getValue();

		// member value
		if (!reader->read()) return JsonParseResult::Error;
		JsonElement2* element;
		JsonParseResult result = detail::JsonHelper::LoadElement(GetOwnerDocument(), reader, &element);
		if (result != JsonParseResult::Success) return result;
		m_memberList.add({ name, element });
	}

	return JsonParseResult::Success;
}

//------------------------------------------------------------------------------
JsonValue2* JsonObject2::getValue(const StringRef& name)
{
	Member* m = m_memberList.find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || !detail::JsonHelper::IsValueType(m->value->getType()))
	{
		auto* ptr = GetOwnerDocument()->NewElement<JsonValue2>();
		m_memberList.add({ name, ptr });
		return ptr;
	}
	return static_cast<JsonValue2*>(m->value);
}

//------------------------------------------------------------------------------
void JsonObject2::setValueInt32(const StringRef& name, int32_t value) { AddMemberInt32(name, value); }
void JsonObject2::setValueString(const StringRef& name, const String& value) { AddMemberString(name, value); }
ISerializeElement* JsonObject2::addObject(const StringRef& name)  { return AddMemberObject(name); }
bool JsonObject2::tryGetValueInt32(const StringRef& name, int32_t* outValue)
{
	auto* v = find(name);
	if (v == nullptr) return false;
	if (v->getType() != JsonValueType::Int32) return false;
	*outValue = static_cast<JsonValue2*>(v)->GetInt32();
	return true;
}
bool JsonObject2::tryGetValueString(const StringRef& name, String* outValue)
{
	auto* v = find(name);
	if (v == nullptr) return false;
	if (v->getType() != JsonValueType::String) return false;
	*outValue = static_cast<JsonValue2*>(v)->getString();
	return true;
}
bool JsonObject2::tryGetObject(const StringRef& name, ISerializeElement** outValue)
{
	auto* v = find(name);
	if (v == nullptr) return false;
	if (v->getType() != JsonValueType::Object) return false;
	*outValue = static_cast<JsonObject2*>(v);
	return true;
}
bool JsonObject2::tryGetArray(const StringRef& name, ISerializeElement** outValue)
{
	auto* v = find(name);
	if (v == nullptr) return false;
	if (v->getType() != JsonValueType::Array) return false;
	*outValue = static_cast<JsonArray2*>(v);
	return true;
}
const String& JsonObject2::getSerializeElementName(int index) const
{
	return m_memberList[index].name;
}
ISerializeElement* JsonObject2::findSerializeElement(const StringRef& name) const
{
	return find(name);
}
void JsonObject2::addSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value)
{
	switch (type)
	{
	case SerializationValueType::Bool: AddMemberBool(name, *static_cast<const bool*>(value)); break;
	case SerializationValueType::Int8: AddMemberInt32(name, *static_cast<const int8_t*>(value)); break;
	case SerializationValueType::Int16: AddMemberInt32(name, *static_cast<const int16_t*>(value)); break;
	case SerializationValueType::Int32: AddMemberInt32(name, *static_cast<const int32_t*>(value)); break;
	case SerializationValueType::Int64: AddMemberInt64(name, *static_cast<const int64_t*>(value)); break;
	case SerializationValueType::UInt8: AddMemberInt32(name, *static_cast<const uint8_t*>(value)); break;
	case SerializationValueType::UInt16: AddMemberInt32(name, *static_cast<const uint16_t*>(value)); break;
	case SerializationValueType::UInt32: AddMemberInt32(name, *static_cast<const uint32_t*>(value)); break;
	case SerializationValueType::UInt64: AddMemberInt64(name, *static_cast<const uint64_t*>(value)); break;
	case SerializationValueType::Float: AddMemberFloat(name, *static_cast<const float*>(value)); break;
	case SerializationValueType::Double: AddMemberDouble(name, *static_cast<const double*>(value)); break;
	case SerializationValueType::String: AddMemberString(name, *static_cast<const String*>(value)); break;
	default:
		LN_UNREACHABLE();
		break;
	}
}
ISerializeElement* JsonObject2::addSerializeMemberNewArray(const StringRef& name)
{
	return AddMemberArray(name);
}
ISerializeElement* JsonObject2::addSerializeMemberNewObject(const StringRef& name)
{
	return AddMemberObject(name);
}

//==============================================================================
// JsonElementCache
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
void JsonElementCache::initialize()
{
	BufferInfo info;
	info.buffer.resize(2048);
	info.used = 0;
	m_buffers.add(info);

	m_elements.reserve(256);
}

//------------------------------------------------------------------------------
void JsonElementCache::Finalize()
{
	for (JsonElement2* e : m_elements)
	{
		e->~JsonElement2();
	}
}

//------------------------------------------------------------------------------
JsonElement2* JsonElementCache::alloc(size_t size)
{
	if (LN_CHECK_ARG(size <= BufferSize)) return nullptr;

	BufferInfo* cur = &m_buffers.getLast();
	if (cur->buffer.getSize() - cur->used < size)
	{
		BufferInfo info;
		info.buffer.resize(2048);
		info.used = 0;
		m_buffers.add(info);
		cur = &m_buffers.getLast();
	}

	JsonElement2* buf = reinterpret_cast<JsonElement2*>(cur->buffer.getData() + cur->used);
	cur->used += size;
	m_elements.add(buf);
	return buf;
}

} // namespace detail

//==============================================================================
// JsonDocument2
//==============================================================================

//------------------------------------------------------------------------------
JsonDocument2::JsonDocument2()
	: JsonObject2(this)
{
	m_cache.initialize();
}

//------------------------------------------------------------------------------
JsonDocument2::~JsonDocument2()
{
	// m_cache 削除前にクリアする必要がある
	Finalize();

	m_cache.Finalize();
}

//------------------------------------------------------------------------------
void JsonDocument2::Parse(const String& text)
{
	StringReader textReader(text);
	JsonReader2 jr(&textReader);
	ParseInternal(&jr);
}

////------------------------------------------------------------------------------
//void JsonDocument2::Parse(const TCHAR* text, int len)
//{
//	LN_FAIL_CHECK_ARG(text != nullptr) return;
//
//	StringReader textReader(String(text, (len < 0) ? (int)StringTraits::tcslen(text) : len));
//	Parse(&textReader);
//}
//
////------------------------------------------------------------------------------
//void JsonDocument2::Parse(TextReader* textReader)
//{
//	LN_FAIL_CHECK_ARG(textReader != nullptr) return;
//
//	JsonReader2 reader(textReader);
//	reader.Read();
//}


//------------------------------------------------------------------------------
void JsonDocument2::save(const StringRef& filePath, JsonFormatting formatting)
{
	StreamWriter w(filePath);
	JsonWriter jw(&w);
	jw.SetFormatting(formatting);
	JsonElement2::save(&jw);
}

//------------------------------------------------------------------------------
void JsonDocument2::load(const StringRef& filePath)
{
	StreamReader r(filePath.getBegin());	// TODO: end
	JsonReader2 jr(&r);
	ParseInternal(&jr);
}

//------------------------------------------------------------------------------
String JsonDocument2::toString(JsonFormatting formatting)
{
	StringWriter w;
	JsonWriter jw(&w);
	jw.SetFormatting(formatting);
	JsonElement2::save(&jw);
	return w.toString();
}

//------------------------------------------------------------------------------
ISerializeElement* JsonDocument2::getRootObject() { return this; }

//------------------------------------------------------------------------------
void JsonDocument2::ParseInternal(JsonReader2* reader)
{
	bool result = reader->read();
	if (LN_CHECK(result, InvalidFormatException)) return;

	JsonToken type = reader->GetTokenType();
	if (LN_CHECK(type == JsonToken::StartObject, InvalidFormatException)) return;

	JsonElement2::load(reader);
}


} // namespace tr
LN_NAMESPACE_END
