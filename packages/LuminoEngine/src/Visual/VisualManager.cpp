
#include "Internal.hpp"
#include "VisualManager.hpp"
#include <LuminoEngine/Visual/SpriteComponent.hpp>

namespace ln {
namespace detail {

//==============================================================================
// VisualManager

VisualManager::VisualManager()
    : m_graphicsManager(nullptr) {
}

VisualManager::~VisualManager() {
}

void VisualManager::init(const Settings& settings) {
    LN_LOG_DEBUG("VisualManager Initialization started.");

    m_graphicsManager = settings.graphicsManager;

    auto* context = RuntimeContext::current();
    context->registerType<Component>();
    context->registerType<VisualComponent>();
    context->registerType<SpriteComponent>();
    context->registerType<SpriteFrame>();
    context->registerType<SpriteSheet>();

    LN_LOG_DEBUG("VisualManager Initialization ended.");
}

void VisualManager::dispose() {
}

} // namespace detail
} // namespace ln
