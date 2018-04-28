﻿
#include "Internal.hpp"

#include <Lumino/UI/UIFrameWindow.hpp>
#include "../Platform/PlatformManager.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../UI/UIManager.hpp"
#include "EngineManager.hpp"
#include "EngineDomain.hpp"

namespace ln {
namespace detail {

//==============================================================================
// EngineSettings

//void EngineSettings::setMainWindowSize(int width, int height)
//{
//	detail::EngineSettings::instance.mainWindowSize.set(width, height);
//}
//
//void EngineSettings::setMainBackBufferSize(int width, int height)
//{
//	detail::EngineSettings::instance.mainBackBufferSize.set(width, height);
//}
//
//void EngineSettings::setMainWindowTitle(const StringRef& title)
//{
//	detail::EngineSettings::instance.mainWindowTitle = title;
//}

//==============================================================================
// EngineManager

EngineManager::EngineManager()
	: m_settings()
	, m_platformManager(nullptr)
	//, m_animationManager(nullptr)
	//, m_inputManager(nullptr)
	//, m_audioManager(nullptr)
	//, m_physicsManager(nullptr)
	//, m_graphicsManager(nullptr)
	//, m_effectManager(nullptr)
	//, m_modelManager(nullptr)
	//, m_assetsManager(nullptr)
	//, m_uiManager(nullptr)
	, m_exitRequested(false)
{
}

void EngineManager::initialize()
{
	initializeAllManagers();

	m_mainWindow = newObject<UIFrameWindow>(m_settings.mainWindowSize);
}

void EngineManager::dispose()
{
	if (m_mainWindow) m_mainWindow->dispose();
	if (m_platformManager) m_platformManager->dispose();
}

void EngineManager::initializeAllManagers()
{
	initializeCommon();
	initializePlatformManager();
	initializeAnimationManager();
	initializeInputManager();
	initializeAudioManager();
	initializePhysicsManager();
	initializeGraphicsManager();
	initializeEffectManager();
	initializeModelManager();
	initializeAssetsManager();
	initializeUIManager();
}

void EngineManager::initializeCommon()
{
}

void EngineManager::initializePlatformManager()
{
	if (!m_platformManager)
	{
		initializeCommon();

		PlatformManager::Settings settings;

		m_platformManager = Ref<PlatformManager>::makeRef();
		m_platformManager->initialize(settings);
	}
}

void EngineManager::initializeAnimationManager()
{
}

void EngineManager::initializeInputManager()
{
}

void EngineManager::initializeAudioManager()
{
}

void EngineManager::initializePhysicsManager()
{
}

void EngineManager::initializeGraphicsManager()
{
	if (!m_graphicsManager)
	{
		initializeCommon();

		GraphicsManager::Settings settings;

		m_graphicsManager = Ref<GraphicsManager>::makeRef();
		m_graphicsManager->initialize(settings);
	}
}

void EngineManager::initializeEffectManager()
{
}

void EngineManager::initializeModelManager()
{
}

void EngineManager::initializeAssetsManager()
{
}

void EngineManager::initializeUIManager()
{
	if (!m_uiManager)
	{
		initializeGraphicsManager();

		UIManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;
		
		m_uiManager = makeRef<UIManager>();
		m_uiManager->initialize(settings);
	}
}

bool EngineManager::updateUnitily()
{
	updateFrame();
	renderFrame();
	presentFrame();
	return !isExitRequested();
}

void EngineManager::updateFrame()
{
	if (m_platformManager) {
		m_platformManager->windowManager()->processSystemEventQueue();
	}
}

void EngineManager::renderFrame()
{
	if (m_mainWindow) {
		m_mainWindow->renderContents();
	}
}

void EngineManager::presentFrame()
{
	if (m_mainWindow) {
		m_mainWindow->present();
	}

	::Sleep(1);
}

void EngineManager::resetFrameDelay()
{
}

void EngineManager::exit()
{
	m_exitRequested = true;
}

bool EngineManager::onPlatformEvent(const PlatformEventArgs& e)
{
	switch (e.type)
	{
	case PlatformEventType::close:
		exit();
		break;
	default:
		break;
	}
	return false;
}

//==============================================================================
// EngineDomain

static EngineManager* g_engineManager = nullptr;

void EngineDomain::release()
{
	if (g_engineManager)
	{
		g_engineManager->dispose();
		g_engineManager->release();
		g_engineManager = nullptr;
	}
}

EngineManager* EngineDomain::engineManager()
{
	if (!g_engineManager)
	{
		g_engineManager = LN_NEW EngineManager();
	}

	return g_engineManager;
}

PlatformManager* EngineDomain::platformManager()
{
	return engineManager()->platformManager();
}

GraphicsManager* EngineDomain::graphicsManager()
{
	return engineManager()->graphicsManager();
}


UIManager* EngineDomain::uiManager()
{
	return engineManager()->uiManager();
}

} // namespace detail
} // namespace ln

