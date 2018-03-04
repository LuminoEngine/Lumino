
#include "../Internal.h"
#include <Lumino/Engine.h>
#include <Lumino/Framework/Application.h>
#include <Lumino/Framework/GameScene.h>
#include <Lumino/UI/UIFrameWindow.h>
#include "GameSceneManager.h"
#include "../EngineManager.h"
#include "../UI/UIManager.h"

LN_NAMESPACE_BEGIN

////==============================================================================
//// Application
////==============================================================================
//
////------------------------------------------------------------------------------
//UIMainWindow* Application::getMainWindow()
//{
//	return EngineManager::Instance->getUIManager()->getMainWindow();
//}
//
////------------------------------------------------------------------------------
//Application::Application()
//	: m_engineManager(nullptr)
//{
//}
//
////------------------------------------------------------------------------------
//Application::~Application()
//{
//}
//
////------------------------------------------------------------------------------
//void Application::initialize(EngineManager* engineManager)
//{
//	if (LN_REQUIRE(engineManager != nullptr)) return;
//	m_engineManager = engineManager;
//}
//
////------------------------------------------------------------------------------
//PlatformWindow* Application::getNativeMainWindow()
//{
//	return m_engineManager->getPlatformManager()->getMainWindow();
//}

//==============================================================================
// Application
//==============================================================================

Application::Application()
{
	//detail::EngineDomain::getEngineManager()->setApplication(this);
}

Application::~Application()
{
}

Ref<UIMainWindow> Application::onCreateMainWindow()
{
	return nullptr;
}

//==============================================================================
// GameApplication
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GameApplication, Object);

//------------------------------------------------------------------------------
GameApplication::GameApplication()
{
	m_gameSceneManager = Ref<detail::GameSceneManager>::makeRef();
}

//------------------------------------------------------------------------------
GameApplication::~GameApplication()
{
	m_gameSceneManager->dispose();
}

//------------------------------------------------------------------------------
void GameApplication::initialize()
{

}

//------------------------------------------------------------------------------
void GameApplication::OnConfigure()
{
}

//------------------------------------------------------------------------------
void GameApplication::run(GameScene* initialScene)
{
	//OnConfigure();

	//Engine::initialize();

	if (initialScene == nullptr)
		m_gameSceneManager->gotoScene(Ref<GameScene>::makeRef());
	else
		m_gameSceneManager->gotoScene(initialScene);

	do
	{
		m_gameSceneManager->updateFrame();

	} while (Engine::update());

	//Engine::terminate();
}

//==============================================================================
// ToolApplication
//==============================================================================
ToolApplication::ToolApplication()
{
}

ToolApplication::~ToolApplication()
{
}

void ToolApplication::run()
{
}

LN_NAMESPACE_END
