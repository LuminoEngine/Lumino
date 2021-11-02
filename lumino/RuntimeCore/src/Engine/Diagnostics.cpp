
#include "Internal.hpp"
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>

namespace ln {

//==============================================================================
// DiagnosticsManager

DiagnosticsManager* DiagnosticsManager::activeDiagnostics()
{
	return EngineContext2::instance()->activeDiagnostics();
}

DiagnosticsManager::DiagnosticsManager()
	: m_items()
	, m_hasError(false)
	, m_hasWarning(false)
{
}

DiagnosticsManager::~DiagnosticsManager()
{
}

void DiagnosticsManager::init()
{
	Object::init();
}

void DiagnosticsManager::reportError(StringRef message)
{
	auto item = makeObject<DiagnosticsItem>();
	item->setMessage(message);
	item->setLevel(DiagnosticsLevel::Error);
	m_items.add(item);
	m_hasError = true;
}

void DiagnosticsManager::reportWarning(StringRef message)
{
	auto item = makeObject<DiagnosticsItem>();
	item->setMessage(message);
	item->setLevel(DiagnosticsLevel::Warning);
	m_items.add(item);
	m_hasWarning = true;
}

void DiagnosticsManager::reportInfo(StringRef message)
{
	auto item = makeObject<DiagnosticsItem>();
	item->setMessage(message);
	item->setLevel(DiagnosticsLevel::Info);
	m_items.add(item);
}

ln::String DiagnosticsManager::toString() const
{
	ln::String str;
	for (auto& item : m_items)
	{
		switch (item->level())
		{
		case DiagnosticsLevel::Error:
			str += _TT("[Error] ") + item->message();
			break;
		case DiagnosticsLevel::Warning:
			str += _TT("[Warning] ") + item->message();
			break;
		case DiagnosticsLevel::Info:
			str += _TT("[Info] ") + item->message();
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
	}
	return str;
}

void DiagnosticsManager::dumpToLog() const
{
	for (auto& item : m_items)
	{
		switch (item->level())
		{
		case DiagnosticsLevel::Error:
			LN_LOG_ERROR(item->message());
			break;
		case DiagnosticsLevel::Warning:
			LN_LOG_WARNING(item->message());
			break;
		case DiagnosticsLevel::Info:
			LN_LOG_INFO(item->message());
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
	}
}

void DiagnosticsManager::registerProfilingItem(ProfilingItem* item)
{
	m_profilingItems.add(item);
	auto section = Ref<ProfilingSection>(LN_NEW ProfilingSection(item), false);
	section->init();
	m_profilingValues[item] = section;
}

void DiagnosticsManager::setCounterValue(const ProfilingItem* key, int64_t value)
{
	auto itr = m_profilingValues.find(key);
	if (itr != m_profilingValues.end()) {
		itr->second->setValue(value);
	}
}

void DiagnosticsManager::commitFrame()
{
	for (auto& pair : m_profilingValues) {
		pair.second->commitFrame();
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

void DiagnosticsItem::init()
{
	Object::init();
}

//==============================================================================
// ProfilingItem

Ref<ProfilingItem> ProfilingItem::Graphics_RenderPassCount;

ProfilingItem::ProfilingItem()
	: m_type(ProfilingItemType::Counter)
	, m_name()
{
}

void ProfilingItem::init(ProfilingItemType type, const StringRef& name)
{
	Object::init();
	m_type = type;
	m_name = name;
}

//==============================================================================
// ProfilingSection

ProfilingSection::ProfilingSection(ProfilingItem* owner)
	: m_owner(owner)
	, m_value(0)
{
}

void ProfilingSection::commitFrame()
{
	m_committedValue = m_value;
}

} // namespace ln

