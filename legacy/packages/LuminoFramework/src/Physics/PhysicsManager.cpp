#include "Internal.hpp"
#include <LuminoFramework/Physics/PhysicsWorld.hpp>
#include <LuminoFramework/Physics/PhysicsWorld2D.hpp>
#include "PhysicsManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// PhysicsManager

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::init(const Settings& settings)
{
    LN_LOG_DEBUG("PhysicsManager Initialization started.");

    LN_LOG_DEBUG("PhysicsManager Initialization ended.");
}

void PhysicsManager::dispose()
{
}

void PhysicsManager::setActivePhysicsWorld(PhysicsWorld* value)
{
    m_activePhysicsWorld = value;
}

const Ref<PhysicsWorld>& PhysicsManager::activePhysicsWorld() const
{
    return m_activePhysicsWorld;
}

void PhysicsManager::setActivePhysicsWorld2D(PhysicsWorld2D* value)
{
    m_activePhysicsWorld2D = value;
}

const Ref<PhysicsWorld2D>& PhysicsManager::activePhysicsWorld2D() const
{
    return m_activePhysicsWorld2D;
}

} // namespace detail
} // namespace ln

