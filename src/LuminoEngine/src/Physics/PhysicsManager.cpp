
#include "Internal.hpp"
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
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
    auto ttest = newObject<PhysicsWorld>();
}

void PhysicsManager::dispose()
{
}

} // namespace detail
} // namespace ln

