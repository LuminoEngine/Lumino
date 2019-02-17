
#include "Internal.hpp"
#include <Box2D/Box2D.h>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

static b2Vec2 LnToB2(const Vector2& v) { return b2Vec2(v.x, v.y); }
static Vector2 B2ToLn(const b2Vec2& v) { return Vector2(v.x, v.y); }

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

Ref<BoxCollisionShape2D> BoxCollisionShape2D::create(const Vector2& size)
{
    return newObject<BoxCollisionShape2D>(size);
}

Ref<BoxCollisionShape2D> BoxCollisionShape2D::create(float width, float height)
{
    return newObject<BoxCollisionShape2D>(width, height);
}

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

void BoxCollisionShape2D::init(float width, float height)
{
    init(Vector2(width, height));
}

b2Shape* BoxCollisionShape2D::box2DShape()
{
	return m_shape.get();
}


//==============================================================================
// PhysicsObject2D

PhysicsObject2D::PhysicsObject2D()
	: m_ownerWorld(nullptr)
    , m_listener(nullptr)
    , m_ownerData(nullptr)
{
}

void PhysicsObject2D::init()
{
	Object::init();
}

EventConnection PhysicsObject2D::connectOnCollisionEnter(Collision2DEventHandler handler)
{
    return m_onCollisionEnter.connect(handler);
}

EventConnection PhysicsObject2D::connectOnCollisionLeave(Collision2DEventHandler handler)
{
    return m_onCollisionLeave.connect(handler);
}

EventConnection PhysicsObject2D::connectOnCollisionStay(Collision2DEventHandler handler)
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
// CollisionBody2D

EventConnection CollisionBody2D::connectOnTriggerEnter(Trigger2DEventHandler handler)
{
    return m_onTriggerEnter.connect(handler);
}

EventConnection CollisionBody2D::connectOnTriggerLeave(Trigger2DEventHandler handler)
{
    return m_onTriggerLeave.connect(handler);
}

EventConnection CollisionBody2D::connectOnTriggerStay(Trigger2DEventHandler handler)
{
    return m_onTriggerStay.connect(handler);
}

void CollisionBody2D::onTriggerEnter(PhysicsObject2D* otherObject)
{
    m_onTriggerEnter.raise(otherObject);
}

void CollisionBody2D::onTriggerLeave(PhysicsObject2D* otherObject)
{
    m_onTriggerLeave.raise(otherObject);
}

void CollisionBody2D::onTriggerStay(PhysicsObject2D* otherObject)
{
    m_onTriggerStay.raise(otherObject);
}

//==============================================================================
// RigidBody2D

Ref<RigidBody2D> RigidBody2D::create(CollisionShape2D* shape)
{
    return newObject<RigidBody2D>();
}

RigidBody2D::RigidBody2D()
    : m_body(nullptr)
    , m_fixture(nullptr)
    , m_mass(0.0f)
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
}

void RigidBody2D::onDispose(bool explicitDisposing)
{
	if (m_body) {
		// m_body->CreateFixture で作成した fixture はこの中で解放される
		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
	}

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
		fixtureDef.friction = 0.0f;			// 摩擦
		fixtureDef.restitution = 0.0f;		// 反発
		fixtureDef.density = m_mass / volume;	// 密度 = 質量 / 体積
		fixtureDef.isSensor = false;
		fixtureDef.filter.categoryBits = 0x0001;
		fixtureDef.filter.maskBits = 0xFFFF;
		fixtureDef.filter.groupIndex = 0;
		m_fixture = m_body->CreateFixture(&fixtureDef);
	}

//    m_body->SetLinearVelocity(LnToB2(m_velocity));

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

    m_body->GetContactList();
}

void RigidBody2D::onAfterStepSimulation()
{
	if (!m_kinematic) {
		m_position = B2ToLn(m_body->GetPosition());
        m_velocity = B2ToLn(m_body->GetLinearVelocity());
	}

    PhysicsObject2D::onAfterStepSimulation();
}

void RigidBody2D::onRemoveFromPhysicsWorld()
{
	if (m_body) {
		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
	}
}



class PhysicsWorld2DDebugDraw : public b2Draw
{
public:
	static const size_t MaxVertexCount = 1024;

	void init()
	{
		m_linesBuffer = newObject<VertexBuffer>(sizeof(Vertex) * MaxVertexCount, GraphicsResourceUsage::Dynamic);
		m_trianglesBuffer = newObject<VertexBuffer>(sizeof(Vertex) * MaxVertexCount, GraphicsResourceUsage::Dynamic);
		m_linesVertexCount = 0;
		m_trianglesVertexCount = 0;
	}

