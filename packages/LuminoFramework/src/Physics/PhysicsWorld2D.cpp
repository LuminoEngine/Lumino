
#include "Internal.hpp"
#include <box2d/box2d.h>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/GPU/VertexLayout.hpp>
#include <LuminoEngine/GPU/VertexBuffer.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoFramework/Physics/PhysicsWorld2D.hpp>
#include <LuminoFramework/Scene/Physics/RigidBody2DComponent.hpp>
#include <LuminoFramework/Scene/WorldObject.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "PhysicsManager.hpp"

namespace ln {

static b2Vec2 LnToB2(const Vector2& v) { return b2Vec2(v.x, v.y); }
static Vector2 B2ToLn(const b2Vec2& v) { return Vector2(v.x, v.y); }

//==============================================================================
// CollisionShape2D

CollisionShape2D::CollisionShape2D()
	: m_position(0, 0)
	, m_rotation(0)
	, m_dirty(true)
{
}

void CollisionShape2D::init()
{
	Object::init();
}

//==============================================================================
// BoxCollisionShape2D

Ref<BoxCollisionShape2D> BoxCollisionShape2D::create()
{
	return makeObject_deprecated<BoxCollisionShape2D>();
}

Ref<BoxCollisionShape2D> BoxCollisionShape2D::create(const Size& size)
{
    return makeObject_deprecated<BoxCollisionShape2D>(size);
}

Ref<BoxCollisionShape2D> BoxCollisionShape2D::create(float width, float height)
{
    return makeObject_deprecated<BoxCollisionShape2D>(width, height);
}

BoxCollisionShape2D::BoxCollisionShape2D()
	: m_shape(nullptr)
	, m_size(1, 1)
{
}

void BoxCollisionShape2D::init()
{
	CollisionShape2D::init();
}

void BoxCollisionShape2D::init(const Size& size)
{
	init();
	m_size = size;
}

void BoxCollisionShape2D::init(float width, float height)
{
    init(Size(width, height));
}

void BoxCollisionShape2D::resolveBox2DShape(b2Body* targetBody, const b2FixtureDef& baseFixtureDef, float mass)
{
	if (!m_shape || isDirty())
	{
		// Box2D はシェイプの境界にわずかなマージンを持たせる。
		// このため、2Dワールドを拡大してみると、キャラクターと地面の間に1px分の隙間が見えてしまうようなことがある。
		// 対策として、マージンを打ち消す (b2_linearSlop * 2) ようにしてみる。
		m_shape = std::make_unique<b2PolygonShape>();
		m_shape->SetAsBox(m_size.width * 0.5f - (b2_linearSlop * 2), m_size.height * 0.5f - (b2_linearSlop * 2), LnToB2(position()), rotation());
		clearDirty();
	}

	b2MassData massData;
	m_shape->ComputeMass(&massData, 1);
	float volume = massData.mass;	// ComputeMass に密度 1 で計算すると、質量 = 体積となる

	b2FixtureDef fixtureDef = baseFixtureDef;
	fixtureDef.shape = m_shape.get();
	fixtureDef.density = mass / volume;	// 密度 = 質量 / 体積
	targetBody->CreateFixture(&fixtureDef);
}

//b2Shape* BoxCollisionShape2D::resolveBox2DShape()
//{
//	if (!m_shape || isDirty())
//	{
//        // Box2D はシェイプの境界にわずかなマージンを持たせる。
//        // このため、2Dワールドを拡大してみると、キャラクターと地面の間に1px分の隙間が見えてしまうようなことがある。
//        // 対策として、マージンを打ち消す (b2_linearSlop * 2) ようにしてみる。
//		m_shape = std::make_unique<b2PolygonShape>();
//		m_shape->SetAsBox(m_size.width * 0.5f - (b2_linearSlop * 2), m_size.height * 0.5f - (b2_linearSlop * 2), LnToB2(position()), rotation());
//		clearDirty();
//	}
//	return m_shape.get();
//}


//==============================================================================
// EdgeCollisionShape2D

Ref<EdgeCollisionShape2D> EdgeCollisionShape2D::create()
{
    return makeObject_deprecated<EdgeCollisionShape2D>();
}

EdgeCollisionShape2D::EdgeCollisionShape2D()
    : m_shape(nullptr)
    , m_points()
    , m_loopEnabled(true)
{
}

void EdgeCollisionShape2D::init()
{
    CollisionShape2D::init();
}

void EdgeCollisionShape2D::addPoint(const Vector2& point)
{
    m_points.push_back(point);
    dirty();
}

void EdgeCollisionShape2D::resolveBox2DShape(b2Body* targetBody, const b2FixtureDef& baseFixtureDef, float mass)
{
    if (!m_shape || isDirty())
    {
        m_shape = std::make_unique<b2ChainShape>();
        if (m_loopEnabled) {
            m_shape->CreateLoop(reinterpret_cast<const b2Vec2*>(m_points.data()), m_points.size());
        }
        else {
            //m_shape->CreateChain(reinterpret_cast<const b2Vec2*>(m_points.data()), m_points.size());
			LN_NOTIMPLEMENTED();
        }
        clearDirty();
    }

	b2FixtureDef fixtureDef = baseFixtureDef;
	fixtureDef.shape = m_shape.get();
	fixtureDef.density = 0.0f;	// Edge は体積を持つことはできない
	targetBody->CreateFixture(&fixtureDef);
}

//==============================================================================
// EdgeListCollisionShape2D

EdgeListCollisionShape2D::EdgeListCollisionShape2D()
{
}

void EdgeListCollisionShape2D::init()
{
	CollisionShape2D::init();
}

void EdgeListCollisionShape2D::addLine(const Vector2& p1, const Vector2& p2)
{
	m_points.push_back({p1, p2});
	dirty();
}

void EdgeListCollisionShape2D::resolveBox2DShape(b2Body* targetBody, const b2FixtureDef& baseFixtureDef, float mass)
{
	if (isDirty())
	{
		m_shapes.clear();
		for (auto& pt : m_points) {
			auto shape = std::make_unique<b2EdgeShape>();
			shape->SetTwoSided(LnToB2(pt.first), LnToB2(pt.second));
			m_shapes.push_back(std::move(shape));
		}
		clearDirty();
	}

	for (auto& shape : m_shapes) {
		b2FixtureDef fixtureDef = baseFixtureDef;
		fixtureDef.shape = shape.get();
		fixtureDef.density = 0.0f;	// Edge は体積を持つことはできない
		targetBody->CreateFixture(&fixtureDef);
	}
}

//==============================================================================
// PhysicsObject2D

PhysicsObject2D::PhysicsObject2D()
	: m_ownerWorld(nullptr)
	, m_position()
	, m_body(nullptr)
    , m_listener(nullptr)
    , m_ownerData(nullptr)
{
}

void PhysicsObject2D::init()
{
	Object::init();
}

void PhysicsObject2D::removeFromPhysicsWorld()
{
	if (m_ownerWorld) {
		m_ownerWorld->removePhysicsObject(this);
	}
}

Ref<EventConnection> PhysicsObject2D::connectOnCollisionEnter(Ref<Contact2DEventHandler> handler)
{
    return m_onCollisionEnter.connect(handler);
}

Ref<EventConnection> PhysicsObject2D::connectOnCollisionLeave(Ref<Contact2DEventHandler> handler)
{
    return m_onCollisionLeave.connect(handler);
}

Ref<EventConnection> PhysicsObject2D::connectOnCollisionStay(Ref<Contact2DEventHandler> handler)
{
    return m_onCollisionStay.connect(handler);
}

void PhysicsObject2D::onCollisionEnter(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    m_onCollisionEnter.raise(otherObject, contact);
}

void PhysicsObject2D::onCollisionLeave(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    m_onCollisionLeave.raise(otherObject, contact);
}

void PhysicsObject2D::onCollisionStay(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    m_onCollisionStay.raise(otherObject, contact);
}

void PhysicsObject2D::onBeforeStepSimulation()
{
    if (m_listener) {
        m_listener->onBeforeStepSimulation();
    }
}

void PhysicsObject2D::onAfterStepSimulation()
{
    if (m_listener) {
        m_listener->onAfterStepSimulation();
    }
}

void PhysicsObject2D::onRemoveFromPhysicsWorld()
{
}

void PhysicsObject2D::attemptAddToActiveWorld()
{
	if (auto& activeWorld = detail::EngineDomain::physicsManager()->activePhysicsWorld2D()) {
		activeWorld->addPhysicsObject(this);
	}
}

void PhysicsObject2D::beginContact(PhysicsObject2D* otherObject)
{
    m_contactBodies.add(otherObject);
    onCollisionEnter(otherObject, nullptr);
    if (m_listener) {
        m_listener->onCollisionEnter(otherObject, nullptr);
    }
}

void PhysicsObject2D::endContact(PhysicsObject2D* otherObject)
{
    if (LN_REQUIRE(m_contactBodies.remove(otherObject))) return;
    onCollisionLeave(otherObject, nullptr);
    if (m_listener) {
        m_listener->onCollisionLeave(otherObject, nullptr);
    }
}


//==============================================================================
// TriggerBody2D

TriggerBody2D::TriggerBody2D()
    : m_group(0x0000FFFF)
    , m_groupMask(0x0000FFFF)
	, m_dirtyFlags(DirtyFlags_All)
    , m_rotation(0.0f)
{
}

void TriggerBody2D::init()
{
    PhysicsObject2D::init();
	attemptAddToActiveWorld();
}

void TriggerBody2D::addCollisionShape(CollisionShape2D* shape)
{
    if (LN_REQUIRE(shape)) return;
    m_shapes.push_back(shape);
	m_dirtyFlags |= DirtyFlags_Shapes;
}

void TriggerBody2D::setCollisionGroup(uint32_t value)
{
    if (m_group != value) {
        m_group = value;
        m_dirtyFlags |= DirtyFlags_Group;
    }
}

void TriggerBody2D::setCollisionGroupMask(uint32_t value)
{
	if (m_groupMask != value) {
		m_groupMask = value;
		m_dirtyFlags |= DirtyFlags_Group;
	}
}

void TriggerBody2D::setPosition(const Vector2& value)
{
    if (m_position != value) {
        m_position = value;
        //m_dirtyFlags |= DirtyFlags_Position;
    }
}

void TriggerBody2D::setRotation(float value)
{
    if (m_rotation != value) {
        m_rotation = value;
        //m_dirtyFlags |= DirtyFlags_Rotation;
    }
}

void TriggerBody2D::onBeforeStepSimulation()
{
    PhysicsObject2D::onBeforeStepSimulation();
	if (!physicsWorld()) return;

    if (!m_body || (m_dirtyFlags & DirtyFlags_Shapes) || (m_dirtyFlags & DirtyFlags_Group))
    {
        removeBodyFromBox2DWorld();

		b2World* world = physicsWorld()->box2DWorld();

        // Box2D の制約として、2つの body の衝突検知はどちらかが b2_dynamicBody である場合のみ行われる。
        // つまり、TriggerBody を b2_staticBody または b2_kinematicBody にしてしまうと、
        // 例えば衝突判定用のkinematicなBodyとの衝突検知ができなくなる。
        // （キャラクターの移動と、攻撃のヒットボックスを別のBodyにし、ヒットボックスは物理移動しない(kinematic)ようなケース）
        // isSensor とどの type を一般的に使うべきかはよさげな資料がみつからなかったが、
        // 実際のところは衝突判定したい対象ごとに変えるべきなのだろう。
        // b2_dynamicBody と組み合わせているテストコードはリポジトリに入っていたが、これが一般的なのだろうか。
        // ただ、Lumino としてはその辺の細かいところまでユーザーに意識させたくない。
        // 最適化の余地がなくなるが、実際にゲーム開発していて躓いたところなので、TriggerBody は基本的に全ての type と衝突できる b2_dynamicBody で作成する。
        // また、通常は kinematic としてふるまいたいので 質量0, 重力影響0 で作成している。
		b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
        bodyDef.gravityScale = 0.0f;
		m_body = world->CreateBody(&bodyDef);

		for (auto& collitionShape : m_shapes)
		{
			b2FixtureDef fixtureDef;
			fixtureDef.isSensor = true;
			fixtureDef.filter.categoryBits = m_group;
			fixtureDef.filter.maskBits = m_groupMask;
			fixtureDef.filter.groupIndex = 0;
			collitionShape->resolveBox2DShape(m_body, fixtureDef, 0.0f);
        }
    }

    m_body->SetTransform(LnToB2(m_position), m_rotation);

    m_dirtyFlags = DirtyFlags_None;
}

void TriggerBody2D::onRemoveFromPhysicsWorld()
{
    removeBodyFromBox2DWorld();
    PhysicsObject2D::onRemoveFromPhysicsWorld();
}

void TriggerBody2D::removeBodyFromBox2DWorld()
{
    if (m_body) {
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
        //m_fixtures.clear();
    }
}

//EventConnection TriggerBody2D::connectOnTriggerEnter(Trigger2DEventHandler handler)
//{
//    return m_onTriggerEnter.connect(handler);
//}
//
//EventConnection TriggerBody2D::connectOnTriggerLeave(Trigger2DEventHandler handler)
//{
//    return m_onTriggerLeave.connect(handler);
//}
//
//EventConnection TriggerBody2D::connectOnTriggerStay(Trigger2DEventHandler handler)
//{
//    return m_onTriggerStay.connect(handler);
//}
//
//void TriggerBody2D::onTriggerEnter(PhysicsObject2D* otherObject)
//{
//    m_onTriggerEnter.raise(otherObject);
//}
//
//void TriggerBody2D::onTriggerLeave(PhysicsObject2D* otherObject)
//{
//    m_onTriggerLeave.raise(otherObject);
//}
//
//void TriggerBody2D::onTriggerStay(PhysicsObject2D* otherObject)
//{
//    m_onTriggerStay.raise(otherObject);
//}

//==============================================================================
// RigidBody2D

Ref<RigidBody2D> RigidBody2D::create()
{
	return makeObject_deprecated<RigidBody2D>();
}

Ref<RigidBody2D> RigidBody2D::create(CollisionShape2D* shape)
{
    return makeObject_deprecated<RigidBody2D>(shape);
}

RigidBody2D::RigidBody2D()
    : m_rotation(0)
    , m_mass(0.0f)
    , m_friction(0.0f)
    , m_restitution(0.0f)
    , m_group(0x00000001)
    , m_groupMask(0x0000FFFF)
    , m_kinematic(false)
    , m_fixedRotation(false)
{
    m_applyCommands.reserve(4);
}

void RigidBody2D::init()
{
	PhysicsObject2D::init();
}

void RigidBody2D::init(CollisionShape2D* shape)
{
    PhysicsObject2D::init();
    addCollisionShape(shape);
	attemptAddToActiveWorld();
}

void RigidBody2D::onDispose(bool explicitDisposing)
{
	LN_DCHECK(m_body);
	//if (m_body) {
	//	// m_body->CreateFixture で作成した fixture はこの中で解放される
	//	m_body->GetWorld()->DestroyBody(m_body);
	//	m_body = nullptr;
	//	//m_fixtures.clear();
	//}

	PhysicsObject2D::onDispose(explicitDisposing);
}

void RigidBody2D::setPosition(const Vector2& value)
{
	m_position = value;
}

void RigidBody2D::setVelocity(const Vector2& value)
{
    m_velocity = value;
    m_applyCommands.push_back({ ApplyType::SetVelocity, value, Vector2::Zero });
    m_modifyVelocityInSim = true;
}

void RigidBody2D::setMass(float value)
{
	m_mass = value;
}

void RigidBody2D::setFriction(float value)
{
    m_friction = value;
}

void RigidBody2D::setRestitution(float value)
{
    m_restitution = value;
}

void RigidBody2D::setKinematic(bool value)
{
	m_kinematic = value;
}

void RigidBody2D::addCollisionShape(CollisionShape2D* shape)
{
	if (LN_REQUIRE(shape)) return;
	m_shapes.push_back(shape);
}

void RigidBody2D::setCollisionGroup(uint32_t value)
{
    m_group = value;
}

void RigidBody2D::setCollisionGroupMask(uint32_t value)
{
    m_groupMask = value;
}

void RigidBody2D::applyForce(const Vector2& force)
{
    applyForce(force, Vector2::Zero);
}

void RigidBody2D::applyForce(const Vector2& force, const Vector2& localPosition)
{
    m_applyCommands.push_back({ ApplyType::Force, force, localPosition });

    //m_force += force;
    //m_torque += b2Cross(point - m_sweep.c, force);
    //m_body->ApplyForce();
}

void RigidBody2D::applyImpulse(const Vector2& impulse)
{
    applyImpulse(impulse, Vector2::Zero);
}

void RigidBody2D::applyImpulse(const Vector2& impulse, const Vector2& localPosition)
{
    m_applyCommands.push_back({ ApplyType::Impulse, impulse, localPosition });

    //m_linearVelocity += m_invMass * impulse;
    //m_angularVelocity += m_invI * b2Cross(point - m_sweep.c, impulse);
    ////m_body->ApplyLinearImpulse();
}

void RigidBody2D::applyTorque(float torque)
{
    m_applyCommands.push_back({ ApplyType::Torque, Vector2(torque, 0), Vector2::Zero });
    //m_torque += torque;
    //m_body->ApplyTorque();
}

void RigidBody2D::applyTorqueImpulse(float torque)
{
    m_applyCommands.push_back({ ApplyType::TorqueImpulse, Vector2(torque, 0), Vector2::Zero });
    //m_angularVelocity += m_invI * impulse;
    //m_body->ApplyAngularImpulse();
}

//　SetLinearVelocity
// SetAngularVelocity(m_angularVelocity)
// ApplyForce(m_force)
// ApplyTorque(m_torque)



void RigidBody2D::onBeforeStepSimulation()
{
    PhysicsObject2D::onBeforeStepSimulation();

	if (physicsWorld() && !m_body)
	{
		b2World* world = physicsWorld()->box2DWorld();

		b2BodyDef bodyDef;
		if (m_kinematic) {
			bodyDef.type = b2_kinematicBody;
		}
		else if (m_mass > 0.0f) {
			bodyDef.type = b2_dynamicBody;
		}
		else {
			bodyDef.type = b2_staticBody;
		}
		bodyDef.position = b2Vec2(m_position.x, m_position.y);
		bodyDef.angle = 0.0f;
		bodyDef.linearVelocity = b2Vec2(0, 0);
		bodyDef.angularVelocity = 0.0f;
		bodyDef.linearDamping = 0.0f;		// 速度減衰 (0.0 ~ 1.0)
		bodyDef.angularDamping = 0.0f;		// 回転減衰 (0.0 ~ 1.0)
		bodyDef.allowSleep = true;	// TODO:
		bodyDef.awake = true;
		bodyDef.fixedRotation = m_fixedRotation;
		bodyDef.bullet = false;
		bodyDef.enabled = true;
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
		bodyDef.gravityScale = 1.0f;
		m_body = world->CreateBody(&bodyDef);

		for (auto& collitionShape : m_shapes)
		{
			b2FixtureDef fixtureDef;
			//fixtureDef.shape = shape;
			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
			fixtureDef.friction = m_friction;
			fixtureDef.restitution = m_restitution;
			fixtureDef.isSensor = false;
			fixtureDef.filter.categoryBits = m_group;
			fixtureDef.filter.maskBits = m_groupMask;
			fixtureDef.filter.groupIndex = 0;
			collitionShape->resolveBox2DShape(m_body, fixtureDef, m_mass);
		}
	}

//    m_body->SetLinearVelocity(LnToB2(m_velocity));

	m_body->SetTransform(LnToB2(m_position), m_body->GetAngle());

    for (auto& c : m_applyCommands)
    {
        switch (c.type)
        {
        case ApplyType::SetVelocity:
            m_body->SetLinearVelocity(LnToB2(c.value));
            break;
        case ApplyType::Force:
            m_body->ApplyForce(LnToB2(c.value), LnToB2(c.center), true);
            break;
        case ApplyType::Impulse:
            m_body->ApplyLinearImpulse(LnToB2(c.value), LnToB2(c.center), true);
            break;
        case ApplyType::Torque:
            m_body->ApplyTorque(c.value.x, true);
            break;
        case ApplyType::TorqueImpulse:
            m_body->ApplyAngularImpulse(c.value.x, true);
            break;
        default:
            LN_UNREACHABLE();
            break;
        }
    }
    m_applyCommands.clear();
    m_modifyVelocityInSim = false;

    m_body->SetAwake(true);
}

void RigidBody2D::onAfterStepSimulation()
{
	if (!m_kinematic) {
		m_position = B2ToLn(m_body->GetPosition());
		m_rotation = m_body->GetAngle();
        if (!m_modifyVelocityInSim) {
            m_velocity = B2ToLn(m_body->GetLinearVelocity());
        }
	}

    PhysicsObject2D::onAfterStepSimulation();
}

void RigidBody2D::onRemoveFromPhysicsWorld()
{
	if (m_body) {
		b2JointEdge* j = m_body->GetJointList();
		while (j) {
			Joint2D* joint = reinterpret_cast<Joint2D*>(j->joint->GetUserData().pointer);
			j = j->next;
			if (joint) {
				joint->destroyJoint();
				physicsWorld()->addJointInternal(joint);
			}
		}


		


		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
		//m_fixtures.clear();
	}
}


//==============================================================================
// Joint2D

Joint2D::Joint2D()
	: m_world(nullptr)
	, m_joint(nullptr)
	, m_creationDirty(true)
{
}

bool Joint2D::init()
{
	if (!Object::init()) return false;
	return true;
}

void Joint2D::onDispose(bool explicitDisposing)
{
	Object::onDispose(explicitDisposing);
}

void Joint2D::attemptAddToActiveWorld()
{
	if (auto& activeWorld = detail::EngineDomain::physicsManager()->activePhysicsWorld2D()) {
		activeWorld->addJoint(this);
	}
}

void Joint2D::setJoint(b2Joint* joint)
{
	LN_DCHECK(!m_joint);
	m_joint = joint;
	joint->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}

void Joint2D::destroyJoint()
{
	if (m_joint) {
		m_world->box2DWorld()->DestroyJoint(m_joint);
		m_joint = nullptr;
	}
}

void Joint2D::removeFormWorld()
{
	if (m_world) {
		m_world->removeJoint(this);
	}
}

//==============================================================================
// SpringJoint2D

Ref<SpringJoint2D> SpringJoint2D::create()
{
	return ln::makeObject_deprecated<ln::SpringJoint2D>();
}

Ref<SpringJoint2D> SpringJoint2D::create(PhysicsObject2D* bodyA, PhysicsObject2D* bodyB)
{
	auto ptr = ln::makeObject_deprecated<ln::SpringJoint2D>();
	ptr->setBodyA(bodyA);
	ptr->setBodyB(bodyB);
	return ptr;
}

SpringJoint2D::SpringJoint2D()
{
}

bool SpringJoint2D::init()
{
	if (!Joint2D::init()) return false;

	attemptAddToActiveWorld();
	return true;
}

bool SpringJoint2D::init(PhysicsObject2D* bodyA, const Vector2& anchor1, PhysicsObject2D* bodyB, const Vector2& anchor2, float stiffness, float damping)
{
	if (!Joint2D::init()) return false;

	m_bodyA = bodyA;
	m_bodyB = bodyB;
	m_anchorA = anchor1;
	m_anchorB = anchor2;
	m_stiffness = stiffness;
	m_damping = damping;

	attemptAddToActiveWorld();
	return true;
}

void SpringJoint2D::setBodyA(PhysicsObject2D* value)
{
	if (m_bodyA != value) {
		m_bodyA = value;
		dirtyCreation();
	}
}

void SpringJoint2D::setBodyB(PhysicsObject2D* value)
{
	if (m_bodyB != value) {
		m_bodyB = value;
		dirtyCreation();
	}
}

void SpringJoint2D::setAnchorA(const Vector2& value)
{
	if (m_anchorA != value) {
		m_anchorA = value;
		dirtyCreation();
	}
}

void SpringJoint2D::setAnchorB(const Vector2& value)
{
	if (m_anchorB != value) {
		m_anchorB = value;
		dirtyCreation();
	}
}

void SpringJoint2D::setLength(float value)
{
	m_length = value;

	if (m_joint) {
		static_cast<b2DistanceJoint*>(m_joint)->SetLength(m_length);
	}
}

bool SpringJoint2D::createJoint()
{
	b2DistanceJointDef jointDef;
	jointDef.Initialize(m_bodyA->m_body, m_bodyB->m_body, LnToB2(m_bodyA->position() + m_anchorA), LnToB2(m_bodyB->position() + m_anchorB));
	jointDef.collideConnected = true;
	if (m_length > 0.0) jointDef.length = m_length;
#if 1
	// ver up 検証まだ
	LN_NOTIMPLEMENTED();
#else
	// 0.1, 1.0 すごくのびる 0.0 まったくのびない
	// 0.1, 0.0 すごくのびる
	// 1.0, 0.0 それなりに伸びて戻ってくる。ほとんど制止しない。
	// 1.0, 1.0 それなりに伸びて戻ってくる。振れ幅は少しずつ小さくなり、しばらくすると制止する。
	jointDef.frequencyHz = 1.0;	
	jointDef.dampingRatio = 0.0;	// 元に戻ろうとする力
#endif
	b2Joint* joint = m_world->box2DWorld()->CreateJoint(&jointDef);
	setJoint(joint);
	return true;
}

//==============================================================================
// RopeJoint2D

Ref<RopeJoint2D> RopeJoint2D::create()
{
	return ln::makeObject_deprecated<ln::RopeJoint2D>();
}

Ref<RopeJoint2D> RopeJoint2D::create(PhysicsObject2D* bodyA, PhysicsObject2D* bodyB)
{
	return ln::makeObject_deprecated<ln::RopeJoint2D>(bodyA, Vector2::Zero, bodyB, Vector2::Zero);
}

RopeJoint2D::RopeJoint2D()
{
}

bool RopeJoint2D::init()
{
	if (!Joint2D::init()) return false;

	attemptAddToActiveWorld();
	return true;
}

bool RopeJoint2D::init(PhysicsObject2D* bodyA, const Vector2& anchorA, PhysicsObject2D* bodyB, const Vector2& anchorB)
{
	if (!Joint2D::init()) return false;

	m_bodyA = bodyA;
	m_bodyB = bodyB;
	m_anchorA = anchorA;
	m_anchorB = anchorB;

	attemptAddToActiveWorld();
	return true;
}

bool RopeJoint2D::createJoint()
{
#if 1
	LN_NOTIMPLEMENTED();
#else
	b2RopeJointDef jointDef;


	jointDef.bodyA = m_bodyA->m_body;
	jointDef.bodyB = m_bodyB->m_body;
	jointDef.localAnchorA = LnToB2(m_anchorA);//LnToB2(m_bodyA->position() + m_anchorA);
	jointDef.localAnchorB = LnToB2(m_anchorB);//LnToB2(m_bodyB->position() + m_anchorB);
	jointDef.maxLength = ((m_bodyA->position() + m_anchorA) - (m_bodyB->position() + m_anchorB)).length();


	b2Joint* joint = m_world->box2DWorld()->CreateJoint(&jointDef);
	setJoint(joint);
#endif
	return true;
}


class PhysicsWorld2DDebugDraw : public b2Draw
{
public:
	static const size_t MaxVertexCount = 1024;

