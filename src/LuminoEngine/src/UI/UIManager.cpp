
#include "Internal.hpp"
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include "UIManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UIManager

UIManager::UIManager()
	: m_graphicsManager(nullptr)
    , m_primaryElement(nullptr)
{
}

void UIManager::initialize(const Settings& settings)
{
	m_graphicsManager = settings.graphicsManager;
}

void UIManager::dispose()
{
}
void UIManager::setPrimaryElement(UIContainerElement* element)
{
    m_primaryElement = element;
}

UIContainerElement* UIManager::primaryElement() const
{
    return m_primaryElement;
}

} // namespace detail
} // namespace ln

