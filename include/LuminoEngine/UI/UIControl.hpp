#pragma once
#include "UIContainerElement.hpp"

namespace ln {

// - フォーカスを持つことができる。
// - フォーカスの移動単位となる。
class UIControl
	: public UIContainerElement
{
public:
    UIControl();
	void init();

private:
};

} // namespace ln

