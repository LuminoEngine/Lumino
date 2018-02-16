
#include "Diagnostics.h"

void DiagManager::add(DiagItem::Level level, const String& message)
{
	m_items.push_back(DiagItem{ level, message });
	Console::writeLine(message);
}
