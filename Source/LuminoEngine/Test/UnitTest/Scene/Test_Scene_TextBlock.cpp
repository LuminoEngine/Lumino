#include <TestConfig.h>
#include <Lumino/Scene/TextBlock.h>

//==============================================================================
class Test_Scene_TextBlock2D : public LuminoGraphicsTest {};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_TextBlock2D, Basic)
{
	{
		auto text = TestEnv::addWorld2D(TextBlock2D::create(_LT("Lumino")));
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_TextBlock2D.Basic1.png")));
		LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_TextBlock2D, AnchorPoint)
{
	{
		auto text1 = TestEnv::addWorld2D(TextBlock2D::create());
		text1->setText(_LT("U-L"));
		text1->setPosition(0, 0, 0);
		text1->setAnchorPoint(0, 0);

		auto text2 = TestEnv::addWorld2D(TextBlock2D::create());
		text2->setText(_LT("U-R"));
		text2->setPosition(160, 0, 0);
		text2->setAnchorPoint(1, 0);

		auto text3 = TestEnv::addWorld2D(TextBlock2D::create());
		text3->setText(_LT("L-L"));
		text3->setPosition(0, 120, 0);
		text3->setAnchorPoint(0, 1);

		auto text4 = TestEnv::addWorld2D(TextBlock2D::create());
		text4->setText(_LT("L-R"));
		text4->setPosition(160, 120, 0);
		text4->setAnchorPoint(1, 1);

		auto text5 = TestEnv::addWorld2D(TextBlock2D::create());
		text5->setText(_LT("C"));
		text5->setPosition(80, 60, 0);
		text5->setAnchorPoint(0.5, 0.5);

		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_TextBlock2D.AnchorPoint1.png")));
		LN_TEST_CLEAN_SCENE;
	}
}
