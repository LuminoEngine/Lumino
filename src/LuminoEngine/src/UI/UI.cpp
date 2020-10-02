
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
	return detail::EngineDomain::uiManager()->styleContext()->mainTheme;
}

} // namespace ln

