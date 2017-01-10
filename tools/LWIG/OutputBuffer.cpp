
#include "OutputBuffer.h"

OutputBuffer::OutputBuffer()
	: m_buffer()
	, m_indentLevel(0)
	, m_indent()
	, m_newLineCode(_T("\r\n"))
{
}

OutputBuffer::OutputBuffer(int indent)
	: OutputBuffer()
{
	for (int i = 0 ; i < count; i++) IncreaseIndent();
}

void OutputBuffer::Clear()
{
	m_sb.Clear();
	m_indentLevel = 0;
	m_indent.Clear();
}

void OutputBuffer::IncreaseIndent()
{
	m_indentLevel++;
	m_indent.Clear();
	for (int i = 0; i < m_indentLevel; i++)
		m_indent += "    ";
}

void OutputBuffer::DecreaseIndent()
{
	m_indentLevel--;
	m_indent.Clear();
	for (int i = 0; i < m_indentLevel; i++)
		m_indent += "    ";
}

void OutputBuffer::NewLine(int count)
{
	for (int i = 0; i < count; i++)
		m_buffer.Append(m_newLineCode);
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
	m_buffer.Append(str);
}

void OutputBuffer::AppendLineInternal(const StringRef& str)
{
	Indent();
	m_buffer.Append(str);
	NewLine();
}

void OutputBuffer::AppendLinesInternal(const StringRef& str)
{
	String ns = str.Replace(_T("\r"), _T(""));
	List<String> lines = ns.Split(_T("\n"));
	for (int i = 0; i < lines.GetCount(); i++)
	{
		if (lines[i].Length > 0) Indent();
		AppendInternal(lines[i]);

		// ÅŒã‚Ìˆê‚Â‚Í‰üs‚µ‚È‚¢
		if (i != lines.GetCount() - 1) NewLine();
	}
}

