
#include "Internal.hpp"
#include "VisualManager.hpp"
#include <LuminoEngine/Visual/SpriteComponent.hpp>
#include <LuminoEngine/Visual/SpriteComponent.hpp>
#include <LuminoEngine/Reflection/Property.hpp>// TODO:

namespace ln {
namespace detail {

//=============================================================================
// VisualManager

VisualManager::VisualManager()
	: m_graphicsManager(nullptr)
{
}

VisualManager::~VisualManager()
{
}

void VisualManager::init(const Settings& settings)
{
    LN_LOG_DEBUG("VisualManager Initialization started.");

	m_graphicsManager = settings.graphicsManager;

	//SpriteComponent::registerType(detail::EngineDomain::engineContext());
	EngineDomain::registerType<SpriteComponent>();
	EngineDomain::registerType<SpriteFrame>();
    EngineDomain::registerType<SpriteSheet>();
	EngineDomain::registerType<VisualComponent>();
	EngineDomain::registerType<SpriteComponent>();

    LN_LOG_DEBUG("VisualManager Initialization ended.");
}

void VisualManager::dispose()
{
}

} // namespace detail
} // namespace ln

