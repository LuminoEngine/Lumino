
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include "Platform/PlatformManager.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"
#include "Game/FpsController.h"

namespace Lumino
{
	
/**
	@brief		Lumino アプリケーションを表します。
*/
class Application
	: public RefObject
{
public:

	/**
		@brief		Application のインスタンスを作成し、アプリケーションを初期化します。
	*/
	static Application* Create();

public:

	/**
		@brief		
		@return		アプリケーションの終了が要求されている場合は false を返します。
	*/
	bool UpdateFrame();

	/**
		@brief		アプリケーションの終了が要求されているかを確認します。
	*/
	bool IsEndRequested() const { return m_endRequested; }

private:
	Application();
	virtual ~Application();
	void Initialize();
	void InitialzePlatformManager();
	void InitialzePhysicsManager();
	void InitialzeGraphicsManager();

private:
	FpsController						m_fpsController;
	RefPtr<Platform::PlatformManager>	m_platformManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	bool								m_endRequested;
};

} // namespace Lumino
