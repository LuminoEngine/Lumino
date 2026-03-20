#include "Internal.hpp"
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/UIContext.hpp>
#include <LuminoFramework/UI/UI.hpp>
#include <LuminoFramework/UI/UIRenderView.hpp>
#include "../Engine/EngineManager.hpp"
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UI

UITheme* UI::mainTheme()
{
	return detail::EngineDomain::uiManager()->styleContext()->mainTheme;
}

void UI::add(UIElement* element)
{
	if (LN_REQUIRE(element)) return;
	detail::EngineDomain::engineManager()->mainUIView()->addChild(element);
}

void UI::remove(UIElement* element)
{
	if (LN_REQUIRE(element)) return;
	element->removeFromLogicalParent();
}

} // namespace ln

