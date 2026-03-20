#include "../Internal.hpp"
#include <LuminoFramework/Scene/Physics/Collision.hpp>
#include <LuminoFramework/Scene/WorldObject.hpp>

namespace ln {

//==============================================================================
// Collision

LN_OBJECT_IMPLEMENT(Collision, Object) {}

Collision::Collision()
    : m_worldObject(nullptr)
    , m_physicsObject(nullptr) {
}

void Collision::init() {
    LN_NOTIMPLEMENTED();
}

void Collision::init(WorldObject* worldObject, PhysicsObject* physicsObject) {
    Object::init();
    m_worldObject = worldObject;
    m_physicsObject = physicsObject;
}

} // namespace ln
