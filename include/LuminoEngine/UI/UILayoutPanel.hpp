#pragma once
#include "UILayoutElement.hpp"

namespace ln {

class UILayoutPanel
	: public Object
    , public detail::ILayoutElement
{
public:
    UILayoutPanel();
	void initialize();

private:
};

} // namespace ln

