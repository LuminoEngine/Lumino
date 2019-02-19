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
