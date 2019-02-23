#include "Common.hpp"

class Test_Physics_2D : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Physics_2D, Basic)
{
	//* [ ] static body 上へ落下
	{
		auto texture1 = Assets::loadTexture(u"Sprite1");

		// ground
		auto sprite1 = Sprite::create(texture1, 10, 1);
		sprite1->setPosition(0, -2, 0);
		auto body1 = RigidBody2DComponent::create();
		body1->addCollisionShape(BoxCollisionShape2D::create(10, 1));
		sprite1->addComponent(body1);

		// box
		auto sprite2 = Sprite::create(texture1, 1, 1);
		sprite2->setPosition(0, 0, 0);
		auto body2 = RigidBody2DComponent::create();
		body2->setMass(5);
		body2->addCollisionShape(BoxCollisionShape2D::create(1, 1));
		sprite2->addComponent(body2);

		for (int i = 0; i < 120; i++) TestEnv::updateFrame();

		float y = sprite2->position().y;
		ASSERT_EQ(true, -1.1 < y && y < -0.9);	// だいたい -1.0 付近にいるはず
		LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Physics_2D, Compound)
{
	// ground
	auto body1 = RigidBody2D::create();
	body1->setPosition(Vector3(0, -3, 0));
	body1->addCollisionShape(BoxCollisionShape2D::create(10, 1));
	Engine::mainWorld()->physicsWorld2D()->addPhysicsObject(body1);

	// boxes
	auto shape1 = BoxCollisionShape2D::create(1, 1);
	auto shape2 = BoxCollisionShape2D::create(1, 1);
	shape2->setPosition(Vector2(1, 1));
	auto body2 = RigidBody2D::create();
	body2->setMass(1);
	body2->addCollisionShape(shape1);
	body2->addCollisionShape(shape2);
	Engine::mainWorld()->physicsWorld2D()->addPhysicsObject(body2);

	for (int i = 0; i < 120; i++) TestEnv::updateFrame();

	// なんとなく時計回りにかたむいてるはず
	ASSERT_EQ(true, body2->rotation() < -0.6);

	Engine::mainWorld()->physicsWorld2D()->removePhysicsObject(body1);
	Engine::mainWorld()->physicsWorld2D()->removePhysicsObject(body2);
}

