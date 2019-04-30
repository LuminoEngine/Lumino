
#include "Internal.hpp"
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include "UIEventArgsPool.hpp"
#include "UIManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UIManager

const float UIManager::MouseButtonClickTimeout = 0.3f;

UIManager::UIManager()
	: m_graphicsManager(nullptr)
    , m_primaryElement(nullptr)
{
}

void UIManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "UIManager Initialization started.";

	m_graphicsManager = settings.graphicsManager;
    m_eventArgsPool = makeRef<EventArgsPool>();
    //m_mainContext = newObject<UIContext>();

    LN_LOG_DEBUG << "UIManager Initialization ended.";
}

void UIManager::dispose()
{
    m_eventArgsPool = nullptr;
    m_mainContext = nullptr;
}

void UIManager::setPrimaryElement(UIContainerElement* element)
{
    m_primaryElement = element;
}

void UIManager::setMainContext(UIContext* context)
{
    m_mainContext = context;
}

UIContainerElement* UIManager::primaryElement() const
{
    return m_primaryElement;
}

} // namespace detail
} // namespace ln

