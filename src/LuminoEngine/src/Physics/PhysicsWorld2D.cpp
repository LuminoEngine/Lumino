
#include "Internal.hpp"
#include <Box2D/Box2D.h>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>

namespace ln {

//==============================================================================
// CollisionShape2D

CollisionShape2D::CollisionShape2D()
{
}

void CollisionShape2D::init()
{
	Object::init();
}

//==============================================================================
// BoxCollisionShape2D

BoxCollisionShape2D::BoxCollisionShape2D()
	: m_shape()
{
}

void BoxCollisionShape2D::init(const Vector2& size)
{
	CollisionShape2D::init();

	m_shape = std::make_unique<b2PolygonShape>();
	m_shape->SetAsBox(size.x * 0.5f, size.y * 0.5f);
}

b2Shape* BoxCollisionShape2D::box2DShape()
{
	return m_shape.get();
}

//==============================================================================
// PhysicsObject2D

PhysicsObject2D::PhysicsObject2D()
	: m_ownerWorld(nullptr)
{
}

void PhysicsObject2D::init()
{
	Object::init();
}

void PhysicsObject2D::onBeforeStepSimulation()
{
}

void PhysicsObject2D::onAfterStepSimulation()
{
}

//==============================================================================
// RigidBody2D

RigidBody2D::RigidBody2D()
	: m_body(nullptr)
	, m_fixture(nullptr)
	, m_mass(0.0f)
	, m_kinematic(false)
{
}

void RigidBody2D::init()
{
	PhysicsObject2D::init();
}

void RigidBody2D::onDispose(bool explicitDisposing)
{
	if (m_body) {
		// m_body->CreateFixture で作成した fixture はこの中で解放される
		physicsWorld()->box2DWorld()->DestroyBody(m_body);
		m_body = nullptr;
	}

	PhysicsObject2D::onDispose(explicitDisposing);
}

void RigidBody2D::setPosition(const Vector2& value)
{
	m_position = value;
}

void RigidBody2D::setMass(float value)
{
	m_mass = value;
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

void RigidBody2D::onBeforeStepSimulation()
{
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
		bodyDef.fixedRotation = false;	// TODO:
		bodyDef.bullet = false;
		bodyDef.active = true;
		bodyDef.userData = this;
		bodyDef.gravityScale = 1.0f;
		m_body = world->CreateBody(&bodyDef);

		b2Shape* shape = m_shapes[0]->box2DShape();	// TODO:

		b2MassData massData;
		m_shapes[0]->box2DShape()->ComputeMass(&massData, 1);
		float volume = massData.mass;	// ComputeMass に密度 1 で計算すると、質量 = 体積となる

		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape;
		fixtureDef.userData = this;
		fixtureDef.friction = 0.2f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.density = m_mass / volume;	// 密度 = 質量 / 体積
		fixtureDef.isSensor = false;
		m_fixture = m_body->CreateFixture(&fixtureDef);
	}
}

void RigidBody2D::onAfterStepSimulation()
{
	if (!m_kinematic) {
		const b2Vec2& pos = m_body->GetPosition();
		m_position = Vector2(pos.x, pos.y);
	}
}

//==============================================================================
// PhysicsWorld2D

PhysicsWorld2D::PhysicsWorld2D()
	: m_world(nullptr)
{
}

void PhysicsWorld2D::init()
{
	b2Vec2 gravity(0.0f, -9.8);
	m_world = LN_NEW b2World(gravity);
}

void PhysicsWorld2D::onDispose(bool explicitDisposing)
{
	m_objects.clear();

	if (m_world) {
		LN_SAFE_DELETE(m_world);
	}
}

void PhysicsWorld2D::addPhysicsObject(PhysicsObject2D* physicsObject)
{
	if (LN_REQUIRE(physicsObject)) return;
	if (LN_REQUIRE(!physicsObject->m_ownerWorld)) return;

	m_objects.add(physicsObject);
	physicsObject->m_ownerWorld = this;
}

void PhysicsWorld2D::stepSimulation(float elapsedSeconds)
{
	// 推奨値 http://box2d.org/manual.pdf
	const int32 velocityIterations = 8;
	const int32 positionIterations = 3;

	for (auto& obj : m_objects) {
		obj->onBeforeStepSimulation();
	}

	m_world->Step(elapsedSeconds, velocityIterations, positionIterations);

	for (auto& obj : m_objects) {
		obj->onAfterStepSimulation();
	}
}

} // namespace ln

