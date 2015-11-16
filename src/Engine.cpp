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
	- トップレベルオブジェクト (ApplicationImpl) はグローバル。
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
#include <Lumino/IO/Console.h>
#include <Lumino/Profiler.h>
#include <Lumino/Audio/AudioManager.h>
#include <Lumino/Engine.h>
#include "Graphics/RendererImpl.h"
#include "Graphics/ProfilerRenderer.h"
#include "Scene/SceneGraphManager.h"
#include "GUI/UIManagerImpl.h"
#include "ApplicationImpl.h"

LN_NAMESPACE_BEGIN

////=============================================================================
//// NativeWindowEventListener
////=============================================================================
//class ApplicationImpl::NativeWindowEventListener
//{
//public:
//	NativeWindowEventListener(ApplicationImpl)
//	{
//	}
//};

//=============================================================================
// ApplicationImpl
//=============================================================================

ApplicationImpl* ApplicationImpl::Instance = NULL;
const TCHAR* ApplicationImpl::LogFileName = _T("lnlog.txt");

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ApplicationImpl* ApplicationImpl::Create(const ApplicationSettings& configData)
{
	RefPtr<ApplicationImpl> app(LN_NEW ApplicationImpl(configData), false);
	//app->Initialize();
	app.SafeAddRef();
	return app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ApplicationImpl::ApplicationImpl(const ApplicationSettings& configData)
	: m_configData(configData)
	, m_fileManager(NULL)
	, m_audioManager(NULL)
	, m_guiManager(NULL)
	, m_sceneGraphManager(NULL)
	, m_profilerRenderer(NULL)
	, m_commonInitied(false)
	, m_endRequested(false)
{
	m_fpsController.SetEnableFpsTest(true);
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_MainThread, 60.0f);	// TODO 
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_RenderThread, 60.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ApplicationImpl::~ApplicationImpl()
{
	LN_SAFE_RELEASE(m_profilerRenderer);

	if (m_platformManager != NULL) {
		m_platformManager->Dispose();
	}

	LN_SAFE_RELEASE(m_sceneGraphManager);

	if (m_guiManager != NULL) {
		m_guiManager->Finalize();
		LN_SAFE_RELEASE(m_guiManager);
	}

	if (m_audioManager != NULL) {
		m_audioManager->Finalize();
		LN_SAFE_RELEASE(m_audioManager);
	}

	LN_SAFE_RELEASE(m_fileManager);

	Console::Free();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::Initialize()
{
	InitialzePlatformManager();
	InitialzeAudioManager();
	InitialzePhysicsManager();
	InitialzeGraphicsManager();
	InitialzeGUIManager();
#ifdef LN_BUILD_SCENE_MODULE
	InitialzeSceneGraphManager();
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::InitializeCommon()
{
	if (!m_commonInitied)
	{
		// ログファイル出力
		if (m_configData.ApplicationLogEnabled) {
			Logger::Initialize(LogFileName);
		}
		// コンソール割り当て
		if (m_configData.ConsoleEnabled) {
			Console::Alloc();
		}

		m_commonInitied = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::InitialzeFileManager()
{
	if (m_fileManager == NULL)
	{
		FileManager::Settings data;
		data.AccessMode = m_configData.FileAccessPriority;
		m_fileManager = LN_NEW FileManager(data);
		for (auto& e : m_configData.ArchiveFileEntryList) {
			m_fileManager->RegisterArchive(e.FilePath, e.Password);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::InitialzePlatformManager()
{
	if (m_platformManager.IsNull())
	{
		InitializeCommon();

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
void ApplicationImpl::InitialzeAudioManager()
{
	if (m_audioManager == NULL)
	{
		InitializeCommon();
		InitialzeFileManager();

		// ユーザー定義のウィンドウハンドルが指定されている場合、
		// ダミーウィンドウクラスを作るために PlatformManager の初期化が必要。
		if (m_configData.UserMainWindow != NULL) {
			InitialzePlatformManager();
		}

		AudioManagerImpl::Settings data;
		data.FileManager = m_fileManager;
		data.StreamCacheObjectCount = m_configData.SoundCacheCapacity.ObjectCount;
		data.StreamSourceCacheMemorySize = m_configData.SoundCacheCapacity.MemorySize;
		data.DMInitMode = m_configData.DirectMusicMode;
#ifdef LN_OS_WIN32
		data.hWnd = (m_platformManager != NULL) ? Platform::PlatformSupport::GetWindowHandle(m_platformManager->GetMainWindow()) : NULL;
#endif
		data.DirectMusicReverbLevel = m_configData.DirectMusicReverbLevel;
		m_audioManager = AudioManagerImpl::Create(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::InitialzePhysicsManager()
{
#ifdef LN_BUILD_SCENE_MODULE
	if (m_physicsManager.IsNull())
	{
		InitializeCommon();
		m_physicsManager.Attach(LN_NEW Physics::PhysicsManager(Physics::SimulationType_Sync));
	}
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::InitialzeGraphicsManager()
{
	if (m_graphicsManager.IsNull())
	{
		InitializeCommon();
		InitialzeFileManager();
		InitialzePlatformManager();
		InitialzePhysicsManager();

		GraphicsManager::ConfigData data;
		data.GraphicsAPI = m_configData.GraphicsAPI;
		data.RenderingType = m_configData.RenderingType;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.FileManager = m_fileManager;
		data.PlatformTextureLoading = true;
#ifdef LN_OS_WIN32
		data.D3D9Device = m_configData.D3D9Device;
#endif
		m_graphicsManager.Attach(LN_NEW GraphicsManager(data));
		GraphicsManager::Instance = m_graphicsManager;

		m_profilerRenderer = LN_NEW ProfilerRenderer(m_graphicsManager, &Profiler::Instance);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::InitialzeDocumentsManager()
{
	if (m_documentsManager.IsNull())
	{
		InitializeCommon();
		InitialzeGraphicsManager();

		Documents::DocumentsManager::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		m_documentsManager.Attach(LN_NEW Documents::DocumentsManager(data));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::InitialzeGUIManager()
{
	if (m_guiManager == NULL)
	{
		InitializeCommon();
		InitialzePlatformManager();
		InitialzeGraphicsManager();
		InitialzeDocumentsManager();

		GUIManagerImpl::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.DocumentsManager = m_documentsManager;
		m_guiManager = LN_NEW GUIManagerImpl();
		m_guiManager->Initialize(data);

		GUIManagerImpl::Instance = m_guiManager;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#ifdef LN_BUILD_SCENE_MODULE
void ApplicationImpl::InitialzeSceneGraphManager()
{
	if (m_sceneGraphManager == NULL)
	{
		InitializeCommon();
		InitialzeFileManager();
		InitialzeGraphicsManager();
		SceneGraphManager::ConfigData data;
		data.FileManager = m_fileManager;
		data.GraphicsManager = m_graphicsManager;
		m_sceneGraphManager = LN_NEW SceneGraphManager(data);
		SceneGraphManager::Instance = m_sceneGraphManager;
	}
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ApplicationImpl::UpdateFrame()
{
	m_endRequested = !m_platformManager->DoEvents();

	if (m_guiManager != NULL) {
		m_guiManager->InjectElapsedTime(m_fpsController.GetElapsedGameTime());

		{	// プロファイリング範囲
			ScopedProfilerSection prof(Profiler::Group_MainThread, Profiler::Section_MainThread_GUILayput);
			m_guiManager->UpdateLayoutOnMainWindow();
		}
	}

	Render();

	m_fpsController.Process();

	Profiler::Instance.SetMainFPS(m_fpsController.GetFps());
	Profiler::Instance.SetMainFPSCapacity(m_fpsController.GetCapacityFps());

	return !m_endRequested;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::Render()
{
	if (m_graphicsManager != NULL)
	{

		Details::Renderer* renderer = m_graphicsManager->GetRenderer();
		SwapChain* swap = m_graphicsManager->GetMainSwapChain();



		renderer->Begin();
		renderer->SetRenderTarget(0, swap->GetBackBuffer());
		renderer->SetDepthBuffer(swap->GetBackBufferDepth());
		renderer->Clear(ClearFlags::All, ColorF::White);

		//m_graphicsManager->GetRenderer()->Clear(Graphics::ClearFlags::All, Graphics::ColorF::White);

		//Graphics::DepthStencilState state;
		//state.DepthEnable = false;
		//state.DepthWriteEnable = false;
		//state.StencilEnable = false;
		//m_graphicsManager->GetRenderer()->SetDepthStencilState(state);

		if (m_guiManager != NULL) {
			m_guiManager->RenderOnMainWindow();
		}

		if (m_profilerRenderer != NULL) {
			//m_profilerRenderer->Render(Vector2(640, 480));	//TODO
		}

		renderer->End();
		swap->Present();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationImpl::ResetFrameDelay()
{
	m_fpsController.RefreshSystemDelay();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ApplicationImpl::OnEvent(const Platform::EventArgs& e)
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
			if (m_guiManager->InjectKeyUp(e.Key.KeyCode, e.Key.IsAlt, e.Key.IsShift, e.Key.IsControl/*, e.Key.Char*/)) { return true; }
		}
		break;
	case Platform::EventType_KeyChar:		//  文字入力
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectTextInput(e.Key.Char)) { return true; }
		}
		break;
	default:
		break;
	}
	return false;
}

//=============================================================================
// Engine
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Engine::Initialize(const ApplicationSettings& settings)
{
	ApplicationImpl::Instance = ApplicationImpl::Create(settings);
	ApplicationImpl::Instance->Initialize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Engine::Finalize()
{
	LN_SAFE_RELEASE(ApplicationImpl::Instance);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Engine::UpdateFrame()
{
	LN_CHECK_STATE(ApplicationImpl::Instance != NULL);
	return ApplicationImpl::Instance->UpdateFrame();
}


LN_NAMESPACE_END
