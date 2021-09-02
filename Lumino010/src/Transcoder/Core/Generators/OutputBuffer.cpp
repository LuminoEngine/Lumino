
#include "OutputBuffer.hpp"

OutputBuffer::OutputBuffer()
	: m_buffer()
	, m_indentLevel(0)
	, m_indent()
	, m_newLineCode("\n")
	, m_commandSeparator(u", ")
	, m_state(State::LineHead)
{
}

OutputBuffer::OutputBuffer(int indent)
	: OutputBuffer()
{
	for (int i = 0 ; i < indent; i++) IncreaseIndent();
}

void OutputBuffer::clear()
{
	m_buffer.clear();
	m_indentLevel = 0;
	m_indent.clear();
	m_state = State::LineHead;
}

void OutputBuffer::IncreaseIndent(int count)
{
	m_indentLevel += count;
	m_indent.clear();
	for (int i = 0; i < m_indentLevel; i++)
		m_indent += _T("    ");
}

void OutputBuffer::DecreaseIndent()
{
	m_indentLevel--;
	m_indent.clear();
	for (int i = 0; i < m_indentLevel; i++)
		m_indent += _T("    ");
}

OutputBuffer& OutputBuffer::NewLine(int count)
{
	for (int i = 0; i < count; i++)
		m_buffer.append(m_newLineCode);

	m_state = State::LineHead;
	return *this;
}

void OutputBuffer::indent()
{
	m_buffer.append(m_indent);
}

ln::String OutputBuffer::toString() const
{
	//return m_buffer.toString();
	return m_buffer;
}

void OutputBuffer::AppendInternal(const ln::StringRef& str)
{
	if (m_state == State::LineHead)
	{
		indent();
		m_state = State::Text;
	}
	m_buffer.append(str);
}

void OutputBuffer::AppendLineInternal(const ln::StringRef& str)
{
	//indent();
	//m_buffer.append(str);
	AppendLinesInternal(str, ln::String::Empty);
}

void OutputBuffer::AppendLinesInternal(const ln::StringRef& str, const ln::StringRef& lineHeader)
{
	ln::String ns = ln::String(str).replace(u"\r", u"");
	ln::List<ln::String> lines = ns.split(u"\n", ln::StringSplitOptions::None);
	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i].length() > 0) indent();
		m_buffer.append(lineHeader);
		m_buffer.append(lines[i]);

		// 最後の一つは改行しない
		if (i != lines.size() - 1) NewLine();
	}
	m_state = State::LineHead;
	NewLine();
}

