#pragma once
#include "Controls/UIControl.hpp"

namespace ln {
class UICommand;

class UIContainerElement
	: public UIControl
{
public:
    UIContainerElement();
    Result init();

private:
};

class UIFrame
    : public UIContainerElement
{
public:
    UIFrame();
    Result init();

private:
};

} // namespace ln

