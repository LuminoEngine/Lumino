
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@details	VerticalContentAlignment Ç∆ HorizontalContentAlignment ÇÃèâä˙ílÇÕÇªÇÍÇºÇÍ Stretch Ç≈Ç∑ÅB
*/
class UIContentControl
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	void setContent(UIElement* element);

protected:
	UIContentControl();
	virtual ~UIContentControl();
	void initialize();
	//virtual void onUpdatingLayout() override;


private:
	UIElement*	m_contentElement;
	bool		m_contentElementModified;
};

LN_NAMESPACE_END
