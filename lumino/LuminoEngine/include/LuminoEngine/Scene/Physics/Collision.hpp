
#pragma once
#include "../Common.hpp"
#include "../../Physics/RigidBody.hpp"
#include "../../Physics/TriggerBody.hpp"

namespace ln {
class PhysicsObject;

/**
 * Collision
 */
LN_CLASS()
class Collision
    : public Object
{
public:
	/** 自分自身と衝突している他の WorldObject */
    LN_METHOD(Property)
    WorldObject* worldObject() const { return m_worldObject; }

    /** 自分自身と衝突している他の PhysicsObject */
    PhysicsObject* physicsObject() const { return m_physicsObject; }

LN_CONSTRUCT_ACCESS:
    Collision();
    virtual ~Collision() = default;
    void init(WorldObject* worldObject, PhysicsObject* physicsObject);

private:
    WorldObject* m_worldObject;
    PhysicsObject* m_physicsObject;
};

} // namespace ln