	void render(RenderingContext* context)
	{
        context->pushState();
        context->setBlendMode(BlendMode::Alpha);

		context->drawPrimitive(
			detail::EngineDomain::renderingManager()->standardVertexDeclaration(),
			m_trianglesBuffer,
			PrimitiveTopology::TriangleList,
			0, m_trianglesVertexCount / 3);

		context->drawPrimitive(
			detail::EngineDomain::renderingManager()->standardVertexDeclaration(),
			m_linesBuffer,
			PrimitiveTopology::LineList,
			0, m_linesVertexCount / 2);

        context->popState();    // TODO: scoped

        m_linesVertexCount = 0;
        m_trianglesVertexCount = 0;
	}

	/// Draw a closed polygon provided in CCW order.
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
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
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
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
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 sinInc = sinf(k_increment);
		float32 cosInc = cosf(k_increment);
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
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 sinInc = sinf(k_increment);
		float32 cosInc = cosf(k_increment);
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
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		addLineVertex(p1, color);
		addLineVertex(p2, color);
	}

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf)
	{
		const float32 k_axisScale = 0.4f;
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

private:
	void addLineVertex(const b2Vec2& v, const b2Color& c)
	{
        if (m_linesVertexCount < MaxVertexCount)
        {
            Vertex* buf = (Vertex*)m_linesBuffer->map(MapMode::Write);
            buf[m_linesVertexCount].position.set(v.x, v.y, 0);
            buf[m_linesVertexCount].color.set(c.r, c.g, c.b, c.a);
            buf[m_linesVertexCount].uv = Vector2::Zero;
            buf[m_linesVertexCount].normal = -Vector3::UnitZ;
            m_linesVertexCount++;
        }
	}

	void addTriangleVertex(const b2Vec2& v, const b2Color& c)
	{
        if (m_trianglesVertexCount < MaxVertexCount)
        {
            Vertex* buf = (Vertex*)m_trianglesBuffer->map(MapMode::Write);
            buf[m_trianglesVertexCount].position.set(v.x, v.y, 0);
            buf[m_trianglesVertexCount].color.set(c.r, c.g, c.b, c.a);
            buf[m_trianglesVertexCount].uv = Vector2::Zero;
            buf[m_trianglesVertexCount].normal = -Vector3::UnitZ;
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
    virtual void BeginContact(b2Contact* contact) override
    {
        auto* bodyA = reinterpret_cast<PhysicsObject2D*>(contact->GetFixtureA()->GetBody()->GetUserData());
        auto* bodyB = reinterpret_cast<PhysicsObject2D*>(contact->GetFixtureB()->GetBody()->GetUserData());
        bodyA->beginContact(bodyB);
        bodyB->beginContact(bodyA);
    }

    virtual void EndContact(b2Contact* contact) override
    {
        auto* bodyA = reinterpret_cast<PhysicsObject2D*>(contact->GetFixtureA()->GetBody()->GetUserData());
        auto* bodyB = reinterpret_cast<PhysicsObject2D*>(contact->GetFixtureB()->GetBody()->GetUserData());
        bodyA->endContact(bodyB);
        bodyB->endContact(bodyA);
    }
};



//==============================================================================
// PhysicsWorld2D

PhysicsWorld2D::PhysicsWorld2D()
	: m_world(nullptr)
{
}

void PhysicsWorld2D::init()
{
	Object::init();

    m_contactListener = std::make_unique<LocalContactListener>();

	m_debugDraw = std::make_unique<PhysicsWorld2DDebugDraw>();
	m_debugDraw->init();
    m_debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit /*| b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit*/);


	b2Vec2 gravity(0.0f, -9.8);
	m_world = LN_NEW b2World(gravity);
    m_world->SetContactListener(m_contactListener.get());
	m_world->SetDebugDraw(m_debugDraw.get());
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

void PhysicsWorld2D::removePhysicsObject(PhysicsObject2D* physicsObject)
{
	if (LN_REQUIRE(physicsObject)) return;
	if (LN_REQUIRE(physicsObject->m_ownerWorld == this)) return;

	physicsObject->onRemoveFromPhysicsWorld();
	if (m_objects.remove(physicsObject)) {
		physicsObject->m_ownerWorld = nullptr;
	}
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

void PhysicsWorld2D::renderDebug(RenderingContext* context)
{
	m_world->DrawDebugData();
	m_debugDraw->render(context);
}

} // namespace ln

