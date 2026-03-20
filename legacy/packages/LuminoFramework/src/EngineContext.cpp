
#include "Internal.hpp"
#include "Engine/EngineManager.hpp"
#include <LuminoEngine/Reflection/VMProperty.hpp>
#include <LuminoFramework/EngineContext.hpp>

namespace ln {

//==============================================================================
// EngineContext

EngineContext* EngineContext::current()
{
	return detail::EngineDomain::engineContext();
}

EngineContext::EngineContext()
	: m_engineManager(nullptr)
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

void EngineContext::initializeEngineManager()
{
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

void EngineContext::disposeEngineManager()
{
	if (m_engineManager) {
		m_engineManager->dispose();
		m_engineManager = nullptr;
	}
}

} // namespace ln

