
#pragma once

#include "Internal.h"
#include <Lumino/Application.h>

namespace Lumino
{

//=============================================================================
// Application
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application* Application::Create()
{
	RefPtr<Application> app(LN_NEW Application());
	app->Initialize();
	app.SafeAddRef();
	return app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application::Application()
	: m_endRequested(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application::~Application()
{
	m_platformManager->Dispose();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::Initialize()
{
	InitialzePlatformManager();
	InitialzePhysicsManager();
	InitialzeGraphicsManager();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzePlatformManager()
{
	if (m_platformManager.IsNull())
	{
		Platform::ApplicationSettings data;
		data.API = Platform::WindowSystemAPI_Win32API;
		data.MainWindowSettings.Title = _T("");
		data.MainWindowSettings.ClientSize.Set(640, 480);
		data.MainWindowSettings.Fullscreen = false;
		data.MainWindowSettings.Resizable = true;
		data.UseInternalUIThread = false;

		m_platformManager.Attach(LN_NEW Platform::PlatformManager());
		m_platformManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzePhysicsManager()
{
	if (m_physicsManager.IsNull())
	{
		m_physicsManager.Attach(LN_NEW Physics::PhysicsManager(Physics::SimulationType_Sync));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzeGraphicsManager()
{
	if (m_graphicsManager.IsNull())
	{
		InitialzePlatformManager();
		InitialzePhysicsManager();

		Graphics::GraphicsManagerConfigData gmcd;
		gmcd.MainWindow = m_platformManager->GetMainWindow();
		gmcd.FileManager = &FileManager::GetInstance();
		m_graphicsManager.Attach(LN_NEW Graphics::GraphicsManager(gmcd));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzeGUIManager()
{
	if (m_guiManager.IsNull())
	{
		InitialzePlatformManager();
		InitialzeGraphicsManager();

		m_guiManager.Attach(LN_NEW GUI::GUIManager());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Application::UpdateFrame()
{
	m_endRequested = !m_platformManager->DoEvents();

	m_fpsController.Process();

	return !m_endRequested;
}

} // namespace Lumino
