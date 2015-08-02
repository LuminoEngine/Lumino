
#pragma once
#include "Internal.h"
#include <Lumino/Profiler.h>
#include <Lumino/Application.h>
#include "Graphics/ProfilerRenderer.h"

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
	, m_profilerRenderer(NULL)
{
	m_fpsController.SetEnableFpsTest(true);
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_MainThread, 60.0f);	// TODO 
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_RenderThread, 60.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application::~Application()
{
	LN_SAFE_RELEASE(m_profilerRenderer);

	if (m_platformManager != NULL) {
		m_platformManager->Dispose();
	}

	if (m_guiManager != NULL) {
		m_guiManager->Finalize();
	}
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
		data.GraphicsAPI = m_configData.GraphicsAPI;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.FileManager = &FileManager::GetInstance();
		data.PlatformTextureLoading = true;
		m_graphicsManager.Attach(LN_NEW Graphics::GraphicsManager(data));

		m_profilerRenderer = LN_NEW Graphics::ProfilerRenderer(m_graphicsManager, &Profiler::Instance);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzeDocumentsManager()
{
	if (m_documentsManager.IsNull())
	{
		InitialzeGraphicsManager();

		Documents::DocumentsManager::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		m_documentsManager.Attach(LN_NEW Documents::DocumentsManager(data));
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
		InitialzeDocumentsManager();

		GUI::GUIManager::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.DocumentsManager = m_documentsManager;
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

	Profiler::Instance.SetMainFPS(m_fpsController.GetFps());
	Profiler::Instance.SetMainFPSCapacity(m_fpsController.GetCapacityFps());

	return !m_endRequested;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::Render()
{
	if (m_profilerRenderer != NULL) {
		m_profilerRenderer->Render(Vector2(640, 480));	//TODO
	}
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
