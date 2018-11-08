
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include "../Rendering/RenderTargetTextureCache.hpp"

#include "../Platform/PlatformManager.hpp"
#include "../Input/InputManager.hpp"
#include "../Audio/AudioManager.hpp"
#include "../Shader/ShaderManager.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Font/FontManager.hpp"
#include "../Mesh/MeshManager.hpp"
#include "../Rendering/RenderingManager.hpp"
#include "../Asset/AssetManager.hpp"
#include "../Visual/VisualManager.hpp"
#include "../Scene/SceneManager.hpp"
#include "../UI/UIManager.hpp"
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
	, m_exitRequested(false)
{
}

EngineManager::~EngineManager()
{
}

void EngineManager::initialize()
{
	initializeAllManagers();

	if (m_uiManager) {
		m_mainWindow = newObject<UIFrameWindow>(m_platformManager->mainWindow(), m_settings.mainBackBufferSize);
		m_mainViewport = newObject<UIViewport>();
		m_mainWindow->m_viewport = m_mainViewport;
	}

    m_mainWorld = newObject<World>();
    m_sceneManager->setActiveWorld(m_mainWorld);
	m_mainCamera = newObject<Camera>();
    m_mainWorldRenderView = newObject<WorldRenderView>();
    m_mainWorldRenderView->setTargetWorld(m_mainWorld);
	m_mainWorldRenderView->setCamera(m_mainCamera);
    m_mainViewport->addRenderView(m_mainWorldRenderView);


    m_mainUIRenderView = newObject<UIRenderView>();
    m_mainViewport->addRenderView(m_mainUIRenderView);

    m_mainUIRoot = newObject<UIContainerElement>();
    m_mainUIRenderView->setRootElement(m_mainUIRoot);
    m_uiManager->setPrimaryElement(m_mainUIRoot);
}

void EngineManager::dispose()
{
	if (m_platformManager) {
		m_platformManager->mainWindow()->detachEventListener(this);
	}

	if (m_mainWindow) m_mainWindow->dispose();
    if (m_sceneManager) m_sceneManager->dispose();
    if (m_visualManager) m_visualManager->dispose();
	if (m_assetManager) m_assetManager->dispose();
	if (m_renderingManager) m_renderingManager->dispose();
	if (m_meshManager) m_meshManager->dispose();
	if (m_fontManager) m_fontManager->dispose();
	if (m_shaderManager) m_shaderManager->dispose();
	if (m_graphicsManager) m_graphicsManager->dispose();
	if (m_audioManager) m_audioManager->dispose();
	if (m_inputManager) m_inputManager->dispose();
	if (m_platformManager) m_platformManager->dispose();
}

void EngineManager::initializeAllManagers()
{
	initializeCommon();
	initializePlatformManager();
	initializeAnimationManager();
	initializeInputManager();
	initializeAudioManager();
	initializePhysicsManager();
	initializeShaderManager();
	initializeGraphicsManager();
	initializeFontManager();
	initializeMeshManager();
	initializeAssetManager();
	initializeRenderingManager();
    initializeVisualManager();
    initializeSceneManager();
	initializeUIManager();
}

void EngineManager::initializeCommon()
{
#if defined(LN_OS_WIN32) || defined(LN_OS_MAC)
	auto log = Path(Path(Environment::executablePath()).parent(), u"lumino.log");
	GlobalLogger::addFileAdapter(log.str().toStdString());
#endif
}

void EngineManager::initializePlatformManager()
{
	if (!m_platformManager)
	{
		initializeCommon();

		PlatformManager::Settings settings;
		settings.mainWindowSettings.title = m_settings.mainWindowTitle;
		settings.mainWindowSettings.clientSize = m_settings.mainWindowSize;
		settings.mainWindowSettings.fullscreen = false;
		settings.mainWindowSettings.resizable = true;
		//settings.mainWindowSettings.userWindow = m_settings.userMainWindow;

		m_platformManager = ln::makeRef<PlatformManager>();
		m_platformManager->initialize(settings);

		m_platformManager->mainWindow()->attachEventListener(this);
	}
}

void EngineManager::initializeAnimationManager()
{
}

void EngineManager::initializeInputManager()
{
	if (!m_inputManager)
	{
		initializePlatformManager();

		InputManager::Settings settings;
		settings.mainWindow = m_platformManager->mainWindow();
		m_inputManager = ln::makeRef<InputManager>();
		m_inputManager->initialize(settings);
	}
}

