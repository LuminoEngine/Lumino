
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/IO/StringReader.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/IO/StreamWriter.h>
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
		JsonParseResult result = value->Load(reader);
		if (result != JsonParseResult::Success) return result;
		*outElement = value;
	}
	else if (type == JsonToken::StartArray)
	{
		auto* value = doc->NewElement<JsonArray2>();
		JsonParseResult result = value->Load(reader);
		if (result != JsonParseResult::Success) return result;
		*outElement = value;
	}
	else if (detail::JsonHelper::IsValueType(type))
	{
		auto* value = doc->NewElement<JsonValue2>();
		JsonParseResult result = value->Load(reader);
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
void JsonValue2::SetString(const StringRef& value)
{
	CheckRelease();
	SetType(JsonValueType::String);
	m_stringCore = LN_NEW ln::detail::GenericStringCore<TCHAR>();
	m_stringCore->assign(value.GetBegin(), value.GetLength());
}

//------------------------------------------------------------------------------
bool JsonValue2::IsNull() const
{
	return GetType() == JsonValueType::Null;
}

//------------------------------------------------------------------------------
bool JsonValue2::GetBool() const
{
	if (LN_CHECK_STATE(GetType() == JsonValueType::Bool)) return false;
	return m_bool;
}

//------------------------------------------------------------------------------
int32_t JsonValue2::GetInt32() const
{
	if (LN_CHECK_STATE(GetType() == JsonValueType::Int32)) return 0;
	return m_int32;
}

//------------------------------------------------------------------------------
int64_t JsonValue2::GetInt64() const
{
	if (LN_CHECK_STATE(GetType() == JsonValueType::Int64)) return 0;
	return m_int64;
}

//------------------------------------------------------------------------------
float JsonValue2::GetFloat() const
{
	if (LN_CHECK_STATE(GetType() == JsonValueType::Float)) return 0;
	return m_float;
}

//------------------------------------------------------------------------------
double JsonValue2::GetDouble() const
{
	if (LN_CHECK_STATE(GetType() == JsonValueType::Double)) return 0;
	return m_double;
}

//------------------------------------------------------------------------------
String JsonValue2::GetString() const
{
	String str;
	ln::detail::StringHelper::AttachStringCore(&str, m_stringCore);
	return str;
}

//------------------------------------------------------------------------------
void JsonValue2::CheckRelease()
{
	if (GetType() == JsonValueType::String)
	{
		m_stringCore->Release();
	}
	SetType(JsonValueType::Null);
}

//------------------------------------------------------------------------------
void JsonValue2::OnSave(JsonWriter* writer)
{
	if (LN_CHECK_ARG(writer != nullptr)) return;
	switch (GetType())
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
			SetString(reader->GetValue());
			break;
		default:
			LN_UNREACHABLE();
			break;
	}
	return JsonParseResult::Success;
}

