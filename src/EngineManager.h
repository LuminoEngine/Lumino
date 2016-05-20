
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include "Platform/PlatformManager.h"
#include <Lumino/Platform/EventListener.h>
#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/Game/FpsController.h>
#include <Lumino/EngineSettings.h>

LN_NAMESPACE_BEGIN

namespace detail { class AnimationManager; }
namespace detail { class InputManager; }
namespace detail { class EffectManager; }
namespace detail { class ModelManager; }
namespace detail { class UIManager; }
namespace detail { class AudioManager; }
class GraphicsManager;
class SceneGraphManager;
class Application;
class EngineDiagViewer;
class AssetsManager;

class EngineManager
	: public RefObject
	, public IEventListener
{
public:
	static EngineManager*	Instance;

	static EngineManager* GetInstance(EngineManager* priority = nullptr);

public:
	static const TCHAR*	LogFileName;

public:

	/**
		@brief		
	*/
	static EngineManager* Create(const EngineSettings& configData);

public:

	bool UpdateFrame();
	bool BeginRendering();
	void EndRendering();
	void Render();
	void ResetFrameDelay();
	bool IsEndRequested() const { return m_endRequested; }
	void Exit();


	PlatformManager* GetPlatformManager() const { return m_platformManager; }
	detail::AudioManager* GetAudioManager() const { return m_audioManager; }
	Physics::PhysicsManager* GetPhysicsManager() const { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	Documents::DocumentsManager* GetDocumentsManager() const { return m_documentsManager; }
	detail::UIManager* GetUIManager() const { return m_uiManager; }
	SceneGraphManager* GetSceneGraphManager() const { return m_sceneGraphManager; }
	//Application* GetApplication() const { return m_application; }

	const FpsController& GetFpsController() const { return m_fpsController; }

protected:
	EngineManager(const EngineSettings& configData);
	virtual ~EngineManager();

public:
	void Initialize();
	void InitializeCommon();
	void InitializeAnimationManager();
	void InitializeFileManager();
	void InitializePlatformManager();
	void InitializeInputManager();
	void InitializeAudioManager();
	void InitializePhysicsManager();
	void InitializeGraphicsManager();
	void InitializeEffectManager();
	void InitializeModelManager();
	void InitializeDocumentsManager();
	void InitializeUIManager();
	void InitializeSceneGraphManager();
	void InitializeAssetsManager();
	virtual bool OnEvent(const PlatformEventArgs& e);

private:
	//class NativeWindowEventListener;
	EngineSettings						m_configData;
	FpsController						m_fpsController;
	detail::AnimationManager*			m_animationManager;
	FileManager*						m_fileManager;
	RefPtr<PlatformManager>	m_platformManager;
	detail::InputManager*				m_inputManager;
	detail::AudioManager*				m_audioManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	GraphicsManager*					m_graphicsManager;
	detail::EffectManager*				m_effectManager;
	detail::ModelManager*				m_modelManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	detail::UIManager*						m_uiManager;
	SceneGraphManager*					m_sceneGraphManager;
	AssetsManager*						m_assetsManager;

	EngineDiagViewer*					m_diagViewer;

	//Application*						m_application;
	
	//NativeWindowEventListener*			m_nativeWindowEventListener;
	bool								m_frameRenderingSkip;
	bool								m_frameRenderd;
	bool								m_commonInitied;
	bool								m_endRequested;
	bool								m_comInitialized;
};

LN_NAMESPACE_END
