
#include "Internal.hpp"
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/Joint.hpp>

namespace ln {

Joint::Joint()
    : m_world(nullptr)
    , m_removing(false)
{
}

void Joint::removeFromPhysicsWorld()
{
    if (LN_REQUIRE(m_world)) return;
    m_world->removeJoint(this);
}

} // namespace ln
