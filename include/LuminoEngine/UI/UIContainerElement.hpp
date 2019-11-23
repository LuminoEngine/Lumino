#pragma once
#include "UIControl.hpp"

namespace ln {
class UICommand;

class UIContainerElement
	: public UIControl
{
public:
    UIContainerElement();
	void init();

private:
};

class UIFrame
    : public UIContainerElement
{
public:
    UIFrame();
    void init();

private:
};

} // namespace ln

