#pragma once
#include <memory>
#include "Common.hpp"

class b2Shape;
class b2PolygonShape;
class b2Body;
class b2Fixture;
class b2World;

namespace ln {
class PhysicsWorld2D;

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

	virtual b2Shape* box2DShape() override;

LN_CONSTRUCT_ACCESS:
	BoxCollisionShape2D();
	virtual ~BoxCollisionShape2D() = default;
	void init(const Vector2& size);

private:
	std::unique_ptr<b2PolygonShape> m_shape;
};



class PhysicsObject2D
	: public Object
{
public:
	PhysicsWorld2D* physicsWorld() const { return m_ownerWorld; }

protected:
	virtual void onBeforeStepSimulation();
	virtual void onAfterStepSimulation();

LN_CONSTRUCT_ACCESS:
	PhysicsObject2D();
	virtual ~PhysicsObject2D() = default;
	void init();

private:
	PhysicsWorld2D* m_ownerWorld;

	friend class PhysicsWorld2D;
};

class RigidBody2D
	: public PhysicsObject2D
{
public:

	/** 位置を設定します。(default: 0, 0) */
	void setPosition(const Vector2& value);

	const Vector2& position() const { return m_position; }

	/** 質量を設定します。0 を設定すると静的なボディとなります。(default: 0.0) */
	void setMass(float value);


	/** キネマティックモードを設定します。 キネマティックモードでは、剛体に力はかかりません。 */
	void setKinematic(bool value);


	void addCollisionShape(CollisionShape2D* shape);

protected:
	virtual void onBeforeStepSimulation() override;
	virtual void onAfterStepSimulation() override;

LN_CONSTRUCT_ACCESS:
	RigidBody2D();
	virtual ~RigidBody2D() = default;
	void init();
	virtual void onDispose(bool explicitDisposing) override;

private:
	b2Body* m_body;
	b2Fixture* m_fixture;
	std::vector<Ref<CollisionShape2D>> m_shapes;
	Vector2 m_position;
	float m_mass;
	bool m_kinematic;
};

class PhysicsWorld2D
	: public Object
{
public:
	void addPhysicsObject(PhysicsObject2D* physicsObject);
	void stepSimulation(float elapsedSeconds);

public: // TODO:
	b2World* box2DWorld() const { return m_world; }

LN_CONSTRUCT_ACCESS:
	PhysicsWorld2D();
	virtual ~PhysicsWorld2D() = default;
	void init();
    virtual void onDispose(bool explicitDisposing) override;

private:
	b2World* m_world;
	List<Ref<PhysicsObject2D>> m_objects;
};

} // namespace ln

