#include <TestConfig.h>

class IntegrateTest_Scene_VisualNode : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(IntegrateTest_Scene_VisualNode, Visible)
{
	// 1“x‘‚¢‚ÄA‰Šúó‘Ô‚Ì‚Æ‚«‚Ì•`‰æ”‚ðŠo‚¦‚Ä‚¨‚­
	Engine::UpdateFrame();
	int defaultCount = EngineDiag::GetVisualNodeDrawCount();

	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
	auto sprite1 = Sprite2D::Create(tex);
	auto sprite2 = Sprite2D::Create(tex);

	sprite1->SetVisible(false);

	Engine::UpdateFrame();
	ASSERT_EQ(defaultCount+1, EngineDiag::GetVisualNodeDrawCount());
}

//------------------------------------------------------------------------------
TEST_F(IntegrateTest_Scene_VisualNode, MaterialUtilities)
{

	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
	auto sprite1 = Sprite2D::Create(tex);
	//auto sprite2 = Sprite2D::Create(tex);

	sprite1->SetOpacity(0.25f);

	//sprite2->SetPosition(10, 10);

	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color::Gray);
	Engine::UpdateFrame();
	int defaultCount2 = EngineDiag::GetVisualNodeDrawCount();



	//ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/IntegrateTest_Scene_Sprite.BlendMode.png")));
	TestEnv::SaveScreenShot(LN_LOCALFILE("test.png"));
}

