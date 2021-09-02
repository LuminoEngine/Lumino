
/*
 * TODO: 後々、3D と 2D は抽象化して統合していいと思う。
 * もしかしたら body や shape は 2D つけたクラス名の方がいいかもしれないけど、world はその必要はないだろう。
 */

#pragma once
#include <memory>
#include "Common.hpp"
#include <LuminoEngine/Graphics/GeometryStructs.hpp>

class b2Shape;
class b2PolygonShape;
class b2ChainShape;
class b2EdgeShape;
class b2Body;
class b2Fixture;
class b2World;
class b2FixtureDef;
class b2Joint;
class b2DistanceJoint;

namespace ln {
class RenderingContext;
class PhysicsObject2D;
class PhysicsWorld2D;
class PhysicsWorld2DDebugDraw; // TODO: detail
class LocalContactListener; // TODO: detail
class ContactPoint2D;
class WorldObject;

using Contact2DEventHandler = Delegate<void(PhysicsObject2D*, ContactPoint2D*)>;
using Trigger2DEventHandler = Delegate<void(PhysicsObject2D*)>;

/** 2D 物理演算用の衝突判定形状です。 */
class CollisionShape2D
	: public Object
{
public:
	/** この形状のローカルオフセットを設定します。 */
	void setPosition(const Vector2& value) { m_position = value; dirty(); }

	/** この形状のローカルオフセットを取得します。 */
	const Vector2& position() const { return m_position; }

	/** この形状のローカルの回転角度を設定します。(単位: radian) */
	void setRotation(float value) { m_rotation = value; dirty(); }

	/** この形状のローカルの回転角度を取得します。(単位: radian) */
	float rotation() const { return m_rotation; }

public:	// TODO: internal
	//virtual b2Shape* resolveBox2DShape() = 0;
	virtual void resolveBox2DShape(b2Body* targetBody, const b2FixtureDef& baseFixtureDef, float mass) = 0;

LN_CONSTRUCT_ACCESS:
	CollisionShape2D();
	virtual ~CollisionShape2D() = default;
	void init();

protected:
	void dirty() { m_dirty = true; }
	void clearDirty() { m_dirty = false; }
	bool isDirty() const { return m_dirty; }

private:
	Vector2 m_position;
	float m_rotation;
	bool m_dirty;
};

/** 四角形の衝突判定形状です。 */
class BoxCollisionShape2D
	: public CollisionShape2D
{
public:
	/** サイズ (1, 1) の BoxCollisionShape2D を作成します。 */
	static Ref<BoxCollisionShape2D> create();

	/** サイズを指定して BoxCollisionShape2D を作成します。 */
    static Ref<BoxCollisionShape2D> create(const Size& size);

	/** サイズを指定して BoxCollisionShape2D を作成します。 */
    static Ref<BoxCollisionShape2D> create(float width, float height);

	/** サイズを設定します。 */
	void setSize(const Size& value) { m_size = value; dirty(); }

	/** サイズを取得します。 */
	const Size& size() const { return m_size; }

protected:
	virtual void resolveBox2DShape(b2Body* targetBody, const b2FixtureDef& baseFixtureDef, float mass) override;

LN_CONSTRUCT_ACCESS:
	BoxCollisionShape2D();
	virtual ~BoxCollisionShape2D() = default;
	void init();
	void init(const Size& size);
    void init(float width, float height);

private:
	std::unique_ptr<b2PolygonShape> m_shape;
	Size m_size;
};

/**
 * 頂点により定義される、連続する任意の線分の衝突判定形状です。
 *
 * エッジは衝突判定の境界を表すものであるため、ボリュームを持ちません。そのため、エッジとエッジの衝突はできません。
 * また、エッジはポリゴンのように閉じている必要はありません。
 */
class EdgeCollisionShape2D
    : public CollisionShape2D
{
public:
    /** EdgeCollisionShape2D を作成します。 */
    static Ref<EdgeCollisionShape2D> create();

    void addPoint(const Vector2& point);
    void addPoint(float x, float y) { addPoint(Vector2(x, y)); }

    /** 始点と終点を自動的に接続するかどうかを設定します。(default: true) */
    bool setLoopEnabled(bool value);

protected:
	virtual void resolveBox2DShape(b2Body* targetBody, const b2FixtureDef& baseFixtureDef, float mass) override;

LN_CONSTRUCT_ACCESS:
    EdgeCollisionShape2D();
    virtual ~EdgeCollisionShape2D() = default;
    void init();

private:
    std::unique_ptr<b2ChainShape> m_shape;
    std::vector<Vector2> m_points;
    bool m_loopEnabled;
};

// 独立した Line のリスト。Strip ではない。主にメモリ効率のための使用する
class EdgeListCollisionShape2D
	: public CollisionShape2D
{
public:
	void addLine(const Vector2& p1, const Vector2& p2);

protected:
	virtual void resolveBox2DShape(b2Body* targetBody, const b2FixtureDef& baseFixtureDef, float mass) override;

LN_CONSTRUCT_ACCESS:
	EdgeListCollisionShape2D();
	virtual ~EdgeListCollisionShape2D() = default;
	void init();

private:
	std::vector<std::unique_ptr<b2EdgeShape>> m_shapes;
	std::vector<std::pair<Vector2, Vector2>> m_points;
};


class PhysicsObject2D
	: public Object
{
public:
	PhysicsWorld2D* physicsWorld() const { return m_ownerWorld; }

	const Vector2& position() const { return m_position; }

	/** このオブジェクトが属している PhysicsWorld2D からこのオブジェクトを除外します。 */
	void removeFromPhysicsWorld();

    /** onTriggerEnter イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
    Ref<EventConnection> connectOnCollisionEnter(Ref<Contact2DEventHandler> handler);

    /** onTriggerLeave イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnCollisionLeave(Ref<Contact2DEventHandler> handler);

    /** onTriggerStay イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnCollisionStay(Ref<Contact2DEventHandler> handler);


public: // TODO: internal
	Vector2 m_position;
	b2Body* m_body;
    void setEventListener(detail::IPhysicsObject2DEventListener* listener) { m_listener = listener; }
    void setOwnerData(void* data) { m_ownerData = data; }
    void* ownerData() const { return m_ownerData; }
    void setCollisionEmitter(Object* value) { m_collisionEmitter = value; }
    const Ref<Object>& collisionEmitter() const { return m_collisionEmitter; }

protected:
    // TODO: internal
    //detail::IPhysicsObject2DEventListener* eventListener() const { return m_listener; }

    /** 他の PhysicsObject2D が、この CollisionBody との接触を開始したときに呼び出されます。*/
    virtual void onCollisionEnter(PhysicsObject2D* otherObject, ContactPoint2D* contact);

    /** 他の PhysicsObject2D が、この CollisionBody との接触を終了したときに呼び出されます。*/
    virtual void onCollisionLeave(PhysicsObject2D* otherObject, ContactPoint2D* contact);

    /** 他の PhysicsObject2D が、この Collider との接触している間呼び出されます。*/
    virtual void onCollisionStay(PhysicsObject2D* otherObject, ContactPoint2D* contact);

	virtual void onBeforeStepSimulation();
	virtual void onAfterStepSimulation();
	virtual void onRemoveFromPhysicsWorld();

	void attemptAddToActiveWorld();

LN_CONSTRUCT_ACCESS:
	PhysicsObject2D();
	virtual ~PhysicsObject2D() = default;
	void init();

private:
    void beginContact(PhysicsObject2D* otherObject);
    void endContact(PhysicsObject2D* otherObject);

	PhysicsWorld2D* m_ownerWorld;
    List<PhysicsObject2D*> m_contactBodies;
    detail::IPhysicsObject2DEventListener* m_listener;
    Event<Contact2DEventHandler> m_onCollisionEnter;
    Event<Contact2DEventHandler> m_onCollisionLeave;
    Event<Contact2DEventHandler> m_onCollisionStay;
    void* m_ownerData;
    Ref<Object> m_collisionEmitter;

	friend class PhysicsWorld2D;
    friend class LocalContactListener;
};

/**
 * 他の物理オブジェクトが領域に侵入したことを検出するためのオブジェクトです。
 * TriggerBody は物理演算に影響を与えません。
 */
class TriggerBody2D
    : public PhysicsObject2D
{
public:

    void addCollisionShape(CollisionShape2D* shape);

    /** 衝突グループを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroup(uint32_t value);

	/** 衝突グループマスクを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
	void setCollisionGroupMask(uint32_t value);

    /** 位置を設定します。(default: 0, 0) */
    void setPosition(const Vector2& value);
    void setPosition(const Vector3& value) { return setPosition(value.xy()); }


    /** 回転角度を設定します。(unit: radian) */
    void setRotation(float value);

    /** 回転角度を取得します。(unit: radian) */
    float rotation() const { return m_rotation; }

    //
    ///** onTriggerEnter イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
    //EventConnection connectOnTriggerEnter(Trigger2DEventHandler handler);

    ///** onTriggerLeave イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
    //EventConnection connectOnTriggerLeave(Trigger2DEventHandler handler);

    ///** onTriggerStay イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
    //EventConnection connectOnTriggerStay(Trigger2DEventHandler handler);

protected:
    virtual void onBeforeStepSimulation() override;
    virtual void onRemoveFromPhysicsWorld() override;

    ///** 他の PhysicsObject2D が、この CollisionBody との接触を開始したときに呼び出されます。*/
    //virtual void onTriggerEnter(PhysicsObject2D* otherObject);

    ///** 他の PhysicsObject2D が、この CollisionBody との接触を終了したときに呼び出されます。*/
    //virtual void onTriggerLeave(PhysicsObject2D* otherObject);

    ///** 他の PhysicsObject2D が、この Collider との接触している間呼び出されます。*/
    //virtual void onTriggerStay(PhysicsObject2D* otherObject);
    
LN_CONSTRUCT_ACCESS:
    TriggerBody2D();
	virtual ~TriggerBody2D() = default;
	void init();

private:
    void removeBodyFromBox2DWorld();

    enum DirtyFlags
    {
        DirtyFlags_None = 0,
        DirtyFlags_Shapes = 1 << 0,
		DirtyFlags_Group = 1 << 1,
        //DirtyFlags_Position = 1 << 2,
        //DirtyFlags_Rotation = 1 << 3,
        DirtyFlags_All = 0xFFFF,
    };

    //std::vector<b2Fixture*> m_fixtures;
    std::vector<Ref<CollisionShape2D>> m_shapes;
    uint32_t m_group;
	uint32_t m_groupMask;
    float m_rotation;
    uint32_t m_dirtyFlags;

    //Event<Trigger2DEventHandler> m_onTriggerEnter;
    //Event<Trigger2DEventHandler> m_onTriggerLeave;
    //Event<Trigger2DEventHandler> m_onTriggerStay;



};

class RigidBody2D
	: public PhysicsObject2D
{
public:
	/** 質量が 0 である RigidBody を作成します。形状は addCollisionShape() で追加してください。 */
	static Ref<RigidBody2D> create();

    /** 指定した形状で、質量が 0 である静的な RigidBody を作成します。 */
    static Ref<RigidBody2D> create(CollisionShape2D* shape);

	/** 位置を設定します。(default: 0, 0) */
	void setPosition(const Vector2& value);
    void setPosition(const Vector3& value) { return setPosition(value.xy()); }

    const Vector2& position() const { return m_position; }

    /** 速度を設定します。 (unit: m/s) */
    void setVelocity(const Vector2& value);
    const Vector2& velocity() const { return m_velocity; }

	/** 回転角度を設定します。(unit: radian) */
	void setRotation(float value) { m_rotation = value; }

	/** 回転角度を取得します。(unit: radian) */
	float rotation() const { return m_rotation; }


	/** 質量を設定します。0 を設定すると静的なボディとなります。(unit: kilograms(kg))(default: 0.0) */
	void setMass(float value);

    /** 摩擦係数を設定します。 */
    void setFriction(float value);

    /** 反発係数を設定します。 */
    void setRestitution(float value);



    void setFixedRotation(bool value) { m_fixedRotation = value; }

	/** キネマティックモードを設定します。 キネマティックモードでは、剛体に力はかかりません。 */
	void setKinematic(bool value);

    bool isDynamic() const { return !isStatic() && !isKinematic(); }
    bool isStatic() const { return m_mass == 0.0f; }
    bool isKinematic() const { return m_kinematic; }

	void addCollisionShape(CollisionShape2D* shape);



    /** 衝突グループを設定します。デフォルトは 0x00000001 で、0番のグループに属することを示します。 */
    void setCollisionGroup(uint32_t value);

    /** 衝突グループマスクを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroupMask(uint32_t value);


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

	//std::vector<b2Fixture*> m_fixtures;
	std::vector<Ref<CollisionShape2D>> m_shapes;
	float m_rotation;
    Vector2 m_velocity;
	float m_mass;
    float m_friction;
    float m_restitution;
    uint32_t m_group;
    uint32_t m_groupMask;
	bool m_kinematic;
    bool m_fixedRotation;
    std::vector<ApplyCommand> m_applyCommands;  // TODO: linerallocator にしたい

    bool m_modifyVelocityInSim = false; // TODO: test
};

class Joint2D
	: public Object
{
public:
	/** Get the physics world. */
	PhysicsWorld2D* physicsWorld() const { return m_world; }

	/** Remove the joint from the world. */
	void removeFormWorld();


LN_CONSTRUCT_ACCESS:
	Joint2D();
	virtual ~Joint2D() = default;
	bool init();
	virtual void onDispose(bool explicitDisposing) override;
	void attemptAddToActiveWorld();

protected:	// TODO: internal
	void setJoint(b2Joint* joint);
	b2Joint* joint() const { return m_joint; }
	virtual bool createJoint() = 0;
	void destroyJoint();
	void dirtyCreation() { m_creationDirty = true; }
	bool isCreationDirty() const { return m_creationDirty; }

	PhysicsWorld2D* m_world;
	b2Joint* m_joint;
	bool m_creationDirty;

	friend class PhysicsWorld2D;
	friend class RigidBody2D;
};


/*
 * SpringJoint2D
 *
 * AnchorPoint は　Body のローカル座標上の点です。(0, 0) は Body の原点 (中心点) です。
 */
class SpringJoint2D
	: public Joint2D
{
public:
	static Ref<SpringJoint2D> create();
	static Ref<SpringJoint2D> create(PhysicsObject2D* bodyA, PhysicsObject2D* bodyB);

	/** Body を設定します。 */
	void setBodyA(PhysicsObject2D* value);

	/** Body を取得します。 */
	PhysicsObject2D* bodyA() const { return m_bodyA; }

	/** Body を設定します。 */
	void setBodyB(PhysicsObject2D* value);

	/** Body を取得します。 */
	PhysicsObject2D* bodyB(const Vector2& value) const { return m_bodyB; }

	/** BodyA 側の AnchorPoint を設定します。(default: (0, 0)) */
	void setAnchorA(const Vector2& value);

	/** BodyA 側の AnchorPoint を取得します。 */
	const Vector2& anchorA() const { return m_anchorA; }

	/** BodyB 側の AnchorPoint を設定します。(default: (0, 0)) */
	void setAnchorB(const Vector2& value);

	/** BodyB 側の AnchorPoint を取得します。 */
	const Vector2& anchorB() const { return m_anchorB; }

	/** ばねの長さを設定します。(default: 0.0)
	 *
	 * これはばねの自然状態の長さで、2 つの Body 間の距離をこの値に保とうとします。
	 * 値が 0.0 以下である場合、Joint 作成時の 2 つの Body 間の距離を使用します。
	 */
	void setLength(float value);

	/** ばねの長さを取得します。 */
	float length() const { return m_length; }


LN_CONSTRUCT_ACCESS:
	SpringJoint2D();
	virtual ~SpringJoint2D() = default;
	bool init();
	bool init(PhysicsObject2D* bodyA, const Vector2& anchor1, PhysicsObject2D* bodyB, const Vector2& anchor2, float stiffness, float damping);

private:
	virtual bool createJoint() override;
	//virtual void destroyJoint() override;

	//b2DistanceJoint* m_joint;
	PhysicsObject2D* m_bodyA;
	PhysicsObject2D* m_bodyB;
	Vector2 m_anchorA;
	Vector2 m_anchorB;
	float m_stiffness;
	float m_damping;
	float m_length;
};


class RopeJoint2D
	: public Joint2D
{
public:
	static Ref<RopeJoint2D> create();
	static Ref<RopeJoint2D> create(PhysicsObject2D* bodyA, PhysicsObject2D* bodyB);

LN_CONSTRUCT_ACCESS:
	RopeJoint2D();
	virtual ~RopeJoint2D() = default;
	bool init();
	bool init(PhysicsObject2D* bodyA, const Vector2& anchorA, PhysicsObject2D* bodyB, const Vector2& anchorB);

private:
	virtual bool createJoint() override;
	//virtual void destroyJoint() override;

	//b2DistanceJoint* m_joint;
	PhysicsObject2D* m_bodyA;
	PhysicsObject2D* m_bodyB;
	Vector2 m_anchorA;
	Vector2 m_anchorB;
};


struct RaycastResult2D
{
    /** レイがヒットした PhysicsObject2D がアタッチされている WorldObject */
    Ref<WorldObject> worldObject;

