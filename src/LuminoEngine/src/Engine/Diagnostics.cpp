
#include "Internal.hpp"
#include <Lumino/Engine/Diagnostics.hpp>

namespace ln {

//==============================================================================
// DiagnosticsManager

DiagnosticsManager::DiagnosticsManager()
	: m_items()
	, m_hasError(false)
	, m_hasWarning(false)
{
}

DiagnosticsManager::~DiagnosticsManager()
{
}

void DiagnosticsManager::reportError(StringRef message)
{
	auto item = newObject<DiagnosticsItem>();
	item->setMessage(message);
	item->setLevel(DiagnosticsLevel::Error);
	m_items.add(item);
	m_hasError = true;
}

void DiagnosticsManager::reportWarning(StringRef message)
{
	auto item = newObject<DiagnosticsItem>();
	item->setMessage(message);
	item->setLevel(DiagnosticsLevel::Warning);
	m_items.add(item);
	m_hasWarning = true;
}

void DiagnosticsManager::reportInfo(StringRef message)
{
	auto item = newObject<DiagnosticsItem>();
	item->setMessage(message);
	item->setLevel(DiagnosticsLevel::Info);
	m_items.add(item);
}

//==============================================================================
// DiagnosticsItem

DiagnosticsItem::DiagnosticsItem()
	: m_string()
	, m_level(DiagnosticsLevel::Error)
{
}

DiagnosticsItem::~DiagnosticsItem()
{
}

} // namespace ln

