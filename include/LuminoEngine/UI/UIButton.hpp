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
    virtual const String& className() const { static String name = u"UIButton"; return name; }

private:
};

} // namespace ln

