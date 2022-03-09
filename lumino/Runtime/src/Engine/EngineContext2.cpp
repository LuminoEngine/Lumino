
#include "Internal.hpp"
#include <LuminoEngine/Base/Task.hpp>
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>

namespace ln {

std::unique_ptr<EngineContext2> EngineContext2::s_instance;

bool EngineContext2::initialize(EngineContext2* sharedContext)
{
    if (sharedContext) {
        LN_NOTIMPLEMENTED();
        return false;
    }

    if (s_instance) return true;
    s_instance = std::unique_ptr<EngineContext2>(LN_NEW EngineContext2());
    return s_instance->init();
}

void EngineContext2::terminate()
{
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

EngineContext2::EngineContext2()
{
}

bool EngineContext2::init()
{
    m_typeInfos.push_back(nullptr); // [0] is dummy

    PredefinedTypes::Char = registerType(_TT("Char"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Int8 = registerType(_TT("Int8"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Int16 = registerType(_TT("Int16"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Int32 = registerType(_TT("Int32"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Int64 = registerType(_TT("Int64"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::UInt8 = registerType(_TT("UInt8"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::UInt16 = registerType(_TT("UInt16"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::UInt32 = registerType(_TT("UInt32"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::UInt64 = registerType(_TT("UInt64"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Float = registerType(_TT("Float"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Double = registerType(_TT("Double"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::String = registerType(_TT("String"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Object = registerType<Object>("Object", nullptr, {});
    PredefinedTypes::List = registerType<Object>("List", nullptr, {});

    // TODO: test
    registerType<ZVTestClass1>();

    m_objectTypeInfo = PredefinedTypes::Object;

    TaskScheduler::init();
    m_mainThreadTaskDispatcher = makeRef<Dispatcher>();

    {
        m_activeDiagnostics = makeObject<DiagnosticsManager>();
        ProfilingItem::Graphics_RenderPassCount = makeObject<ProfilingItem>(ProfilingItemType::Counter, _TT("RenderPass count"));
        m_activeDiagnostics->registerProfilingItem(ProfilingItem::Graphics_RenderPassCount);
    }

    return true;
}

void EngineContext2::dispose()
{
    if (m_mainThreadTaskDispatcher) {
        m_mainThreadTaskDispatcher->dispose();
        m_mainThreadTaskDispatcher = nullptr;
    }
    TaskScheduler::finalizeInternal();

    m_typeInfos.clear();
    m_typeInfoSet.clear();
}

void EngineContext2::registerModule(Module* mod)
{
    if (LN_REQUIRE(!mod->m_context)) return;
    mod->m_context = this;
    m_modules.add(mod);
}

void EngineContext2::unregisterModule(Module* mod)
{
    m_modules.removeIf([&](const auto& x) { return x == mod; });
}

TypeInfo* EngineContext2::acquireTypeInfo(const StringView& name/*, TypeInfo* baseType, const std::function<Ref<Object>()>& factory*/)
{
    auto* r = findTypeInfo(name);
    if (r) {
        return r;
    }
    else {
        auto typeInfo = makeRef<TypeInfo>(name);
        //typeInfo->m_factory = factory;
        typeInfo->m_id = m_typeInfos.size();
        m_typeInfos.push_back(typeInfo);
        m_typeInfoSet.insert({ typeInfo->name(), typeInfo });
        return typeInfo;
    }
}

} // namespace ln