void EngineManager::initializeAudioManager()
{
	if (!m_audioManager)
	{
		initializeCommon();

		AudioManager::Settings settings;
		m_audioManager = ln::makeRef<AudioManager>();
		m_audioManager->initialize(settings);
	}
}

void EngineManager::initializePhysicsManager()
{
}

void EngineManager::initializeShaderManager()
{
	if (!m_shaderManager)
	{
		initializeGraphicsManager();

		ShaderManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;

		m_shaderManager = ln::makeRef<ShaderManager>();
		m_shaderManager->initialize(settings);
	}
}

void EngineManager::initializeGraphicsManager()
{
	if (!m_graphicsManager)
	{
		initializePlatformManager();

		GraphicsManager::Settings settings;
		settings.mainWindow = m_platformManager->mainWindow();

		m_graphicsManager = ln::makeRef<GraphicsManager>();
		m_graphicsManager->initialize(settings);
	}
}

void EngineManager::initializeFontManager()
{
	if (!m_fontManager)
	{
		initializeAssetManager();

		FontManager::Settings settings;
		settings.assetManager = m_assetManager;

		m_fontManager = ln::makeRef<FontManager>();
		m_fontManager->initialize(settings);
	}
}

void EngineManager::initializeMeshManager()
{
	if (!m_meshManager)
	{
		initializeGraphicsManager();
		initializeAssetManager();

		MeshManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;
		settings.assetManager = m_assetManager;

		m_meshManager = ln::makeRef<MeshManager>();
		m_meshManager->initialize(settings);
	}
}

void EngineManager::initializeRenderingManager()
{
	if (!m_renderingManager)
	{
		initializeGraphicsManager();

		RenderingManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;

		m_renderingManager = ln::makeRef<RenderingManager>();
		m_renderingManager->initialize(settings);
	}
}

void EngineManager::initializeAssetManager()
{
    if (!m_assetManager)
    {
        AssetManager::Settings settings;

        m_assetManager = ln::makeRef<AssetManager>();
        m_assetManager->initialize(settings);
    }
}

void EngineManager::initializeVisualManager()
{
    if (!m_visualManager)
    {
        initializeGraphicsManager();

        VisualManager::Settings settings;
        settings.graphicsManager = m_graphicsManager;

        m_visualManager = ln::makeRef<VisualManager>();
        m_visualManager->initialize(settings);
    }
}

void EngineManager::initializeSceneManager()
{
    if (!m_sceneManager)
    {
        m_sceneManager = ln::makeRef<SceneManager>();
        m_sceneManager->initialize();
    }
}

void EngineManager::initializeUIManager()
{
	if (!m_uiManager)
	{
		initializeGraphicsManager();

		UIManager::Settings settings;
		settings.graphicsManager = m_graphicsManager;
		
		m_uiManager = makeRef<UIManager>();
		m_uiManager->initialize(settings);
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
	if (m_inputManager) {
		m_inputManager->preUpdateFrame();
	}

	if (m_platformManager) {
		m_platformManager->windowManager()->processSystemEventQueue();
	}

	// いくつかの入力状態は onEvent 経由で Platform モジュールから Input モジュールに伝えられる。
	// このときはまだ押されているかどうかだけを覚えておく。
	// 次に InputManager::updateFrame で、現在時間を考慮して各種状態を更新する。
	if (m_inputManager) {
		m_inputManager->updateFrame(0.016);	// TODO: time
	}

	if (m_audioManager) {
		m_audioManager->update();
	}

    if (m_mainWorld) {
        m_mainWorld->updateFrame(0.016);	// TODO: time
    }

    //if (m_mainWindow) {
    //    m_mainWindow->
    //}
}

void EngineManager::renderFrame()
{
    if (m_renderingManager) {
        m_renderingManager->frameBufferCache()->beginRenderSection();
    }

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
	if (m_mainWindow) {
		m_mainWindow->present();
	}

    if (m_renderingManager) {
        m_renderingManager->frameBufferCache()->endRenderSection();
    }

    // TODO:
	Thread::sleep(1);
}

void EngineManager::resetFrameDelay()
{
}

void EngineManager::exit()
{
	m_exitRequested = true;
}

bool EngineManager::onPlatformEvent(const PlatformEventArgs& e)
{
	switch (e.type)
	{
	case PlatformEventType::close:
		exit();
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

} // namespace detail
} // namespace ln