	void init()
	{
		m_linesBuffer = makeObject_deprecated<VertexBuffer>(sizeof(Vertex) * MaxVertexCount, GraphicsResourceUsage::Dynamic);
		m_trianglesBuffer = makeObject_deprecated<VertexBuffer>(sizeof(Vertex) * MaxVertexCount, GraphicsResourceUsage::Dynamic);
		m_linesVertexCount = 0;
		m_trianglesVertexCount = 0;
	}

	void render(RenderingContext* context)
	{
        context->pushState();
        context->setBlendMode(BlendMode::Alpha);
		context->setShadingModel(ShadingModel::Unlit);

		context->drawPrimitive(
			detail::RenderingManager::instance()->standardVertexDeclaration(),
			m_trianglesBuffer,
			PrimitiveTopology::TriangleList,
			0, m_trianglesVertexCount / 3);

		context->drawPrimitive(
			detail::RenderingManager::instance()->standardVertexDeclaration(),
			m_linesBuffer,
			PrimitiveTopology::LineList,
			0, m_linesVertexCount / 2);

        context->popState();    // TODO: scoped

        m_linesVertexCount = 0;
        m_trianglesVertexCount = 0;
	}

	/// Draw a closed polygon provided in CCW order.
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		b2Vec2 p1 = vertices[vertexCount - 1];
		for (int32 i = 0; i < vertexCount; ++i)
		{
			b2Vec2 p2 = vertices[i];
			addLineVertex(p1, color);
			addLineVertex(p2, color);
			p1 = p2;
		}
	}

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

