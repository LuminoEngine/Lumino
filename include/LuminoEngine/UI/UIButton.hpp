#pragma once
#include "UIControl.hpp"

namespace ln {
class UITextBlock;

class UIButtonBase
	: public UIControl
{
public:
	UIButtonBase();
	void init();

    void setText(const StringRef& text);

protected:
    virtual const String& elementName() const  override { static String name = u"UIButton"; return name; }
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
    Ref<UITextBlock> m_textContent;
    bool m_isPressed;
};

class UIButton
	: public UIButtonBase
{
public:
	UIButton();
	void init();

protected:
};

class UIToggleButton
	: public UIButtonBase
{
LN_CONSTRUCT_ACCESS:
	UIToggleButton();
	void init();

private:
};

} // namespace ln

