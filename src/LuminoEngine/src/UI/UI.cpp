
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UI.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
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