//------------------------------------------------------------------------------
SerializationValueType JsonValue2::GetSerializationValueType() const
{
	switch (GetType())
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
void JsonArray2::AddBool(bool value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetBool(value);
	m_itemList.Add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddInt32(int32_t value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetInt64(value);
	m_itemList.Add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddInt64(int64_t value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetInt64(value);
	m_itemList.Add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddFloat(float value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetFloat(value);
	m_itemList.Add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddDouble(double value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetDouble(value);
	m_itemList.Add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::AddString(const StringRef& value)
{
	auto ptr = GetOwnerDocument()->NewElement<JsonValue2>();
	ptr->SetString(value);
	m_itemList.Add(ptr);
}

//------------------------------------------------------------------------------
JsonArray2* JsonArray2::AddArray()
{
	auto ptr = GetOwnerDocument()->NewElement<JsonArray2>();
	m_itemList.Add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
JsonObject2* JsonArray2::AddObject()
{
	auto ptr = GetOwnerDocument()->NewElement<JsonObject2>();
	m_itemList.Add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
void JsonArray2::OnSave(JsonWriter* writer)
{
	if (LN_CHECK_ARG(writer != nullptr)) return;
	writer->WriteStartArray();
	for (JsonElement2* item : m_itemList)
	{
		item->Save(writer);
	}
	writer->WriteEndArray();
}

//------------------------------------------------------------------------------
JsonParseResult JsonArray2::OnLoad(JsonReader2* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;

	// この時点で reader は StartArray('[') を指している

	while (reader->Read())
	{
		if (reader->GetTokenType() == JsonToken::EndArray) return JsonParseResult::Success;	// end scope

		// member value
		JsonElement2* element;
		JsonParseResult result = detail::JsonHelper::LoadElement(GetOwnerDocument(), reader, &element);
		if (result != JsonParseResult::Success) return result;
		m_itemList.Add(element);
	}

	return JsonParseResult::Success;
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
	GetValue(name)->SetNull();
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberBool(const StringRef& name, bool value)
{
	GetValue(name)->SetBool(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberInt32(const StringRef& name, int32_t value)
{
	GetValue(name)->SetInt32(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberInt64(const StringRef& name, int64_t value)
{
	GetValue(name)->SetInt64(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberFloat(const StringRef& name, float value)
{
	GetValue(name)->SetFloat(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberDouble(const StringRef& name, double value)
{
	GetValue(name)->SetDouble(value);
}

//------------------------------------------------------------------------------
void JsonObject2::AddMemberString(const StringRef& name, const StringRef& value)
{
	GetValue(name)->SetString(value);
}

//------------------------------------------------------------------------------
JsonArray2* JsonObject2::AddMemberArray(const StringRef& name)
{
	Member* m = m_memberList.Find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || m->value->GetType() != JsonValueType::Array)
	{
		auto* ptr = GetOwnerDocument()->NewElement<JsonArray2>();
		m_memberList.Add({ name, ptr });
		return ptr;
	}
	return static_cast<JsonArray2*>(m->value);
}

//------------------------------------------------------------------------------
JsonObject2* JsonObject2::AddMemberObject(const StringRef& name)
{
	Member* m = m_memberList.Find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || m->value->GetType() != JsonValueType::Object)
	{
		auto* ptr = GetOwnerDocument()->NewElement<JsonObject2>();
		m_memberList.Add({ name, ptr });
		return ptr;
	}
	return static_cast<JsonObject2*>(m->value);
}

//------------------------------------------------------------------------------
JsonElement2* JsonObject2::Find(const StringRef& name) const
{
	Member* m = m_memberList.Find([name](const Member& m) { return m.name == name; });
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
		writer->WritePropertyName(m.name.c_str(), m.name.GetLength());
		m.value->Save(writer);
	}

	writer->WriteEndObject();
}

//------------------------------------------------------------------------------
JsonParseResult JsonObject2::OnLoad(JsonReader2* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;

	// この時点で reader は StartObject('{') を指している

	while (reader->Read())
	{
		if (reader->GetTokenType() == JsonToken::EndObject) return JsonParseResult::Success;	// end scope

		// member name
		if (reader->GetTokenType() != JsonToken::PropertyName) return JsonParseResult::Error;
		String name = reader->GetValue();

		// member value
		if (!reader->Read()) return JsonParseResult::Error;
		JsonElement2* element;
		JsonParseResult result = detail::JsonHelper::LoadElement(GetOwnerDocument(), reader, &element);
		if (result != JsonParseResult::Success) return result;
		m_memberList.Add({ name, element });
	}

	return JsonParseResult::Success;
}

//------------------------------------------------------------------------------
JsonValue2* JsonObject2::GetValue(const StringRef& name)
{
	Member* m = m_memberList.Find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || !detail::JsonHelper::IsValueType(m->value->GetType()))
	{
		auto* ptr = GetOwnerDocument()->NewElement<JsonValue2>();
		m_memberList.Add({ name, ptr });
		return ptr;
	}
	return static_cast<JsonValue2*>(m->value);
}

//------------------------------------------------------------------------------
void JsonObject2::SetValueInt32(const StringRef& name, int32_t value) { AddMemberInt32(name, value); }
void JsonObject2::SetValueString(const StringRef& name, const String& value) { AddMemberString(name, value); }
ISerializeElement* JsonObject2::AddObject(const StringRef& name)  { return AddMemberObject(name); }
bool JsonObject2::TryGetValueInt32(const StringRef& name, int32_t* outValue)
{
	auto* v = Find(name);
	if (v == nullptr) return false;
	if (v->GetType() != JsonValueType::Int32) return false;
	*outValue = static_cast<JsonValue2*>(v)->GetInt32();
	return true;
}
bool JsonObject2::TryGetValueString(const StringRef& name, String* outValue)
{
	auto* v = Find(name);
	if (v == nullptr) return false;
	if (v->GetType() != JsonValueType::String) return false;
	*outValue = static_cast<JsonValue2*>(v)->GetString();
	return true;
}
bool JsonObject2::TryGetObject(const StringRef& name, ISerializeElement** outValue)
{
	auto* v = Find(name);
	if (v == nullptr) return false;
	if (v->GetType() != JsonValueType::Object) return false;
	*outValue = static_cast<JsonObject2*>(v);
	return true;
}
bool JsonObject2::TryGetArray(const StringRef& name, ISerializeElement** outValue)
{
	auto* v = Find(name);
	if (v == nullptr) return false;
	if (v->GetType() != JsonValueType::Array) return false;
	*outValue = static_cast<JsonArray2*>(v);
	return true;
}
ISerializeElement* JsonObject2::FindSerializeElement(const StringRef& name) const
{
	return Find(name);
}

//==============================================================================
// JsonElementCache
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
void JsonElementCache::Initialize()
{
	BufferInfo info;
	info.buffer.Resize(2048);
	info.used = 0;
	m_buffers.Add(info);

	m_elements.Reserve(256);
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
JsonElement2* JsonElementCache::Alloc(size_t size)
{
	if (LN_CHECK_ARG(size <= BufferSize)) return nullptr;

	BufferInfo* cur = &m_buffers.GetLast();
	if (cur->buffer.GetSize() - cur->used < size)
	{
		BufferInfo info;
		info.buffer.Resize(2048);
		info.used = 0;
		m_buffers.Add(info);
		cur = &m_buffers.GetLast();
	}

	JsonElement2* buf = reinterpret_cast<JsonElement2*>(cur->buffer.GetData() + cur->used);
	cur->used += size;
	m_elements.Add(buf);
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
	m_cache.Initialize();
}

//------------------------------------------------------------------------------
JsonDocument2::~JsonDocument2()
{
	// m_cache 削除前にクリアする必要がある
	Finalize();

	m_cache.Finalize();
}

////------------------------------------------------------------------------------
//void JsonDocument2::Parse(const String& text)
//{
//	StringReader textReader(text);
//	Parse(&textReader);
//}
//
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
void JsonDocument2::Save(const StringRef& filePath, JsonFormatting formatting)
{
	StreamWriter w(filePath);
	JsonWriter jw(&w);
	jw.SetFormatting(formatting);
	JsonElement2::Save(&jw);
}

//------------------------------------------------------------------------------
void JsonDocument2::Load(const StringRef& filePath)
{
	StreamReader r(filePath.GetBegin());	// TODO: end
	JsonReader2 jr(&r);

	bool result = jr.Read();
	if (LN_CHECK(result, InvalidFormatException)) return;

	JsonToken type = jr.GetTokenType();
	if (LN_CHECK(type == JsonToken::StartObject, InvalidFormatException)) return;

	JsonElement2::Load(&jr);
}

//------------------------------------------------------------------------------
ISerializeElement* JsonDocument2::GetRootObject() { return this; }

} // namespace tr
LN_NAMESPACE_END
