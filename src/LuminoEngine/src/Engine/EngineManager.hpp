#pragma once
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include <LuminoEngine/Platform/PlatformEvent.hpp>

namespace ln {
class UIFrameWindow;
class UIViewport;
class UIRenderView;
class UIContainerElement;
class World;
class WorldRenderView;
class Camera;

namespace detail {
class PlatformManager;
class AnimationManager;
class PhysicsManager;
class InputManager;
class AudioManager;
class AnimationManager;
class ShaderManager;
class GraphicsManager;
class FontManager;
class MeshManager;
class RenderingManager;
class EffectManager;
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
	SizeI mainWindowSize = SizeI(640, 480);
	SizeI mainBackBufferSize = SizeI(640, 480);
	String mainWindowTitle = _T("Lumino");
    List<EngineSettingsAssetArchiveEntry> assetArchives;
};

class EngineManager
	: public RefObject
	, public IPlatforEventListener
{
public:
	EngineManager();
	virtual ~EngineManager();

	void initialize();
	void dispose();

	void initializeAllManagers();
	void initializeCommon();
	void initializePlatformManager();
	void initializeAnimationManager();
	void initializeInputManager();
	void initializeAudioManager();
	void initializePhysicsManager();
	void initializeShaderManager();
	void initializeGraphicsManager();
	void initializeFontManager();
	void initializeMeshManager();
	void initializeRenderingManager();
	//void initializeEffectManager();
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
	void exit();

	EngineSettings& settings() { return m_settings; }
	const Ref<PlatformManager>& platformManager() const { return m_platformManager; }
    const Ref<AnimationManager>& animationManager() const { return m_animationManager; }
	const Ref<InputManager>& inputManager() const { return m_inputManager; }
	const Ref<AudioManager>& audioManager() const { return m_audioManager; }
	const Ref<ShaderManager>& shaderManager() const { return m_shaderManager; }
	const Ref<GraphicsManager>& graphicsManager() const { return m_graphicsManager; }
	const Ref<FontManager>& fontManager() const { return m_fontManager; }
	const Ref<MeshManager>& meshManager() const { return m_meshManager; }
	const Ref<RenderingManager>& renderingManager() const { return m_renderingManager; }
    const Ref<AssetManager>& assetManager() const { return m_assetManager; }
    const Ref<VisualManager>& visualManager() const { return m_visualManager; }
	const Ref<SceneManager>& sceneManager() const { return m_sceneManager; }
    const Ref<UIManager>& uiManager() const { return m_uiManager; }

	const Ref<UIFrameWindow>& mainWindow() const { return m_mainWindow; }
    const Ref<UIViewport>& mainViewport() const { return m_mainViewport; }
    const Ref<Camera>& mainCamera() const { return m_mainCamera; }

private:
	virtual bool onPlatformEvent(const PlatformEventArgs& e) override;

	EngineSettings m_settings;

	Ref<PlatformManager>				m_platformManager;
	Ref<AnimationManager>			m_animationManager;
	Ref<InputManager>				m_inputManager;
	Ref<AudioManager>				m_audioManager;
	//Ref<PhysicsManager>		m_physicsManager;
	Ref<ShaderManager> m_shaderManager;
	Ref<GraphicsManager>			m_graphicsManager;
	Ref<FontManager> m_fontManager;
	Ref<MeshManager>				m_meshManager;
	Ref<RenderingManager>			m_renderingManager;
	//Ref<EffectManager>				m_effectManager;
	//Ref<ModelManager>				m_modelManager;
	Ref<AssetManager>						m_assetManager;
    Ref<VisualManager>					m_visualManager;
    Ref<SceneManager>					m_sceneManager;
	Ref<UIManager>					m_uiManager;

	Ref<UIFrameWindow> m_mainWindow;
	Ref<UIViewport> m_mainViewport;
    Ref<UIRenderView> m_mainUIRenderView;   // m_mainViewport の ViewBox 内部に配置する
    Ref<UIContainerElement> m_mainUIRoot;   // m_mainUIRenderView の RootElement
    Ref<World> m_mainWorld;
    Ref<WorldRenderView> m_mainWorldRenderView;
	Ref<Camera> m_mainCamera;

	bool m_exitRequested;
};

} // namespace detail
} // namespace ln

