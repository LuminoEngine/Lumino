
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
	
/**
	@brief		Lumino アプリケーションを表します。
*/
class Application
	: public RefObject
	, public Platform::IEventListener
{
public:

	/**
		@brief		Application のインスタンスを作成し、アプリケーションを初期化します。
	*/
	static Application* Create(const ApplicationConfigData& configData);

public:

	/**
		@brief		
		@return		アプリケーションの終了が要求されている場合は false を返します。
	*/
	bool UpdateFrame();

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


	Graphics::GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	GUI::GUIManager* GetGUIManager() const { return m_guiManager; }

private:
	Application(const ApplicationConfigData& configData);
	virtual ~Application();
	void Initialize();
	void InitialzePlatformManager();
	void InitialzePhysicsManager();
	void InitialzeGraphicsManager();
	void InitialzeDocumentsManager();
	void InitialzeGUIManager();
	virtual bool OnEvent(const Platform::EventArgs& e);

private:
	//class NativeWindowEventListener;
	ApplicationConfigData				m_configData;
	FpsController						m_fpsController;
	RefPtr<Platform::PlatformManager>	m_platformManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	RefPtr<GUI::GUIManager>				m_guiManager;
	
	//NativeWindowEventListener*			m_nativeWindowEventListener;
	bool								m_endRequested;
};

} // namespace Lumino
