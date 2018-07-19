
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

void DiagnosticsManager::initialize()
{
	Object::initialize();
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

void DiagnosticsManager::dumpToLog() const
{
	for (auto& item : m_items)
	{
		switch (item->level())
		{
		case DiagnosticsLevel::Error:
			LN_LOG_ERROR << item->message();
			break;
		case DiagnosticsLevel::Warning:
			LN_LOG_WARNING << item->message();
			break;
		case DiagnosticsLevel::Info:
			LN_LOG_INFO << item->message();
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
	}
}

//==============================================================================
// DiagnosticsItem

DiagnosticsItem::DiagnosticsItem()
	: m_level(DiagnosticsLevel::Error)
	, m_string()
{
}

DiagnosticsItem::~DiagnosticsItem()
{
}

void DiagnosticsItem::initialize()
{
	Object::initialize();
}

} // namespace ln

