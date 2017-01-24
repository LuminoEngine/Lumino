
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
	LN_CHECK_ARG(m_textWriter != nullptr);
	m_levelStack.Reserve(32);
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
	m_levelStack.Push(Level(false));
	OnStartObject();
}

//------------------------------------------------------------------------------
void JsonWriter::WriteEndObject()
{
	LN_CHECK_ARG(m_levelStack.GetCount() >= 1);
	LN_CHECK_ARG(!m_levelStack.GetTop().inArray);

	AutoComplete(JsonToken::EndObject);
	m_levelStack.Pop();
	OnEndObject();

	if (!m_levelStack.IsEmpty())	// ルート要素のクローズに備える
	{
		m_levelStack.GetTop().valueCount++;
		m_levelStack.GetTop().justSawContainerEnd = true;
	}
}

//------------------------------------------------------------------------------
void JsonWriter::WriteStartArray()
{
	AutoComplete(JsonToken::StartArray);
	m_levelStack.Push(Level(true));
	OnStartArray();
}

//------------------------------------------------------------------------------
void JsonWriter::WriteEndArray()
{
	LN_CHECK_ARG(m_levelStack.GetCount() >= 2);
	LN_CHECK_ARG(m_levelStack.GetTop().inArray);

	AutoComplete(JsonToken::EndArray);
	m_levelStack.Pop();
	OnEndArray();
	m_levelStack.GetTop().valueCount++;
	m_levelStack.GetTop().justSawContainerEnd = true;
}

//------------------------------------------------------------------------------
void JsonWriter::WritePropertyName(const TCHAR* str, int length)
{
	LN_CHECK_ARG(m_levelStack.GetCount() >= 1);
	length = (length <= -1) ? (int)StringTraits::tcslen(str) : length;

	AutoComplete(JsonToken::PropertyName);
	OnKey(str, length);
	m_levelStack.GetTop().justSawKey = true;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteNull()
{
	LN_CHECK_ARG(m_levelStack.GetCount() >= 1);

	AutoComplete(JsonToken::Null);
	OnNull();
	m_levelStack.GetTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteBool(bool value)
{
	LN_CHECK_ARG(m_levelStack.GetCount() >= 1);

	AutoComplete(JsonToken::Boolean);
	OnBool(value);
	m_levelStack.GetTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteInt32(int32_t value)
{
	LN_FAIL_CHECK_ARG(m_levelStack.GetCount() >= 1) return;
	AutoComplete(JsonToken::Double);
	OnInt32(value);
	m_levelStack.GetTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteInt64(int64_t value)
{
	LN_FAIL_CHECK_ARG(m_levelStack.GetCount() >= 1) return;
	AutoComplete(JsonToken::Double);
	OnInt64(value);
	m_levelStack.GetTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteFloat(float value)
{
	LN_FAIL_CHECK_ARG(m_levelStack.GetCount() >= 1) return;
	AutoComplete(JsonToken::Double);
	OnFloat(value);
	m_levelStack.GetTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteDouble(double value)
{
	LN_CHECK_ARG(m_levelStack.GetCount() >= 1);

	AutoComplete(JsonToken::Double);
	OnDouble(value);
	m_levelStack.GetTop().valueCount++;
}

//------------------------------------------------------------------------------
void JsonWriter::WriteString(const TCHAR* str, int length)	// TODO: StringRef
{
	LN_CHECK_ARG(m_levelStack.GetCount() >= 1);
	length = (length <= -1) ? (int)StringTraits::tcslen(str) : length;

	AutoComplete(JsonToken::String);
	OnString(str, length);
	m_levelStack.GetTop().valueCount++;
}

//------------------------------------------------------------------------------
bool JsonWriter::IsComplete() const
{
	return m_levelStack.IsEmpty();
}

//------------------------------------------------------------------------------
void JsonWriter::AutoComplete(JsonToken token)
{
	if (!m_levelStack.IsEmpty())
	{
		Level& level = m_levelStack.GetTop();

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
				m_textWriter->Write(_T(' '));	// : after space
			}

			if (!level.justSawKey)
			{
				if (token == JsonToken::StartObject || token == JsonToken::StartArray || token == JsonToken::PropertyName)
				{
					m_textWriter->WriteLine();
					for (int i = 0; i < m_levelStack.GetCount(); i++)
					{
						m_textWriter->Write(_T(' '));
						m_textWriter->Write(_T(' '));
					}
				}
				else if (token == JsonToken::EndObject || (token == JsonToken::EndArray && level.justSawContainerEnd))
				{
					m_textWriter->WriteLine();
					for (int i = 0; i < m_levelStack.GetCount() - 1; i++)
					{
						m_textWriter->Write(_T(' '));
						m_textWriter->Write(_T(' '));
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
			m_textWriter->Write(_T(','));
		}
	}
	else {
		m_textWriter->Write(_T(':'));
	}
}

//------------------------------------------------------------------------------
void JsonWriter::OnStartObject()
{
	m_textWriter->Write(_T('{'));
}

//------------------------------------------------------------------------------
void JsonWriter::OnEndObject()
{
	m_textWriter->Write(_T('}'));
}

//------------------------------------------------------------------------------
void JsonWriter::OnStartArray()
{
	m_textWriter->Write(_T('['));
}

//------------------------------------------------------------------------------
void JsonWriter::OnEndArray()
{
	m_textWriter->Write(_T(']'));
}

//------------------------------------------------------------------------------
void JsonWriter::OnKey(const TCHAR* str, int length)
{
	OnString(str, length);
}

//------------------------------------------------------------------------------
void JsonWriter::OnNull()
{
	m_textWriter->Write(_T("null"), 4);
}

//------------------------------------------------------------------------------
void JsonWriter::OnBool(bool value)
{
	if (value) {
		m_textWriter->Write(_T("true"), 4);
	}
	else {
		m_textWriter->Write(_T("false"), 5);
	}
}

//------------------------------------------------------------------------------
void JsonWriter::OnInt32(int32_t value)
{
	m_textWriter->Write(value);
}

//------------------------------------------------------------------------------
void JsonWriter::OnInt64(int64_t value)
{
	m_textWriter->Write(value);
}

//------------------------------------------------------------------------------
void JsonWriter::OnFloat(float value)
{
	m_textWriter->Write(value);
}

//------------------------------------------------------------------------------
void JsonWriter::OnDouble(double value)
{
	m_textWriter->Write(value);
}

//------------------------------------------------------------------------------
void JsonWriter::OnString(const TCHAR* str, int length)
{
	m_textWriter->Write(_T("\""), 1);
	m_textWriter->Write(str, length);
	m_textWriter->Write(_T("\""), 1);
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
