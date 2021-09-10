
#include "Internal.hpp"
#include "Runtime/RuntimeManager.hpp"
#include "Engine/EngineManager.hpp"
#include <LuminoEngine/Reflection/VMProperty.hpp>
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
}

void EngineContext::disposeEngineManager()
{
	if (m_engineManager) {
		m_engineManager->dispose();
		m_engineManager = nullptr;
	}
}

} // namespace ln