		for (int32 i = 1; i < vertexCount - 1; ++i)
		{
			addTriangleVertex(vertices[0], fillColor);
			addTriangleVertex(vertices[i + 1], fillColor);
			addTriangleVertex(vertices[i], fillColor);
		}

		b2Vec2 p1 = vertices[vertexCount - 1];
		for (int32 i = 0; i < vertexCount; ++i)
		{
			b2Vec2 p2 = vertices[i];
			addLineVertex(p1, color);
			addLineVertex(p2, color);
			p1 = p2;
		}
	}

	/// Draw a circle.
	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override
	{
		const float k_segments = 16.0f;
		const float k_increment = 2.0f * b2_pi / k_segments;
		float sinInc = sinf(k_increment);
		float cosInc = cosf(k_increment);
		b2Vec2 r1(1.0f, 0.0f);
		b2Vec2 v1 = center + radius * r1;
		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			addLineVertex(v1, color);
			addLineVertex(v2, color);
			r1 = r2;
			v1 = v2;
		}
	}

	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override
	{
		const float k_segments = 16.0f;
		const float k_increment = 2.0f * b2_pi / k_segments;
		float sinInc = sinf(k_increment);
		float cosInc = cosf(k_increment);
		b2Vec2 v0 = center;
		b2Vec2 r1(cosInc, sinInc);
		b2Vec2 v1 = center + radius * r1;
		b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			addTriangleVertex(v0, fillColor);
			addTriangleVertex(v2, fillColor);
			addTriangleVertex(v1, fillColor);
			r1 = r2;
			v1 = v2;
		}

		r1.Set(1.0f, 0.0f);
		v1 = center + radius * r1;
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			addLineVertex(v1, color);
			addLineVertex(v2, color);
			r1 = r2;
			v1 = v2;
		}

		// Draw a line fixed in the circle to animate rotation.
		b2Vec2 p = center + radius * axis;
		addLineVertex(center, color);
		addLineVertex(p, color);
	}

	/// Draw a line segment.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
	{
		addLineVertex(p1, color);
		addLineVertex(p2, color);
	}

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	void DrawTransform(const b2Transform& xf) override
	{
		const float k_axisScale = 0.4f;
		b2Color red(1.0f, 0.0f, 0.0f);
		b2Color green(0.0f, 1.0f, 0.0f);
		b2Vec2 p1 = xf.p, p2;

		addLineVertex(p1, red);
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		addLineVertex(p2, red);

		addLineVertex(p1, green);
		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		addLineVertex(p2, green);
	}

	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override
	{
		LN_NOTIMPLEMENTED();
	}

