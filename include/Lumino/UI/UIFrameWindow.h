
#pragma once
#include "UIElement.h"
//#include "../Graphics/Viewport.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIFrameWindow
	: public Object
{
	LN_UI_TYPEINFO_DECLARE();
public:

	PlatformWindow* GetPlatformWindow() const { return m_platformWindow; }

protected:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void Initialize(detail::UIManager* manager, PlatformWindow* platformWindow);

private:
	detail::UIManager*	m_manager;
	PlatformWindow*		m_platformWindow;
	//Viewport*			m_backBufferViewport;
};


/**
	@brief		
*/
class UIMainWindow
	: public UIFrameWindow
{
	LN_UI_TYPEINFO_DECLARE();
public:
	UIContext* GetMainUIContext() const { return m_mainUIContext; }

LN_INTERNAL_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void Initialize(detail::UIManager* manager, PlatformWindow* platformWindow);

	void InjectElapsedTime(float elapsedTime);
	void UpdateLayout(const SizeF& viewSize);	// TODO: ゆくゆくは SwapChain や Viewport も UIFrameWindow にもってくる。そのとき、この viewSize はいらなくなる
	void Render(GraphicsContext* g);

private:
	UIContext*	m_mainUIContext;
};

LN_NAMESPACE_END
