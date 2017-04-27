
#include "../Internal.h"
#include <Lumino/IO/PathName.h>
#include <Lumino/IO/StreamWriter.h>
#include <Lumino/IO/StringWriter.h>
#include <Lumino/Xml/XmlWriter.h>

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// XmlWriter
//==============================================================================
const TCHAR* XmlWriter::DefaultIndentString = _T("  ");

//------------------------------------------------------------------------------
XmlWriter::XmlWriter(TextWriter* textWriter)
	: m_textWriter()
	, m_state(State_Start)
	, m_indentString(DefaultIndentString)
	, m_quoteChar(_T('"'))
{
	Initialize(textWriter);
}

//------------------------------------------------------------------------------
XmlWriter::~XmlWriter()
{

}

//------------------------------------------------------------------------------
void XmlWriter::Initialize(TextWriter* textWriter)
{
	m_textWriter = textWriter;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteStartDocument()
{
	if (LN_CHECK_STATE(m_state == State_Start)) return;

	m_textWriter->Write(_T("<?xml "));
	m_textWriter->Write(_T("version=\"1.0\""));
	m_textWriter->Write(_T(" encoding=\""));
	m_textWriter->Write(m_textWriter->GetEncoding()->GetName());
	m_textWriter->Write(_T("\""));
	m_textWriter->Write(_T("?>"));
	m_state = State_Prolog;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteEndDocument()
{
	m_state = State_Start;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteStartElement(const String& name)
{
	PreWrite(XmlNodeType::Element);
	m_textWriter->Write(_T('<'));
	m_textWriter->Write(name);

	ElementInfo info;
	info.Name = name;
	info.IndentSkip = false;
	m_elementStack.Push(info);
	m_state = State_StartElement;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteEndElement()
{
	if (LN_CHECK_STATE(!m_elementStack.IsEmpty())) return;
	if (LN_CHECK_STATE(m_state == State_Prolog || m_state == State_StartElement || m_state == State_Attribute || m_state == State_Text)) return;

	PreWrite(XmlNodeType::EndElement);
	if (m_state == State_StartElement || m_state == State_Attribute) {
		//m_textWriter->Write(_T(" />"));
	}
	else
	{
		m_textWriter->Write(_T("</"));
		m_textWriter->Write(m_elementStack.GetTop().Name);
		m_textWriter->Write(_T('>'));
	}

	m_elementStack.Pop();
	m_state = State_Prolog;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteAttribute(const String& name, const String& value)
{
	WriteStartAttribute(name);
	WriteStringInternal(value.c_str(), value.GetLength(), true);
	WriteEndAttribute();
}

//------------------------------------------------------------------------------
void XmlWriter::WriteString(const String& text)
{
	PreWrite(XmlNodeType::Text);
	WriteStringInternal(text.c_str(), text.GetLength(), false);
	m_state = State_Text;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteComment(const String& text)
{
	if (text.IndexOf(_T("--")) >= 0 ||
		(!text.IsEmpty() && text[text.GetLength() - 1] == '-')){
		LN_THROW(0, ArgumentException, _T("Invalidate Comment chars."))
	}

	PreWrite(XmlNodeType::Comment);
	m_textWriter->Write(_T("<!--"), 4);
	m_textWriter->Write(text);
	m_textWriter->Write(_T("-->"), 3);
	m_state = State_Prolog;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteCData(const String& text)
{
	if (text.IndexOf(_T("]]>")) >= 0) { LN_THROW(0, ArgumentException, _T("Invalidate CDATA chars.")) }

	PreWrite(XmlNodeType::CDATA);
	m_textWriter->Write(_T("<![CDATA["), 9);
	m_textWriter->Write(text);
	m_textWriter->Write(_T("]]>"), 3);
	m_state = State_Prolog;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteElementString(const String& elementName, const String& text)
{
	WriteStartElement(elementName);
	WriteString(text);
	WriteEndElement();
}

//------------------------------------------------------------------------------
void XmlWriter::WriteStartAttribute(const String& name)
{
	if (LN_CHECK_STATE(m_state == State_StartElement || m_state == State_Attribute)) return;

	m_textWriter->Write(_T(' '));
	m_textWriter->Write(name);
	m_textWriter->Write(_T("=\""));
	m_state = State_Attribute;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteEndAttribute()
{
	m_textWriter->Write(_T("\""));
	m_state = State_StartElement;
}

//------------------------------------------------------------------------------
void XmlWriter::WriteStringInternal(const TCHAR* str, int len, bool inAttribute)
{
	if (str == NULL || len == 0) { return; }

	Encoding* enc = Encoding::GetTCharEncoding();
	const TCHAR* begin = str;
	const TCHAR* end = begin + len;
	const TCHAR* pos = begin;
	while (pos < end)
	{
		int extra = enc->GetLeadExtraLength(pos, end - pos);
		if (extra > 0) 
		{
			// 先行バイトだった。そのまま進める
			pos += extra;
		}
		else
		{
			TCHAR ch = *pos;
			switch (ch)
			{
			case (char)0xA:
			case (char)0xD:
				m_textWriter->Write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;

				if (inAttribute) {
					// 3.3.3 属性値正規化
					m_textWriter->Write(_T(' '));
				}
				else {
					m_textWriter->Write(ch);
				}
				break;
			case '<':
				m_textWriter->Write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				m_textWriter->Write(_T("&lt;"), 4);
				break;
			case '>':
				m_textWriter->Write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				m_textWriter->Write(_T("&gt;"), 4);
				break;
			case '&':
				m_textWriter->Write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				m_textWriter->Write(_T("&amp;"), 5);
				break;
			case '\'':
				m_textWriter->Write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				if (inAttribute && m_quoteChar == ch) {
					m_textWriter->Write(_T("&apos;"), 6);
				}
				else {
					m_textWriter->Write('\'');
				}
				break;
			case '"':
				m_textWriter->Write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				if (inAttribute && m_quoteChar == ch) {
					m_textWriter->Write(_T("&quot;"), 6);
				}
				else {
					m_textWriter->Write('"');
				}
				break;
			default:

				

				break;
			}

		}

		++pos;
	}

	if (begin < pos)
	{
		m_textWriter->Write(begin, pos - begin);
	}
}

//------------------------------------------------------------------------------
void XmlWriter::PreWrite(XmlNodeType type)
{
	switch (type)
	{
	case XmlNodeType::Element:
	case XmlNodeType::CDATA:
	case XmlNodeType::Comment:
		if (m_state == XmlNodeType::Attribute) {	// 要素のネスト
			WriteEndAttribute();
			WriteStartTagEnd(false);
		}
		else if (m_state == State_StartElement) {
			WriteStartTagEnd(false);
		}
		if (type == XmlNodeType::CDATA) {
			// CDATA は改行しない
			m_elementStack.GetTop().IndentSkip = true;
		}
		if (m_state != State_Start) {
			Indent(false);	
		}
		break;
	case XmlNodeType::EndElement:
		if (m_state == State_StartElement) {
			WriteStartTagEnd(true);		// まだ開始タグ中なのに End が来たら空タグ
		}
		else {
			Indent(true);
		}
		break;
	case XmlNodeType::Text:
		if (m_state == State_StartElement) {
			WriteStartTagEnd(false);
		}
		// Text は前のタグとの間に改行やインデントをしない。
		// また、次の終了タグを書き込むときも改行やインデントしない。
		m_elementStack.GetTop().IndentSkip = true;
		break;
	}
}

//------------------------------------------------------------------------------
// 要素の開始タグを閉じる
//------------------------------------------------------------------------------
void XmlWriter::WriteStartTagEnd(bool empty)
{
	if (empty) {
		m_textWriter->Write(_T(" />"));
	}
	else {
		m_textWriter->Write(_T(">"));
	}
}

//------------------------------------------------------------------------------
//	beforeEndElement : true の場合、この後に終了タグを入れようとしている。
//------------------------------------------------------------------------------
void XmlWriter::Indent(bool beforeEndElement)
{
	//if (m_state = State_Prolog) {
	//	m_textWriter->WriteLine();
	//}

	if (m_elementStack.IsEmpty()) {
		m_textWriter->WriteLine();
	}
	//else if (m_elementStack.GetCount() == 1) {
	//	m_textWriter->WriteLine();
	//}
	else if (!m_elementStack.GetTop().IndentSkip)
	{
		m_textWriter->WriteLine();
		int level = m_elementStack.GetCount();
		if (beforeEndElement) { --level; }
		for (int i = 0; i < level; ++i) {
			m_textWriter->Write(m_indentString);
		}
	}
}


//==============================================================================
// XmlFileWriter
//==============================================================================

//------------------------------------------------------------------------------
XmlFileWriter::XmlFileWriter(const PathName& filePath, Encoding* encoding)
	: XmlWriter(NULL)
{
	RefPtr<StreamWriter> file(LN_NEW StreamWriter(filePath, encoding, FileWriteMode_Truncate), false);
	Initialize(file);
}

//------------------------------------------------------------------------------
XmlFileWriter::~XmlFileWriter()
{

}


//==============================================================================
// XmlStringWriter
//==============================================================================

//------------------------------------------------------------------------------
XmlStringWriter::XmlStringWriter()
	: XmlWriter(NULL)
	, m_stringWriter(NULL)
{
	m_stringWriter = LN_NEW StringWriter();
	Initialize(m_stringWriter);
}

//------------------------------------------------------------------------------
XmlStringWriter::~XmlStringWriter()
{
	LN_SAFE_RELEASE(m_stringWriter);
}

//------------------------------------------------------------------------------
void XmlStringWriter::SetNewLine(const String& newLine)
{
	m_stringWriter->SetNewLine(newLine);
}

//------------------------------------------------------------------------------
String XmlStringWriter::ToString()
{
	return m_stringWriter->ToString();
}

} // namespace tr
LN_NAMESPACE_END
