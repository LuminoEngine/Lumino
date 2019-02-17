
/*
 * TODO: 後々、3D と 2D は抽象化して統合していいと思う。
 * もしかしたら body や shape は 2D つけたクラス名の方がいいかもしれないけど、world はその必要はないだろう。
 */

#pragma once
#include <memory>
#include "Common.hpp"

class b2Shape;
class b2PolygonShape;
class b2Body;
class b2Fixture;
class b2World;

namespace ln {
class RenderingContext;
class PhysicsObject2D;
class PhysicsWorld2D;
class PhysicsWorld2DDebugDraw; // TODO: detail
class LocalContactListener; // TODO: detail
class ContactPoint2D;

using Collision2DEventHandler = std::function<void(PhysicsObject2D*, ContactPoint2D*)>;
using Trigger2DEventHandler = std::function<void(PhysicsObject2D*)>;

class CollisionShape2D
	: public Object
{
public:

public:	// TODO: internal
	virtual b2Shape* box2DShape() = 0;

LN_CONSTRUCT_ACCESS:
	CollisionShape2D();
	virtual ~CollisionShape2D() = default;
	void init();

private:
};


class BoxCollisionShape2D
	: public CollisionShape2D
{
public:
    static Ref<BoxCollisionShape2D> create(const Vector2& size);
    static Ref<BoxCollisionShape2D> create(float width, float height);

	virtual b2Shape* box2DShape() override;

LN_CONSTRUCT_ACCESS:
	BoxCollisionShape2D();
	virtual ~BoxCollisionShape2D() = default;
	void init(const Vector2& size);
    void init(float width, float height);

private:
	std::unique_ptr<b2PolygonShape> m_shape;
};



class PhysicsObject2D
	: public Object
{
public:
	PhysicsWorld2D* physicsWorld() const { return m_ownerWorld; }


    /** onTriggerEnter イベントの通知を受け取るコールバックを登録します。*/
    LN_METHOD(Event)
    EventConnection connectOnCollisionEnter(Collision2DEventHandler handler);

    /** onTriggerLeave イベントの通知を受け取るコールバックを登録します。*/
    LN_METHOD(Event)
    EventConnection connectOnCollisionLeave(Collision2DEventHandler handler);

    /** onTriggerStay イベントの通知を受け取るコールバックを登録します。*/
    LN_METHOD(Event)
    EventConnection connectOnCollisionStay(Collision2DEventHandler handler);


protected:

    /** 他の PhysicsObject2D が、この CollisionBody との接触を開始したときに呼び出されます。*/
    virtual void onCollisionEnter(PhysicsObject2D* otherObject, ContactPoint2D* contact);

    /** 他の PhysicsObject2D が、この CollisionBody との接触を終了したときに呼び出されます。*/
    virtual void onCollisionLeave(PhysicsObject2D* otherObject, ContactPoint2D* contact);

    /** 他の PhysicsObject2D が、この Collider との接触している間呼び出されます。*/
    virtual void onCollisionStay(PhysicsObject2D* otherObject, ContactPoint2D* contact);

	virtual void onBeforeStepSimulation();
	virtual void onAfterStepSimulation();
	virtual void onRemoveFromPhysicsWorld();

LN_CONSTRUCT_ACCESS:
	PhysicsObject2D();
	virtual ~PhysicsObject2D() = default;
	void init();

private:
	PhysicsWorld2D* m_ownerWorld;
    List<PhysicsObject2D*> m_contactBodies;
    Event<Collision2DEventHandler> m_onCollisionEnter;
    Event<Collision2DEventHandler> m_onCollisionLeave;
    Event<Collision2DEventHandler> m_onCollisionStay;

	friend class PhysicsWorld2D;
    friend class LocalContactListener;
};

class CollisionBody2D
    : public PhysicsObject2D
{
public:
    
    /** onTriggerEnter イベントの通知を受け取るコールバックを登録します。*/
    LN_METHOD(Event)
    EventConnection connectOnTriggerEnter(Trigger2DEventHandler handler);

    /** onTriggerLeave イベントの通知を受け取るコールバックを登録します。*/
    LN_METHOD(Event)
    EventConnection connectOnTriggerLeave(Trigger2DEventHandler handler);

    /** onTriggerStay イベントの通知を受け取るコールバックを登録します。*/
    LN_METHOD(Event)
    EventConnection connectOnTriggerStay(Trigger2DEventHandler handler);

protected:

    /** 他の PhysicsObject2D が、この CollisionBody との接触を開始したときに呼び出されます。*/
    virtual void onTriggerEnter(PhysicsObject2D* otherObject);

    /** 他の PhysicsObject2D が、この CollisionBody との接触を終了したときに呼び出されます。*/
    virtual void onTriggerLeave(PhysicsObject2D* otherObject);

    /** 他の PhysicsObject2D が、この Collider との接触している間呼び出されます。*/
    virtual void onTriggerStay(PhysicsObject2D* otherObject);
    
LN_CONSTRUCT_ACCESS:
    CollisionBody2D();
	virtual ~CollisionBody2D() = default;
	void init();

private:
    Event<Trigger2DEventHandler> m_onTriggerEnter;
    Event<Trigger2DEventHandler> m_onTriggerLeave;
    Event<Trigger2DEventHandler> m_onTriggerStay;
};

class RigidBody2D
	: public PhysicsObject2D
{
public:

    /** 指定した形状で、質量が 0 である静的な RigidBody を作成します。 */
    static Ref<RigidBody2D> create(CollisionShape2D* shape);

	/** 位置を設定します。(default: 0, 0) */
	void setPosition(const Vector2& value);
    void setPosition(const Vector3& value) { return setPosition(value.xy()); }

    const Vector2& position() const { return m_position; }

    /** 速度を設定します。 (unit: m/s) */
    void setVelocity(const Vector2& value);
    const Vector2& velocity() const { return m_velocity; }


	/** 質量を設定します。0 を設定すると静的なボディとなります。(unit: kilograms(kg))(default: 0.0) */
	void setMass(float value);


    void setFixedRotation(bool value) { m_fixedRotation = value; }

	/** キネマティックモードを設定します。 キネマティックモードでは、剛体に力はかかりません。 */
	void setKinematic(bool value);

    bool isDynamic() const { return !isStatic() && !isKinematic(); }
    bool isStatic() const { return m_mass == 0.0f; }
    bool isKinematic() const { return m_kinematic; }

	void addCollisionShape(CollisionShape2D* shape);



    /** 重心に力を加えます。(単位: Newtons (N)) */
    void applyForce(const Vector2& force);

    /** 指定したローカル位置に力を加えます。 */
    void applyForce(const Vector2& force, const Vector2& localPosition);

    /** 重心に衝撃を与えます。 */
    void applyImpulse(const Vector2& impulse);

    /** 指定したローカル位置に衝撃を与えます。 */
    void applyImpulse(const Vector2& impulse, const Vector2& localPosition);

    /** トルクをかけます。 */
    void applyTorque(float torque);

    /** トルク衝撃を与えます。 */
    void applyTorqueImpulse(float torque);


public: // TODO: internal
	void setEventListener(detail::IPhysicsObjectEventListener* listener) { m_listener = listener; }

protected:
	virtual void onBeforeStepSimulation() override;
	virtual void onAfterStepSimulation() override;
	virtual void onRemoveFromPhysicsWorld() override;

LN_CONSTRUCT_ACCESS:
	RigidBody2D();
	virtual ~RigidBody2D() = default;
	void init();
    void init(CollisionShape2D* shape);
	virtual void onDispose(bool explicitDisposing) override;

private:
    enum class ApplyType
    {
        SetVelocity,
        Force,
        Impulse,
        Torque,
        TorqueImpulse,
    };

    struct ApplyCommand
    {
        ApplyType type;
        Vector2 value;
        Vector2 center;
    };

	detail::IPhysicsObjectEventListener* m_listener;
	b2Body* m_body;
	b2Fixture* m_fixture;
	std::vector<Ref<CollisionShape2D>> m_shapes;
	Vector2 m_position;
    Vector2 m_velocity;
	float m_mass;
	bool m_kinematic;
    bool m_fixedRotation;
    std::vector<ApplyCommand> m_applyCommands;  // TODO: linerallocator にしたい
};

class PhysicsWorld2D
	: public Object
{
public:
	void addPhysicsObject(PhysicsObject2D* physicsObject);
	void removePhysicsObject(PhysicsObject2D* physicsObject);
	void stepSimulation(float elapsedSeconds);
	void renderDebug(RenderingContext* context);

public: // TODO:
	b2World* box2DWorld() const { return m_world; }

LN_CONSTRUCT_ACCESS:
	PhysicsWorld2D();
	virtual ~PhysicsWorld2D() = default;
	void init();
    virtual void onDispose(bool explicitDisposing) override;

private:
	b2World* m_world;
    std::unique_ptr<LocalContactListener> m_contactListener;
	std::unique_ptr<PhysicsWorld2DDebugDraw> m_debugDraw;
	List<Ref<PhysicsObject2D>> m_objects;
};

} // namespace ln

