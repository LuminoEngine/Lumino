
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/Json/JsonWriter.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// JsonWriter
//==============================================================================
//------------------------------------------------------------------------------
JsonWriter::JsonWriter(TextWriter* textWriter)
	: m_formatting(JsonFormatting::None)
	, m_textWriter(textWriter)
{
	if (LN_CHECK_ARG(m_textWriter != nullptr)) return;
	m_levelStack.reserve(32);
}

//------------------------------------------------------------------------------
JsonWriter::~JsonWriter()
{
}

//------------------------------------------------------------------------------
void JsonWriter::SetFormatting(JsonFormatting formatting)
{
	m_formatting = formatting;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteStartObject()
{
	AutoComplete(JsonToken::StartObject);
	m_levelStack.push(Level(false));
	OnStartObject();
}

//------------------------------------------------------------------------------
void JsonWriter::WriteEndObject()
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;
	if (LN_CHECK_ARG(!m_levelStack.getTop().inArray)) return;

	AutoComplete(JsonToken::EndObject);
	m_levelStack.pop();
	OnEndObject();

	if (!m_levelStack.isEmpty())	// ルート要素のクローズに備える
	{
		m_levelStack.getTop().valueCount++;
		m_levelStack.getTop().justSawContainerEnd = true;
	}
}

//------------------------------------------------------------------------------
void JsonWriter::WriteStartArray()
{
	AutoComplete(JsonToken::StartArray);
	m_levelStack.push(Level(true));
	OnStartArray();
}

//------------------------------------------------------------------------------
void JsonWriter::WriteEndArray()
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 2)) return;
	if (LN_CHECK_ARG(m_levelStack.getTop().inArray)) return;

	AutoComplete(JsonToken::EndArray);
	m_levelStack.pop();
	OnEndArray();
	m_levelStack.getTop().valueCount++;
	m_levelStack.getTop().justSawContainerEnd = true;
}

//------------------------------------------------------------------------------
void JsonWriter::WritePropertyName(const TCHAR* str, int length)
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;
	length = (length <= -1) ? (int)StringTraits::tcslen(str) : length;

	AutoComplete(JsonToken::PropertyName);
	OnKey(str, length);
	m_levelStack.getTop().justSawKey = true;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteNull()
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;

	AutoComplete(JsonToken::Null);
	OnNull();
	m_levelStack.getTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteBool(bool value)
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;

	AutoComplete(JsonToken::Boolean);
	OnBool(value);
	m_levelStack.getTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteInt32(int32_t value)
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;
	AutoComplete(JsonToken::Double);
	OnInt32(value);
	m_levelStack.getTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteInt64(int64_t value)
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;
	AutoComplete(JsonToken::Double);
	OnInt64(value);
	m_levelStack.getTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteFloat(float value)
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;
	AutoComplete(JsonToken::Double);
	OnFloat(value);
	m_levelStack.getTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteDouble(double value)
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;

	AutoComplete(JsonToken::Double);
	OnDouble(value);
	m_levelStack.getTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteString(const TCHAR* str, int length)	// TODO: StringRef
{
	if (LN_CHECK_ARG(m_levelStack.getCount() >= 1)) return;
	length = (length <= -1) ? (int)StringTraits::tcslen(str) : length;

	AutoComplete(JsonToken::String);
	OnString(str, length);
	m_levelStack.getTop().valueCount++;
}

//------------------------------------------------------------------------------
bool JsonWriter::IsComplete() const
{
	return m_levelStack.isEmpty();
}

//------------------------------------------------------------------------------
void JsonWriter::AutoComplete(JsonToken token)
{
	if (!m_levelStack.isEmpty())
	{
		Level& level = m_levelStack.getTop();

		if (token != JsonToken::EndObject && token != JsonToken::EndArray)
		{
			if (level.justSawKey) {
				OnPrefix(PrefixType_Key, level.valueCount);
			}
			else if (level.valueCount > 0)
			{
				if (level.inArray) {
					OnPrefix(PrefixType_Array, level.valueCount);
				}
				else {
					OnPrefix(PrefixType_Object, level.valueCount);
				}
			}
		}

		if (m_formatting == JsonFormatting::Indented)
		{
			if (level.justSawKey)
			{
				m_textWriter->write(_T(' '));	// : after space
			}

			if (!level.justSawKey)
			{
				if (token == JsonToken::StartObject || token == JsonToken::StartArray || token == JsonToken::PropertyName)
				{
					m_textWriter->writeLine();
					for (int i = 0; i < m_levelStack.getCount(); i++)
					{
						m_textWriter->write(_T(' '));
						m_textWriter->write(_T(' '));
					}
				}
				else if (token == JsonToken::EndObject || (token == JsonToken::EndArray && level.justSawContainerEnd))
				{
					m_textWriter->writeLine();
					for (int i = 0; i < m_levelStack.getCount() - 1; i++)
					{
						m_textWriter->write(_T(' '));
						m_textWriter->write(_T(' '));
					}
				}
			}
		}

		level.justSawKey = false;
		level.justSawContainerEnd = false;
	}
}

//------------------------------------------------------------------------------
void JsonWriter::OnPrefix(PrefixType type, int valueCount)
{
	if (type == PrefixType_Array || type == PrefixType_Object) {
		if (valueCount > 0) {
			m_textWriter->write(_T(','));
		}
	}
	else {
		m_textWriter->write(_T(':'));
	}
}

//------------------------------------------------------------------------------
void JsonWriter::OnStartObject()
{
	m_textWriter->write(_T('{'));
}

//------------------------------------------------------------------------------
void JsonWriter::OnEndObject()
{
	m_textWriter->write(_T('}'));
}

//------------------------------------------------------------------------------
void JsonWriter::OnStartArray()
{
	m_textWriter->write(_T('['));
}

//------------------------------------------------------------------------------
void JsonWriter::OnEndArray()
{
	m_textWriter->write(_T(']'));
}

//------------------------------------------------------------------------------
void JsonWriter::OnKey(const TCHAR* str, int length)
{
	OnString(str, length);
}

//------------------------------------------------------------------------------
void JsonWriter::OnNull()
{
	m_textWriter->write(_T("null"), 4);
}

//------------------------------------------------------------------------------
void JsonWriter::OnBool(bool value)
{
	if (value) {
		m_textWriter->write(_T("true"), 4);
	}
	else {
		m_textWriter->write(_T("false"), 5);
	}
}

//------------------------------------------------------------------------------
void JsonWriter::OnInt32(int32_t value)
{
	m_textWriter->write(value);
}

//------------------------------------------------------------------------------
void JsonWriter::OnInt64(int64_t value)
{
	m_textWriter->write(value);
}

//------------------------------------------------------------------------------
void JsonWriter::OnFloat(float value)
{
	m_textWriter->write(value);
}

//------------------------------------------------------------------------------
void JsonWriter::OnDouble(double value)
{
	m_textWriter->write(value);
}

//------------------------------------------------------------------------------
void JsonWriter::OnString(const TCHAR* str, int length)
{
	m_textWriter->write(_T("\""), 1);
	m_textWriter->write(str, length);
	m_textWriter->write(_T("\""), 1);
}

//------------------------------------------------------------------------------
//void JsonWriter::OnIndent(int level)
//{
//	m_textWriter->WriteLine();
//	while (level > 0)
//	{
//		m_textWriter->Write(_T(' '));
//		--level;
//	}
//}

} // namespace tr
LN_NAMESPACE_END
