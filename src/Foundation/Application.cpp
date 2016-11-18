
#include "../Internal.h"
#include <Lumino/Engine.h>
#include <Lumino/Foundation/Application.h>
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
	LN_CHECK_ARG(engineManager != nullptr);
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

//------------------------------------------------------------------------------
GameApplication::GameApplication()
{
}

//------------------------------------------------------------------------------
GameApplication::~GameApplication()
{
}

//------------------------------------------------------------------------------
void GameApplication::OnConfigure()
{
}

//------------------------------------------------------------------------------
void GameApplication::Run()
{
	try
	{
		EngineSettings::SetGraphicsAPI(GraphicsAPI::DirectX9);
		EngineSettings::SetGraphicsRenderingType(GraphicsRenderingType::Immediate);

		OnConfigure();

		Engine::Initialize();

		do
		{

		} while (Engine::Update());
	}
	catch (...)
	{
		Engine::Terminate();
		throw;
	}
	Engine::Terminate();
}

LN_NAMESPACE_END