    /** レイがヒットした PhysicsObject2D */
    Ref<PhysicsObject2D> physicsObject;

    /** レイがヒットしたワールド上の位置 */
    Vector2 point;

    /** レイがヒットしたサーフェスの法線 */
    Vector2 normal;

    /** レイの原点から衝突点までの距離 */
    float distance;
};

class PhysicsWorld2D
	: public Object
{
public:

	/** Change the global gravity. */
	void setGravity(const Vector2& value) { m_gravity = value; }

	/** Get the global gravity. */
	Vector2 gravity() const { return m_gravity; }
	
	bool raycast(const Vector3& origin, const Vector2& direction, float maxDistance/* = Math::Inf*/, uint32_t layerMask /*= 0xFFFFFFFF*/, bool queryTrigger /*= false*/, RaycastResult2D* outResult = nullptr);
    bool raycast(const Vector3& origin, const Vector2& direction, float maxDistance, uint32_t layerMask, RaycastResult2D* outResult = nullptr) { return raycast(origin, direction, maxDistance, layerMask, false, outResult); }


	void addPhysicsObject(PhysicsObject2D* physicsObject);
	void removePhysicsObject(PhysicsObject2D* physicsObject);
	void addJoint(Joint2D* joint);
	void removeJoint(Joint2D* joint);

	void stepSimulation(float elapsedSeconds);
	void renderDebug(RenderingContext* context);

public: // TODO:
	b2World* box2DWorld() const { return m_world; }

LN_CONSTRUCT_ACCESS:
	PhysicsWorld2D();
	virtual ~PhysicsWorld2D() = default;
	void init();
    virtual void onDispose(bool explicitDisposing) override;

public: // TODO:
	void removeInternal(PhysicsObject2D* physicsObject);
	void addJointInternal(Joint2D* joint);
	void removeJointInternal(Joint2D* joint);
	void updateList();
	void updateBodyAndJointList();

	Vector2 m_gravity;

	b2World* m_world;
    std::unique_ptr<LocalContactListener> m_contactListener;
	std::unique_ptr<PhysicsWorld2DDebugDraw> m_debugDraw;

	List<Ref<PhysicsObject2D>> m_objects;
	std::vector<Ref<PhysicsObject2D>> m_delayAddBodies;
	std::vector<PhysicsObject2D*> m_removeList;

	List<Ref<Joint2D>> m_joints;
	std::vector<Ref<Joint2D>> m_delayAddJoints;
	std::vector<Joint2D*> m_delayRemoveJoints;

	bool m_inStepSimulation;
};

/** 2D 物理システムのグローバル設定とヘルパー関数です。 */
class Physics2D
{
public:
    /** アクティブな物理ワールドに対してレイキャストを行います。 */
    static bool raycast(const Vector3& origin, const Vector2& direction, float maxDistance, uint32_t layerMask, bool queryTrigger, RaycastResult2D* outResult = nullptr);

    /** アクティブな物理ワールドに対してレイキャストを行います。 */
    static bool raycast(const Vector3& origin, const Vector2& direction, float maxDistance, uint32_t layerMask, RaycastResult2D* outResult = nullptr);

};

} // namespace ln

