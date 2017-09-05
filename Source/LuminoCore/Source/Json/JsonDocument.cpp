
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
void JsonDocument::parse(const String& text)
{
	StringReader textReader(text);
	parse(&textReader);
}
void JsonDocument::parse(const Char* text, int len)
{
	if (LN_CHECK_ARG(text != nullptr)) return;

	StringReader textReader(String(text, (len < 0) ? (int)StringTraits::tcslen(text) : len));
	parse(&textReader);
}

//------------------------------------------------------------------------------
void JsonDocument::parse(TextReader* textReader)
{
	JsonDOMHandler handler(this);
	JsonReader reader(&handler);
	reader.parse(textReader);
	handler.build();
}



//==============================================================================
// JsonHelper
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
bool JsonHelper::isValueType(JsonToken type)
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
bool JsonHelper::isValueType(JsonValueType type)
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
JsonParseResult JsonHelper::loadElement(JsonDocument2* doc, JsonReader2* reader, JsonElement2** outElement)
{
	if (LN_CHECK_ARG(doc != nullptr)) return JsonParseResult::Error;
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;
	if (LN_CHECK_ARG(outElement != nullptr)) return JsonParseResult::Error;

	JsonToken type = reader->getTokenType();
	if (type == JsonToken::StartObject)
	{
		auto* value = doc->newElement<JsonObject2>();
		JsonParseResult result = value->load(reader);
		if (result != JsonParseResult::Success) return result;
		*outElement = value;
	}
	else if (type == JsonToken::StartArray)
	{
		auto* value = doc->newElement<JsonArray2>();
		JsonParseResult result = value->load(reader);
		if (result != JsonParseResult::Success) return result;
		*outElement = value;
	}
	else if (detail::JsonHelper::isValueType(type))
	{
		auto* value = doc->newElement<JsonValue2>();
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
	checkRelease();
}

//------------------------------------------------------------------------------
void JsonValue2::setNull()
{
	checkRelease();
	setType(JsonValueType::Null);
}

//------------------------------------------------------------------------------
void JsonValue2::setBool(bool value)
{
	checkRelease();
	setType(JsonValueType::Bool);
	m_bool = value;
}

//------------------------------------------------------------------------------
void JsonValue2::setInt32(int32_t value)
{
	checkRelease();
	setType(JsonValueType::Int32);
	m_int32 = value;
}

//------------------------------------------------------------------------------
void JsonValue2::setInt64(int64_t value)
{
	checkRelease();
	setType(JsonValueType::Int64);
	m_int64 = value;
}

//------------------------------------------------------------------------------
void JsonValue2::setFloat(float value)
{
	checkRelease();
	setType(JsonValueType::Float);
	m_float = value;
}

//------------------------------------------------------------------------------
void JsonValue2::setDouble(double value)
{
	checkRelease();
	setType(JsonValueType::Double);
	m_double = value;
}

//------------------------------------------------------------------------------
void JsonValue2::setString(const StringRef& value)
{
	checkRelease();
	setType(JsonValueType::String);
	m_stringCore = LN_NEW ln::detail::GenericStringCore<TCHAR>();
	m_stringCore->assign(value.getBegin(), value.getLength());
}

//------------------------------------------------------------------------------
bool JsonValue2::isNull() const
{
	return getType() == JsonValueType::Null;
}

//------------------------------------------------------------------------------
bool JsonValue2::getBool() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Bool)) return false;
	return m_bool;
}

//------------------------------------------------------------------------------
int32_t JsonValue2::getInt32() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Int32)) return 0;
	return m_int32;
}

//------------------------------------------------------------------------------
int64_t JsonValue2::getInt64() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Int64)) return 0;
	return m_int64;
}

//------------------------------------------------------------------------------
float JsonValue2::getFloat() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Float)) return 0;
	return m_float;
}

//------------------------------------------------------------------------------
double JsonValue2::getDouble() const
{
	if (LN_CHECK_STATE(getType() == JsonValueType::Double)) return 0;
	return m_double;
}

//------------------------------------------------------------------------------
String JsonValue2::getString() const
{
#ifdef LN_USTRING
	return m_stringCore->c_str();
#else
	String str;
	ln::detail::StringHelper::attachStringCore(&str, m_stringCore);
	return str;
#endif
}

