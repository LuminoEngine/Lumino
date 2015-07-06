
#pragma once

#include "Internal.h"
#include <Lumino/Application.h>

namespace Lumino
{

////=============================================================================
//// NativeWindowEventListener
////=============================================================================
//class Application::NativeWindowEventListener
//{
//public:
//	NativeWindowEventListener(Application)
//	{
//	}
//};

//=============================================================================
// Application
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application* Application::Create(const ApplicationConfigData& configData)
{
	RefPtr<Application> app(LN_NEW Application(configData));
	app->Initialize();
	app.SafeAddRef();
	return app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfigData& configData)
	: m_configData(configData)
	, m_endRequested(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application::~Application()
{
	m_platformManager->Dispose();
	m_guiManager->Finalize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::Initialize()
{
	InitialzePlatformManager();
	InitialzePhysicsManager();
	InitialzeGraphicsManager();
	InitialzeGUIManager();
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

		// イベントリスナー登録
		m_platformManager->GetMainWindow()->AttachEventListener(this, 0);
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

		Graphics::GraphicsManagerConfigData data;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.FileManager = &FileManager::GetInstance();
		data.PlatformTextureLoading = true;
		m_graphicsManager.Attach(LN_NEW Graphics::GraphicsManager(data));
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

		GUI::GUIManager::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		data.MainWindow = m_platformManager->GetMainWindow();
		m_guiManager.Attach(LN_NEW GUI::GUIManager());
		m_guiManager->Initialize(data);
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::ResetFrameDelay()
{
	m_fpsController.RefreshSystemDelay();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Application::OnEvent(const Platform::EventArgs& e)
{
	switch (e.Type)
	{
	case Platform::EventType_Quit:	///< アプリ終了要求
	case Platform::EventType_Close:			///< ウィンドウが閉じられようとしている
		break;

	case Platform::EventType_MouseDown:		// ウスボタンが押された
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectMouseButtonDown(e.Mouse.Button, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case Platform::EventType_MouseUp:			// マウスボタンが離された
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectMouseButtonUp(e.Mouse.Button, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case Platform::EventType_MouseMove:		// マウスが移動した
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectMouseMove(e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case Platform::EventType_MouseWheel:		// マウスホイールが操作された
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectMouseWheel(e.Mouse.Delta, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case Platform::EventType_KeyDown:	// キー押下
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectKeyDown(e.Key.KeyCode, e.Key.IsAlt, e.Key.IsShift, e.Key.IsControl)) { return true; }
		}
		break;
	case Platform::EventType_KeyUp:		//  キー押し上げ
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectKeyUp(e.Key.KeyCode, e.Key.IsAlt, e.Key.IsShift, e.Key.IsControl)) { return true; }
		}
		break;

	default:
		break;
	}
	return false;
}

} // namespace Lumino
