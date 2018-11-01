
#include "Internal.hpp"
#include "UIManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UIManager

UIManager::UIManager()
	: m_graphicsManager(nullptr)
{
}

void UIManager::initialize(const Settings& settings)
{
	m_graphicsManager = settings.graphicsManager;
}

void UIManager::dispose()
{
}

} // namespace detail
} // namespace ln

