
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
const Char* XmlWriter::DefaultIndentString = _TT("  ");

//------------------------------------------------------------------------------
XmlWriter::XmlWriter(TextWriter* textWriter)
	: m_textWriter()
	, m_state(State_Start)
	, m_indentString(DefaultIndentString)
	, m_quoteChar(_LT('"'))
{
	initialize(textWriter);
}

//------------------------------------------------------------------------------
XmlWriter::~XmlWriter()
{

}

//------------------------------------------------------------------------------
void XmlWriter::initialize(TextWriter* textWriter)
{
	m_textWriter = textWriter;
}

//------------------------------------------------------------------------------
void XmlWriter::writeStartDocument()
{
	if (LN_REQUIRE(m_state == State_Start)) return;

	m_textWriter->write(_TT("<?xml "));
	m_textWriter->write(_TT("version=\"1.0\""));
	m_textWriter->write(_TT(" encoding=\""));
	m_textWriter->write(m_textWriter->getEncoding()->getName());
	m_textWriter->write(_TT("\""));
	m_textWriter->write(_TT("?>"));
	m_state = State_Prolog;
}

//------------------------------------------------------------------------------
void XmlWriter::writeEndDocument()
{
	m_state = State_Start;
}

//------------------------------------------------------------------------------
void XmlWriter::writeStartElement(const String& name)
{
	preWrite(XmlNodeType::Element);
	m_textWriter->write(_LT('<'));
	m_textWriter->write(name);

	ElementInfo info;
	info.Name = name;
	info.IndentSkip = false;
	m_elementStack.push(info);
	m_state = State_StartElement;
}

//------------------------------------------------------------------------------
void XmlWriter::writeEndElement()
{
	if (LN_REQUIRE(!m_elementStack.isEmpty())) return;
	if (LN_REQUIRE(m_state == State_Prolog || m_state == State_StartElement || m_state == State_Attribute || m_state == State_Text)) return;

	preWrite(XmlNodeType::EndElement);
	if (m_state == State_StartElement || m_state == State_Attribute) {
		//m_textWriter->Write(_LT(" />"));
	}
	else
	{
		m_textWriter->write(_LT("</"));
		m_textWriter->write(m_elementStack.getTop().Name);
		m_textWriter->write(_LT('>'));
	}

	m_elementStack.pop();
	m_state = State_Prolog;
}

//------------------------------------------------------------------------------
void XmlWriter::writeAttribute(const String& name, const String& value)
{
	writeStartAttribute(name);
	writeStringInternal(value.c_str(), value.getLength(), true);
	writeEndAttribute();
}

//------------------------------------------------------------------------------
void XmlWriter::writeString(const String& text)
{
	preWrite(XmlNodeType::Text);
	writeStringInternal(text.c_str(), text.getLength(), false);
	m_state = State_Text;
}

//------------------------------------------------------------------------------
void XmlWriter::writeComment(const String& text)
{
	if (text.indexOf(_TT("--")) >= 0 ||
		(!text.isEmpty() && text[text.getLength() - 1] == '-'))
	{
		LN_REQUIRE(0, _TT("Invalidate Comment chars."));
		return;
	}

	preWrite(XmlNodeType::Comment);
	m_textWriter->write(_TT("<!--"), 4);
	m_textWriter->write(text);
	m_textWriter->write(_TT("-->"), 3);
	m_state = State_Prolog;
}

//------------------------------------------------------------------------------
void XmlWriter::writeCData(const String& text)
{
	if (text.indexOf(_TT("]]>")) >= 0)
	{
		LN_REQUIRE(0, _TT("Invalidate CDATA chars."));
		return;
	}

	preWrite(XmlNodeType::CDATA);
	m_textWriter->write(_TT("<![CDATA["), 9);
	m_textWriter->write(text);
	m_textWriter->write(_TT("]]>"), 3);
	m_state = State_Prolog;
}

//------------------------------------------------------------------------------
void XmlWriter::writeElementString(const String& elementName, const String& text)
{
	writeStartElement(elementName);
	writeString(text);
	writeEndElement();
}

//------------------------------------------------------------------------------
void XmlWriter::writeStartAttribute(const String& name)
{
	if (LN_REQUIRE(m_state == State_StartElement || m_state == State_Attribute)) return;

	m_textWriter->write(_TT(' '));
	m_textWriter->write(name);
	m_textWriter->write(_TT("=\""));
	m_state = State_Attribute;
}

//------------------------------------------------------------------------------
void XmlWriter::writeEndAttribute()
{
	m_textWriter->write(_TT("\""));
	m_state = State_StartElement;
}