//------------------------------------------------------------------------------
void JsonValue2::checkRelease()
{
	if (getType() == JsonValueType::String)
	{
		m_stringCore->release();
	}
	setType(JsonValueType::Null);
}

//------------------------------------------------------------------------------
void JsonValue2::onSave(JsonWriter* writer)
{
	if (LN_CHECK_ARG(writer != nullptr)) return;
	switch (getType())
	{
		case JsonValueType::Null:
			writer->writeNull();
			break;
		case JsonValueType::Bool:
			writer->writeBool(m_bool);
			break;
		case JsonValueType::Int32:
			writer->writeInt32(m_int32);
			break;
		case JsonValueType::Int64:
			writer->writeInt64(m_int64);
			break;
		case JsonValueType::Float:
			writer->writeFloat(m_float);
			break;
		case JsonValueType::Double:
			writer->writeDouble(m_double);
			break;
		case JsonValueType::String:
			writer->writeString(m_stringCore->c_str(), m_stringCore->length());
			break;
		default:
			LN_UNREACHABLE();
			break;
	}
}

//------------------------------------------------------------------------------
JsonParseResult JsonValue2::onLoad(JsonReader2* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;

	switch (reader->getTokenType())
	{
		case JsonToken::Int32:
			setInt32(reader->getInt32Value());
			break;
		case JsonToken::Int64:
			setInt64(reader->getInt64Value());
			break;
		case JsonToken::Float:
			setFloat(reader->getFloatValue());
			break;
		case JsonToken::Double:
			setDouble(reader->getDoubleValue());
			break;
		case JsonToken::Null:
			setNull();
			break;
		case JsonToken::Boolean:
			setBool(reader->getBoolValue());
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
	setType(JsonValueType::Array);
}

//------------------------------------------------------------------------------
JsonArray2::~JsonArray2()
{
}

//------------------------------------------------------------------------------
void JsonArray2::addNull()
{
	auto ptr = getOwnerDocument()->newElement<JsonValue2>();
	ptr->setNull();
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::addBool(bool value)
{
	auto ptr = getOwnerDocument()->newElement<JsonValue2>();
	ptr->setBool(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::addInt32(int32_t value)
{
	auto ptr = getOwnerDocument()->newElement<JsonValue2>();
	ptr->setInt64(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::addInt64(int64_t value)
{
	auto ptr = getOwnerDocument()->newElement<JsonValue2>();
	ptr->setInt64(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::addFloat(float value)
{
	auto ptr = getOwnerDocument()->newElement<JsonValue2>();
	ptr->setFloat(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::addDouble(double value)
{
	auto ptr = getOwnerDocument()->newElement<JsonValue2>();
	ptr->setDouble(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
void JsonArray2::addString(const StringRef& value)
{
	auto ptr = getOwnerDocument()->newElement<JsonValue2>();
	ptr->setString(value);
	m_itemList.add(ptr);
}

//------------------------------------------------------------------------------
JsonArray2* JsonArray2::addArray()
{
	auto ptr = getOwnerDocument()->newElement<JsonArray2>();
	m_itemList.add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
JsonObject2* JsonArray2::addObject()
{
	auto ptr = getOwnerDocument()->newElement<JsonObject2>();
	m_itemList.add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
void JsonArray2::onSave(JsonWriter* writer)
{
	if (LN_CHECK_ARG(writer != nullptr)) return;
	writer->writeStartArray();
	for (JsonElement2* item : m_itemList)
	{
		item->save(writer);
	}
	writer->writeEndArray();
}

//------------------------------------------------------------------------------
JsonParseResult JsonArray2::onLoad(JsonReader2* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;

	// この時点で reader は StartArray('[') を指している

	while (reader->read())
	{
		if (reader->getTokenType() == JsonToken::EndArray) return JsonParseResult::Success;	// end scope

		// member value
		JsonElement2* element;
		JsonParseResult result = detail::JsonHelper::loadElement(getOwnerDocument(), reader, &element);
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
	case SerializationValueType::Null: addNull(); break;
	case SerializationValueType::Bool: addBool(*static_cast<const bool*>(value)); break;
	case SerializationValueType::Int8: addInt32(*static_cast<const int8_t*>(value)); break;
	case SerializationValueType::Int16: addInt32(*static_cast<const int16_t*>(value)); break;
	case SerializationValueType::Int32: addInt32(*static_cast<const int32_t*>(value)); break;
	case SerializationValueType::Int64: addInt64(*static_cast<const int64_t*>(value)); break;
	case SerializationValueType::UInt8: addInt32(*static_cast<const uint8_t*>(value)); break;
	case SerializationValueType::UInt16: addInt32(*static_cast<const uint16_t*>(value)); break;
	case SerializationValueType::UInt32: addInt32(*static_cast<const uint32_t*>(value)); break;
	case SerializationValueType::UInt64: addInt64(*static_cast<const uint64_t*>(value)); break;
	case SerializationValueType::Float: addFloat(*static_cast<const float*>(value)); break;
	case SerializationValueType::Double: addDouble(*static_cast<const double*>(value)); break;
	case SerializationValueType::String: addString(*static_cast<const String*>(value)); break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

//------------------------------------------------------------------------------
ISerializeElement* JsonArray2::addSerializeItemNewArray()
{
	return addArray();
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
	setType(JsonValueType::Object);
}

//------------------------------------------------------------------------------
JsonObject2::~JsonObject2()
{
}

//------------------------------------------------------------------------------
void JsonObject2::addMemberNull(const StringRef& name)
{
	getValue(name)->setNull();
}

//------------------------------------------------------------------------------
void JsonObject2::addMemberBool(const StringRef& name, bool value)
{
	getValue(name)->setBool(value);
}

//------------------------------------------------------------------------------
void JsonObject2::addMemberInt32(const StringRef& name, int32_t value)
{
	getValue(name)->setInt32(value);
}

//------------------------------------------------------------------------------
void JsonObject2::addMemberInt64(const StringRef& name, int64_t value)
{
	getValue(name)->setInt64(value);
}

//------------------------------------------------------------------------------
void JsonObject2::addMemberFloat(const StringRef& name, float value)
{
	getValue(name)->setFloat(value);
}

//------------------------------------------------------------------------------
void JsonObject2::addMemberDouble(const StringRef& name, double value)
{
	getValue(name)->setDouble(value);
}

//------------------------------------------------------------------------------
void JsonObject2::addMemberString(const StringRef& name, const StringRef& value)
{
	getValue(name)->setString(value);
}

//------------------------------------------------------------------------------
JsonArray2* JsonObject2::addMemberArray(const StringRef& name)
{
	Member* m = m_memberList.find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || m->value->getType() != JsonValueType::Array)
	{
		auto* ptr = getOwnerDocument()->newElement<JsonArray2>();
		m_memberList.add({ name, ptr });
		return ptr;
	}
	return static_cast<JsonArray2*>(m->value);
}

//------------------------------------------------------------------------------
JsonObject2* JsonObject2::addMemberObject(const StringRef& name)
{
	Member* m = m_memberList.find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || m->value->getType() != JsonValueType::Object)
	{
		auto* ptr = getOwnerDocument()->newElement<JsonObject2>();
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
void JsonObject2::onSave(JsonWriter* writer)
{
	if (LN_CHECK_ARG(writer != nullptr)) return;

	writer->writeStartObject();

	for (auto& m : m_memberList)
	{
		writer->writePropertyName(m.name.c_str(), m.name.getLength());
		m.value->save(writer);
	}

	writer->writeEndObject();
}

//------------------------------------------------------------------------------
JsonParseResult JsonObject2::onLoad(JsonReader2* reader)
{
	if (LN_CHECK_ARG(reader != nullptr)) return JsonParseResult::Error;

	// この時点で reader は StartObject('{') を指している

	while (reader->read())
	{
		if (reader->getTokenType() == JsonToken::EndObject) return JsonParseResult::Success;	// end scope

		// member name
		if (reader->getTokenType() != JsonToken::PropertyName) return JsonParseResult::Error;
		String name = reader->getValue();

		// member value
		if (!reader->read()) return JsonParseResult::Error;
		JsonElement2* element;
		JsonParseResult result = detail::JsonHelper::loadElement(getOwnerDocument(), reader, &element);
		if (result != JsonParseResult::Success) return result;
		m_memberList.add({ name, element });
	}

	return JsonParseResult::Success;
}

//------------------------------------------------------------------------------
JsonValue2* JsonObject2::getValue(const StringRef& name)
{
	Member* m = m_memberList.find([name](const Member& m) { return m.name == name; });
	if (m == nullptr || !detail::JsonHelper::isValueType(m->value->getType()))
	{
		auto* ptr = getOwnerDocument()->newElement<JsonValue2>();
		m_memberList.add({ name, ptr });
		return ptr;
	}
	return static_cast<JsonValue2*>(m->value);
}

//------------------------------------------------------------------------------
void JsonObject2::setValueInt32(const StringRef& name, int32_t value) { addMemberInt32(name, value); }
void JsonObject2::setValueString(const StringRef& name, const String& value) { addMemberString(name, value); }
ISerializeElement* JsonObject2::addObject(const StringRef& name)  { return addMemberObject(name); }
bool JsonObject2::tryGetValueInt32(const StringRef& name, int32_t* outValue)
{
	auto* v = find(name);
	if (v == nullptr) return false;
	if (v->getType() != JsonValueType::Int32) return false;
	*outValue = static_cast<JsonValue2*>(v)->getInt32();
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
	case SerializationValueType::Bool: addMemberBool(name, *static_cast<const bool*>(value)); break;
	case SerializationValueType::Int8: addMemberInt32(name, *static_cast<const int8_t*>(value)); break;
	case SerializationValueType::Int16: addMemberInt32(name, *static_cast<const int16_t*>(value)); break;
	case SerializationValueType::Int32: addMemberInt32(name, *static_cast<const int32_t*>(value)); break;
	case SerializationValueType::Int64: addMemberInt64(name, *static_cast<const int64_t*>(value)); break;
	case SerializationValueType::UInt8: addMemberInt32(name, *static_cast<const uint8_t*>(value)); break;
	case SerializationValueType::UInt16: addMemberInt32(name, *static_cast<const uint16_t*>(value)); break;
	case SerializationValueType::UInt32: addMemberInt32(name, *static_cast<const uint32_t*>(value)); break;
	case SerializationValueType::UInt64: addMemberInt64(name, *static_cast<const uint64_t*>(value)); break;
	case SerializationValueType::Float: addMemberFloat(name, *static_cast<const float*>(value)); break;
	case SerializationValueType::Double: addMemberDouble(name, *static_cast<const double*>(value)); break;
	case SerializationValueType::String: addMemberString(name, *static_cast<const String*>(value)); break;
	default:
		LN_UNREACHABLE();
		break;
	}
}
ISerializeElement* JsonObject2::addSerializeMemberNewArray(const StringRef& name)
{
	return addMemberArray(name);
}
ISerializeElement* JsonObject2::addSerializeMemberNewObject(const StringRef& name)
{
	return addMemberObject(name);
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
void JsonElementCache::finalize()
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
	clear();

	m_cache.finalize();
}

//------------------------------------------------------------------------------
void JsonDocument2::parse(const String& text)
{
	StringReader textReader(text);
	JsonReader2 jr(&textReader);
	parseInternal(&jr);
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
	jw.setFormatting(formatting);
	JsonElement2::save(&jw);
}

//------------------------------------------------------------------------------
void JsonDocument2::load(const StringRef& filePath)
{
	StreamReader r(filePath.getBegin());	// TODO: end
	JsonReader2 jr(&r);
	parseInternal(&jr);
}

//------------------------------------------------------------------------------
String JsonDocument2::toString(JsonFormatting formatting)
{
	StringWriter w;
	JsonWriter jw(&w);
	jw.setFormatting(formatting);
	JsonElement2::save(&jw);
	return w.toString();
}

//------------------------------------------------------------------------------
ISerializeElement* JsonDocument2::getRootObject() { return this; }

//------------------------------------------------------------------------------
void JsonDocument2::parseInternal(JsonReader2* reader)
{
	bool result = reader->read();
	if (LN_CHECK(result, InvalidFormatException)) return;

	JsonToken type = reader->getTokenType();
	if (LN_CHECK(type == JsonToken::StartObject, InvalidFormatException)) return;

	JsonElement2::load(reader);
}


} // namespace tr
LN_NAMESPACE_END