private:
	void addLineVertex(const b2Vec2& v, const b2Color& c)
	{
        if (m_linesVertexCount < MaxVertexCount)
        {
            Vertex* buf = (Vertex*)m_linesBuffer->writableData();
			buf[m_linesVertexCount].set(Vector3(v.x, v.y, 0), -Vector3::UnitZ, Vector2::Zero, Color(c.r, c.g, c.b, c.a));
            m_linesVertexCount++;
        }
    }

    void addTriangleVertex(const b2Vec2& v, const b2Color& c)
    {
        if (m_trianglesVertexCount < MaxVertexCount)
        {
            Vertex* buf = (Vertex*)m_trianglesBuffer->writableData();
			buf[m_trianglesVertexCount].set(Vector3(v.x, v.y, 0), -Vector3::UnitZ, Vector2::Zero, Color(c.r, c.g, c.b, c.a));
            m_trianglesVertexCount++;
        }
    }

    Ref<VertexBuffer> m_linesBuffer;
    Ref<VertexBuffer> m_trianglesBuffer;
    size_t m_linesVertexCount;
    size_t m_trianglesVertexCount;
};

//==============================================================================
// PhysicsWorld2D

class LocalContactListener
    : public b2ContactListener
{
public:
    // 新しく接触を開始したときのコールバック。接触中は呼ばれない。
    void BeginContact(b2Contact* contact) override
    {
        auto* bodyA = reinterpret_cast<PhysicsObject2D*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        auto* bodyB = reinterpret_cast<PhysicsObject2D*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
        bodyA->beginContact(bodyB);
        bodyB->beginContact(bodyA);
    }

    void EndContact(b2Contact* contact) override
    {
        auto* bodyA = reinterpret_cast<PhysicsObject2D*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        auto* bodyB = reinterpret_cast<PhysicsObject2D*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
        bodyA->endContact(bodyB);
        bodyB->endContact(bodyA);
    }
};