//------------------------------------------------------------------------------
void XmlWriter::writeStringInternal(const Char* str, int len, bool inAttribute)
{
	if (str == NULL || len == 0) { return; }

	Encoding* enc = Encoding::getTCharEncoding();
	const Char* begin = str;
	const Char* end = begin + len;
	const Char* pos = begin;
	while (pos < end)
	{
		int extra = enc->getLeadExtraLength(pos, end - pos);
		if (extra > 0) 
		{
			// 先行バイトだった。そのまま進める
			pos += extra;
		}
		else
		{
			Char ch = *pos;
			switch (ch)
			{
			case (char)0xA:
			case (char)0xD:
				m_textWriter->write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;

				if (inAttribute) {
					// 3.3.3 属性値正規化
					m_textWriter->write(_LT(' '));
				}
				else {
					m_textWriter->write(ch);
				}
				break;
			case '<':
				m_textWriter->write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				m_textWriter->write(_LT("&lt;"), 4);
				break;
			case '>':
				m_textWriter->write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				m_textWriter->write(_LT("&gt;"), 4);
				break;
			case '&':
				m_textWriter->write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				m_textWriter->write(_LT("&amp;"), 5);
				break;
			case '\'':
				m_textWriter->write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				if (inAttribute && m_quoteChar == ch) {
					m_textWriter->write(_LT("&apos;"), 6);
				}
				else {
					m_textWriter->write('\'');
				}
				break;
			case '"':
				m_textWriter->write(begin, pos - begin);	// pos の前までを出力
				begin = pos + 1;
				if (inAttribute && m_quoteChar == ch) {
					m_textWriter->write(_LT("&quot;"), 6);
				}
				else {
					m_textWriter->write('"');
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
		m_textWriter->write(begin, pos - begin);
	}
}

//------------------------------------------------------------------------------
void XmlWriter::preWrite(XmlNodeType type)
{
	switch (type)
	{
	case XmlNodeType::Element:
	case XmlNodeType::CDATA:
	case XmlNodeType::Comment:
		if (m_state == XmlNodeType::Attribute) {	// 要素のネスト
			writeEndAttribute();
			writeStartTagEnd(false);
		}
		else if (m_state == State_StartElement) {
			writeStartTagEnd(false);
		}
		if (type == XmlNodeType::CDATA) {
			// CDATA は改行しない
			m_elementStack.getTop().IndentSkip = true;
		}
		if (m_state != State_Start) {
			indent(false);	
		}
		break;
	case XmlNodeType::EndElement:
		if (m_state == State_StartElement) {
			writeStartTagEnd(true);		// まだ開始タグ中なのに End が来たら空タグ
		}
		else {
			indent(true);
		}
		break;
	case XmlNodeType::Text:
		if (m_state == State_StartElement) {
			writeStartTagEnd(false);
		}
		// Text は前のタグとの間に改行やインデントをしない。
		// また、次の終了タグを書き込むときも改行やインデントしない。
		m_elementStack.getTop().IndentSkip = true;
		break;
	}
}

//------------------------------------------------------------------------------
// 要素の開始タグを閉じる
//------------------------------------------------------------------------------
void XmlWriter::writeStartTagEnd(bool empty)
{
	if (empty) {
		m_textWriter->write(_TT(" />"));
	}
	else {
		m_textWriter->write(_TT(">"));
	}
}

//------------------------------------------------------------------------------
//	beforeEndElement : true の場合、この後に終了タグを入れようとしている。
//------------------------------------------------------------------------------
void XmlWriter::indent(bool beforeEndElement)
{
	//if (m_state = State_Prolog) {
	//	m_textWriter->WriteLine();
	//}

	if (m_elementStack.isEmpty()) {
		m_textWriter->writeLine();
	}
	//else if (m_elementStack.GetCount() == 1) {
	//	m_textWriter->WriteLine();
	//}
	else if (!m_elementStack.getTop().IndentSkip)
	{
		m_textWriter->writeLine();
		int level = m_elementStack.getCount();
		if (beforeEndElement) { --level; }
		for (int i = 0; i < level; ++i) {
			m_textWriter->write(m_indentString);
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
	Ref<StreamWriter> file(LN_NEW StreamWriter(filePath, encoding, FileWriteMode_Truncate), false);
	initialize(file);
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
	initialize(m_stringWriter);
}

//------------------------------------------------------------------------------
XmlStringWriter::~XmlStringWriter()
{
	LN_SAFE_RELEASE(m_stringWriter);
}

//------------------------------------------------------------------------------
void XmlStringWriter::setNewLine(const String& newLine)
{
	m_stringWriter->setNewLine(newLine);
}

//------------------------------------------------------------------------------
String XmlStringWriter::toString()
{
	return m_stringWriter->toString();
}

} // namespace tr
LN_NAMESPACE_END
