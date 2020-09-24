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
    /** この PhysicsObject が含まれている PhysicsWorld を取得します。 */
    PhysicsWorld* physicsWorld() const;

    /** この PhysicsObject が含まれている PhysicsWorld からこの PhysicsObject を除外します。 */
    void removeFromPhysicsWorld();

    const List<PhysicsObject*>& contactBodies() const { return m_contactBodies; }

public: // TODO: internal
    void setEventListener(detail::IPhysicsObjectEventListener* listener) { m_listener = listener; }
    void setOwnerData(void* data) { m_ownerData = data; }
    void* ownerData() const { return m_ownerData; }

    PhysicsObjectType physicsObjectType() const { return m_resourceType; }
protected:
    /** 他の PhysicsObject が、この PhysicsObject との接触を開始したときに呼び出されます。*/
    virtual void onCollisionEnter(PhysicsObject* otherObject, ContactPoint* contact);

    /** 他の PhysicsObject が、この PhysicsObject との接触を終了したときに呼び出されます。*/
    virtual void onCollisionLeave(PhysicsObject* otherObject, ContactPoint* contact);

    /** 他の PhysicsObject が、この PhysicsObject との接触している間呼び出されます。*/
    virtual void onCollisionStay(PhysicsObject* otherObject, ContactPoint* contact);

    // 各プロパティの遅延評価を実行する。WorldObject からの FeedForward はここでは行わない
    virtual void onPrepareStepSimulation();
    virtual void onAfterStepSimulation();


LN_CONSTRUCT_ACCESS:
    PhysicsObject(PhysicsObjectType type);
    virtual ~PhysicsObject();
    void init();


private:
    virtual void removeFromBtWorld() = 0;

    void setPhysicsWorld(PhysicsWorld* owner);
    void beginContact(PhysicsObject* otherObject);
    void endContact(PhysicsObject* otherObject);

    PhysicsObjectType m_resourceType;
    PhysicsWorld* m_ownerWorld;
    bool m_removing;

    List<PhysicsObject*> m_contactBodies;
    detail::IPhysicsObjectEventListener* m_listener = nullptr;
    void* m_ownerData = nullptr;

    friend class PhysicsWorld;
};

} // namespace ln

