#pragma once
#include "UILayoutElement.hpp"

namespace ln {

class UIElement
	: public Object
    , public detail::ILayoutElement
{
public:
    UIElement();
	void initialize();

private:
};

} // namespace ln

