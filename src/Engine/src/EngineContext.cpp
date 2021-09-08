
#include "Internal.hpp"
#include "Runtime/RuntimeManager.hpp"
#include "Engine/EngineManager.hpp"
#include <LuminoEngine/Engine/VMProperty.hpp>
#include <LuminoEngine/EngineContext.hpp>

namespace ln {

//==============================================================================
// EngineContext

EngineContext* EngineContext::current()
{
	return detail::EngineDomain::engineContext();
}

EngineContext::EngineContext()
	: m_runtimeManager(nullptr)
	, m_engineManager(nullptr)
{
	internalInit();
}

EngineContext::~EngineContext()
{
}

void EngineContext::internalInit()
{
	if (!m_init) {
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

		m_objectTypeInfo = PredefinedTypes::Object;
		m_init = true;
	}
}

void EngineContext::initializeRuntimeManager()
{
	if (!m_runtimeManager) {
		m_runtimeManager = makeRef<detail::RuntimeManager>();
		m_runtimeManager->init(detail::RuntimeManager::s_globalSettings);
	}
}

void EngineContext::initializeEngineManager()
{
	initializeRuntimeManager();

	if (!m_engineManager) {
		if (engineManagerPreInit) {
			engineManagerPreInit();
		}

		m_engineManager = makeRef<detail::EngineManager>();
		m_engineManager->init(detail::EngineManager::s_settings);

		if (engineManagerPostInit) {
			engineManagerPostInit();
		}
	}
}

void EngineContext::disposeRuntimeManager()
{
	disposeEngineManager();

	if (m_runtimeManager) {
		m_runtimeManager->dispose();
		m_runtimeManager = nullptr;
	}

	m_typeInfos.clear();
	m_typeInfoSet.clear();
}

void EngineContext::disposeEngineManager()
{
	if (m_engineManager) {
		m_engineManager->dispose();
		m_engineManager = nullptr;
	}
}

TypeInfo* EngineContext::acquireTypeInfo(const StringRef& name/*, TypeInfo* baseType, const std::function<Ref<Object>()>& factory*/)
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

