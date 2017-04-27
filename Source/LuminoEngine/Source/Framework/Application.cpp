
#include "../Internal.h"
#include <Lumino/Engine.h>
#include <Lumino/Framework/Application.h>
#include <Lumino/Framework/GameScene.h>
#include "GameSceneManager.h"
#include "../EngineManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Application
//==============================================================================

//------------------------------------------------------------------------------
Application::Application()
	: m_engineManager(nullptr)
{
}

//------------------------------------------------------------------------------
Application::~Application()
{
}

//------------------------------------------------------------------------------
void Application::Initialize(EngineManager* engineManager)
{
	if (LN_CHECK_ARG(engineManager != nullptr)) return;
	m_engineManager = engineManager;
}

//------------------------------------------------------------------------------
PlatformWindow* Application::GetNativeMainWindow()
{
	return m_engineManager->GetPlatformManager()->GetMainWindow();
}

//==============================================================================
// GameApplication
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GameApplication, Object);

//------------------------------------------------------------------------------
GameApplication::GameApplication()
{
	m_gameSceneManager = RefPtr<detail::GameSceneManager>::MakeRef();
}

//------------------------------------------------------------------------------
GameApplication::~GameApplication()
{
	m_gameSceneManager->Finalize();
}

//------------------------------------------------------------------------------
void GameApplication::Initialize()
{

}

//------------------------------------------------------------------------------
void GameApplication::OnConfigure()
{
}

//------------------------------------------------------------------------------
void GameApplication::Run(GameScene* initialScene)
{
	//OnConfigure();

	//Engine::Initialize();

	if (initialScene == nullptr)
		m_gameSceneManager->GotoScene(RefPtr<GameScene>::MakeRef());
	else
		m_gameSceneManager->GotoScene(initialScene);

	do
	{
		m_gameSceneManager->UpdateFrame();

	} while (Engine::Update());

	//Engine::Terminate();
}

LN_NAMESPACE_END
