#pragma once
#include "Common.hpp"

namespace ln {
class PhysicsWorld;
class CollisionShape;

enum class PhysicsObjectType
{
    RigidBody,
    SoftBody,
    TriggerBody,
    Joint,
};

class PhysicsObject
	: public Object
{
public:
    PhysicsWorld* physicsWorld() const;

    /** このオブジェクトが属している PhysicsWorld2D からこのオブジェクトを除外します。 */
    void removeFromPhysicsWorld();

public: // TODO: internal
    void setEventListener(detail::IPhysicsObjectEventListener* listener) { m_listener = listener; }
    void setOwnerData(void* data) { m_ownerData = data; }
    void* ownerData() const { return m_ownerData; }

protected:
    /** 他の PhysicsObject が、この PhysicsObject との接触を開始したときに呼び出されます。*/
    virtual void onCollisionEnter(PhysicsObject* otherObject, ContactPoint* contact);

    /** 他の PhysicsObject が、この PhysicsObject との接触を終了したときに呼び出されます。*/
    virtual void onCollisionLeave(PhysicsObject* otherObject, ContactPoint* contact);

    /** 他の PhysicsObject が、この PhysicsObject との接触している間呼び出されます。*/
    virtual void onCollisionStay(PhysicsObject* otherObject, ContactPoint* contact);


    virtual void onBeforeStepSimulation();
    virtual void onAfterStepSimulation();

    virtual void onRemoveFromPhysicsWorld() = 0;

LN_CONSTRUCT_ACCESS:
    PhysicsObject(PhysicsObjectType type);
    virtual ~PhysicsObject();
    void init();


private:
    void setPhysicsWorld(PhysicsWorld* owner);
    PhysicsObjectType physicsObjectType() const { return m_resourceType; }
    //bool isRemovingFromWorld() const { return m_removingFromWorld; }
    //void setRemovingFromWorld(bool value) { m_removingFromWorld = value; }
    void beginContact(PhysicsObject* otherObject);
    void endContact(PhysicsObject* otherObject);

    PhysicsObjectType m_resourceType;
    PhysicsWorld* m_ownerWorld;
    bool m_removingFromWorld;

    List<PhysicsObject*> m_contactBodies;
    detail::IPhysicsObjectEventListener* m_listener = nullptr;
    void* m_ownerData = nullptr;

    friend class PhysicsWorld;
};

} // namespace ln

