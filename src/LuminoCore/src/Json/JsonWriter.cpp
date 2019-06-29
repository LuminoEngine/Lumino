
#include "Internal.hpp"
#include <LuminoCore/Math/Math.hpp>
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/Json/JsonWriter.hpp>

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
    autoComplete(JsonNode::StartObject);
    m_levelStack.push(Level(false));
    onStartObject();
}

void JsonWriter::writeEndObject()
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;
    if (LN_REQUIRE(!m_levelStack.top().inArray))
        return;

    autoComplete(JsonNode::EndObject);
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
    autoComplete(JsonNode::StartArray);
    m_levelStack.push(Level(true));
    onStartArray();
}

void JsonWriter::writeEndArray()
{
    if (LN_REQUIRE(m_levelStack.top().inArray))
        return;

    autoComplete(JsonNode::EndArray);
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
    if (LN_REQUIRE(!m_levelStack.top().inArray))
        return;

    autoComplete(JsonNode::PropertyName);
    onKey(str.data(), str.length());
    m_levelStack.top().justSawKey = true;
}

void JsonWriter::writeNull()
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonNode::Null);
    onNull();
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeBool(bool value)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonNode::Boolean);
    onBool(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeInt32(int32_t value)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;
    autoComplete(JsonNode::Int32);
    onInt32(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeInt64(int64_t value)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;
    autoComplete(JsonNode::Int64);
    onInt64(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeFloat(float value)
{
    if (Math::isNaN(value)) {
        writeString(u"NaN");    // like Json.NET
        return;
    }
    if (Math::isInf(value)) {
        writeString(u"Inf");    // like Json.NET
        return;
    }

    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;
    autoComplete(JsonNode::Float);
    onFloat(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeDouble(double value)
{
    if (Math::isNaN(value)) {
        writeString(u"NaN");    // like Json.NET
        return;
    }
    if (Math::isInf(value)) {
        writeString(u"Inf");    // like Json.NET
        return;
    }

    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonNode::Double);
    onDouble(value);
    m_levelStack.top().valueCount++;
}

void JsonWriter::writeString(const StringRef& str)
{
    if (LN_REQUIRE(m_levelStack.size() >= 1))
        return;

    autoComplete(JsonNode::String);
    onString(str.data(), str.length());
    m_levelStack.top().valueCount++;
}

bool JsonWriter::isComplete() const
{
    return m_levelStack.empty();
}

void JsonWriter::autoComplete(JsonNode token)
{
    if (!m_levelStack.empty()) {
        Level& level = m_levelStack.top();

        if (token != JsonNode::EndObject && token != JsonNode::EndArray) {
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
                if (token == JsonNode::StartObject || token == JsonNode::StartArray || token == JsonNode::PropertyName) {
                    m_textWriter->writeLine();
                    for (int i = 0; i < m_levelStack.size(); i++) {
                        m_textWriter->write(_LT(' '));
                        m_textWriter->write(_LT(' '));
                    }
                } else if (token == JsonNode::EndObject || (token == JsonNode::EndArray && level.justSawContainerEnd)) {
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
    m_textWriter->write(StringRef(_LT("null"), 4));
}

void JsonWriter::onBool(bool value)
{
    if (value) {
        m_textWriter->write(StringRef(_LT("true"), 4));
    } else {
        m_textWriter->write(StringRef(_LT("false"), 5));
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
    m_textWriter->write('"');

    for (int i = 0; i < length; i++) {
        switch (str[i]) {
            case '"': {
                Char s[] = {'\\', '"'};
                m_textWriter->write(StringRef(s, 2));
                break;
            }
            case '\\': {
                Char s[] = {'\\', '\\'};
                m_textWriter->write(StringRef(s, 2));
                break;
            }
#if 0   // ファイル出力の時は / エスケープしなくてもいい。外部編集可能な設定を書き出すときにエスケープされていると少し混乱するので、今はスキップしておく。
        // ちなみに rapidjson も同様にエスケープされない。
            case '/': {
                Char s[] = {'\\', '/'};
                m_textWriter->write(StringRef(s, 2));
                break;
            }
#endif
            case '\b': {
                Char s[] = {'\\', 'b'};
                m_textWriter->write(StringRef(s, 2));
                break;
            }
            case '\f': {
                Char s[] = {'\\', 'f'};
                m_textWriter->write(StringRef(s, 2));
                break;
            }
            case '\n': {
                Char s[] = {'\\', 'n'};
                m_textWriter->write(StringRef(s, 2));
                break;
            }
            case '\r': {
                Char s[] = {'\\', 'r'};
                m_textWriter->write(StringRef(s, 2));
                break;
            }
            case '\t': {
                Char s[] = {'\\', 't'};
                m_textWriter->write(StringRef(s, 2));
                break;
            }
            default:
                m_textWriter->write(str[i]);
                break;
        }
    }

    m_textWriter->write('"');
}

} // namespace ln