//==============================================================================
// PhysicsWorld2D

PhysicsWorld2D::PhysicsWorld2D()
    : m_world(nullptr)
    , m_inStepSimulation(false)
{
}

void PhysicsWorld2D::init()
{
    Object::init();

    m_contactListener = std::make_unique<LocalContactListener>();

    m_debugDraw = std::make_unique<PhysicsWorld2DDebugDraw>();
    m_debugDraw->init();
    m_debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit /*| b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit*/);

	m_gravity = Vector2(0.0f, -9.8);
    m_world = LN_NEW b2World(LnToB2(m_gravity));
    m_world->SetContactListener(m_contactListener.get());
    m_world->SetDebugDraw(m_debugDraw.get());
}

void PhysicsWorld2D::onDispose(bool explicitDisposing)
{
	updateBodyAndJointList();
	for (int i = m_joints.size() - 1; i >= 0; i--) {
		removeJointInternal(m_joints[i]);
	}
	m_joints.clear();

    for (int i = m_objects.size() - 1; i >= 0; i--) {
        removePhysicsObject(m_objects[i]);
    }
    m_objects.clear();

    if (m_world) {
        LN_SAFE_DELETE(m_world);
    }
}

bool PhysicsWorld2D::raycast(const Vector3& origin, const Vector2& direction, float maxDistance, uint32_t layerMask, bool queryTrigger, RaycastResult2D* outResult)
{
    class RayCastCallback
        : public b2RayCastCallback
    {
    public:
        RaycastResult2D* result;
        Vector3 origin;
        uint32_t layerMask;
        bool queryTrigger;
        bool hit;

        float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
        {
            if (!queryTrigger && fixture->IsSensor()) {
                return -1;  // continue
            }

            if ((fixture->GetFilterData().categoryBits & layerMask) != 0) {
                hit = true;
                if (result) {
                    result->physicsObject = reinterpret_cast<PhysicsObject2D*>(fixture->GetBody()->GetUserData().pointer);
                    result->point = B2ToLn(point);
                    result->normal = B2ToLn(normal);
                    result->distance = (result->point - origin.xy()).length();
                    auto* ownerComponent = reinterpret_cast<RigidBody2DComponent*>(result->physicsObject->ownerData());
                    if (ownerComponent) {
                        result->worldObject = ownerComponent->worldObject();
                    }
                }
                return 0;   // hit
            }

            return -1;  // continue
        }

    } callback;
    callback.result = outResult;
    callback.origin = origin;
    callback.layerMask = layerMask;
    callback.queryTrigger = queryTrigger;
    callback.hit = false;

    m_world->RayCast(&callback, LnToB2(origin.xy()), LnToB2((origin.xy() + direction * maxDistance)));

    return callback.hit;
}

