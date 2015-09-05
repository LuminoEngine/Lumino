
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include "Platform/PlatformManager.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"
#include "Documents/DocumentsManager.h"
#include "GUI/GUIManager.h"
#include "Game/FpsController.h"
#include "ApplicationConfigData.h"

namespace Lumino
{
namespace Audio { class AudioManager; }
namespace Graphics { class ProfilerRenderer; }
	
/**
	@brief		Lumino アプリケーションを表します。
*/
class Application
	: public RefObject
	, public Platform::IEventListener
{
public:
	/**
		@brief		
	*/
	class ConfigData
	{
	public:
		Graphics::GraphicsAPI	GraphicsAPI;
		Graphics::RenderingType	RenderingType;

		/** ユーザー定義のウィンドウハンドル (windows の場合は HWND、X11 は Window*。ただし、X11 は未対応) */
		void*	UserMainWindow;

#ifdef LN_OS_WIN32
		/** 既に作成済みの IDirect3DDevice9 インターフェイスを利用する場合、そのポインタを指定します。*/
		void*	D3D9Device;
#endif

	public:
		ConfigData()
			: GraphicsAPI(Graphics::GraphicsAPI::DirectX9)
			, RenderingType(Graphics::RenderingType::Deferred)
			, UserMainWindow(NULL)
#ifdef LN_OS_WIN32
			, D3D9Device(NULL)
#endif
		{}
	};

public:

	/**
		@brief		Application のインスタンスを作成し、アプリケーションを初期化します。
	*/
	static Application* Create(const Application::ConfigData& configData);

public:

	/**
		@brief		
		@return		アプリケーションの終了が要求されている場合は false を返します。
	*/
	bool UpdateFrame();

	/**
		@brief		
	*/
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


	Platform::PlatformManager* GetPlatformManager() const { return m_platformManager; }
	Audio::AudioManager* GetAudioManager() const { return m_audioManager; }
	Physics::PhysicsManager* GetPhysicsManager() const { return m_physicsManager; }
	Graphics::GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	Documents::DocumentsManager* GetDocumentsManager() const { return m_documentsManager; }
	GUI::GUIManager* GetGUIManager() const { return m_guiManager; }

protected:
	Application(const Application::ConfigData& configData);
	virtual ~Application();
	void Initialize();
	void InitialzePlatformManager();
	void InitialzeAudioManager();
	void InitialzePhysicsManager();
	void InitialzeGraphicsManager();
	void InitialzeDocumentsManager();
	void InitialzeGUIManager();
	virtual bool OnEvent(const Platform::EventArgs& e);

private:
	//class NativeWindowEventListener;
	Application::ConfigData				m_configData;
	FpsController						m_fpsController;
	RefPtr<Platform::PlatformManager>	m_platformManager;
	Audio::AudioManager*				m_audioManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	RefPtr<GUI::GUIManager>				m_guiManager;

	Graphics::ProfilerRenderer*			m_profilerRenderer;
	
	//NativeWindowEventListener*			m_nativeWindowEventListener;
	bool								m_endRequested;
};

} // namespace Lumino
