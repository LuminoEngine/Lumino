
#include "Internal.hpp"
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>

namespace ln {

//==============================================================================
// DiagnosticsManager

DiagnosticsManager* DiagnosticsManager::activeDiagnostics() {
    return EngineContext2::instance()->activeDiagnostics();
}

DiagnosticsManager::DiagnosticsManager()
    : m_items()
    , m_hasError(false)
    , m_hasWarning(false)
    , m_outputToStdErr(false) {
}

DiagnosticsManager::~DiagnosticsManager() {
}

Result<> DiagnosticsManager::init() {
    return Object::init();
}

void DiagnosticsManager::reportError(StringView message) {
    report(DiagnosticsLevel::Error, message);
}

void DiagnosticsManager::reportWarning(StringView message) {
    report(DiagnosticsLevel::Warning, message);
}

void DiagnosticsManager::reportInfo(StringView message) {
    report(DiagnosticsLevel::Info, message);
}

void DiagnosticsManager::report(DiagnosticsLevel level, const String& message) {
    auto item = makeObject_deprecated<DiagnosticsItem>();
    item->setMessage(message);
    item->setLevel(level);
    m_items.add(item);
    if (level >= DiagnosticsLevel::Warning) {
        m_hasWarning = true;
    }
    if (level >= DiagnosticsLevel::Error) {
        m_hasError = true;
    }
    if (m_outputToStdErr) {
        std::cerr << item->toString() << std::endl;
    }
}

ln::String DiagnosticsManager::toString() const {
    ln::String str;
    for (auto& item : m_items) {
        str += item->toString();
    }
    return str;
}

void DiagnosticsManager::dumpToLog() const {
    for (auto& item : m_items) {
        switch (item->level()) {
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

void DiagnosticsManager::registerProfilingItem(ProfilingItem* item) {
    m_profilingItems.add(item);
    auto section = Ref<ProfilingSection>(LN_NEW ProfilingSection(item), false);
    section->init();
    m_profilingValues[item] = section;
}

void DiagnosticsManager::setCounterValue(const ProfilingItem* key, int64_t value) {
    auto itr = m_profilingValues.find(key);
    if (itr != m_profilingValues.end()) {
        itr->second->setValue(value);
    }
}

void DiagnosticsManager::commitFrame() {
    for (auto& pair : m_profilingValues) {
        pair.second->commitFrame();
    }
}

//==============================================================================
// DiagnosticsItem

DiagnosticsItem::DiagnosticsItem()
    : m_level(DiagnosticsLevel::Error)
    , m_message() {
}

DiagnosticsItem::~DiagnosticsItem() {
}

void DiagnosticsItem::init() {
    Object::init();
}

ln::String DiagnosticsItem::toString() const {
    switch (m_level) {
        case DiagnosticsLevel::Error:
            return _TT("[Error] ") + m_message;
            break;
        case DiagnosticsLevel::Warning:
            return _TT("[Warning] ") + m_message;
            break;
        case DiagnosticsLevel::Info:
            return _TT("[Info] ") + m_message;
            break;
        default:
            LN_UNREACHABLE();
            return U"";
    }
}


//==============================================================================
// ProfilingItem

Ref<ProfilingItem> ProfilingItem::Graphics_RenderPassCount;

ProfilingItem::ProfilingItem()
    : m_type(ProfilingItemType::Counter)
    , m_name() {
}

void ProfilingItem::init(ProfilingItemType type, const StringView& name) {
    Object::init();
    m_type = type;
    m_name = name;
}

//==============================================================================
// ProfilingSection

ProfilingSection::ProfilingSection(ProfilingItem* owner)
    : m_owner(owner)
    , m_value(0) {
}

void ProfilingSection::commitFrame() {
    m_committedValue = m_value;
}

} // namespace ln
