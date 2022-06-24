
#include "Internal.hpp"
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/IO/Path.hpp>
#include <LuminoCore/IO/Stream.hpp>
#include <LuminoCore/IO/StringReader.hpp>
#include <LuminoCore/IO/StreamReader.hpp>
#include <LuminoCore/IO/StreamWriter.hpp>
#include <LuminoCore/IO/StringWriter.hpp>
#include <LuminoCore/Json/JsonReader.hpp>
#include <LuminoCore/Json/JsonWriter.hpp>
#include <LuminoCore/Json/JsonDocument.hpp>
#include <LuminoCore/Base/LinearAllocator.hpp>

namespace ln {

//==============================================================================
// JsonHelper

namespace detail {

bool JsonHelper::isValueType(JsonNode type)
{
    return type == JsonNode::Null ||
           type == JsonNode::Boolean ||
           type == JsonNode::Int32 ||
           type == JsonNode::Int64 ||
           type == JsonNode::Float ||
           type == JsonNode::Double ||
           type == JsonNode::String;
}

bool JsonHelper::isValueType(JsonElementType type)
{
    return type == JsonElementType::Null ||
           type == JsonElementType::Bool ||
           type == JsonElementType::Int32 ||
           type == JsonElementType::Int64 ||
           type == JsonElementType::Float ||
           type == JsonElementType::Double ||
           type == JsonElementType::String;
}

JsonParseResult JsonHelper::loadElement(JsonDocument* doc, JsonReader* reader, JsonElement** outElement)
{
    if (LN_REQUIRE(doc != nullptr))
        return JsonParseResult::Error;
    if (LN_REQUIRE(reader != nullptr))
        return JsonParseResult::Error;
    if (LN_REQUIRE(outElement != nullptr))
        return JsonParseResult::Error;

    JsonNode type = reader->nodeType();
    if (type == JsonNode::StartObject) {
        auto* value = doc->newElement<JsonObject>();
        JsonParseResult result = value->load(reader);
        if (result != JsonParseResult::Success)
            return result;
        *outElement = value;
    } else if (type == JsonNode::StartArray) {
        auto* value = doc->newElement<JsonArray>();
        JsonParseResult result = value->load(reader);
        if (result != JsonParseResult::Success)
            return result;
        *outElement = value;
    } else if (detail::JsonHelper::isValueType(type)) {
        auto* value = doc->newElement<JsonValue>();
        JsonParseResult result = value->load(reader);
        if (result != JsonParseResult::Success)
            return result;
        *outElement = value;
    } else {
        if (LN_ENSURE(0))
            return JsonParseResult::Error;
    }
    return JsonParseResult::Success;
}

} // namespace detail

//==============================================================================
// JsonElement

JsonElement::JsonElement(JsonDocument* owner)
    : m_ownerDoc(owner)
    , m_type(JsonElementType::Null)
{
}

JsonElement::~JsonElement()
{
}

//==============================================================================
// JsonObject

JsonValue::JsonValue(JsonDocument* ownerDoc)
    : JsonElement(ownerDoc)
{
}

JsonValue::~JsonValue()
{
    checkRelease();
}

void JsonValue::setNullValue()
{
    checkRelease();
    setType(JsonElementType::Null);
}

void JsonValue::setBoolValue(bool value)
{
    checkRelease();
    setType(JsonElementType::Bool);
    m_bool = value;
}

void JsonValue::setInt32Value(int32_t value)
{
    checkRelease();
    setType(JsonElementType::Int32);
    m_int32 = value;
}

void JsonValue::setInt64Value(int64_t value)
{
    checkRelease();
    setType(JsonElementType::Int64);
    m_int64 = value;
}

void JsonValue::setFloatValue(float value)
{
    checkRelease();
    setType(JsonElementType::Float);
    m_float = value;
}

void JsonValue::setDoubleValue(double value)
{
    checkRelease();
    setType(JsonElementType::Double);
    m_double = value;
}

void JsonValue::setStringValue(const StringView& value)
{
    checkRelease();
    setType(JsonElementType::String);
    m_string = LN_NEW String(value.data(), value.length());
}

bool JsonValue::isNull() const
{
    return type() == JsonElementType::Null;
}

bool JsonValue::boolValue() const
{
    if (LN_REQUIRE(type() == JsonElementType::Bool))
        return false;
    return m_bool;
}

int32_t JsonValue::int32Value() const
{
    if (LN_REQUIRE(type() == JsonElementType::Int32))
        return 0;
    return m_int32;
}

int64_t JsonValue::int64Value() const
{
    if (LN_REQUIRE(type() == JsonElementType::Int64))
        return 0;
    return m_int64;
}

float JsonValue::floatValue() const
{
    if (LN_REQUIRE(type() == JsonElementType::Float))
        return 0;
    return m_float;
}

double JsonValue::doubleValue() const
{
    if (LN_REQUIRE(type() == JsonElementType::Double))
        return 0;
    return m_double;
}

const String& JsonValue::stringValue() const
{
    if (LN_REQUIRE(type() == JsonElementType::String))
        return String::Empty;
    return *m_string;
}

void JsonValue::checkRelease()
{
    if (type() == JsonElementType::String) {
        LN_SAFE_DELETE(m_string);
    }
    setType(JsonElementType::Null);
}

void JsonValue::onSave(JsonWriter* writer)
{
    if (LN_REQUIRE(writer != nullptr))
        return;
    switch (type()) {
        case JsonElementType::Null:
            writer->writeNull();
            break;
        case JsonElementType::Bool:
            writer->writeBool(m_bool);
            break;
        case JsonElementType::Int32:
            writer->writeInt32(m_int32);
            break;
        case JsonElementType::Int64:
            writer->writeInt64(m_int64);
            break;
        case JsonElementType::Float:
            writer->writeFloat(m_float);
            break;
        case JsonElementType::Double:
            writer->writeDouble(m_double);
            break;
        case JsonElementType::String:
            writer->writeString(*m_string);
            break;
        default:
            LN_UNREACHABLE();
            break;
    }
}

JsonParseResult JsonValue::onLoad(JsonReader* reader)
{
    if (LN_REQUIRE(reader != nullptr))
        return JsonParseResult::Error;

    switch (reader->nodeType()) {
        case JsonNode::Int32:
            setInt32Value(reader->int32Value());
            break;
        case JsonNode::Int64:
            setInt64Value(reader->int64Value());
            break;
        case JsonNode::Float:
            setFloatValue(reader->floatValue());
            break;
        case JsonNode::Double:
            setDoubleValue(reader->doubleValue());
            break;
        case JsonNode::Null:
            setNullValue();
            break;
        case JsonNode::Boolean:
            setBoolValue(reader->boolValue());
            break;
        case JsonNode::String:
            setStringValue(reader->value());
            break;
        default:
            LN_UNREACHABLE();
            break;
    }
    return JsonParseResult::Success;
}

//==============================================================================
// JsonObject

JsonArray::JsonArray(JsonDocument* ownerDoc)
    : JsonElement(ownerDoc)
{
    setType(JsonElementType::Array);
}

JsonArray::~JsonArray()
{
}

void JsonArray::addNullValue()
{
    auto ptr = ownerDocument()->newElement<JsonValue>();
    ptr->setNullValue();
    m_itemList.add(ptr);
}

void JsonArray::addBoolValue(bool value)
{
    auto ptr = ownerDocument()->newElement<JsonValue>();
    ptr->setBoolValue(value);
    m_itemList.add(ptr);
}

void JsonArray::addInt32Value(int32_t value)
{
    auto ptr = ownerDocument()->newElement<JsonValue>();
    ptr->setInt64Value(value);
    m_itemList.add(ptr);
}

void JsonArray::addInt64Value(int64_t value)
{
    auto ptr = ownerDocument()->newElement<JsonValue>();
    ptr->setInt64Value(value);
    m_itemList.add(ptr);
}

void JsonArray::addFloatValue(float value)
{
    auto ptr = ownerDocument()->newElement<JsonValue>();
    ptr->setFloatValue(value);
    m_itemList.add(ptr);
}

void JsonArray::addDoubleValue(double value)
{
    auto ptr = ownerDocument()->newElement<JsonValue>();
    ptr->setDoubleValue(value);
    m_itemList.add(ptr);
}

void JsonArray::addStringValue(const StringView& value)
{
    auto ptr = ownerDocument()->newElement<JsonValue>();
    ptr->setStringValue(value);
    m_itemList.add(ptr);
}

JsonArray* JsonArray::addArray()
{
    auto ptr = ownerDocument()->newElement<JsonArray>();
    m_itemList.add(ptr);
    return ptr;
}

JsonObject* JsonArray::addObject()
{
    auto ptr = ownerDocument()->newElement<JsonObject>();
    m_itemList.add(ptr);
    return ptr;
}

void JsonArray::onSave(JsonWriter* writer)
{
    if (LN_REQUIRE(writer != nullptr))
        return;
    writer->writeStartArray();
    for (JsonElement* item : m_itemList) {
        item->save(writer);
    }
    writer->writeEndArray();
}

JsonParseResult JsonArray::onLoad(JsonReader* reader)
{
    if (LN_REQUIRE(reader != nullptr))
        return JsonParseResult::Error;

    // この時点で reader は StartArray('[') を指している

    while (reader->read()) {
        if (reader->nodeType() == JsonNode::EndArray)
            return JsonParseResult::Success; // end scope

        // member value
        JsonElement* element;
        JsonParseResult result = detail::JsonHelper::loadElement(ownerDocument(), reader, &element);
        if (result != JsonParseResult::Success)
            return result;
        m_itemList.add(element);
    }

    return JsonParseResult::Success;
}

//==============================================================================
// JsonObject

JsonObject::JsonObject(JsonDocument* ownerDoc)
    : JsonElement(ownerDoc)
{
    setType(JsonElementType::Object);
}

JsonObject::~JsonObject()
{
}

void JsonObject::addNullValue(const StringView& name)
{
    getValue(name)->setNullValue();
}

void JsonObject::addBoolValue(const StringView& name, bool value)
{
    getValue(name)->setBoolValue(value);
}

void JsonObject::addInt32Value(const StringView& name, int32_t value)
{
    getValue(name)->setInt32Value(value);
}

void JsonObject::addInt64Value(const StringView& name, int64_t value)
{
    getValue(name)->setInt64Value(value);
}

void JsonObject::addFloatValue(const StringView& name, float value)
{
    getValue(name)->setFloatValue(value);
}

void JsonObject::addDoubleValue(const StringView& name, double value)
{
    getValue(name)->setDoubleValue(value);
}

void JsonObject::addStringValue(const StringView& name, const StringView& value)
{
    getValue(name)->setStringValue(value);
}

JsonArray* JsonObject::addArray(const StringView& name)
{
    int index = m_memberList.indexOfIf([name](const Member& m) { return m.name == name; });
    if (index >= 0) {
        Member& m = m_memberList[index];
        if (m.value->type() == JsonElementType::Array) {
            return static_cast<JsonArray*>(m.value);
        }
    }

    auto* ptr = ownerDocument()->newElement<JsonArray>();
    m_memberList.add({name, ptr});
    return ptr;
}

JsonObject* JsonObject::addObject(const StringView& name)
{

    int index = m_memberList.indexOfIf([name](const Member& m) { return m.name == name; });
    if (index >= 0) {
        Member& m = m_memberList[index];
        if (m.value->type() == JsonElementType::Object) {
            return static_cast<JsonObject*>(m.value);
        }
    }

    auto* ptr = ownerDocument()->newElement<JsonObject>();
    m_memberList.add({name, ptr});
    return ptr;
}

JsonElement* JsonObject::find(const StringView& name) const
{
    int index = m_memberList.indexOfIf([name](const Member& m) { return m.name == name; });
    if (index >= 0)
        return m_memberList[index].value;
    else
        return nullptr;
}

void JsonObject::onSave(JsonWriter* writer)
{
    if (LN_REQUIRE(writer != nullptr))
        return;

    writer->writeStartObject();

    for (auto& m : m_memberList) {
        writer->writePropertyName(StringView(m.name));
        m.value->save(writer);
    }

    writer->writeEndObject();
}

JsonParseResult JsonObject::onLoad(JsonReader* reader)
{
    if (LN_REQUIRE(reader != nullptr))
        return JsonParseResult::Error;

    // この時点で reader は StartObject('{') を指している

    while (reader->read()) {
        if (reader->nodeType() == JsonNode::EndObject)
            return JsonParseResult::Success; // end scope

        // member name
        if (reader->nodeType() != JsonNode::PropertyName)
            return JsonParseResult::Error;

        String name = reader->value();

        // member value
        if (!reader->read())
            return JsonParseResult::Error;
        JsonElement* element;
        JsonParseResult result = detail::JsonHelper::loadElement(ownerDocument(), reader, &element);
        if (result != JsonParseResult::Success)
            return result;
        m_memberList.add({name, element});
    }

    return JsonParseResult::Success;
}

JsonValue* JsonObject::getValue(const StringView& name)
{
    int index = m_memberList.indexOfIf([name](const Member& m) { return m.name == name; });
    if (index >= 0) {
        Member& m = m_memberList[index];
        if (detail::JsonHelper::isValueType(m.value->type())) {
            return static_cast<JsonValue*>(m.value);
        }
    }

    auto* ptr = ownerDocument()->newElement<JsonValue>();
    m_memberList.add({name, ptr});
    return ptr;
}

//==============================================================================
// JsonElementCache

namespace detail {

void JsonElementCache::initialize()
{
    //BufferInfo info;
    //info.buffer.resize(2048);
    //info.used = 0;
    //m_buffers.add(info);
	m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();
	m_linearAllocator = makeRef<LinearAllocator>(m_linearAllocatorPageManager);

    m_elements.reserve(256);
}

void JsonElementCache::finalize()
{
    for (JsonElement* e : m_elements) {
        e->~JsonElement();
    }

	m_linearAllocator = nullptr;
	m_linearAllocatorPageManager = nullptr;
}

JsonElement* JsonElementCache::alloc(size_t size)
{
    //if (LN_REQUIRE(size <= BufferSize))
    //    return nullptr;

    //BufferInfo* cur = &m_buffers.back();
    //if (cur->buffer.size() - cur->used < size) {
    //    BufferInfo info;
    //    info.buffer.resize(2048);
    //    info.used = 0;
    //    m_buffers.add(info);
    //    cur = &m_buffers.back();
    //}

    //JsonElement* buf = reinterpret_cast<JsonElement*>(cur->buffer.data() + cur->used);
    //cur->used += size;
	JsonElement* buf = reinterpret_cast<JsonElement*>(m_linearAllocator->allocate(size));
    m_elements.add(buf);
    return buf;
}

} // namespace detail

//==============================================================================
// JsonDocument

JsonDocument::JsonDocument()
    //: JsonObject(this)
    : m_rootElement(nullptr)
{
    m_cache.initialize();
}

JsonDocument::~JsonDocument()
{
    // m_cache 削除前にクリアする必要がある
    //clear();

    m_cache.finalize();
}

void JsonDocument::setRootArray()
{
    m_rootElement = newElement<JsonArray>();
}

void JsonDocument::setRootObject()
{
    m_rootElement = newElement<JsonObject>();
}

void JsonDocument::parse(const String& text)
{
    StringReader textReader(text);
    JsonReader jr(&textReader);
    parseInternal(&jr);
}

void JsonDocument::save(const StringView& filePath, JsonFormatting formatting)
{
    StreamWriter w(filePath);
    JsonWriter jw(&w);
    jw.setFormatting(formatting);
    m_rootElement->save(&jw);
}

void JsonDocument::load(const StringView& filePath)
{
    StreamReader r(filePath.data()); // TODO: end
    JsonReader jr(&r);
    parseInternal(&jr);
}

String JsonDocument::toString(JsonFormatting formatting)
{
    StringWriter w;
    JsonWriter jw(&w);
    jw.setFormatting(formatting);
    m_rootElement->save(&jw);
    return w.toString();
}

void JsonDocument::parseInternal(JsonReader* reader)
{
    bool result = reader->read();
    if (LN_ENSURE(result))
        return;

    JsonNode type = reader->nodeType();

    switch (type) {
        case JsonNode::StartObject: {
            auto* value = newElement<JsonObject>();
            value->load(reader);
            m_rootElement = value;
            break;
        }
        case JsonNode::StartArray: {
            auto* value = newElement<JsonArray>();
            value->load(reader);
            m_rootElement = value;
            break;
        }
        case JsonNode::Int32:
        case JsonNode::Int64:
        case JsonNode::Float:
        case JsonNode::Double:
        case JsonNode::Null:
        case JsonNode::Boolean:
        case JsonNode::String: {
            auto* value = newElement<JsonValue>();
            value->load(reader);
            m_rootElement = value;
            break;
        }
        default:
            LN_ENSURE(0);
            return;
    }
}

} // namespace ln
