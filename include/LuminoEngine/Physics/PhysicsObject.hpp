#pragma once
#include "Common.hpp"

namespace ln {
class PhysicsWorld;
class CollisionShape;

enum class PhysicsObjectType
{
    RigidBody,
    SoftBody,
    //CollisionBody,
    //Joint,
};

class PhysicsObject
	: public Object
{
public:
    PhysicsWorld* physicsWorld() const;

protected:
    virtual void onBeforeStepSimulation();
    virtual void onAfterStepSimulation();

LN_CONSTRUCT_ACCESS:
    PhysicsObject(PhysicsObjectType type);
    virtual ~PhysicsObject();
    void initialize();


private:
    void setPhysicsWorld(PhysicsWorld* owner);
    PhysicsObjectType physicsObjectType() const { return m_resourceType; }
    //bool isRemovingFromWorld() const { return m_removingFromWorld; }
    //void setRemovingFromWorld(bool value) { m_removingFromWorld = value; }

    PhysicsObjectType m_resourceType;
    PhysicsWorld* m_ownerWorld;
    bool m_removingFromWorld;

    friend class PhysicsWorld;
};

} // namespace ln

