#pragma once
#include <LuminoEngine/Engine/Common.hpp>
#include <LuminoEngine/Asset/Common.hpp>
#include <LuminoEngine/Graphics/Common.hpp>
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include <LuminoEngine/Platform/PlatformEvent.hpp>
#include "FpsController.hpp"

namespace ln {
class EngineContext;
class UIContext;
class UIFrameWindow;
class UIViewport;
class UIRenderView;
class UIContainerElement;
class PhysicsWorld;
class PhysicsWorld2D;
class World;
class WorldRenderView;
class Camera;
class AmbientLight;
class DirectionalLight;

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
    Flags<EngineFeature> features = EngineFeature::Public;
	String bundleIdentifier = u"lumino";
	SizeI mainWindowSize = SizeI(640, 480);
	SizeI mainBackBufferSize = SizeI(640, 480);
	String mainWindowTitle = u"Lumino";
	AssetStorageAccessPriority assetStorageAccessPriority = AssetStorageAccessPriority::DirectoryFirst;
    List<EngineSettingsAssetArchiveEntry> assetArchives;
	List<Path> assetDirectories;
	GraphicsAPI graphicsAPI = GraphicsAPI::Default;
	bool standaloneFpsControl = false;
	int frameRate = 60;
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
	void initializePlatformManager();
	void initializeAnimationManager();
	void initializeInputManager();
	void initializeAudioManager();
	void initializeShaderManager();
	void initializeGraphicsManager();
	void initializeFontManager();
	void initializeMeshManager();
	void initializeRenderingManager();
	//void initializeEffectManager();
    void initializePhysicsManager();
	void initializeAssetManager();
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
	const Ref<EngineContext>& engineContext() const { return m_engineContext; }
	const Ref<PlatformManager>& platformManager() const { return m_platformManager; }
    const Ref<AnimationManager>& animationManager() const { return m_animationManager; }
	const Ref<InputManager>& inputManager() const { return m_inputManager; }
	const Ref<AudioManager>& audioManager() const { return m_audioManager; }
	const Ref<ShaderManager>& shaderManager() const { return m_shaderManager; }
	const Ref<GraphicsManager>& graphicsManager() const { return m_graphicsManager; }
	const Ref<FontManager>& fontManager() const { return m_fontManager; }
	const Ref<MeshManager>& meshManager() const { return m_meshManager; }
	const Ref<RenderingManager>& renderingManager() const { return m_renderingManager; }
    const Ref<PhysicsManager>& physicsManager() const { return m_physicsManager; }
    const Ref<AssetManager>& assetManager() const { return m_assetManager; }
    const Ref<VisualManager>& visualManager() const { return m_visualManager; }
	const Ref<SceneManager>& sceneManager() const { return m_sceneManager; }
    const Ref<UIManager>& uiManager() const { return m_uiManager; }

    const FpsController& fpsController() const { return m_fpsController; }

	const Path& persistentDataPath() const { return m_persistentDataPath; }

	const Ref<UIFrameWindow>& mainWindow() const { return m_mainWindow; }
    const Ref<UIViewport>& mainViewport() const { return m_mainViewport; }
	const Ref<UIContainerElement>& mainUIRoot() const { return m_mainUIRoot; }
    const Ref<World>& mainWorld() const { return m_mainWorld; }
    const Ref<Camera>& mainCamera() const { return m_mainCamera; }
    const Ref<AmbientLight>& mainAmbientLight() const { return m_mainAmbientLight; }
    const Ref<DirectionalLight>& mainDirectionalLight() const { return m_mainDirectionalLight; }
    const Ref<WorldRenderView>& mainRenderView() const { return m_mainWorldRenderView; }
    const Ref<PhysicsWorld>& mainPhysicsWorld() const { return m_mainPhysicsWorld; }
    const Ref<PhysicsWorld2D>& mainPhysicsWorld2D() const { return m_mainPhysicsWorld2D; }

private:
	virtual bool onPlatformEvent(const PlatformEventArgs& e) override;

	EngineSettings m_settings;

	Ref<EngineContext> m_engineContext;
	Ref<PlatformManager>				m_platformManager;
	Ref<AnimationManager>			m_animationManager;
	Ref<InputManager>				m_inputManager;
	Ref<AudioManager>				m_audioManager;
	Ref<ShaderManager> m_shaderManager;
	Ref<GraphicsManager>			m_graphicsManager;
	Ref<FontManager> m_fontManager;
	Ref<MeshManager>				m_meshManager;
	Ref<RenderingManager>			m_renderingManager;
	//Ref<EffectManager>				m_effectManager;
	//Ref<ModelManager>				m_modelManager;
	Ref<PhysicsManager>		m_physicsManager;
	Ref<AssetManager>						m_assetManager;
    Ref<VisualManager>					m_visualManager;
    Ref<SceneManager>					m_sceneManager;
	Ref<UIManager>					m_uiManager;
	FpsController m_fpsController;

	Path m_persistentDataPath;

    Ref<UIContext> m_mainUIContext;
	Ref<UIFrameWindow> m_mainWindow;
	Ref<UIViewport> m_mainViewport;
    Ref<UIRenderView> m_mainUIRenderView;   // m_mainViewport の ViewBox 内部に配置する
    Ref<UIContainerElement> m_mainUIRoot;   // m_mainUIRenderView の RootElement
    Ref<World> m_mainWorld;
    Ref<Camera> m_mainCamera;
    Ref<AmbientLight> m_mainAmbientLight;
    Ref<DirectionalLight> m_mainDirectionalLight;
    Ref<WorldRenderView> m_mainWorldRenderView;
    Ref<PhysicsWorld> m_mainPhysicsWorld;
    Ref<PhysicsWorld2D> m_mainPhysicsWorld2D;

	bool m_exitRequested;
};

} // namespace detail
} // namespace ln

