#pragma once
#include <LuminoEngine/Engine/Common.hpp>
#include <LuminoEngine/Asset/Common.hpp>
#include <LuminoEngine/Graphics/Common.hpp>
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include <LuminoEngine/Platform/PlatformEvent.hpp>
#include "FpsController.hpp"

namespace ln {
class DiagnosticsManager;
class EngineContext;
class Application;
class UIContext;
class UIMainWindow;
class UIViewport;
class UIRenderView;
class UIControl;
class PhysicsWorld;
class PhysicsWorld2D;
class World;
class Scene;
class WorldRenderView;
class Camera;
class AmbientLight;
class DirectionalLight;
class UIEventArgs;
class Dispatcher;

namespace detail {
class PlatformManager;
class AnimationManager;
class InputManager;
class AudioManager;
class ShaderManager;
class GraphicsManager;
class FontManager;
class MeshManager;
class RenderingManager;
class EffectManager;
class PhysicsManager;
class AssetManager;
class VisualManager;
class SceneManager;
class UIManager;

struct EngineSettingsAssetArchiveEntry
{
    Path filePath;
    String password;    // TODO: 平文保持はよくない。MD5
};

struct EngineSettings
{
    Application* application = nullptr;
    Flags<EngineFeature> features = EngineFeature::Experimental;
	String bundleIdentifier = u"lumino";
	SizeI mainWindowSize = SizeI(640, 480);
	SizeI mainBackBufferSize = SizeI(0, 0);
	String mainWindowTitle = u"Lumino";
	AssetStorageAccessPriority assetStorageAccessPriority = AssetStorageAccessPriority::DirectoryFirst;
    List<EngineSettingsAssetArchiveEntry> assetArchives;
	List<Path> assetDirectories;
	GraphicsAPI graphicsAPI = GraphicsAPI::Default;

	bool engineLogEnabled = false;
	String engineLogFilePath;

    intptr_t userMainWindow = 0;
	bool standaloneFpsControl = false;
	int frameRate = 60;
	bool debugToolEnabled = false;

    bool defaultObjectsCreation = true;
    bool useGLFWWindowSystem = true;
    bool graphicsContextManagement = true;
    bool externalMainLoop = true;
    bool externalRenderingManagement = false;
    bool autoCoInitialize = true;

