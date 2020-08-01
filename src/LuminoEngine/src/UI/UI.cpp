
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UI.hpp>
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UI

UITheme* UI::mainTheme()
{
	return detail::EngineDomain::uiManager()->mainContext()->styleContext()->mainTheme;
}

} // namespace ln

