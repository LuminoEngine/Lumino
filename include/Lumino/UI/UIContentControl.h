
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIContentControl
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:

	void SetContent(UIElement* element);

protected:
	UIContentControl();
	virtual ~UIContentControl();
	void Initialize(detail::UIManager* manager);
	virtual void OnUpdatingLayout() override;

private:
	UIPanel*	m_contentHost;		// m_contentElement ÇÃí«â¡êÊ (êeóvëf)
	UIElement*	m_contentElement;
	bool		m_contentElementModified;
};

LN_NAMESPACE_END
