/*
[2016/3/9] レンダリングパス
	一番えらいのは Viewport クラスにしたい。
	Engine や Window はコレに対して Render() を呼ぶだけでOK、みたいな。

	Editor モードのとき、Viewport でペインを分けると再描画の範囲を限定できる。
	1つのWidgetに再描画が必要になってもウィンドウ全体を再描画する必要はない。
	→ Viewport は レンダーターゲットを持つ。

	Game モードのときは基本的に Viewport は 1つだけになると思う。

	Window 内は Viweport の階層構造。
	ルートの Viewport は Window 全体で、swapchain のレンダーターゲットをアタッチするのがいいかも。

	GameMode (毎フレーム)
		1.Scene 描画。Scene 内のすべてのカメラのレンダーターゲットに描画する
		2.Viewport 描画。ルートから順に、レンダーターゲットをバックバッファに描画する

	EditorMode
		再描画要求あったときだけ↑のを行う。

	ImageEffect
		ポストエフェクト。カメラにくっつく。Unity と同じかんじ。


	参考:UE4
	https://shikihuiku.wordpress.com/2014/05/09/%E3%83%A1%E3%83%A2-ue4%E3%81%AErendering%E3%81%AE%E5%91%BC%E3%81%B0%E3%82%8C%E6%96%B9/


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
	- トップレベルオブジェクト (EngineManager) はグローバル。
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
#include "Animation/AnimationManager.h"
#include <Lumino/Platform/PlatformWindow.h>
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include <Lumino/Engine.h>
#include "Graphics/RendererImpl.h"
#include "Graphics/RenderingThread.h"
#include "Graphics/GraphicsManager.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include "Scene/SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include "Effect/EffectManager.h"
#include "UI/UIManager.h"
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/Foundation/Application.h>
#include "AssetsManager.h"
#include "EngineDiagRenderer.h"
#include "EngineManager.h"

LN_NAMESPACE_BEGIN

////=============================================================================
//// NativeWindowEventListener
////=============================================================================
//class EngineManager::NativeWindowEventListener
//{
//public:
//	NativeWindowEventListener(EngineManager)
//	{
//	}
//};

//=============================================================================
// EngineManager
//=============================================================================

EngineManager* EngineManager::Instance = nullptr;
const TCHAR* EngineManager::LogFileName = _T("lnlog.txt");

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineManager* EngineManager::GetInstance(EngineManager* priority)
{
	if (priority != nullptr)
		return priority;
	return EngineManager::Instance;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineManager* EngineManager::Create(const EngineSettings& configData)
{
	RefPtr<EngineManager> app(LN_NEW EngineManager(configData), false);
	//app->Initialize();
	app.SafeAddRef();
	return app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineManager::EngineManager(const EngineSettings& configData)
	: m_configData(configData)
	, m_animationManager(nullptr)
	, m_fileManager(nullptr)
	, m_inputManager(nullptr)
	, m_audioManager(nullptr)
	, m_graphicsManager(nullptr)
	, m_effectManager(nullptr)
	, m_modelManager(nullptr)
	, m_uiManager(nullptr)
	, m_sceneGraphManager(nullptr)
	, m_assetsManager(nullptr)
	, m_diagRenderer(nullptr)
	, m_application(nullptr)
	, m_frameRenderingSkip(false)
	, m_frameRenderd(false)
	, m_commonInitied(false)
	, m_endRequested(false)
	, m_comInitialized(false)
{
	m_fpsController.SetEnableFpsTest(true);
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_MainThread, 60.0f);	// TODO 
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_RenderThread, 60.0f);



	// COM 初期化
	if (m_configData.autoCoInitialize && SUCCEEDED(::CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		// エラーにはしない。別の設定で COM が初期化済みだったりすると失敗することがあるが、COM 自体は使えるようになっている
		m_comInitialized = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineManager::~EngineManager()
{
	LN_SAFE_RELEASE(m_application);

	LN_SAFE_RELEASE(m_diagRenderer);

	if (m_assetsManager != nullptr)
	{
		m_assetsManager->Finalize();
		LN_SAFE_RELEASE(m_assetsManager);
	}

	if (m_graphicsManager != nullptr)
	{
		// 先に描画スレッドを終了しておく。
		// 他モジュールで発行されたコマンドがまだ実行待機中にそのモジュールが解放されるとマズイ。
		m_graphicsManager->Finalize();
	}

	if (m_platformManager != nullptr) {
		m_platformManager->Dispose();
	}
	if (m_sceneGraphManager != nullptr) {
		m_sceneGraphManager->ReleaseDefaultSceneGraph();
		LN_SAFE_RELEASE(m_sceneGraphManager);
	}
	if (m_modelManager != nullptr) {
		m_modelManager->Finalize();
		LN_SAFE_RELEASE(m_modelManager);
	}
	if (m_effectManager != nullptr) {
		m_effectManager->Finalize();
		LN_SAFE_RELEASE(m_effectManager);
	}
	if (m_uiManager != nullptr) {
		m_uiManager->Finalize();
		LN_SAFE_RELEASE(m_uiManager);
	}

	LN_SAFE_RELEASE(m_graphicsManager);

	if (m_physicsManager != nullptr) {
		m_physicsManager->Finalize();
		m_physicsManager.SafeRelease();
		//LN_SAFE_RELEASE(m_physicsManager);
	}
#ifdef LN_BUILD_AUDIO_MODULE
	if (m_audioManager != nullptr) {
		m_audioManager->Finalize();
		LN_SAFE_RELEASE(m_audioManager);
	}
#endif
	if (m_inputManager != nullptr) {
		m_inputManager->Finalize();
		LN_SAFE_RELEASE(m_inputManager);
	}

	LN_SAFE_RELEASE(m_fileManager);

	LN_SAFE_RELEASE(m_animationManager);

	Console::Free();


	if (m_comInitialized)
	{
		::CoUninitialize();
		m_comInitialized = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::Initialize()
{
	InitializePlatformManager();
	InitializeInputManager();
	InitializeAudioManager();
	InitializePhysicsManager();
	InitializeGraphicsManager();
	InitializeEffectManager();
	InitializeModelManager();
	InitializeUIManager();
#ifdef LN_BUILD_SCENE_MODULE
	InitializeSceneGraphManager();
#endif
	InitializeAssetsManager();
	m_application = LN_NEW detail::InternalApplicationImpl();	// TODO: とりあえず
	m_application->Initialize(this);

	EngineDiagCore::Instance.Initialize(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeCommon()
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
void EngineManager::InitializeAnimationManager()
{
	if (m_animationManager == nullptr)
	{
		m_animationManager = LN_NEW detail::AnimationManager();
		m_animationManager->Initialize();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeFileManager()
{
	if (m_fileManager == nullptr)
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
void EngineManager::InitializePlatformManager()
{
	if (m_platformManager.IsNull())
	{
		InitializeCommon();

		PlatformManager::Settings data;
		data.API = WindowSystemAPI_Win32API;
		data.MainWindowSettings.Title = m_configData.mainWindowTitle;
		data.MainWindowSettings.ClientSize = m_configData.mainWindowSize;
		data.MainWindowSettings.Fullscreen = false;
		data.MainWindowSettings.Resizable = true;
		data.MainWindowSettings.UserWindow = m_configData.UserMainWindow;
		data.UseInternalUIThread = false;

		m_platformManager.Attach(LN_NEW PlatformManager());
		m_platformManager->Initialize(data);

		// イベントリスナー登録
		m_platformManager->GetMainWindow()->AttachEventListener(this, 0);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeInputManager()
{
	if (m_inputManager == nullptr)
	{
		InitializePlatformManager();

		detail::InputManager::Settings data;
		data.mainWindow = m_platformManager->GetMainWindow();

		m_inputManager = LN_NEW detail::InputManager();
		m_inputManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeAudioManager()
{
#ifdef LN_BUILD_AUDIO_MODULE
	if (m_audioManager == nullptr)
	{
		InitializeCommon();
		InitializeFileManager();

		// ユーザー定義のウィンドウハンドルが指定されている場合、
		// ダミーウィンドウクラスを作るために PlatformManager の初期化が必要。
		if (m_configData.UserMainWindow != nullptr) {
			InitializePlatformManager();
		}

		AudioManagerImpl::Settings data;
		data.FileManager = m_fileManager;
		data.StreamCacheObjectCount = m_configData.soundCacheCapacity.objectCount;
		data.StreamSourceCacheMemorySize = m_configData.soundCacheCapacity.memorySize;
		data.DMInitMode = m_configData.directMusicMode;
#ifdef LN_OS_WIN32
		data.hWnd = (m_platformManager != nullptr) ? PlatformSupport::GetWindowHandle(m_platformManager->GetMainWindow()) : nullptr;
#endif
		data.DirectMusicReverbLevel = m_configData.DirectMusicReverbLevel;
		m_audioManager = AudioManagerImpl::Create(data);
	}
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializePhysicsManager()
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
void EngineManager::InitializeGraphicsManager()
{
	if (m_graphicsManager == nullptr)
	{
		InitializeCommon();
		InitializeAnimationManager();
		InitializeFileManager();
		InitializePlatformManager();
		InitializePhysicsManager();

		GraphicsManager::ConfigData data;
		data.GraphicsAPI = m_configData.GraphicsAPI;
		data.RenderingType = m_configData.RenderingType;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.backBufferSize = m_configData.backBufferSize;
		data.animationManager = m_animationManager;
		data.FileManager = m_fileManager;
		data.PlatformTextureLoading = true;
#ifdef LN_OS_WIN32
		data.D3D9Device = m_configData.D3D9Device;
#endif
		m_graphicsManager = LN_NEW GraphicsManager();
		m_graphicsManager->Initialize(data);

		m_diagRenderer = LN_NEW EngineDiagRenderer();
		m_diagRenderer->Initialize(m_graphicsManager, &EngineDiagCore::Instance);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeEffectManager()
{
	if (m_effectManager == nullptr)
	{
		InitializeCommon();
		InitializeGraphicsManager();

		detail::EffectManager::Settings data;
		data.fileManager = m_fileManager;
		data.audioManager = m_audioManager;
		data.graphicsManager = m_graphicsManager;
		m_effectManager = LN_NEW detail::EffectManager();
		m_effectManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeModelManager()
{
	if (m_modelManager == nullptr)
	{
		InitializeCommon();
		InitializeGraphicsManager();
		InitializePhysicsManager();

		detail::ModelManager::Settings data;
		data.FileManager = m_fileManager;
		data.PhysicsManager = m_physicsManager;
		data.GraphicsManager = m_graphicsManager;
		data.ModelCoreCacheSize = 32;
		data.ModelCoreCacheMemorySize = 0;
		m_modelManager = LN_NEW detail::ModelManager();
		m_modelManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeDocumentsManager()
{
#if 0
	if (m_documentsManager.IsNull())
	{
		InitializeCommon();
		InitializeGraphicsManager();

		Documents::DocumentsManager::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		m_documentsManager.Attach(LN_NEW Documents::DocumentsManager(data));
	}
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeUIManager()
{
	if (m_uiManager == nullptr)
	{
		InitializeCommon();
		InitializePlatformManager();
		InitializeGraphicsManager();
		InitializeDocumentsManager();

		detail::UIManager::Settings data;
		data.graphicsManager = m_graphicsManager;
		data.mainWindow = m_platformManager->GetMainWindow();
		data.documentsManager = m_documentsManager;
		m_uiManager = LN_NEW detail::UIManager();
		m_uiManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#ifdef LN_BUILD_SCENE_MODULE
void EngineManager::InitializeSceneGraphManager()
{
	if (m_sceneGraphManager == nullptr)
	{
		InitializeCommon();
		InitializeFileManager();
		InitializeGraphicsManager();
		InitializePhysicsManager();
		InitializeEffectManager();
		InitializeModelManager();
		SceneGraphManager::ConfigData data;
		data.FileManager = m_fileManager;
		data.GraphicsManager = m_graphicsManager;
		data.PhysicsManager = m_physicsManager;
		data.effectManager = m_effectManager;
		data.modelManager = m_modelManager;
		data.mainViewport = m_graphicsManager->GetMainViewport();
		m_sceneGraphManager = LN_NEW SceneGraphManager(data);
		m_sceneGraphManager->CreateDefaultSceneGraph();
		SceneGraphManager::Instance = m_sceneGraphManager;
	}
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeAssetsManager()
{
	if (m_assetsManager == nullptr)
	{
		m_assetsManager = LN_NEW AssetsManager();
		m_assetsManager->Initialize(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool EngineManager::UpdateFrame()
{
	if (!m_platformManager->DoEvents())
	{
		m_endRequested = true;
	}

	if (m_animationManager != nullptr)
	{
		m_animationManager->AdvanceTime(m_fpsController.GetElapsedGameTime());
	}

	if (m_inputManager != nullptr) {
		m_inputManager->UpdateFrame();
	}

	if (m_sceneGraphManager != nullptr) {
		m_sceneGraphManager->UpdateFrameDefaultSceneGraph(m_fpsController.GetElapsedGameTime());
	}

	if (m_uiManager != nullptr)
	{
		m_uiManager->GetDefaultUIContext()->InjectElapsedTime(m_fpsController.GetElapsedGameTime());

		{	// プロファイリング範囲
			ScopedProfilerSection prof(Profiler::Group_MainThread, Profiler::Section_MainThread_GUILayput);
			const Size& size = m_graphicsManager->GetMainSwapChain()->GetBackBuffer()->GetSize();
			m_uiManager->GetDefaultUIContext()->GetMainWindowView()->UpdateLayout(SizeF(static_cast<float>(size.width), static_cast<float>(size.height)));
		}
	}

	// 手動描画されていなければここで自動描画する
	if (!m_frameRenderd)
	{
		if (BeginRendering())
		{
			Render();
			EndRendering();
		}
	}

	m_fpsController.Process();

	Profiler::Instance.SetMainFPS(m_fpsController.GetFps());
	Profiler::Instance.SetMainFPSCapacity(m_fpsController.GetCapacityFps());

	m_frameRenderd = false;
	return !m_endRequested;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool EngineManager::BeginRendering()
{
	m_frameRenderingSkip = true;
	if (m_graphicsManager == nullptr) return false;

	// 描画遅延の確認
	bool delay = false;
	if (m_graphicsManager->GetRenderingType() == RenderingType::Deferred)
	{
		if (m_graphicsManager->GetRenderingThread()->IsRunning()) {
			delay = true;
		}
	}
	else {
		// TODO:
	}
	if (delay) {
		return false;
	}

	m_frameRenderingSkip = false;



	if (m_effectManager != nullptr) {
		m_effectManager->PreRender();	// Effekseer の更新スレッドを開始するのはここ
	}

	Details::Renderer* renderer = m_graphicsManager->GetRenderer();
	SwapChain* swap = m_graphicsManager->GetMainSwapChain();
	renderer->Begin();
	renderer->SetRenderTarget(0, swap->GetBackBuffer());
	renderer->SetDepthBuffer(swap->GetBackBufferDepth());
	renderer->Clear(ClearFlags::All, ColorF::Black);

	m_frameRenderd = true;
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::EndRendering()
{
	if (m_graphicsManager == nullptr || m_frameRenderingSkip) return;

	Details::Renderer* renderer = m_graphicsManager->GetRenderer();
	SwapChain* swap = m_graphicsManager->GetMainSwapChain();
	m_graphicsManager->SwitchActiveContext(nullptr);
	renderer->End();
	swap->Present();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::Render()
{
	if (m_graphicsManager != nullptr)
	{
		//// 描画遅延の確認
		//bool delay = false;
		//if (m_graphicsManager->GetRenderingType() == RenderingType::Deferred)
		//{
		//	if (m_graphicsManager->GetRenderingThread()->IsRunning()) {
		//		delay = true;
		//	}
		//}
		//else {
		//	// TODO:
		//}
		//if (delay) {
		//	m_frameRenderingSkip = true;
		//	return;
		//}
		//m_frameRenderingSkip = false;


		//if (m_effectManager != nullptr) {
		//	m_effectManager->PreRender();	// Effekseer の更新スレッドを開始するのはここ
		//}

		Details::Renderer* renderer = m_graphicsManager->GetRenderer();
		SwapChain* swap = m_graphicsManager->GetMainSwapChain();



		//renderer->Begin();
		//renderer->SetRenderTarget(0, swap->GetBackBuffer());
		//renderer->SetDepthBuffer(swap->GetBackBufferDepth());
		//renderer->Clear(ClearFlags::All, ColorF::White);


		//m_graphicsManager->GetRenderer()->Clear(Graphics::ClearFlags::All, Graphics::ColorF::White);

		//Graphics::DepthStencilState state;
		//state.DepthEnable = false;
		//state.DepthWriteEnable = false;
		//state.StencilEnable = false;
		//m_graphicsManager->GetRenderer()->SetDepthStencilState(state);

		//if (m_sceneGraphManager != nullptr) {
		//	m_sceneGraphManager->RenderDefaultSceneGraph(swap->GetBackBuffer());
		//}

		if (m_graphicsManager != nullptr)
		{
			m_graphicsManager->GetMainViewport()->Render();
		}

		if (m_uiManager != nullptr) {
			m_uiManager->GetDefaultUIContext()->Render();
		}

		if (m_diagRenderer != nullptr && m_diagRenderer->IsVisible())
		{
			GraphicsContext* g = m_graphicsManager->GetGraphicsContext();
			g->Clear(ClearFlags::Depth, ColorF::White);
			g->Set2DRenderingMode(-1,1);	// TODO
			m_diagRenderer->Render(g, Vector2(640, 480));	//TODO
		}

	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::ResetFrameDelay()
{
	m_fpsController.RefreshSystemDelay();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::Exit()
{
	m_endRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool EngineManager::OnEvent(const PlatformEventArgs& e)
{
	UILayoutView* uiView = nullptr;
	if (m_uiManager != nullptr)
	{
		uiView = m_uiManager->GetDefaultUIContext()->GetMainWindowView();
	}


	switch (e.type)
	{
	case PlatformEventType::Quit:	// アプリ終了要求
	case PlatformEventType::Close:	// ウィンドウが閉じられようとしている
		break;

	case PlatformEventType::MouseDown:		// マウスボタンが押された
		if (uiView != nullptr)
		{
			if (uiView->InjectMouseButtonDown(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr)
		{
			if (m_sceneGraphManager->GetDefault3DSceneGraph()->InjectMouseButtonDown(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseUp:			// マウスボタンが離された
		if (uiView != nullptr)
		{
			if (uiView->InjectMouseButtonUp(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr)
		{
			if (m_sceneGraphManager->GetDefault3DSceneGraph()->InjectMouseButtonUp(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseMove:		// マウスが移動した
		if (uiView != nullptr)
		{
			if (uiView->InjectMouseMove(e.mouse.x, e.mouse.y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr)
		{
			if (m_sceneGraphManager->GetDefault3DSceneGraph()->InjectMouseMove(e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseWheel:		// マウスホイールが操作された
		if (uiView != nullptr)
		{
			if (uiView->InjectMouseWheel(e.mouse.wheelDelta, e.mouse.x, e.mouse.y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr)
		{
			if (m_sceneGraphManager->GetDefault3DSceneGraph()->InjectMouseWheel(e.mouse.wheelDelta)) { return true; }
		}
		break;
	case PlatformEventType::KeyDown:	// キー押下
		if (uiView != nullptr)
		{
			if (uiView->InjectKeyDown(e.key.keyCode, e.key.isAlt, e.key.isShift, e.key.isControl)) { return true; }
		}

		// デバッグ表示切替
		if (m_configData.acceleratorKeys.toggleShowDiag != nullptr &&
			m_configData.acceleratorKeys.toggleShowDiag->EqualKeyInput(e.key.keyCode, e.key.isAlt, e.key.isShift, e.key.isControl) &&
			m_diagRenderer != nullptr)
		{
			m_diagRenderer->SetVisible(!m_diagRenderer->IsVisible());
		}
		break;
	case PlatformEventType::KeyUp:		//  キー押し上げ
		if (uiView != nullptr)
		{
			if (uiView->InjectKeyUp(e.key.keyCode, e.key.isAlt, e.key.isShift, e.key.isControl/*, e.Key.keyChar*/)) { return true; }
		}
		break;
	case PlatformEventType::KeyChar:		//  文字入力
		if (uiView != nullptr)
		{
			if (uiView->InjectTextInput(e.key.keyChar)) { return true; }
		}
		break;
	case PlatformEventType::WindowSizeChanged:
		if (m_graphicsManager != nullptr)
		{
			m_graphicsManager->GetMainSwapChain()->Resize(Size(e.size.width, e.size.height));
		}
		break;
	default:
		break;
	}

	if (m_inputManager != nullptr) {
		m_inputManager->OnEvent(e);
	}
	return false;
}

LN_NAMESPACE_END
