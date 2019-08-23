
#include "Internal.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include <LuminoEngine/Scene/Light.hpp>
#include "../Graphics/RenderTargetTextureCache.hpp"

#include "../Platform/PlatformManager.hpp"
#include "../Animation/AnimationManager.hpp"
#include "../Input/InputManager.hpp"
#include "../Audio/AudioManager.hpp"
#include "../Shader/ShaderManager.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Font/FontManager.hpp"
#include "../Mesh/MeshManager.hpp"
#include "../Rendering/RenderingManager.hpp"
#include "../Effect/EffectManager.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../Asset/AssetManager.hpp"
#include "../Visual/VisualManager.hpp"
#include "../Scene/SceneManager.hpp"
#include "../UI/UIManager.hpp"
#include "../Runtime/RuntimeManager.hpp"
#include "EngineManager.hpp"
#include "EngineDomain.hpp"

namespace ln {
namespace detail {

//==============================================================================
// EngineSettings

//void EngineSettings::setMainWindowSize(int width, int height)
//{
//	detail::EngineSettings::instance.mainWindowSize.set(width, height);
//}
//
//void EngineSettings::setMainBackBufferSize(int width, int height)
//{
//	detail::EngineSettings::instance.mainBackBufferSize.set(width, height);
//}
//
//void EngineSettings::setMainWindowTitle(const StringRef& title)
//{
//	detail::EngineSettings::instance.mainWindowTitle = title;
//}

//==============================================================================
// EngineManager

EngineManager::EngineManager()
	: m_settings()
	, m_platformManager(nullptr)
	//, m_animationManager(nullptr)
	//, m_inputManager(nullptr)
	, m_audioManager(nullptr)
	//, m_physicsManager(nullptr)
	//, m_graphicsManager(nullptr)
	//, m_effectManager(nullptr)
	//, m_modelManager(nullptr)
	//, m_uiManager(nullptr)
	, m_assetManager(nullptr)
    //, m_application(nullptr)
    , m_timeScale(1.0f)
	, m_exitRequested(false)
    , m_showDebugFpsEnabled(false)
#if defined(LN_OS_WIN32)
    , m_comInitialized(false)
    , m_oleInitialized(false)
#endif
{
}

EngineManager::~EngineManager()
{
}

void EngineManager::init()
{
    LN_LOG_DEBUG << "EngineManager Initialization started.";

	// check settings
	{
		if (m_settings.bundleIdentifier.isEmpty()) {
			m_settings.bundleIdentifier = u"lumino";
		}

		if (m_settings.bundleIdentifier.contains(u"lumino", CaseSensitivity::CaseInsensitive)) {
			LN_WARNING("Bundle Identifier It may not be set.");
		}
	}

	// setup pathes
	{
#if defined(LN_OS_WIN32)
        m_persistentDataPath = Path(Environment::specialFolderPath(SpecialFolder::ApplicationData), m_settings.bundleIdentifier);
#else
        m_persistentDataPath = u""; // TODO:
#endif
    }
	
	m_engineContext = makeRef<EngineContext>();


	initializeAllManagers();

    //if (!m_settings.externalRenderingManagement) {
    //    m_graphicsManager->enterRendering();
    //}

	m_fpsController.setFrameRate(m_settings.frameRate);
	m_fpsController.setEnableFpsTest(true);

    if (m_settings.defaultObjectsCreation)
    {
        if (m_uiManager) {

			setMainWindow(makeObject<UIMainWindow>());

            m_mainViewport = makeObject<UIViewport>();
            m_mainWindow->addElement(m_mainViewport);
        }

        if (m_sceneManager)
        {
            m_mainWorld = makeObject<World>();
            m_sceneManager->setActiveWorld(m_mainWorld);

            //m_mainAmbientLight = makeObject<AmbientLight>();
            //m_mainDirectionalLight = makeObject<DirectionalLight>();

            m_mainCamera = makeObject<Camera>();
            m_mainWorldRenderView = makeObject<WorldRenderView>();
            m_mainWorldRenderView->setTargetWorld(m_mainWorld);
            m_mainWorldRenderView->setCamera(m_mainCamera);
            m_mainWorldRenderView->setClearMode(RenderViewClearMode::ColorAndDepth);
            m_mainViewport->addRenderView(m_mainWorldRenderView);


            m_mainUIRenderView = makeObject<UIRenderView>();
            m_mainViewport->addRenderView(m_mainUIRenderView);

            m_mainUIRoot = makeObject<UIContainerElement>();
            m_mainUIRoot->setHorizontalAlignment(HAlignment::Stretch);
            m_mainUIRoot->setVerticalAlignment(VAlignment::Stretch);
			m_mainUIRoot->m_isHitTestVisible = false;
            m_mainUIRenderView->setRootElement(m_mainUIRoot);
            m_uiManager->setPrimaryElement(m_mainUIRoot);

            m_mainPhysicsWorld = m_mainWorld->physicsWorld();
            m_mainPhysicsWorld2D = m_mainWorld->physicsWorld2D();

            m_physicsManager->setActivePhysicsWorld2D(m_mainPhysicsWorld2D);
        }
    }


    LN_LOG_DEBUG << "EngineManager Initialization ended.";
}

void EngineManager::dispose()
{
    if (m_uiManager) {
        m_uiManager->setPrimaryElement(nullptr);
        m_uiManager->setMainContext(nullptr);
    }

	if (m_platformManager) {
		m_platformManager->mainWindow()->detachEventListener(this);
	}

    m_mainPhysicsWorld = nullptr;
    m_mainPhysicsWorld2D = nullptr;


    // Main world contents.
    {
        if (m_mainWorldRenderView) {
            m_mainWorldRenderView->dispose();
            m_mainWorldRenderView = nullptr;
        }
        if (m_mainAmbientLight) {
            m_mainAmbientLight->dispose();
            m_mainAmbientLight = nullptr;
        }
        if (m_mainDirectionalLight) {
            m_mainDirectionalLight->dispose();
            m_mainDirectionalLight = nullptr;
        }
        if (m_mainCamera) {
            m_mainCamera->dispose();
            m_mainCamera = nullptr;
        }
        if (m_mainWorld) {
            m_mainWorld->dispose();
            m_mainWorld = nullptr;
        }
    }

    // Main UI contents.
    {
        if (m_mainUIRoot) {
            m_mainUIRoot->dispose();
            m_mainUIRoot = nullptr;
        }
        if (m_mainUIRenderView) {
            m_mainUIRenderView->dispose();
            m_mainUIRenderView = nullptr;
        }
        if (m_mainViewport) {
            m_mainViewport->dispose();
            m_mainViewport = nullptr;
        }
        if (m_mainWindow) {
            m_mainWindow->dispose();
            m_mainWindow = nullptr;
        }
        if (m_mainUIContext) {
            m_mainUIContext->dispose();
            m_mainUIContext = nullptr;
        }
    }

    //if (!m_settings.externalRenderingManagement) {
    //    if (m_graphicsManager) {
    //        m_graphicsManager->leaveRendering();
    //    }
    //}

	if (m_runtimeManager) m_runtimeManager->dispose();
    if (m_uiManager) m_uiManager->dispose();
    if (m_sceneManager) m_sceneManager->dispose();
    if (m_visualManager) m_visualManager->dispose();
	if (m_assetManager) m_assetManager->dispose();
    if (m_physicsManager) m_physicsManager->dispose();
    if (m_effectManager) m_effectManager->dispose();
	if (m_renderingManager) m_renderingManager->dispose();
	if (m_meshManager) m_meshManager->dispose();
	if (m_fontManager) m_fontManager->dispose();
	if (m_shaderManager) m_shaderManager->dispose();
	if (m_graphicsManager) m_graphicsManager->dispose();
	if (m_audioManager) m_audioManager->dispose();
	if (m_inputManager) m_inputManager->dispose();
    if (m_animationManager) m_animationManager->dispose();
	if (m_platformManager) m_platformManager->dispose();

#if defined(LN_OS_WIN32)
    if (m_oleInitialized) {
        ::OleUninitialize();
        m_oleInitialized = false;
    }

    if (m_comInitialized) {
        ::CoUninitialize();
        m_comInitialized = false;
    }
#endif
}

void EngineManager::initializeAllManagers()
{
	initializeCommon();
	initializePlatformManager();
	initializeAnimationManager();
	initializeInputManager();
	initializeAudioManager();
	initializeShaderManager();
	initializeGraphicsManager();
	initializeFontManager();
	initializeMeshManager();
    initializePhysicsManager();
	initializeAssetManager();
	initializeRenderingManager();
    initializeEffectManager();
    initializeVisualManager();
    initializeSceneManager();
	initializeUIManager();
	initializeRuntimeManager();
}

void EngineManager::initializeCommon()
{
#if defined(LN_OS_WIN32) || defined(LN_OS_MAC)
	auto log = Path(Path(Environment::executablePath()).parent(), u"lumino.log");
	GlobalLogger::addFileAdapter(log.str().toStdString());
#endif
#if defined(LN_OS_WIN32)

    // CoInitializeEx は ShowWindow() ～ DestroyWindow() の外側で呼び出さなければならない。
    // http://blog.techlab-xe.net/archives/400
    // 例えば ウィンドウ作成→DirectInput初期化みたいにするとき、Input モジュールの中で CoInitializeEx しているとこの罠にはまる。
    // とりあえず、Platform モジュールでは COM は使わないが、他のモジュールとの連携に備え、初期化しておく。

    if (m_settings.autoCoInitialize && SUCCEEDED(::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
    {
        // エラーにはしない。別の設定で COM が初期化済みだったりすると失敗することがあるが、COM 自体は使えるようになっている
        m_comInitialized = true;
    }

    // OleInitialize() するためには、CoInitializeEx() が STA(COINIT_APARTMENTTHREADED) で初期化されている必要がある
    if (SUCCEEDED(::OleInitialize(NULL))) {
        m_oleInitialized = true;
    }
#endif
}

void EngineManager::initializePlatformManager()
{
	if (!m_platformManager && m_settings.features.hasFlag(EngineFeature::Application))
	{
		initializeCommon();

		PlatformManager::Settings settings;
        settings.useGLFWWindowSystem = m_settings.useGLFWWindowSystem;
		settings.mainWindowSettings.title = m_settings.mainWindowTitle;
		settings.mainWindowSettings.clientSize = m_settings.mainWindowSize;
		settings.mainWindowSettings.fullscreen = false;
		settings.mainWindowSettings.resizable = true;
		settings.mainWindowSettings.userWindow = m_settings.userMainWindow;

		m_platformManager = ln::makeRef<PlatformManager>();
		m_platformManager->init(settings);

		m_platformManager->mainWindow()->attachEventListener(this);
	}
}

void EngineManager::initializeAnimationManager()
{
    if (!m_animationManager && m_settings.features.hasFlag(EngineFeature::Rendering))
    {
        AnimationManager::Settings settings;
        m_animationManager = ln::makeRef<AnimationManager>();
        m_animationManager->init(settings);
    }
}

void EngineManager::initializeInputManager()
{
	if (!m_inputManager && m_settings.features.hasFlag(EngineFeature::Input))
	{
		initializePlatformManager();

		InputManager::Settings settings;
		settings.mainWindow = m_platformManager->mainWindow();
		m_inputManager = ln::makeRef<InputManager>();
		m_inputManager->init(settings);
	}
}

void EngineManager::initializeAudioManager()
{
	if (!m_audioManager && m_settings.features.hasFlag(EngineFeature::Audio))
	{
		initializeCommon();
        initializeAssetManager();

		AudioManager::Settings settings;
        settings.assetManager = m_assetManager;

		m_audioManager = ln::makeRef<AudioManager>();
		m_audioManager->init(settings);
	}
}

void EngineManager::initializeShaderManager()
{
	if (!m_shaderManager && m_settings.features.hasFlag(EngineFeature::Graphics))
	{
		initializeGraphicsManager();

		ShaderManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;

		m_shaderManager = ln::makeRef<ShaderManager>();
		m_shaderManager->init(settings);
	}
}

void EngineManager::initializeGraphicsManager()
{
	if (!m_graphicsManager && m_settings.features.hasFlag(EngineFeature::Graphics))
	{
		initializePlatformManager();

		GraphicsManager::Settings settings;
		settings.mainWindow = (m_settings.graphicsContextManagement) ? m_platformManager->mainWindow() : nullptr;
		settings.graphicsAPI = m_settings.graphicsAPI;

		m_graphicsManager = ln::makeRef<GraphicsManager>();
		m_graphicsManager->init(settings);
	}
}

void EngineManager::initializeFontManager()
{
	if (!m_fontManager && m_settings.features.hasFlag(EngineFeature::Graphics))
	{
		initializeAssetManager();

		FontManager::Settings settings;
		settings.assetManager = m_assetManager;

		m_fontManager = ln::makeRef<FontManager>();
		m_fontManager->init(settings);
	}
}

void EngineManager::initializeMeshManager()
{
	if (!m_meshManager && m_settings.features.hasFlag(EngineFeature::Rendering))
	{
		initializeGraphicsManager();
		initializeAssetManager();

		MeshManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;
		settings.assetManager = m_assetManager;

		m_meshManager = ln::makeRef<MeshManager>();
		m_meshManager->init(settings);
	}
}

void EngineManager::initializeRenderingManager()
{
	if (!m_renderingManager && m_settings.features.hasFlag(EngineFeature::Rendering))
	{
		initializeGraphicsManager();

		RenderingManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;
        settings.fontManager = m_fontManager;

		m_renderingManager = ln::makeRef<RenderingManager>();
		m_renderingManager->init(settings);
	}
}

void EngineManager::initializeEffectManager()
{
    if (!m_effectManager && m_settings.features.hasFlag(EngineFeature::Rendering))
    {
        EffectManager::Settings settings;

        m_effectManager = ln::makeRef<EffectManager>();
        m_effectManager->init(settings);
    }
}

void EngineManager::initializePhysicsManager()
{
    if (!m_physicsManager && m_settings.features.hasFlag(EngineFeature::Rendering))
    {
        PhysicsManager::Settings settings;

        m_physicsManager = ln::makeRef<PhysicsManager>();
        m_physicsManager->init(settings);
    }
}

void EngineManager::initializeAssetManager()
{
    if (!m_assetManager && m_settings.features.hasFlag(EngineFeature::Application))
    {
        AssetManager::Settings settings;

        m_assetManager = ln::makeRef<AssetManager>();
        m_assetManager->init(settings);
        m_assetManager->setAssetStorageAccessPriority(m_settings.assetStorageAccessPriority);

        for (auto& e : m_settings.assetArchives) {
            m_assetManager->addAssetArchive(e.filePath, e.password);
        }
		for (auto& e : m_settings.assetDirectories) {
			m_assetManager->addAssetDirectory(e);
		}
    }
}

void EngineManager::initializeVisualManager()
{
    if (!m_visualManager && m_settings.features.hasFlag(EngineFeature::Rendering))
    {
        initializeGraphicsManager();

        VisualManager::Settings settings;
        settings.graphicsManager = m_graphicsManager;

        m_visualManager = ln::makeRef<VisualManager>();
        m_visualManager->init(settings);
    }
}

void EngineManager::initializeSceneManager()
{
    if (!m_sceneManager && m_settings.features.hasFlag(EngineFeature::Rendering))
    {
        initializeAnimationManager();

        m_sceneManager = ln::makeRef<SceneManager>();
        m_sceneManager->init();

        m_animationManager->setSceneManager(m_sceneManager);
    }
}

void EngineManager::initializeUIManager()
{
	if (!m_uiManager && m_settings.features.hasFlag(EngineFeature::Application))
	{
		initializeGraphicsManager();

		UIManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;
        settings.application = m_settings.application;
		
		m_uiManager = makeRef<UIManager>();
		m_uiManager->init(settings);

        m_mainUIContext = makeObject<UIContext>();
        m_uiManager->setMainContext(m_mainUIContext);
	}
}

void EngineManager::initializeRuntimeManager()
{
	if (!m_runtimeManager)
	{
		RuntimeManager::Settings settings;

		m_runtimeManager = makeRef<RuntimeManager>();
		m_runtimeManager->init(settings);
	}
}

bool EngineManager::updateUnitily()
{
	updateFrame();
	renderFrame();
	presentFrame();
	return !isExitRequested();
}

void EngineManager::updateFrame()
{
    float elapsedSeconds = 0.016 * m_timeScale; // TODO: time

    //------------------------------------------------
    // Pre update phase

	if (m_inputManager) {
		m_inputManager->preUpdateFrame();
	}

	if (m_platformManager) {
		m_platformManager->windowManager()->processSystemEventQueue(EventProcessingMode::Polling);
	}
	if (m_uiManager) {
		m_uiManager->dispatchPostedEvents();
	}

	if (m_mainUIContext) {
		// onUpdate のユーザー処理として、2D <-> 3D 変換したいことがあるが、それには ViewPixelSize が必要になる。
		// 初期化直後や、Platform からの SizeChanged イベントの直後に一度レイアウトを更新することで、
		// ユーザー処理の前に正しい ViewPixelSize を計算しておく。
		m_mainUIContext->updateStyleTree();
		//m_mainUIContext->updateLayoutTree();
		m_mainWindow->updateLayoutTree();
	}

    //------------------------------------------------
    // Main update phase

    if (m_mainWindow) {
        m_mainWindow->updateFrame(elapsedSeconds);
    }

	// いくつかの入力状態は onEvent 経由で Platform モジュールから Input モジュールに伝えられる。
	// このときはまだ押されているかどうかだけを覚えておく。
	// 次に InputManager::updateFrame で、現在時間を考慮して各種状態を更新する。
	if (m_inputManager) {
		m_inputManager->updateFrame(elapsedSeconds);
	}

	if (m_audioManager) {
		m_audioManager->update(elapsedSeconds);
	}

    if (m_mainWorld) {
        m_mainWorld->updateFrame(elapsedSeconds);
    }

    if (m_sceneManager) {
        m_sceneManager->updateFrame();
    }

    //------------------------------------------------
    // Post update phase


}

void EngineManager::renderFrame()
{
    //if (m_renderingManager) {
    //    m_renderingManager->frameBufferCache()->beginRenderSection();
    //}

	if (m_mainWindow) {
		m_mainWindow->renderContents();
	}

    //// DrawList 構築
    //if (m_mainWorld) {
    //    m_mainWorld->render();
    //}

    //// DrawList を実際に描画
    //if (m_mainWorldRenderView) {
    //    m_mainWorldRenderView->render(m_graphicsManager->graphicsContext());
    //}
}

void EngineManager::presentFrame()
{
    //m_effectManager->testDraw();

	if (m_mainWindow) {
		m_mainWindow->present();
	}

    //if (m_renderingManager) {
    //    m_renderingManager->frameBufferCache()->endRenderSection();
    //}


	if (m_settings.standaloneFpsControl) {
		m_fpsController.process();
	}
	else {
		m_fpsController.processForMeasure();
	}

    if (m_showDebugFpsEnabled) {
        m_platformManager->mainWindow()->setWindowTitle(String::format(u"FPS:{0:F1}({1:F1})", m_fpsController.getFps(), m_fpsController.getCapacityFps()));
    }
}

void EngineManager::resetFrameDelay()
{
	m_fpsController.refreshSystemDelay();
}

void EngineManager::quit()
{
	m_exitRequested = true;
}

const Path& EngineManager::persistentDataPath() const
{
#if !defined(LN_OS_WIN32)
    LN_NOTIMPLEMENTED();
#endif
    return m_persistentDataPath;
}

void EngineManager::setMainWindow(ln::UIMainWindow* window)
{
	if (LN_REQUIRE(window)) return;
	if (LN_REQUIRE(!m_mainWindow)) return;
	m_mainWindow = window;
	m_mainWindow->setupPlatformWindow(m_platformManager->mainWindow(), m_settings.mainBackBufferSize);
	m_mainUIContext->setLayoutRootElement(m_mainWindow);

	// TODO: SwapChain だけでいいはず
	m_mainWindow->m_graphicsContext = m_graphicsManager->mainWindowGraphicsContext();
}

bool EngineManager::onPlatformEvent(const PlatformEventArgs& e)
{
	switch (e.type)
	{
	case PlatformEventType::close:
		quit();
		break;
	default:
		break;
	}


	// 他のモジュールにイベントを渡した後、まだ処理されていなければ input へ回す
	if (m_inputManager) {
		m_inputManager->onEvent(e);
	}

	return false;
}

//==============================================================================
// EngineDomain

static EngineManager* g_engineManager = nullptr;

void EngineDomain::release()
{
	if (g_engineManager)
	{
		g_engineManager->dispose();
		RefObjectHelper::release(g_engineManager);
		g_engineManager = nullptr;
	}
}

EngineManager* EngineDomain::engineManager()
{
	if (!g_engineManager)
	{
		g_engineManager = LN_NEW EngineManager();
	}

	return g_engineManager;
}

PlatformManager* EngineDomain::platformManager()
{
	return engineManager()->platformManager();
}

AnimationManager* EngineDomain::animationManager()
{
    return engineManager()->animationManager();
}

InputManager* EngineDomain::inputManager()
{
	return engineManager()->inputManager();
}

AudioManager * EngineDomain::audioManager()
{
	return engineManager()->audioManager();
}

ShaderManager* EngineDomain::shaderManager()
{
	return engineManager()->shaderManager();
}

GraphicsManager* EngineDomain::graphicsManager()
{
	return engineManager()->graphicsManager();
}

FontManager* EngineDomain::fontManager()
{
	return engineManager()->fontManager();
}

MeshManager* EngineDomain::meshManager()
{
	return engineManager()->meshManager();
}

RenderingManager* EngineDomain::renderingManager()
{
	return engineManager()->renderingManager();
}

EffectManager* EngineDomain::effectManager()
{
    return engineManager()->effectManager();
}

PhysicsManager* EngineDomain::physicsManager()
{
    return engineManager()->physicsManager();
}

AssetManager* EngineDomain::assetManager()
{
    return engineManager()->assetManager();
}

VisualManager* EngineDomain::visualManager()
{
    return engineManager()->visualManager();
}

SceneManager* EngineDomain::sceneManager()
{
    return engineManager()->sceneManager();
}

UIManager* EngineDomain::uiManager()
{
	return engineManager()->uiManager();
}

RuntimeManager* EngineDomain::runtimeManager()
{
	return engineManager()->runtimeManager();
}

EngineContext* EngineDomain::engineContext()
{
    return engineManager()->engineContext();
}

} // namespace detail
} // namespace ln

