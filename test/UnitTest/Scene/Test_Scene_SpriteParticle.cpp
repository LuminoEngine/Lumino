#include <TestConfig.h>


class Test_Scene_SpriteParticle : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};


//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, Basic)
{
	auto particleModel1 = SpriteParticleModel::Create();
	particleModel1->m_spawnRate = 1;
	//particleModel1->m_minPosition.Set(-10, 0, 0);
	//particleModel1->m_maxPosition.Set(10, 0, 0);
	//particleModel1->m_minVelocity.Set(-5, 5, 0);
	//particleModel1->m_maxVelocity.Set(5, 5, 0);
	//particleModel1->m_minAccel.Set(0, -8, 0);
	//particleModel1->m_maxAccel.Set(0, -8, 0);
	particleModel1->m_lifeTimeMin = 10.0;
	particleModel1->m_lifeTimeMax = 10.0;
	particleModel1->m_emitterDuration = 10.0;
	particleModel1->SetTexture(Texture2D::Create(LN_LOCALFILE("TestData/Particle1.png")));
	auto particle1 = SpriteParticle::Create3D(particleModel1);

	while (Engine::UpdateFrame())
	{

	}
	//TestEnv::SaveScreenShot(LN_TEMPFILE("test.png"));


}
