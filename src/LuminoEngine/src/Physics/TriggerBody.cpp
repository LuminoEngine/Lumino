
#include "Internal.hpp"
#include <LuminoEngine/Physics/TriggerBody.hpp>
#include "PhysicsManager.hpp"
#include "BulletUtils.hpp"

namespace ln {

//==============================================================================
// TriggerBody

Ref<TriggerBody> TriggerBody::create(CollisionShape* shape)
{
    return makeObject<TriggerBody>(shape);
}

TriggerBody::TriggerBody()
    : PhysicsObject(PhysicsObjectType::TriggerBody)
{
}

void TriggerBody::init()
{
    Object::init();
}

void TriggerBody::init(CollisionShape* shape)
{
    init();
    addCollisionShape(shape);
}

void TriggerBody::addCollisionShape(CollisionShape* shape)
{
    m_shapeManager.addShape(shape);
}

void TriggerBody::setCollisionGroup(uint32_t value)
{
    if (m_group != value) {
        m_group = value;
        m_dirtyFlags |= DirtyFlags_Group;
    }
}

void TriggerBody::setCollisionGroupMask(uint32_t value)
{
    if (m_groupMask != value) {
        m_groupMask = value;
        m_dirtyFlags |= DirtyFlags_Group;
    }
}

void TriggerBody::setPosition(const Vector3& value)
{
    //if (m_position != value) {
    //    m_position = value;
    //}
}

void TriggerBody::onBeforeStepSimulation()
{
}

void TriggerBody::onAfterStepSimulation()
{
}

} // namespace ln

