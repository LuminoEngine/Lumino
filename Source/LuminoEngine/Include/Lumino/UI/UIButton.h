
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
	LN_ROUTED_EVENT(UIMouseEventArgs, ClickEvent);		/**< Click ルーティングイベントの識別子 */

public:
	static UIButtonPtr Create();

public:
	UIEvent<UIMouseEventArgs>		click;		/**< */

	void SetText(const StringRef& text);

LN_CONSTRUCT_ACCESS:
	UIButton();
	virtual ~UIButton();
	void Initialize(detail::UIManager* manager);

protected:
	virtual void OnClick();

	// UIElement interface
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
	virtual void OnMouseDown(UIMouseEventArgs* e) override;
	virtual void OnMouseUp(UIMouseEventArgs* e) override;

private:
	bool	m_isPressed;
};

LN_NAMESPACE_END