void PhysicsWorld2D::addPhysicsObject(PhysicsObject2D* physicsObject)
{
	if (LN_REQUIRE(physicsObject)) return;
	if (LN_REQUIRE(!physicsObject->m_ownerWorld)) return;
	physicsObject->m_ownerWorld = this;
	m_delayAddBodies.push_back(physicsObject);
	//if (m_inStepSimulation) {
	//	LN_NOTIMPLEMENTED();
	//}
	//else {
	//	m_objects.add(physicsObject);
	//	//physicsObject->onBeforeStepSimulation();
	//}

}

void PhysicsWorld2D::removePhysicsObject(PhysicsObject2D* physicsObject)
{
	if (LN_REQUIRE(physicsObject)) return;
	if (LN_REQUIRE(physicsObject->m_ownerWorld == this)) return;

	//if (m_inStepSimulation) {
		m_removeList.push_back(physicsObject);
	//}
	//else {
	//	removeInternal(physicsObject);
	//}
}

// 追加系はすべて遅延追加とする。
// よく初期設定で
// auto node1 = ln::RigidBody2D::create(nodeShape);
// node1->setMass(1);
// node1->setPosition(ln::Vector3(-1, 0, 0));
// のようにするが、create 時点での World 追加で b2Body が作られた後、
// setMass したのでまた後で作り直さなければならなくなる。
void PhysicsWorld2D::addJoint(Joint2D* joint)
{
	if (LN_REQUIRE(joint)) return;
	if (LN_REQUIRE(!joint->m_world)) return;
	joint->m_world = this;

	//if (m_inStepSimulation) {
		m_delayAddJoints.push_back(joint);
	//}
	//else {
	//	addJointInternal(joint);
	//}
}