	EngineSettings()
	{
#ifdef LN_DEBUG
		engineLogEnabled = true;
		debugToolEnabled = true;
#endif
	}
};

class EngineManager
	: public RefObject
	, public IPlatforEventListener
{
public:
	EngineManager();
	virtual ~EngineManager();

	void init();
	void dispose();

	void initializeAllManagers();
	void initializeCommon();
    void initializeAssetManager();
	void initializePlatformManager();
	void initializeAnimationManager();
	void initializeInputManager();
	void initializeAudioManager();
	void initializeShaderManager();
	void initializeGraphicsManager();
	void initializeFontManager();
	void initializeMeshManager();
	void initializeRenderingManager();
	void initializeEffectManager();
    void initializePhysicsManager();
    void initializeVisualManager();
    void initializeSceneManager();
	void initializeUIManager();

	bool updateUnitily();
	void updateFrame();
	void renderFrame();		// Engine 内部管理のレンダリング
	void presentFrame();	// swap。renderFrame() と分けているのは、間に コールバック以外の Engine 外部のレンダリングを許可するため
	void resetFrameDelay();
	bool isExitRequested() const { return m_exitRequested; }
	void quit();

	EngineSettings& settings() { return m_settings; }
    const Ref<AssetManager>& assetManager() const { return m_assetManager; }
	const Ref<PlatformManager>& platformManager() const { return m_platformManager; }
    const Ref<AnimationManager>& animationManager() const { return m_animationManager; }
	const Ref<InputManager>& inputManager() const { return m_inputManager; }
	const Ref<AudioManager>& audioManager() const { return m_audioManager; }
	const Ref<ShaderManager>& shaderManager() const { return m_shaderManager; }
	const Ref<GraphicsManager>& graphicsManager() const { return m_graphicsManager; }
	const Ref<FontManager>& fontManager() const { return m_fontManager; }
	const Ref<MeshManager>& meshManager() const { return m_meshManager; }
	const Ref<RenderingManager>& renderingManager() const { return m_renderingManager; }
    const Ref<EffectManager>& effectManager() const { return m_effectManager; }
    const Ref<PhysicsManager>& physicsManager() const { return m_physicsManager; }
    const Ref<VisualManager>& visualManager() const { return m_visualManager; }
	const Ref<SceneManager>& sceneManager() const { return m_sceneManager; }
    const Ref<UIManager>& uiManager() const { return m_uiManager; }

	const Ref<Dispatcher>& mainThreadTaskDispatcher() const { return m_mainThreadTaskDispatcher; }
    const FpsController& fpsController() const { return m_fpsController; }
	const Ref<DiagnosticsManager>& activeDiagnostics() const { return m_activeDiagnostics; }

    const Path& persistentDataPath() const;
    void setTimeScale(float value) { m_timeScale = value; }
    //void setShowDebugFpsEnabled(bool value) { m_showDebugFpsEnabled = value; }
	void setMainWindow(ln::UIMainWindow* window);

    const Ref<UIContext>& mainUIContext() const { return m_mainUIContext; }
	const Ref<UIMainWindow>& mainWindow() const { return m_mainWindow; }
    const Ref<UIViewport>& mainViewport() const { return m_mainViewport; }
	const Ref<UIControl>& mainUIView() const { return m_mainUIRoot; }
    const Ref<World>& mainWorld() const { return m_mainWorld; }
    const Ref<Camera>& mainCamera() const { return m_mainCamera; }
    const Ref<WorldRenderView>& mainRenderView() const { return m_mainWorldRenderView; }
    const Ref<PhysicsWorld>& mainPhysicsWorld() const { return m_mainPhysicsWorld; }
    const Ref<PhysicsWorld2D>& mainPhysicsWorld2D() const { return m_mainPhysicsWorld2D; }

private:
	enum class DebugToolMode
	{
		Disable,
		Hidden,
		Minimalized,
		Activated,
	};

	//struct DebugToolState
	//{
	//	bool 
	//};

	virtual bool onPlatformEvent(const PlatformEventArgs& e) override;
	void handleImGuiDebugLayer(UIEventArgs* e);
	bool toggleDebugToolMode();
	void setDebugToolMode(DebugToolMode mode);

	EngineSettings m_settings;

    Ref<AssetManager> m_assetManager;
	Ref<PlatformManager>				m_platformManager;
	Ref<AnimationManager>			m_animationManager;
	Ref<InputManager>				m_inputManager;
	Ref<AudioManager>				m_audioManager;
	Ref<ShaderManager> m_shaderManager;
	Ref<GraphicsManager>			m_graphicsManager;
	Ref<FontManager> m_fontManager;
	Ref<MeshManager>				m_meshManager;
	Ref<RenderingManager>			m_renderingManager;
	Ref<EffectManager>				m_effectManager;
	Ref<PhysicsManager>		m_physicsManager;
    Ref<VisualManager>					m_visualManager;
    Ref<SceneManager>					m_sceneManager;
	Ref<UIManager>					m_uiManager;
    FpsController m_fpsController;
	Ref<Dispatcher> m_mainThreadTaskDispatcher;

	Ref<DiagnosticsManager> m_activeDiagnostics;

    //Application* m_application;
	Path m_persistentDataPath;

    Ref<UIContext> m_mainUIContext;
	Ref<UIMainWindow> m_mainWindow;
	Ref<UIViewport> m_mainViewport;
    Ref<UIRenderView> m_mainUIRenderView;   // m_mainViewport の ViewBox 内部に配置する
    Ref<UIControl> m_mainUIRoot;   // m_mainUIRenderView の RootElement
    Ref<World> m_mainWorld;
    Ref<Scene> m_mainScene;
    Ref<Camera> m_mainCamera;
    Ref<WorldRenderView> m_mainWorldRenderView;
    Ref<PhysicsWorld> m_mainPhysicsWorld;
    Ref<PhysicsWorld2D> m_mainPhysicsWorld2D;

    float m_timeScale;
	bool m_exitRequested;
 //   bool m_showDebugFpsEnabled;
	//bool m_debugToolEnabled;
	DebugToolMode m_debugToolMode;


#if defined(LN_OS_WIN32)
    bool m_comInitialized;
    bool m_oleInitialized;
#endif
};

} // namespace detail
} // namespace ln

