#include <TestConfig.h>

#if 0

class Test_Physics_3D : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Physics_3D, Basic)
{
	// <Test> 地面が無いので下のほうへ落ちていく
	{
		auto shape1 = BoxCollisionShape::create(1, 2, 3);
		auto body1 = RigidBodyComponent::create(shape1);
		for (int i = 0; i < 10; i++) Engine::update();
		ASSERT_EQ(true, body1->getWorldTransform().getPosition().y < 0);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Physics_3D, TriggerCollider)
{
	// <Test> 
	{
		auto col1 = BoxCollisionShape::create(1, 1, 1);
		auto col2 = BoxCollisionShape::create(1, 1, 1);
		auto body1 = CollisionBodyComponent::create(col1);
		auto body2 = CollisionBodyComponent::create(col2);
		body1->setTrigger(true);
		body2->setTrigger(true);

		int count1 = 0;
		int count2 = 0;
		int count3 = 0;
		body1->connectOnTriggerEnter([&count1](PhysicsComponent* obj) { count1++; });
		body2->connectOnTriggerEnter([&count1](PhysicsComponent* obj) { count1++; });
		body1->connectOnTriggerStay([&count2](PhysicsComponent* obj) { count2++; });
		body2->connectOnTriggerStay([&count2](PhysicsComponent* obj) { count2++; });
		body1->connectOnTriggerLeave([&count3](PhysicsComponent* obj) { count3++; });
		body2->connectOnTriggerLeave([&count3](PhysicsComponent* obj) { count3++; });

		for (int i = 0; i < 10; i++) Engine::update();

		body1->getOwnerWorld()->removePhysicsObject(body1);
		body2->getOwnerWorld()->removePhysicsObject(body2);

		ASSERT_EQ(true, body1->getPhysicsObjectTransform().isIdentity());
		ASSERT_EQ(true, body2->getPhysicsObjectTransform().isIdentity());

		ASSERT_EQ(2, count1);	// 2回の接触開始
		ASSERT_EQ(20, count2);	// 相互に10フレーム接触し続ける
		ASSERT_EQ(2, count3);	// 2回の接触終了
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Physics_3D, MeshCollisionShape)
{
	//dynamic_cast<ln::CameraViewportLayer2*>(ln::Engine::getDefault3DLayer())->setDebugDrawFlags(ln::WorldDebugDrawFlags::PhysicsInfo);

	// <Test> YZ 平面で X+ 向きの 四角形 MeshSphere に、左右から剛体をぶつける → 裏表に関係なく、双方に跳ね返る
	{
		auto mesh = MeshResource::create();
		mesh->addSquare(
			Vertex{ Vector3(0, 10, -10) },
			Vertex{ Vector3(0, -10, -10) },
			Vertex{ Vector3(0, -10, 10) },
			Vertex{ Vector3(0, 10, 10) });

		auto s1 = MeshCollisionShape::create(mesh);
		auto b1 = CollisionBodyComponent::create(s1);

		auto s2 = BoxCollisionShape::create(1, 1, 1);
		auto b2 = RigidBodyComponent::create(s2);
		b2->setPosition(3, 0, 5);
		b2->applyImpulse(Vector3(-10, 0, 0));

		auto b3 = RigidBodyComponent::create(s2);
		b3->setPosition(-3, 0, -5);
		b3->applyImpulse(Vector3(10, 0, 0));

		for (int i = 0; i < 60; i++)  Engine::update();

		ASSERT_EQ(true, b2->getWorldTransform().getPosition().x > 0);
		ASSERT_EQ(true, b3->getWorldTransform().getPosition().x < 0);
	}
}

#endif