void PhysicsWorld2D::removeJoint(Joint2D* joint)
{
	if (LN_REQUIRE(joint)) return;
	if (LN_REQUIRE(joint->m_world == this)) return;

	//if (m_inStepSimulation) {
		m_delayRemoveJoints.push_back(joint);
	//}
	//else {
	//	removeJointInternal(joint);
	//}

}

void PhysicsWorld2D::stepSimulation(float elapsedSeconds)
{
	m_inStepSimulation = true;





	updateBodyAndJointList();


	// 推奨値 http://box2d.org/manual.pdf
	const int32 velocityIterations = 8;
	const int32 positionIterations = 3;

	m_world->SetGravity(b2Vec2(m_gravity.x, m_gravity.y));
	m_world->Step(elapsedSeconds, velocityIterations, positionIterations);

	for (auto& obj : m_objects) {
		obj->onAfterStepSimulation();
	}

	m_inStepSimulation = false;
}

void PhysicsWorld2D::renderDebug(RenderingContext* context)
{
	m_world->DebugDraw();
	m_debugDraw->render(context);
}

void PhysicsWorld2D::removeInternal(PhysicsObject2D* physicsObject)
{
	physicsObject->onRemoveFromPhysicsWorld();
	if (m_objects.remove(physicsObject)) {
		physicsObject->m_ownerWorld = nullptr;
	}
}

