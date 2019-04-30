#pragma once
#include "UIContainerElement.hpp"

namespace ln {
class Material;

class UIWindow
    : public UIContainerElement
{
public:
    static Ref<UIWindow> create();

LN_CONSTRUCT_ACCESS:
    UIWindow();
	void init();

protected:

private:
};

} // namespace ln

