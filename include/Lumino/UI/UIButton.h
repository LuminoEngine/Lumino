
#pragma once
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN
class UIButton;
using UIButtonPtr = RefPtr<UIButton>;

/**
	@brief		
*/
class UIButton
	: public UIContentControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static UIButtonPtr Create();

public:
	void SetText(const StringRef& text);

protected:
	UIButton();
	virtual ~UIButton();
	void Initialize(detail::UIManager* manager);

private:
};

LN_NAMESPACE_END