void PhysicsWorld2D::addJointInternal(Joint2D* joint)
{
	m_joints.add(joint);
	joint->createJoint();
	joint->m_creationDirty = false;
}

void PhysicsWorld2D::removeJointInternal(Joint2D* joint)
{
	joint->destroyJoint();
	m_joints.remove(joint);
	joint->m_world = nullptr;
}

void PhysicsWorld2D::updateBodyAndJointList()
{
	// Deletion is joint priority.
	if (!m_delayRemoveJoints.empty()) {
		for (auto& joint : m_delayRemoveJoints) {
			removeJointInternal(joint);
		}
		m_delayRemoveJoints.clear();
	}
	// Destroy body
	if (!m_removeList.empty()) {
		for (auto& body : m_removeList) {
			removeInternal(body);
		}
		m_removeList.clear();
	}

	// Creation is boyd priority.
	if (!m_delayAddBodies.empty()) {
		for (auto& body : m_delayAddBodies) {
			m_objects.add(body);
		}
		m_delayAddBodies.clear();
	}
	for (auto& obj : m_objects) {
		obj->onBeforeStepSimulation();
	}

	// Create joint.
	if (!m_delayAddJoints.empty()) {
		for (auto& joint : m_delayAddJoints) {
			addJointInternal(joint);
		}
		m_delayAddJoints.clear();
	}
	for (auto& joint : m_joints) {
		if (joint->m_creationDirty) {
			joint->destroyJoint();
			joint->createJoint();
			joint->m_creationDirty = false;
		}
	}

}

//==============================================================================
// Physics2D

bool Physics2D::raycast(const Vector3& origin, const Vector2& direction, float maxDistance, uint32_t layerMask, bool queryTrigger, RaycastResult2D* outResult)
{
    return detail::EngineDomain::physicsManager()->activePhysicsWorld2D()->raycast(origin, direction, maxDistance, layerMask, queryTrigger, outResult);
}

bool Physics2D::raycast(const Vector3& origin, const Vector2& direction, float maxDistance, uint32_t layerMask, RaycastResult2D* outResult)
{
    return detail::EngineDomain::physicsManager()->activePhysicsWorld2D()->raycast(origin, direction, maxDistance, layerMask, false, outResult);
}

} // namespace ln

