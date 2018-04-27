
#include "Internal.hpp"
#include <Lumino/Base/String.hpp>
#include <Lumino/Json/JsonWriter.hpp>

namespace ln {

//==============================================================================
// JsonWriter

JsonWriter::JsonWriter(TextWriter* textWriter)
    : m_formatting(JsonFormatting::None)
    , m_textWriter(textWriter)
{
    if (LN_REQUIRE(m_textWriter != nullptr))
        return;
}

JsonWriter::~JsonWriter()
{
}

void JsonWriter::setFormatting(JsonFormatting formatting)
{
    m_formatting = formatting;
}

void JsonWriter::writeStartObject()
{
    autoComplete(JsonToken::StartObject);
    m_levelStack.push(Level(false));
    onStartObject();
}

void JsonWriter::writeEndObject()
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;
    if (LN_REQUIRE(!m_levelStack.top().inArray))
        return;

    autoComplete(JsonToken::EndObject);
    m_levelStack.pop();
    onEndObject();

    if (!m_levelStack.empty()) // Prepare to close the root element
    {
        m_levelStack.top().valueCount++;
        m_levelStack.top().justSawContainerEnd = true;
    }
}

void JsonWriter::writeStartArray()
{
    autoComplete(JsonToken::StartArray);
    m_levelStack.push(Level(true));
    onStartArray();
}

void JsonWriter::writeEndArray()
{
    if (LN_REQUIRE(m_levelStack.top().inArray))
        return;

    autoComplete(JsonToken::EndArray);
    m_levelStack.pop();
    onEndArray();

    if (!m_levelStack.empty()) {
        m_levelStack.top().valueCount++;
        m_levelStack.top().justSawContainerEnd = true;
    }
}

void JsonWriter::writePropertyName(const StringRef& str)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonToken::PropertyName);
    onKey(str.getBegin(), str.length());
    m_levelStack.top().justSawKey = true;
}

void JsonWriter::writeNull()
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonToken::Null);
    onNull();
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeBool(bool value)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonToken::Boolean);
    onBool(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeInt32(int32_t value)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;
    autoComplete(JsonToken::Int32);
    onInt32(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeInt64(int64_t value)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;
    autoComplete(JsonToken::Int64);
    onInt64(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeFloat(float value)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;
    autoComplete(JsonToken::Float);
    onFloat(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeDouble(double value)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonToken::Double);
    onDouble(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeString(const StringRef& str) // TODO: StringRef
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonToken::String);
    onString(str.getBegin(), str.length());
    m_levelStack.top().valueCount++;
}

bool JsonWriter::isComplete() const
{
    return m_levelStack.empty();
}

void JsonWriter::autoComplete(JsonToken token)
{
    if (!m_levelStack.empty()) {
        Level& level = m_levelStack.top();

        if (token != JsonToken::EndObject && token != JsonToken::EndArray) {
            if (level.justSawKey) {
                onPrefix(PrefixType::Key, level.valueCount);
            } else if (level.valueCount > 0) {
                if (level.inArray) {
                    onPrefix(PrefixType::Array, level.valueCount);
                } else {
                    onPrefix(PrefixType::Object, level.valueCount);
                }
            }
        }

        if (m_formatting == JsonFormatting::Indented) {
            if (level.justSawKey) {
                m_textWriter->write(_LT(' ')); // : after space
            }

            if (!level.justSawKey) {
                if (token == JsonToken::StartObject || token == JsonToken::StartArray || token == JsonToken::PropertyName) {
                    m_textWriter->writeLine();
                    for (int i = 0; i < m_levelStack.size(); i++) {
                        m_textWriter->write(_LT(' '));
                        m_textWriter->write(_LT(' '));
                    }
                } else if (token == JsonToken::EndObject || (token == JsonToken::EndArray && level.justSawContainerEnd)) {
                    m_textWriter->writeLine();
                    for (int i = 0; i < ((int)m_levelStack.size()) - 1; i++) {
                        m_textWriter->write(_LT(' '));
                        m_textWriter->write(_LT(' '));
                    }
                }
            }
        }

        level.justSawKey = false;
        level.justSawContainerEnd = false;
    }
}

void JsonWriter::onPrefix(PrefixType type, int valueCount)
{
    if (type == PrefixType::Array || type == PrefixType::Object) {
        if (valueCount > 0) {
            m_textWriter->write(_LT(','));
        }
    } else {
        m_textWriter->write(_LT(':'));
    }
}

void JsonWriter::onStartObject()
{
    m_textWriter->write(_LT('{'));
}

void JsonWriter::onEndObject()
{
    m_textWriter->write(_LT('}'));
}

void JsonWriter::onStartArray()
{
    m_textWriter->write(_LT('['));
}

void JsonWriter::onEndArray()
{
    m_textWriter->write(_LT(']'));
}

void JsonWriter::onKey(const Char* str, int length)
{
    onString(str, length);
}

void JsonWriter::onNull()
{
    m_textWriter->write(_LT("null"), 4);
}

void JsonWriter::onBool(bool value)
{
    if (value) {
        m_textWriter->write(_LT("true"), 4);
    } else {
        m_textWriter->write(_LT("false"), 5);
    }
}

void JsonWriter::onInt32(int32_t value)
{
    m_textWriter->write(value);
}

void JsonWriter::onInt64(int64_t value)
{
    m_textWriter->write(value);
}

void JsonWriter::onFloat(float value)
{
    m_textWriter->write(value);
}

void JsonWriter::onDouble(double value)
{
    m_textWriter->write(value);
}

void JsonWriter::onString(const Char* str, int length)
{
    m_textWriter->write(_TT("\""), 1);

    // TODO: 1文字ずつじゃ少し重いか・・・？
    for (int i = 0; i < length; i++) {
        switch (str[i]) {
            case '"':
                m_textWriter->write(_T('\\'));
                m_textWriter->write(_T('"'));
                break;
            case '\\':
                m_textWriter->write(_T('\\'));
                m_textWriter->write(_T('\\'));
                break;
            case '/':
                m_textWriter->write(_T('\\'));
                m_textWriter->write(_T('/'));
                break;
            case '\b':
                m_textWriter->write(_T('\\'));
                m_textWriter->write(_T('b'));
                break;
            case '\f':
                m_textWriter->write(_T('\\'));
                m_textWriter->write(_T('f'));
                break;
            case '\n':
                m_textWriter->write(_T('\\'));
                m_textWriter->write(_T('n'));
                break;
            case '\r':
                m_textWriter->write(_T('\\'));
                m_textWriter->write(_T('r'));
                break;
            case '\t':
                m_textWriter->write(_T('\\'));
                m_textWriter->write(_T('t'));
                break;
            default:
                m_textWriter->write(str[i]);
                break;
        }
    }

    m_textWriter->write(_TT("\""), 1);
}

} // namespace ln
