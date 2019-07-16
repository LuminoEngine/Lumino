#pragma once
#include "UIControl.hpp"

namespace ln {
class UITextBlock;

class UIButton
	: public UIControl
{
public:
    UIButton();
	void init();

    void setText(const StringRef& text);

protected:
    virtual const String& className() const  override { static String name = u"UIButton"; return name; }
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
    Ref<UITextBlock> m_textContent;
    bool m_isPressed;
};

} // namespace ln

