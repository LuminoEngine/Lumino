
#include "Internal.h"
#include "DiagnosticsManager.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
//=============================================================================
// DiagnosticsManager
//=============================================================================

//=============================================================================
// DiagnosticsItemSet
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DiagnosticsItemSet::Report(DiagnosticsCode code)
{
	DiagnosticsItem item;
	item.m_code = code;
	item.m_lineNumber = m_currentLineNumber;
	item.m_columnNumber = m_currentColumnNumber;
	m_items.Add(item);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DiagnosticsItemSet::Report(DiagnosticsCode code, StringA option1)
{
	DiagnosticsItem item;
	item.m_code = code;
	item.m_lineNumber = m_currentLineNumber;
	item.m_columnNumber = m_currentColumnNumber;
	item.m_options.Add(option1);
	m_items.Add(item);
}

} // namespace Parser
LN_NAMESPACE_END

