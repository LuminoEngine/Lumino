
#pragma once
#include "../Component.hpp"
#include "../../Physics/PhysicsWorld2D.hpp"

namespace ln {
class Collision2D;
class RigidBody2DComponent;

using Collision2DEventHandler = Delegate<void(Collision2D*)>;

class Collision2D
    : public Object
{
public:
	/** 自分自身と衝突している他の WorldObject */
    WorldObject* worldObject() const { return m_worldObject; }

    /** 自分自身と衝突している他の PhysicsObject2D */
    PhysicsObject2D* physicsObject() const { return m_physicsObject; }

LN_CONSTRUCT_ACCESS:
    Collision2D();
    virtual ~Collision2D() = default;
    void init(WorldObject* worldObject/*, RigidBody2DComponent* component*/, PhysicsObject2D* physicsObject);

private:
    WorldObject* m_worldObject;
    PhysicsObject2D* m_physicsObject;
};

class RigidBody2DComponent
	: public Component
	, protected detail::IPhysicsObject2DEventListener
{
public:
	static Ref<RigidBody2DComponent> create();

    void setVelocity(const Vector2& value) { m_body->setVelocity(value); }
    void setVelocity(float x, float y) { setVelocity(Vector2(x, y)); }

    const Vector2& velocity() const { return m_body->velocity(); }

    void setMass(float value);

    /** キネマティックモードを設定します。 キネマティックモードでは、剛体に力はかかりません。 */
    void setKinematic(bool value) { m_body->setKinematic(value); }

    /** 摩擦係数を設定します。 */
    void setFriction(float value) { m_body->setFriction(value); }

    /** 反発係数を設定します。 */
    void setRestitution(float value) { m_body->setRestitution(value); }

    void addCollisionShape(CollisionShape2D* shape);

    void setFixedRotation(bool value) { m_body->setFixedRotation(value); }

    /** 衝突グループを設定します。デフォルトは 0x00000001 で、0番のグループに属することを示します。 */
    void setCollisionGroup(uint32_t value) { m_body->setCollisionGroup(value); }

    /** 衝突グループマスクを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroupMask(uint32_t value) { m_body->setCollisionGroupMask(value); }

    /** 重心に力を加えます。 */
    void applyForce(const Vector2& force) { m_body->applyForce(force); }

    /** 指定したローカル位置に力を加えます。 */
    void applyForce(const Vector2& force, const Vector2& localPosition) { m_body->applyForce(force, localPosition); }

    /** 重心に衝撃を与えます。 */
    void applyImpulse(const Vector2& impulse) { m_body->applyImpulse(impulse); }

    /** 指定したローカル位置に衝撃を与えます。 */
    void applyImpulse(const Vector2& impulse, const Vector2& localPosition) { m_body->applyImpulse(impulse, localPosition); }

    /** トルクをかけます。 */
    void applyTorque(float torque) { m_body->applyTorque(torque); }

    /** トルク衝撃を与えます。 */
    void applyTorqueImpulse(float torque) { m_body->applyTorqueImpulse(torque); }

    /** このコンポーネントと関連づいている RigidBody2D を取得します。 */
    RigidBody2D* rigidBody() const;
    

    /** onTriggerEnter イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnCollisionEnter(Ref<Collision2DEventHandler> handler);

    /** onTriggerLeave イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnCollisionLeave(Ref<Collision2DEventHandler> handler);

    /** onTriggerStay イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnCollisionStay(Ref<Collision2DEventHandler> handler);

LN_CONSTRUCT_ACCESS:
	RigidBody2DComponent();
	virtual ~RigidBody2DComponent() = default;
	void init();
	virtual void onDispose(bool explicitDisposing) override;

protected:
	virtual void onBeforeStepSimulation() override;
	virtual void onAfterStepSimulation() override;
    virtual void onCollisionEnter(PhysicsObject2D* otherObject, ContactPoint2D* contact) override;
    virtual void onCollisionLeave(PhysicsObject2D* otherObject, ContactPoint2D* contact) override;
    virtual void onCollisionStay(PhysicsObject2D* otherObject, ContactPoint2D* contact) override;

private:
	Ref<RigidBody2D> m_body;
    Event<Collision2DEventHandler> m_onCollisionEnter;
    Event<Collision2DEventHandler> m_onCollisionLeave;
    Event<Collision2DEventHandler> m_onCollisionStay;
};

class TriggerBody2DComponent
    : public Component
    , protected detail::IPhysicsObject2DEventListener
{
public:
    static Ref<TriggerBody2DComponent> create();

    void addCollisionShape(CollisionShape2D* shape);

    /** 衝突グループマスクを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroupMask(uint32_t value) { m_body->setCollisionGroupMask(value); }



    /** onTriggerEnter イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnCollisionEnter(Ref<Collision2DEventHandler> handler);

    /** onTriggerLeave イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnCollisionLeave(Ref<Collision2DEventHandler> handler);

    /** onTriggerStay イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnCollisionStay(Ref<Collision2DEventHandler> handler);

LN_CONSTRUCT_ACCESS:
    TriggerBody2DComponent();
    virtual ~TriggerBody2DComponent() = default;
    void init();
    virtual void onDispose(bool explicitDisposing) override;

protected:
    virtual void onBeforeStepSimulation() override;
    virtual void onAfterStepSimulation() override;
    virtual void onCollisionEnter(PhysicsObject2D* otherObject, ContactPoint2D* contact) override;
    virtual void onCollisionLeave(PhysicsObject2D* otherObject, ContactPoint2D* contact) override;
    virtual void onCollisionStay(PhysicsObject2D* otherObject, ContactPoint2D* contact) override;

private:
    Ref<TriggerBody2D> m_body;
    Event<Collision2DEventHandler> m_onCollisionEnter;
    Event<Collision2DEventHandler> m_onCollisionLeave;
    Event<Collision2DEventHandler> m_onCollisionStay;
};

} // namespace ln
