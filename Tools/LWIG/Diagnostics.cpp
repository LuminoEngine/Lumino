
#include "Diagnostics.h"

void DiagManager::add(DiagItem::Level level, const String& message, const String& location)
{
	m_items.push_back(DiagItem{ level, message, location });
	Console::writeLine(message);
}

void DiagManager::dump()
{
	for (auto& item : m_items)
	{
		// TODO: to stderr
		Console::writeLine(String::format(_T("{0}: {2}"), item.location, item.level, item.message));
	}
}
