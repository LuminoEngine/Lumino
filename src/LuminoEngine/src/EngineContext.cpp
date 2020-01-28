
#include "Internal.hpp"
#include "Runtime/RuntimeManager.hpp"
#include "Engine/EngineManager.hpp"
#include <LuminoEngine/EngineContext.hpp>

namespace ln {

//==============================================================================
// EngineContext

EngineContext* EngineContext::current()
{
	return detail::EngineDomain::engineContext();
}

EngineContext::EngineContext()
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
		m_objectTypeInfo = registerType<Object>("Object", nullptr, {});
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
		m_engineManager = makeRef<detail::EngineManager>();
		m_engineManager->init(detail::EngineManager::s_settings);
	}
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

