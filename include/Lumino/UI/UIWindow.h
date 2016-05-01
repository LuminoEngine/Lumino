
#pragma once
#include "UIElement.h"
//#include "../Graphics/Viewport.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIWindow
	: public Object
{
public:

	PlatformWindow* GetPlatformWindow() const { return m_platformWindow; }

LN_INTERNAL_ACCESS:	// TODO: いまはとりあえず内部用途
	UIWindow();
	virtual ~UIWindow();
	void InitializeDefault(detail::UIManager* manager, PlatformWindow* platformWindow);

private:
	detail::UIManager*	m_manager;
	PlatformWindow*		m_platformWindow;
	//Viewport*			m_backBufferViewport;
};

LN_NAMESPACE_END
