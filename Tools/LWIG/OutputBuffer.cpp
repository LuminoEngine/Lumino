
#include "OutputBuffer.h"

OutputBuffer::OutputBuffer()
	: m_buffer()
	, m_indentLevel(0)
	, m_indent()
	, m_newLineCode(_T("\r\n"))
	, m_state(State::LineHead)
{
}

OutputBuffer::OutputBuffer(int indent)
	: OutputBuffer()
{
	for (int i = 0 ; i < indent; i++) IncreaseIndent();
}

void OutputBuffer::Clear()
{
	m_buffer.Clear();
	m_indentLevel = 0;
	m_indent.Clear();
	m_state = State::LineHead;
}

void OutputBuffer::IncreaseIndent()
{
	m_indentLevel++;
	m_indent.Clear();
	for (int i = 0; i < m_indentLevel; i++)
		m_indent += _T("    ");
}

void OutputBuffer::DecreaseIndent()
{
	m_indentLevel--;
	m_indent.Clear();
	for (int i = 0; i < m_indentLevel; i++)
		m_indent += _T("    ");
}

OutputBuffer& OutputBuffer::NewLine(int count)
{
	for (int i = 0; i < count; i++)
		m_buffer.Append(m_newLineCode);

	m_state = State::LineHead;
	return *this;
}

void OutputBuffer::Indent()
{
	m_buffer.Append(m_indent);
}

String OutputBuffer::ToString() const
{
	return m_buffer.ToString();
}

void OutputBuffer::AppendInternal(const StringRef& str)
{
	if (m_state == State::LineHead)
	{
		Indent();
		m_state = State::Text;
	}
	m_buffer.Append(str);
}

void OutputBuffer::AppendLineInternal(const StringRef& str)
{
	Indent();
	m_buffer.Append(str);
	NewLine();
}

void OutputBuffer::AppendLinesInternal(const StringRef& str, const StringRef& lineHeader)
{
	String ns = String(str).Replace(_T("\r"), _T(""));
	List<String> lines = ns.Split(_T("\n"));
	for (int i = 0; i < lines.GetCount(); i++)
	{
		if (lines[i].GetLength() > 0) Indent();
		m_buffer.Append(lineHeader);
		m_buffer.Append(lines[i]);

		// ÅŒã‚Ìˆê‚Â‚Í‰üs‚µ‚È‚¢
		if (i != lines.GetCount() - 1) NewLine();
	}
	m_state = State::LineHead;
}

