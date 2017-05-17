
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIWindow
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIWindow> Create();

protected:
	virtual void OnRoutedEvent(UIEventArgs* e) override;
	virtual void OnLayoutPanelChanged(UILayoutPanel* newPanel) override;
	virtual void OnRender(DrawingContext* g) override;

LN_CONSTRUCT_ACCESS:
	UIWindow();
	virtual ~UIWindow();
	void Initialize();

private:
	Vector2	m_dragStartWindowPosition;
	Vector2	m_dragStartLocalPosition;
	bool	m_isDragging;
};

LN_NAMESPACE_END
