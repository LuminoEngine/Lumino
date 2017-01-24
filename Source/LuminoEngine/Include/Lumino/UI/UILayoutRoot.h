
#pragma once
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UILayoutRoot
	: public UIContentControl
{
	LN_UI_TYPEINFO_DECLARE();
public:

LN_INTERNAL_ACCESS:
	UILayoutRoot();
	virtual ~UILayoutRoot();
	void Initialize(detail::UIManager* manager, UILayoutView* owner);
	
protected:
	virtual void ActivateInternal(UIElement* child);

private:
	UILayoutView*	m_owner;
};

LN_NAMESPACE_END
