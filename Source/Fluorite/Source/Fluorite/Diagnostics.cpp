
#include "Internal.h"
#include <Fluorite/Diagnostics.h>

namespace fl {

//==============================================================================
// DiagnosticsItemSet
//==============================================================================

//------------------------------------------------------------------------------
DiagnosticsItemSet::DiagnosticsItemSet(const PathNameA& absFilePath)
	: m_absFilePath(absFilePath)
{
}

//------------------------------------------------------------------------------
void DiagnosticsItemSet::Report(DiagnosticsCode code)
{
	DiagnosticsItem item;
	item.m_code = code;
	item.m_lineNumber = m_currentLineNumber;
	item.m_columnNumber = m_currentColumnNumber;
	m_items.Add(item);
}

//------------------------------------------------------------------------------
void DiagnosticsItemSet::Report(DiagnosticsCode code, flString option1)
{
	DiagnosticsItem item;
	item.m_code = code;
	item.m_lineNumber = m_currentLineNumber;
	item.m_columnNumber = m_currentColumnNumber;
	item.m_options.Add(option1);
	m_items.Add(item);
}

//==============================================================================
// DiagnosticsManager
//==============================================================================
//------------------------------------------------------------------------------
void DiagnosticsManager::clear()
{
	m_itemSetList.clear();
}

//------------------------------------------------------------------------------
DiagnosticsItemSet* DiagnosticsManager::CreateItemSet(const PathNameA& absFilePath)
{
	auto ptr = RefPtr<DiagnosticsItemSet>::MakeRef(absFilePath);
	m_itemSetList.Add(ptr);
	return ptr;
}

} // namespace fl


