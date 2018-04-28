#pragma once
#include <Lumino/Graphics/GeometryStructs.hpp>
#include <Lumino/Platform/PlatformEvent.hpp>

namespace ln {
class UIFrameWindow;

namespace detail {
class PlatformManager;
class AnimationManager;
class PhysicsManager;
class InputManager;
class AudioManager;
class AnimationManager;
class GraphicsManager;
class EffectManager;
class ModelManager;
class AssetsManager;
class UIManager;

struct EngineSettings
{
	SizeI mainWindowSize = SizeI(640, 480);
	SizeI mainBackBufferSize = SizeI(640, 480);
	String mainWindowTitle = _T("Lumino");
};

class EngineManager
	: public RefObject
	, public IPlatforEventListener
{
public:
	EngineManager();
	virtual ~EngineManager() = default;

	void initialize();
	void dispose();

	void initializeAllManagers();
	void initializeCommon();
	void initializePlatformManager();
	void initializeAnimationManager();
	void initializeInputManager();
	void initializeAudioManager();
	void initializePhysicsManager();
	void initializeGraphicsManager();
	void initializeEffectManager();
	void initializeModelManager();
	void initializeAssetsManager();
	void initializeUIManager();

	bool updateUnitily();
	void updateFrame();
	void renderFrame();		// Engine 内部管理のレンダリング
	void presentFrame();	// swap。renderFrame() と分けているのは、間に コールバック以外の Engine 外部のレンダリングを許可するため
	void resetFrameDelay();
	bool isExitRequested() const { return m_exitRequested; }
	void exit();

	const Ref<PlatformManager>& platformManager() const { return m_platformManager; }
	const Ref<GraphicsManager>& graphicsManager() const { return m_graphicsManager; }
	const Ref<UIManager>& uiManager() const { return m_uiManager; }

private:
	virtual bool onPlatformEvent(const PlatformEventArgs& e) override;

	EngineSettings m_settings;

	Ref<PlatformManager>				m_platformManager;
	//Ref<AnimationManager>			m_animationManager;
	//Ref<InputManager>				m_inputManager;
	//Ref<AudioManager>				m_audioManager;
	//Ref<PhysicsManager>		m_physicsManager;
	Ref<GraphicsManager>			m_graphicsManager;
	//Ref<EffectManager>				m_effectManager;
	//Ref<ModelManager>				m_modelManager;
	//Ref<AssetsManager>						m_assetsManager;
	Ref<UIManager>					m_uiManager;

	Ref<UIFrameWindow> m_mainWindow;

	bool m_exitRequested;
};

} // namespace detail
} // namespace ln

