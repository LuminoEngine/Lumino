#pragma once
#include "UIControl.hpp"

namespace ln {

class UIButton
	: public UIControl
{
public:
    UIButton();
	void init();

protected:
    virtual const String& className() const  override { static String name = u"UIButton"; return name; }
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
    bool m_isPressed;
};

} // namespace ln

