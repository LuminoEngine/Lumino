
#include "Internal.h"
#include <Lumino/IO/Console.h>
#include <Lumino/Profiler.h>
#include "Animation/AnimationManager.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Platform/PlatformSupport.h>
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include <Lumino/Engine.h>
#include <Lumino/World.h>
#include "Graphics/RHIRenderingContext.h"
#include "Graphics/RenderingThread.h"
#include "Graphics/GraphicsManager.h"
#include "Documents/DocumentsManager.h"
#include "Scene/SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include "Effect/EffectManager.h"
#include "UI/UIManager.h"
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/Framework/Application.h>
#include "AssetsManager.h"
#include "EngineDiagViewer.h"
#include "EngineManager.h"
#include "Resource.h"

LN_NAMESPACE_BEGIN


//==============================================================================
// RuntimeResource
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(RuntimeResource, Object);

//------------------------------------------------------------------------------
RuntimeResource::RuntimeResource()
{
}

//------------------------------------------------------------------------------
RuntimeResource::~RuntimeResource()
{
}

//------------------------------------------------------------------------------
void RuntimeResource::finalize_()
{
	Dispose();
}

//------------------------------------------------------------------------------
void RuntimeResource::Dispose()
{
}

//==============================================================================
// EngineSettings
//==============================================================================

detail::EngineSettings detail::EngineSettings::instance;

//------------------------------------------------------------------------------
void EngineSettings::setMainWindowSize(const SizeI& size)
{
	detail::EngineSettings::instance.mainWindowSize = size;
}
void EngineSettings::setMainWindowSize(int width, int height)
{
	detail::EngineSettings::instance.mainWindowSize.set(width, height);
}

//------------------------------------------------------------------------------
void EngineSettings::setMainBackBufferSize(const SizeI& size)
{
	detail::EngineSettings::instance.mainBackBufferSize = size;
}
void EngineSettings::setMainBackBufferSize(int width, int height)
{
	detail::EngineSettings::instance.mainBackBufferSize.set(width, height);
}

//------------------------------------------------------------------------------
void EngineSettings::SetMainWindowTitle(const StringRef& title)
{
	detail::EngineSettings::instance.mainWindowTitle = title;
}

//------------------------------------------------------------------------------
void EngineSettings::SetEngineLogEnabled(bool enabled)
{
	detail::EngineSettings::instance.engineLogEnabled = enabled;
}

//------------------------------------------------------------------------------
void EngineSettings::SetGraphicsAPI(GraphicsAPI graphicsAPI)
{
	detail::EngineSettings::instance.graphicsAPI = graphicsAPI;
}

//------------------------------------------------------------------------------
void EngineSettings::SetGraphicsRenderingType(GraphicsRenderingType renderingType)
{
	detail::EngineSettings::instance.renderingType = renderingType;
}

//------------------------------------------------------------------------------
void EngineSettings::SetFpuPreserveEnabled(bool enabled)
{
	detail::EngineSettings::instance.fpuPreserveEnabled = enabled;
}

//------------------------------------------------------------------------------
void EngineSettings::SetDirectMusicMode(DirectMusicMode mode)
{
	detail::EngineSettings::instance.directMusicMode = mode;
}

//------------------------------------------------------------------------------
void EngineSettings::SetDirectMusicReverbLevel(float level)
{
	detail::EngineSettings::instance.DirectMusicReverbLevel = level;
}

//------------------------------------------------------------------------------
void EngineSettings::SetUserWindowHandle(intptr_t hWnd)	// Qt とかは windows.h を隠蔽しているので、型を HWND にしてしまうと #include <windows.h> 必要だったりといろいろ面倒。
{
	detail::EngineSettings::instance.userMainWindow = hWnd;
}

//------------------------------------------------------------------------------
void EngineSettings::SetD3D9Device(void* device)
{
	detail::EngineSettings::instance.D3D9Device = device;
}

//==============================================================================
// EngineManager
//==============================================================================

EngineManager* EngineManager::Instance = nullptr;
const TCHAR* EngineManager::LogFileName = _T("EngineLog.txt");

