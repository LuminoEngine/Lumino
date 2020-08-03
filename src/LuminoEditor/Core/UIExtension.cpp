
#include "UIExtension.hpp"

namespace ln {
    
Size NavigationMenuItem::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    return UIIcon::measureOverride(layoutContext, constraint);
}

Size NavigationMenuItem::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    return UIIcon::arrangeOverride(layoutContext, finalArea);
}

NavigationMenuItem::NavigationMenuItem()
{
}

void NavigationMenuItem::init()
{
    UIIcon::init();
}



} // namespace lna
