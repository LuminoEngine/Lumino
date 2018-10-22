
#include "Internal.hpp"
#include "VisualManager.hpp"

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

void VisualManager::initialize(const Settings& settings)
{
	m_graphicsManager = settings.graphicsManager;
}

void VisualManager::dispose()
{
}

} // namespace detail
} // namespace ln

