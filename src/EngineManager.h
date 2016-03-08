
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include <Lumino/Platform/PlatformManager.h>
#include <Lumino/Platform/EventListener.h>
#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/Game/FpsController.h>
#include <Lumino/EngineSettings.h>

LN_NAMESPACE_BEGIN

namespace detail { class InputManager; }
namespace detail { class EffectManager; }
namespace detail { class ModelManager; }
namespace detail { class UIManager; }
class AudioManagerImpl;
class ProfilerRenderer;
class GraphicsManager;
class SceneGraphManager;
class Application;

class EngineManager
	: public RefObject
	, public IEventListener
{
public:
	static EngineManager*	Instance;

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

	/**
		@brief		遅延をリセットします。
		@details	リソースのロード等で時間がかかり長い時間更新処理が行われなかった場合、
					UpdateFrame() は本来あるべき時間に追いつこうとしてしばらくの間ノーウェイトでフレーム更新が行われます。
					その間はアプリケーションが非常に高速に動作しているように見えてしまします。
					これを回避するため、時間のかかる処理の直後でこの関数を呼ぶことで、FPS 制御に遅延が発生していないことを伝えます。
	*/
	void ResetFrameDelay();

	/**
		@brief		アプリケーションの終了が要求されているかを確認します。
	*/
	bool IsEndRequested() const { return m_endRequested; }


	PlatformManager* GetPlatformManager() const { return m_platformManager; }
	AudioManagerImpl* GetAudioManager() const { return m_audioManager; }
	Physics::PhysicsManager* GetPhysicsManager() const { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	Documents::DocumentsManager* GetDocumentsManager() const { return m_documentsManager; }
	detail::UIManager* GetUIManager() const { return m_uiManager; }
	SceneGraphManager* GetSceneGraphManager() const { return m_sceneGraphManager; }
	Application* GetApplication() const { return m_application; }

protected:
	EngineManager(const EngineSettings& configData);
	virtual ~EngineManager();

public:
	void Initialize();
	void InitializeCommon();
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
	virtual bool OnEvent(const PlatformEventArgs& e);

private:
	//class NativeWindowEventListener;
	EngineSettings						m_configData;
	FpsController						m_fpsController;
	FileManager*						m_fileManager;
	RefPtr<PlatformManager>	m_platformManager;
	detail::InputManager*				m_inputManager;
	AudioManagerImpl*					m_audioManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	GraphicsManager*					m_graphicsManager;
	detail::EffectManager*				m_effectManager;
	detail::ModelManager*				m_modelManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	detail::UIManager*						m_uiManager;
	SceneGraphManager*					m_sceneGraphManager;

	ProfilerRenderer*			m_profilerRenderer;

	Application*						m_application;
	
	//NativeWindowEventListener*			m_nativeWindowEventListener;
	bool								m_frameRenderingSkip;
	bool								m_frameRenderd;
	bool								m_commonInitied;
	bool								m_endRequested;
	bool								m_comInitialized;
};

LN_NAMESPACE_END
