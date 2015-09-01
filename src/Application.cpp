/*


[2015/8/30] プロパティの継承
	リビジョンカウントを用いた方法
	・親～孫間の要素のプロパティが変更された時は？
		そのプロパティの独自設定ON/OFFが切り替わった時、全ての子の同じプロパティに再更新フラグを立てる。
		→全子走査は時間かかる・・・親のリビジョンを操作するのは？
			→ルートまでデフォルトだった時対応できない
				→ルートを継承元として参照する。
	・ツリーからツリーが切り離された/追加された時は？
		切り離された/追加されたツリーの全ノードのプロパティの再更新フラグをONにする。


[2015/7/31] トップレベルインターフェイス
	- 継承による拡張
	- 言語バインダを実装する上での使いやすさ
	- Variant への保持しやすさ
	- アプリを実装する上での使いやすさ

	以下のような方針で。
	- トップレベルオブジェクト (Application) はグローバル。
	  (完全にグローバルではなく、インスタンスのポインタをグローバル変数に入れておくイメージ。必要に応じて継承し、拡張できる)
	- 



	・Font font = Font::CreateBitmapFont();
	

	・FontPtr font = Font::CreateBitmapFont();

	- スタックへの生成を許可するか？
		完全に禁止することは出来ない。派生させればなんとでも出来てしまう。
	

	・Siv3D、セガGameLib
		公開するのはスマートポインタクラス。実体は可能な限り見せない。
		→ 派生させて拡張できない。GUI のユーザーコントロールとか作れないことになる。
		ただ、Siv3D の GUI は static Create() が shared_ptr 返していた。

	・SDL2、GLFW、GDI+、Nux なんかはトップレベルオブジェクトはグローバルインスタンス。
	  OpenSceneGraph もたぶんそう。
*/


#pragma once
#include "Internal.h"
#include <Lumino/Profiler.h>
#include <Lumino/Application.h>
#include <Lumino/Audio/AudioManager.h>
#include <Lumino/Graphics/Renderer.h>
#include "Graphics/ProfilerRenderer.h"
#include "ApplicationContext.h"

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
Application* Application::Create(const Application::ConfigData& configData)
{
	RefPtr<Application> app(LN_NEW Application(configData));
	app->Initialize();
	app.SafeAddRef();
	return app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application::Application(const Application::ConfigData& configData)
	: m_configData(configData)
	, m_audioManager(NULL)
	, m_profilerRenderer(NULL)
	, m_endRequested(false)
{
	m_fpsController.SetEnableFpsTest(true);
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_MainThread, 60.0f);	// TODO 
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_RenderThread, 60.0f);

	if (ApplicationContext::GetCurrent() == NULL) {
		ApplicationContext::SetCurrent(this);
	}
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

	if (m_audioManager != NULL) {
		m_audioManager->Finalize();
		LN_SAFE_RELEASE(m_audioManager);
	}

	if (ApplicationContext::GetCurrent() == this) {
		ApplicationContext::SetCurrent(NULL);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::Initialize()
{
	InitialzePlatformManager();
	InitialzeAudioManager();
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
		Platform::PlatformManager::Settings data;
		data.API = Platform::WindowSystemAPI_Win32API;
		data.MainWindowSettings.Title = _T("");
		data.MainWindowSettings.ClientSize.Set(640, 480);
		data.MainWindowSettings.Fullscreen = false;
		data.MainWindowSettings.Resizable = true;
		data.MainWindowSettings.UserWindow = m_configData.UserMainWindow;
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
void Application::InitialzeAudioManager()
{
	if (m_audioManager == NULL)
	{
		// ユーザー定義のウィンドウハンドルが指定されている場合、
		// ダミーウィンドウクラスを作るために PlatformManager の初期化が必要。
		if (m_configData.UserMainWindow != NULL) {
			InitialzePlatformManager();
		}

		Audio::AudioManager::Settings data;
		data.FileManager = &FileManager::GetInstance();
		data.StreamCacheObjectCount = 32;
		data.StreamSourceCacheMemorySize = 0;
		data.DMInitMode = Audio::DirectMusicInitMode_NotUse;//Audio::DirectMusicInitMode_ThreadRequest;
#ifdef LN_WIN32
		data.hWnd = (m_platformManager != NULL) ? Platform::PlatformSupport::GetWindowHandle(m_platformManager->GetMainWindow()) : NULL;
#endif
		m_audioManager = Audio::AudioManager::Create(data);
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

		Graphics::GraphicsManager::ConfigData data;
		data.GraphicsAPI = m_configData.GraphicsAPI;
		data.RenderingType = m_configData.RenderingType;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.FileManager = &FileManager::GetInstance();
		data.PlatformTextureLoading = true;
#ifdef LN_WIN32
		data.D3D9Device = m_configData.D3D9Device;
#endif
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
	if (m_graphicsManager != NULL)
	{
		m_graphicsManager->GetRenderer()->Begin();
		//m_graphicsManager->GetRenderer()->Clear(Graphics::ClearFlags::All, Graphics::ColorF::White);

		//Graphics::DepthStencilState state;
		//state.DepthEnable = false;
		//state.DepthWriteEnable = false;
		//state.StencilEnable = false;
		//m_graphicsManager->GetRenderer()->SetDepthStencilState(state);

		if (m_guiManager != NULL) {
			m_guiManager->Render();
		}

		if (m_profilerRenderer != NULL) {
			m_profilerRenderer->Render(Vector2(640, 480));	//TODO
		}

		m_graphicsManager->GetRenderer()->End();
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
