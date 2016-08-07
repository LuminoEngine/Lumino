#include <TestConfig.h>

class Test_Scene_Sprite : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, BlendMode)
{
	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
	auto sprite1 = Sprite2D::Create(tex);
	auto sprite2 = Sprite2D::Create(tex);
	auto sprite3 = Sprite2D::Create(tex);
	auto sprite4 = Sprite2D::Create(tex);
	auto sprite5 = Sprite2D::Create(tex);
	auto sprite6 = Sprite2D::Create(tex);

	sprite2->SetPosition(10, 10);
	sprite2->SetBlendMode(BlendMode::Add);

	sprite3->SetPosition(20, 20);
	sprite3->SetBlendMode(BlendMode::Subtract);

	sprite4->SetPosition(30, 30);
	sprite4->SetBlendMode(BlendMode::AddAlphaDisabled);

	sprite5->SetPosition(40, 40);
	sprite5->SetBlendMode(BlendMode::SubtractAlphaDisabled);

	sprite6->SetPosition(50, 50);
	sprite6->SetBlendMode(BlendMode::MultiplyAlphaDisabled);

	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color32::Gray);

	Engine::UpdateFrame();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/IntegrateTest_Scene_Sprite.BlendMode.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Issues_Volkoff)
{
	// <Issues> デフォルトのサンプラステートの繰り返しモードは Repert になる。
	{
		auto sprite1 = Sprite2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		sprite1->SetSrcRect(32, 0, 32, 32);
		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Scene_Sprite.Issues_Volkoff_1.png"), 100));
	}
}