//------------------------------------------------------------------------------
EngineManager* EngineManager::getInstance(EngineManager* priority)
{
	if (priority != nullptr)
		return priority;
	return EngineManager::Instance;
}

//------------------------------------------------------------------------------
EngineManager* EngineManager::create(const detail::EngineSettings& configData)
{
	RefPtr<EngineManager> app(LN_NEW EngineManager(configData), false);
	app.safeAddRef();
	return app;
}

//------------------------------------------------------------------------------
EngineManager::EngineManager(const detail::EngineSettings& configData)
	: m_configData(configData)
	, m_fpsController()
	, m_frameUpdateMode(FrameUpdateMode::VariableOnGameTime)
	, m_animationManager(nullptr)
	, m_fileManager(nullptr)
	, m_inputManager(nullptr)
	, m_audioManager(nullptr)
	, m_physicsManager(nullptr)
	, m_graphicsManager(nullptr)
	, m_effectManager(nullptr)
	, m_modelManager(nullptr)
	, m_documentsManager(nullptr)
	, m_uiManager(nullptr)
	, m_sceneGraphManager(nullptr)
	, m_assetsManager(nullptr)
	, m_diagViewer(nullptr)
	, m_defaultWorld2D(nullptr)
	, m_defaultWorld3D(nullptr)
	//, m_frameRenderingSkip(false)
	, m_frameRenderd(false)
	, m_commonInitied(false)
	, m_endRequested(false)
	, m_comInitialized(false)
{
	m_fpsController.setEnableFpsTest(true);
	Profiler::Instance.setBaseFrameRate(Profiler::Group_MainThread, 60.0f);	// TODO
	Profiler::Instance.setBaseFrameRate(Profiler::Group_RenderThread, 60.0f);


#if defined(LN_OS_WIN32)
	// COM 初期化
	if (m_configData.autoCoInitialize && SUCCEEDED(::CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		// エラーにはしない。別の設定で COM が初期化済みだったりすると失敗することがあるが、COM 自体は使えるようになっている
		m_comInitialized = true;
	}
#endif
}

//------------------------------------------------------------------------------
EngineManager::~EngineManager()
{
	if (m_uiManager != nullptr)
	{
		m_uiManager->releaseGameModeMainFrame();
	}

	m_defaultWorld2D.safeRelease();
	m_defaultWorld3D.safeRelease();

	LN_SAFE_RELEASE(m_diagViewer);

	if (m_assetsManager != nullptr)
	{
		m_assetsManager->Finalize();
		LN_SAFE_RELEASE(m_assetsManager);
	}

	if (m_graphicsManager != nullptr)
	{
		// 先に描画スレッドを終了しておく。
		// 他モジュールで発行されたコマンドがまだ実行待機中にそのモジュールが解放されるとマズイ。
		m_graphicsManager->Dispose();
	}

	if (m_platformManager != nullptr)
	{
		m_platformManager->getMainWindow()->detachEventListener(this);
		m_platformManager->Dispose();
	}
	if (m_sceneGraphManager != nullptr) {
		m_sceneGraphManager->releaseDefaultSceneGraph();
		LN_SAFE_RELEASE(m_sceneGraphManager);
	}
	if (m_modelManager != nullptr) {
		m_modelManager->Finalize();
		LN_SAFE_RELEASE(m_modelManager);
	}
	if (m_effectManager != nullptr)
	{
		m_graphicsManager->removeDeviceResetListener(m_effectManager);
		m_effectManager->Finalize();
		LN_SAFE_RELEASE(m_effectManager);
	}
	if (m_uiManager != nullptr) {
		m_uiManager->Finalize();
		LN_SAFE_RELEASE(m_uiManager);
	}

	LN_SAFE_RELEASE(m_documentsManager);

	LN_SAFE_RELEASE(m_graphicsManager);

	if (m_physicsManager != nullptr) {
		m_physicsManager->Finalize();
		m_physicsManager.safeRelease();
		//LN_SAFE_RELEASE(m_physicsManager);
	}
	if (m_audioManager != nullptr) {
		m_audioManager->Finalize();
		LN_SAFE_RELEASE(m_audioManager);
	}
	if (m_inputManager != nullptr) {
		m_inputManager->Finalize();
		LN_SAFE_RELEASE(m_inputManager);
	}

	LN_SAFE_RELEASE(m_fileManager);

	LN_SAFE_RELEASE(m_animationManager);


	Profiler2::finalizeGlobalSections();

#if defined(LN_OS_WIN32)
	if (m_comInitialized)
	{
		::CoUninitialize();
		m_comInitialized = false;
	}
#endif
}

//------------------------------------------------------------------------------
void EngineManager::initialize()
{
	Profiler2::initializeGlobalSections();

	initializePlatformManager();
	initializeInputManager();
	initializeAudioManager();
	initializePhysicsManager();
	initializeGraphicsManager();
	initializeEffectManager();
	initializeModelManager();
	initializeUIManager();
	initializeSceneGraphManager();
	initializeAssetsManager();

	EngineDiagCore::Instance.initialize(this);

	m_defaultWorld2D = newObject<World2D>();
	m_defaultWorld3D = newObject<World3D>();
	m_uiManager->createGameModeMainFrame(m_defaultWorld2D, m_defaultWorld3D);
	m_uiManager->getMainWindow()->setDelayedRenderingSkip(m_configData.delayedRenderingSkip);
}

//------------------------------------------------------------------------------
void EngineManager::initializeCommon()
{
	if (!m_commonInitied)
	{
		InternalResource::initializeEngineResource();

		// ログファイル出力
		if (m_configData.engineLogEnabled)
		{
			Logger::initialize(LogFileName);
			//Logger2::initialize("log.txt");
			//LN_LOG_INFO("Lumino 1.0.0");
			//LN_LOG_INFO << "Lumino 1.0.0";
			//LN_LOG_INFO << L"Lumino 1.0.0";
		}
		m_commonInitied = true;
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeAnimationManager()
{
	if (m_animationManager == nullptr)
	{
		m_animationManager = LN_NEW detail::AnimationManager();
		m_animationManager->initialize();
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeFileManager()
{
	if (m_fileManager == nullptr)
	{
		FileManager::Settings data;
		data.AccessMode = m_configData.FileAccessPriority;
		m_fileManager = LN_NEW FileManager(data);
		for (auto& e : m_configData.ArchiveFileEntryList) {
			m_fileManager->registerArchive(e.filePath, e.password);
		}
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializePlatformManager()
{
	if (m_platformManager.isNull())
	{
		initializeCommon();

		PlatformManager::Settings data;
		data.windowSystemAPI = m_configData.windowSystemAPI;
		data.mainWindowSettings.title = m_configData.mainWindowTitle;
		data.mainWindowSettings.clientSize = m_configData.mainWindowSize;
		data.mainWindowSettings.fullscreen = false;
		data.mainWindowSettings.resizable = true;
		data.mainWindowSettings.userWindow = m_configData.userMainWindow;
		data.useInternalUIThread = false;

		m_platformManager.attach(LN_NEW PlatformManager());
		m_platformManager->initialize(data);

		// イベントリスナー登録
		m_platformManager->getMainWindow()->attachEventListener(this, 0);
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeInputManager()
{
	if (m_inputManager == nullptr)
	{
		initializePlatformManager();

		detail::InputManager::Settings data;
		data.mainWindow = m_platformManager->getMainWindow();

		m_inputManager = LN_NEW detail::InputManager();
		m_inputManager->initialize(data);
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeAudioManager()
{
	if (m_audioManager == nullptr)
	{
		initializeCommon();
		initializeFileManager();

		// ユーザー定義のウィンドウハンドルが指定されている場合、
		// ダミーウィンドウクラスを作るために PlatformManager の初期化が必要。
		if (m_configData.userMainWindow != 0)
		{
			initializePlatformManager();
		}

		detail::AudioManager::Settings settings;
		settings.fileManager = m_fileManager;
		settings.streamCacheObjectCount = m_configData.soundCacheCapacity.objectCount;
		settings.streamSourceCacheMemorySize = m_configData.soundCacheCapacity.memorySize;
		settings.directMusicInitMode = m_configData.directMusicMode;
#ifdef LN_OS_WIN32
		settings.hWnd = (m_platformManager != nullptr) ? PlatformSupport::getWindowHandle(m_platformManager->getMainWindow()) : nullptr;
#endif
		settings.directMusicReverbLevel = m_configData.DirectMusicReverbLevel;
		m_audioManager = LN_NEW detail::AudioManager();
		m_audioManager->initialize(settings);
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializePhysicsManager()
{
	if (m_physicsManager.isNull())
	{
		initializeCommon();
		m_physicsManager = RefPtr<detail::PhysicsManager>::makeRef();
		m_physicsManager->initialize();
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeGraphicsManager()
{
	if (m_graphicsManager == nullptr)
	{
		initializeCommon();
		initializeAnimationManager();
		initializeFileManager();
		initializePlatformManager();
		initializePhysicsManager();

		detail::GraphicsManager::ConfigData data;
		data.graphicsAPI = m_configData.graphicsAPI;
		data.renderingType = m_configData.renderingType;
		data.mainWindow = m_platformManager->getMainWindow();
		data.backBufferSize = m_configData.mainBackBufferSize;
		data.animationManager = m_animationManager;
		data.fileManager = m_fileManager;
		data.physicsManager = m_physicsManager;
		data.platformTextureLoading = true;
		data.diag = &EngineDiagCore::Instance;
#ifdef LN_OS_WIN32
		data.D3D9Device = m_configData.D3D9Device;
#endif
		m_graphicsManager = LN_NEW detail::GraphicsManager();
		m_graphicsManager->initialize(data);

		m_diagViewer = LN_NEW EngineDiagViewer();
		m_diagViewer->initialize(this, &EngineDiagCore::Instance);
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeEffectManager()
{
	if (m_effectManager == nullptr)
	{
		initializeCommon();
		initializeGraphicsManager();

		detail::EffectManager::Settings data;
		data.fileManager = m_fileManager;
		data.audioManager = m_audioManager;
		data.graphicsManager = m_graphicsManager;
		m_effectManager = LN_NEW detail::EffectManager();
		m_effectManager->initialize(data);

		m_graphicsManager->addDeviceResetListener(m_effectManager);
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeModelManager()
{
	if (m_modelManager == nullptr)
	{
		initializeCommon();
		initializeGraphicsManager();
		initializePhysicsManager();

		detail::ModelManager::ConfigData data;
		data.fileManager = m_fileManager;
		data.physicsManager = m_physicsManager;
		data.graphicsManager = m_graphicsManager;
		data.modelCoreCacheSize = 32;
		data.modelCoreCacheMemorySize = 0;
		m_modelManager = LN_NEW detail::ModelManager();
		m_modelManager->initialize(data);
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeDocumentsManager()
{
	if (m_documentsManager == nullptr)
	{
		initializeCommon();
		initializeGraphicsManager();

		detail::DocumentsManager::ConfigData data;
		data.graphicsManager = m_graphicsManager;
		m_documentsManager = LN_NEW detail::DocumentsManager();
		m_documentsManager->initialize(data);
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeUIManager()
{
	if (m_uiManager == nullptr)
	{
		initializeCommon();
		initializeFileManager();
		initializeAnimationManager();
		initializePlatformManager();
		initializeGraphicsManager();
		initializeDocumentsManager();
		initializeAssetsManager();

		detail::UIManager::Settings data;
		data.fileManager = m_fileManager;
		data.animationManager = m_animationManager;
		data.platformManager = m_platformManager;
		data.graphicsManager = m_graphicsManager;
		data.assetsManager = m_assetsManager;
		data.documentsManager = m_documentsManager;
		data.defaultSkinFilePath = m_configData.defaultSkinFilePath;
		m_uiManager = LN_NEW detail::UIManager();
		m_uiManager->initialize(data);

	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeSceneGraphManager()
{
	if (m_sceneGraphManager == nullptr)
	{
		initializeCommon();
		initializeFileManager();
		initializeGraphicsManager();
		initializePhysicsManager();
		initializeEffectManager();
		initializeModelManager();
		initializeUIManager();
		initializeDocumentsManager();

		SceneGraphManager::ConfigData data;
		data.engineDiag = &EngineDiagCore::Instance;
		data.FileManager = m_fileManager;
		data.GraphicsManager = m_graphicsManager;
		data.PhysicsManager = m_physicsManager;
		data.effectManager = m_effectManager;
		data.modelManager = m_modelManager;
		data.documentsManager = m_documentsManager;
		m_sceneGraphManager = LN_NEW SceneGraphManager(data);
		m_sceneGraphManager->createDefaultSceneGraph();
		SceneGraphManager::Instance = m_sceneGraphManager;
	}
}

//------------------------------------------------------------------------------
void EngineManager::initializeAssetsManager()
{
	if (m_assetsManager == nullptr)
	{
		m_assetsManager = LN_NEW AssetsManager();
		m_assetsManager->initialize(this);
	}
}

//------------------------------------------------------------------------------
bool EngineManager::updateUnitily()
{
	updateFrame();
	//if (BeginRendering())
	{
		renderFrame();
		presentFrame();
	}
	return !isEndRequested();
}

//------------------------------------------------------------------------------
void EngineManager::updateFrame()
{
	m_fpsController.process();

	Profiler::Instance.setMainFPS(m_fpsController.getFps());
	Profiler::Instance.setMainFPSCapacity(m_fpsController.getCapacityFps());

	m_diagViewer->updateFrame();


	// フレームの始めで確定してみる
	Profiler2::commitFrame();

	ScopedProfilingSection2 section2(ProfilingKeys::Engine_UpdateFrame);


	//float deltaTime = m_fixedDeltaTime;
	//if (deltaTime == 0.0f)
	//{
	//	deltaTime = m_fpsController.getElapsedGameTime();
	//}

	// select runtime notification time source
	float deltaTime = 0.0f;
	if (m_frameUpdateMode == FrameUpdateMode::Fixed)
		deltaTime = 1.0f / m_fpsController.getFrameRate();
	else if (m_frameUpdateMode == FrameUpdateMode::VariableOnGameTime)
		deltaTime = m_fpsController.getElapsedGameTime();
	else if (m_frameUpdateMode == FrameUpdateMode::VariableOnRealTime)
		deltaTime = m_fpsController.getTotalRealTime();



	if (m_inputManager != nullptr) {
		m_inputManager->preUpdateFrame();
	}

	if (!m_platformManager->doEvents())
	{
		m_endRequested = true;
	}

	if (m_animationManager != nullptr)
	{
		m_animationManager->advanceTime(deltaTime);
	}

	if (m_inputManager != nullptr) {
		m_inputManager->updateFrame();
	}

	if (m_sceneGraphManager != nullptr) {
		m_sceneGraphManager->updateFrameDefaultSceneGraph(deltaTime);
	}

	if (m_defaultWorld2D != nullptr)
	{
		m_defaultWorld2D->reginUpdateFrame();
		m_defaultWorld2D->updateFrame(deltaTime);
	}
	if (m_defaultWorld3D != nullptr)
	{
		m_defaultWorld3D->reginUpdateFrame();
		m_defaultWorld3D->updateFrame(deltaTime);
	}


	if (m_uiManager != nullptr)
	{
		for (auto& window : m_uiManager->getWindows())
		{
			//window->injectElapsedTime(deltaTime);
			window->updateFrame();

			{	// プロファイリング範囲
				ScopedProfilerSection prof(Profiler::Group_MainThread, Profiler::Section_MainThread_GUILayput);
				//const SizeI& size = m_graphicsManager->getMainSwapChain()->getBackBuffer()->GetSize();
				//m_uiManager->getMainWindow()->updateLayout(Size(static_cast<float>(size.width), static_cast<float>(size.height)));

				// TODO: 内部に閉じ込める
				window->updateLayout(m_uiManager->getMainWindow()->getPlatformWindow()->getSize().toFloatSize());
			}
		}

	}
}

//------------------------------------------------------------------------------
//bool EngineManager::BeginRendering()
//{
//	m_frameRenderingSkip = true;
//	if (m_graphicsManager == nullptr || m_uiManager == nullptr) return nullptr;
//
//	// 描画遅延の確認
//	bool skip = false;
//	if (m_graphicsManager->getRenderingType() == GraphicsRenderingType::Threaded)
//	{
//		if (m_configData.delayedRenderingSkip &&
//			m_graphicsManager->getRenderingThread()->isRunning())
//		{
//			skip = true;
//		}
//	}
//	else {
//		// TODO:
//	}
//
//	if (skip)
//		return false;
//
//	m_frameRenderingSkip = false;
//
//
//
//	//if (m_effectManager != nullptr) {
//	//	m_effectManager->PreRender();	// Effekseer の更新スレッドを開始するのはここ
//	//	// TODO: これも UIMainWindow::BeginRendring の中かなぁ・・・
//	//}
//
//
//	m_frameRenderd = true;
//	return true;
//}

//------------------------------------------------------------------------------
void EngineManager::renderFrame()
{
	if (m_graphicsManager != nullptr)
	{
		ScopedProfilingSection2 section2(ProfilingKeys::Engine_RenderFrame);

		EngineDiagCore::Instance.resetGraphicsFrameReport();	// TODO: GameMode のみ？
		EngineDiagCore::Instance.resetVisualNodeDrawCount();	// TODO: GameMode のみ？


		for (auto& window : m_uiManager->getWindows())
		{
			window->renderContents();
		}
		//
		//if (m_uiManager != nullptr) {
		//	g->Clear(ClearFlags::Depth, ColorF::White);	// TODO
		//	g->Set2DRenderingMode(-1, 1);	// TODO
		//	m_uiManager->getMainWindow()->renderUI();
		//}

		if (m_diagViewer != nullptr)
		{
			//m_diagViewer->render(g, Vector2(640, 480));	//TODO
		}

		//g->PopState();
	}
}

//------------------------------------------------------------------------------
void EngineManager::endRendering()
{
}

//------------------------------------------------------------------------------
void EngineManager::presentFrame()
{
	if (m_graphicsManager == nullptr/* || m_frameRenderingSkip*/) return;

	for (auto& window : m_uiManager->getWindows())
	{
		ScopedProfilingSection2 section2(ProfilingKeys::Engine_PresentFrame);
		window->presentRenderingContexts();
	}
}

//------------------------------------------------------------------------------
void EngineManager::resetFrameDelay()
{
	m_fpsController.refreshSystemDelay();
}

//------------------------------------------------------------------------------
void EngineManager::exit()
{
	m_endRequested = true;
}

//------------------------------------------------------------------------------
World2D* EngineManager::getDefaultWorld2D() const
{
	return m_defaultWorld2D;
}

//------------------------------------------------------------------------------
World3D* EngineManager::getDefaultWorld3D() const
{
	return m_defaultWorld3D;
}

//------------------------------------------------------------------------------
detail::PhysicsManager* EngineManager::getPhysicsManager() const
{
	return m_physicsManager;
}

//------------------------------------------------------------------------------
bool EngineManager::onEvent(const PlatformEventArgs& e)
{
	UILayoutView* uiView = nullptr;
	if (m_uiManager != nullptr)
	{
		uiView = m_uiManager->getMainWindow();
	}


	switch (e.type)
	{
	case PlatformEventType::Quit:	// アプリ終了要求
	case PlatformEventType::close:	// ウィンドウが閉じられようとしている
		break;

	case PlatformEventType::MouseDown:		// マウスボタンが押された
		if (uiView != nullptr)
		{
			if (uiView->injectMouseButtonDown(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseUp:			// マウスボタンが離された
		if (uiView != nullptr)
		{
			if (uiView->injectMouseButtonUp(e.mouse.button, e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseMove:		// マウスが移動した
		if (uiView != nullptr)
		{
			if (uiView->injectMouseMove(e.mouse.x, e.mouse.y)) { return true; }
		}
		break;
	case PlatformEventType::MouseWheel:		// マウスホイールが操作された
		if (uiView != nullptr)
		{
			if (uiView->injectMouseWheel(e.wheel.delta)) { return true; }
		}
		break;
	case PlatformEventType::KeyDown:
		if (uiView != nullptr)
		{
			if (uiView->injectKeyDown(e.key.keyCode, e.key.modifierKeys)) { return true; }
		}

		//// デバッグ表示切替
		//if (m_configData.acceleratorKeys.toggleShowDiag != nullptr &&
		//	m_configData.acceleratorKeys.toggleShowDiag->EqualKeyInput(e.key.keyCode, e.key.modifierKeys) &&
		//	m_diagViewer != nullptr)
		//{
		//	m_diagViewer->toggleDisplayMode();
		//}
		break;
	case PlatformEventType::KeyUp:
		if (uiView != nullptr)
		{
			if (uiView->injectKeyUp(e.key.keyCode, e.key.modifierKeys/*, e.Key.keyChar*/)) { return true; }
		}
		break;
	case PlatformEventType::KeyChar:
		if (uiView != nullptr)
		{
			if (uiView->injectTextInput(e.key.keyChar)) { return true; }
		}
		break;
	case PlatformEventType::WindowSizeChanged:
		//if (m_graphicsManager != nullptr)
		//{
		//	m_graphicsManager->getMainSwapChain()->Resize(SizeI(e.size.width, e.size.height));
		//}
		//if (uiView != nullptr)
		//{
		//	if (uiView->InjectViewportSizeChanged(e.size.width, e.size.height)) { return true; }
		//}
		break;
	default:
		break;
	}

	if (m_inputManager != nullptr) {
		m_inputManager->onEvent(e);
	}
	return false;
}

////------------------------------------------------------------------------------
//void EngineManager::onLostDevice()
//{
//	if (m_effectManager != nullptr)
//	{
//		m_effectManager->onLostDevice();
//	}
//}
//
////------------------------------------------------------------------------------
//void EngineManager::onResetDevice()
//{
//	if (m_effectManager != nullptr)
//	{
//		m_effectManager->onResetDevice();
//	}
//}



//==============================================================================
// EngineDomain
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
PhysicsWorld* EngineDomain::getPhysicsWorld3D()
{
	return EngineManager::getInstance()->getDefaultWorld3D()->getPhysicsWorld3D();
}

//------------------------------------------------------------------------------
EngineManager* EngineDomain::getEngineManager()
{
	return EngineManager::getInstance();
}

//------------------------------------------------------------------------------
AnimationManager* EngineDomain::getAnimationManager()
{
	return EngineManager::getInstance()->getAnimationManager();
}

//------------------------------------------------------------------------------
GraphicsManager* EngineDomain::getGraphicsManager()
{
	return EngineManager::getInstance()->getGraphicsManager();
}

//------------------------------------------------------------------------------
ModelManager* EngineDomain::getModelManager()
{
	return EngineManager::getInstance()->getModelManager();
}

//------------------------------------------------------------------------------
UIManager* EngineDomain::getUIManager()
{
	return EngineManager::getInstance()->getUIManager();
}

//------------------------------------------------------------------------------
SceneGraphManager* EngineDomain::getSceneGraphManager()
{
	return EngineManager::getInstance()->getSceneGraphManager();
}

//------------------------------------------------------------------------------
SceneGraph2D* EngineDomain::getDefaultSceneGraph2D()
{
	return EngineManager::getInstance()->getDefaultWorld2D()->getSceneGraph2D();
}

//------------------------------------------------------------------------------
SceneGraph3D* EngineDomain::getDefaultSceneGraph3D()
{
	return EngineManager::getInstance()->getDefaultWorld3D()->getSceneGraph3D();
}

//------------------------------------------------------------------------------
World2D* EngineDomain::getDefaultWorld2D()
{
	return EngineManager::getInstance()->getDefaultWorld2D();
}

//------------------------------------------------------------------------------
World3D* EngineDomain::getDefaultWorld3D()
{
	return EngineManager::getInstance()->getDefaultWorld3D();
}

} // namespace detail

LN_NAMESPACE_END
