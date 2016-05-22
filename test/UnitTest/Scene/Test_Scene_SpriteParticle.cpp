#include <TestConfig.h>


class Test_Scene_SpriteParticle : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		Engine::SetFixedDeltaTime(1.0f / 60);
	}
	virtual void TearDown()
	{
		Engine::SetFixedDeltaTime(0.0f);
	}
};


//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, Default)
{
	auto particleModel1 = SpriteParticleModel::Create();
	particleModel1->SetTexture(Texture2D::Create(LN_LOCALFILE("TestData/Particle1.png")));
	auto particle1 = SpriteParticle::Create3D(particleModel1);

	// Create 直後、すぐ表示されている。
	Engine::UpdateFrame();
	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.Basic_1.png")));

	// 1秒後、消える。
	for (int i = 0; i < 60; ++i)
	{
		Engine::UpdateFrame();
	}
	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.Basic_2.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, PositionVelocityAccel)
{
	auto m = SpriteParticleModel::Create();
	m->SetSpawnRate(10);
	m->SetLifeTime(10.0f);
	m->SetPositionRange(Vector3(-10, 0, 0), Vector3(10, 0, 0));
	m->SetVelocity(Vector3(0, 5, 0));
	m->SetAccel(Vector3(0, -8, 0));
	m->SetTexture(Texture2D::Create(LN_LOCALFILE("TestData/Particle1.png")));
	m->SetRandomSeed(12345);
	auto particle1 = SpriteParticle::Create3D(m);

	// TODO: カメラ位置固定しておかないと、初期値が変わった時に対応できないかも

	for (int i = 0; i < 60; ++i) { Engine::UpdateFrame(); }

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.PositionVelocityAccel.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, RandomBaseValue)
{
	auto m = SpriteParticleModel::Create();
	m->SetSpawnRate(10);
	m->SetLifeTime(10.0f);
	m->SetRandomBaseValueRange(0.0f, 1.0f);
	m->SetPositionRange(Vector3(-10, 0, 0), Vector3(10, 0, 0), ParticleRandomSource::ByBaseValue);
	m->SetSizeRange(0.5f, 3.0f, ParticleRandomSource::ByBaseValue);
	m->SetTexture(Texture2D::Create(LN_LOCALFILE("TestData/Particle1.png")));
	m->SetRandomSeed(12345);
	auto particle1 = SpriteParticle::Create3D(m);

	for (int i = 0; i < 60; ++i) { Engine::UpdateFrame(); }

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.RandomBaseValue.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, Billboard)
{
	auto m = SpriteParticleModel::Create();
	m->SetLifeTime(10.0f);
	m->SetSize(10.0f);
	m->SetTexture(Texture2D::Create(LN_LOCALFILE("TestData/Particle1.png")));
	auto particle1 = SpriteParticle::Create3D(m);

	// 斜め上から見下ろす
	Camera::GetDefault3DCamera()->SetPosition(20, 20, -20.0f);

	Engine::UpdateFrame();

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.Billboard.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, MovementDirection)
{
	auto m = SpriteParticleModel::Create();
	m->SetLifeTime(10.0f);
	m->SetSize(10.0f);
	m->SetVelocity(Vector3(0.0, 0, 2.0));
	m->m_particleDirection = ParticleDirection::MovementDirection;
	m->SetTexture(Texture2D::Create(LN_LOCALFILE("TestData/Particle2.png")));
	auto particle1 = SpriteParticle::Create3D(m);

	// 斜め上から見下ろす
	Camera::GetDefault3DCamera()->SetPosition(20, 20, -20.0f);

	for (int i = 0; i < 60; ++i) { Engine::UpdateFrame(); }

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.MovementDirection.png")));
	//TestEnv::SaveScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.MovementDirection.png"));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, MaxParticles)
{
	auto m = SpriteParticleModel::Create();
	m->SetSpawnRate(10);
	m->SetLifeTime(10.0f);
	m->SetPositionRange(Vector3(-10, 0, 0), Vector3(10, 0, 0));
	m->SetTexture(Texture2D::Create(LN_LOCALFILE("TestData/Particle1.png")));
	m->SetRandomSeed(12345);
	m->SetMaxParticles(1);
	auto particle1 = SpriteParticle::Create3D(m);

	Camera::GetDefault3DCamera()->SetPosition(0, 0, -20.0f);

	// 同時表示数は1個。また、繰り返しても配列外アクセスとかしないこと。
	for (int i = 0; i < 60; ++i) { Engine::UpdateFrame(); }

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.MaxParticles.png")));
}
