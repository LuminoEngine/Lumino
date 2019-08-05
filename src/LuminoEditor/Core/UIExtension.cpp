
#include "UIExtension.hpp"

namespace ln {
    
Size NavigationMenuItem::measureOverride(const Size& constraint)
{
    return UIIcon::measureOverride(constraint);
}

Size NavigationMenuItem::arrangeOverride(const Size& finalSize)
{
    return UIIcon::arrangeOverride(finalSize);
}

NavigationMenuItem::NavigationMenuItem()
{
}

void NavigationMenuItem::init()
{
    UIIcon::init();
}



} // namespace lna
