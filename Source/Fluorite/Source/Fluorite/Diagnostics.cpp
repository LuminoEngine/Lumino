
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
	m_items.add(item);
}

//------------------------------------------------------------------------------
void DiagnosticsItemSet::Report(DiagnosticsCode code, flString option1)
{
	DiagnosticsItem item;
	item.m_code = code;
	item.m_lineNumber = m_currentLineNumber;
	item.m_columnNumber = m_currentColumnNumber;
	item.m_options.push_back(option1);
	m_items.add(item);
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
	auto ptr = Ref<DiagnosticsItemSet>::makeRef(absFilePath);
	m_itemSetList.add(ptr);
	return ptr;
}

} // namespace fl


