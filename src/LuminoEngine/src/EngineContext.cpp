
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
	: m_runtimeManager(makeRef<detail::RuntimeManager>())
	, m_engineManager(makeRef<detail::EngineManager>())
{
	internalInit();
}

EngineContext::~EngineContext()
{
	m_typeInfos.clear();
}

void EngineContext::internalInit()
{
	if (!m_init) {
		m_typeInfos.push_back(nullptr); // [0] is dummy
		
		PredefinedTypes::Bool = makeRef<TypeInfo>("Bool", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::Char = makeRef<TypeInfo>("Char", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::Int8 = makeRef<TypeInfo>("Int8", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::Int16 = makeRef<TypeInfo>("Int16", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::Int32 = makeRef<TypeInfo>("Int32", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::Int64 = makeRef<TypeInfo>("Int64", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::UInt8 = makeRef<TypeInfo>("UInt8", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::UInt16 = makeRef<TypeInfo>("UInt16", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::UInt32 = makeRef<TypeInfo>("UInt32", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::UInt64 = makeRef<TypeInfo>("UInt64", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::Float = makeRef<TypeInfo>("Float", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::Double = makeRef<TypeInfo>("Double", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::String = makeRef<TypeInfo>("String", nullptr, TypeInfoClass::Primitive);
		PredefinedTypes::Object = registerType<Object>("Object", nullptr, {});
		PredefinedTypes::List = registerType<Object>("List", nullptr, {});
		registerType(PredefinedTypes::Bool);
		registerType(PredefinedTypes::Char);
		registerType(PredefinedTypes::Int8);
		registerType(PredefinedTypes::Int16);
		registerType(PredefinedTypes::Int32);
		registerType(PredefinedTypes::UInt8);
		registerType(PredefinedTypes::UInt16);
		registerType(PredefinedTypes::UInt32);
		registerType(PredefinedTypes::UInt64);
		registerType(PredefinedTypes::Float);
		registerType(PredefinedTypes::Double);
		registerType(PredefinedTypes::String);

		m_objectTypeInfo = PredefinedTypes::Object;
		m_init = true;
	}
}

void EngineContext::initializeRuntimeManager()
{
	m_runtimeManager->init(detail::RuntimeManager::s_globalSettings);
}

void EngineContext::initializeEngineManager()
{
	initializeRuntimeManager();

	m_engineManager->init(detail::EngineManager::s_settings);
}

void EngineContext::disposeRuntimeManager()
{
	disposeEngineManager();

	if (m_runtimeManager) {
		m_runtimeManager->dispose();
		m_runtimeManager = nullptr;
	}
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

