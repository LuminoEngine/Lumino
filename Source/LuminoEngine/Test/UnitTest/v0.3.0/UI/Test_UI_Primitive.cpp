#include <TestConfig.h>

//==============================================================================
class Test_UI_TextBlock : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_TextBlock, DefaultLayout)
{
	/*
		レイアウトルートに直接 setContent() しただけの場合は、左上に表示される、で正しい。(WPF)
		WPF の TextBlock を同様にデフォルト状態で配置すると、
		- Widht,Height は NaN
		- ActualWidth,ActualHeight はウィンドウいっぱい
		となる。
	*/

	auto uiRoot = Application::getMainWindow();
	auto textBlock1 = UITextBlock::create();
	textBlock1->setText(_LT("TextBlock"));
	uiRoot->addChild(textBlock1);

	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_TextBlock.Basic.png")));
	uiRoot->removeChild(textBlock1);
}


//==============================================================================
class Test_UI_Image : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_Image, Basic)
{
	/*
		WPF の Image はデフォルトでは Stratch だが、Lumino は None。
		経験的にだが、スケーリングしないで使うことのほうが多い。
	*/

	// <Test> アルファブレンドは最初から有効
	{
		auto uiRoot = Application::getMainWindow();
		auto image1 = UIImage::create(LN_LOCALFILE("../Graphics/TestData/Sprite2.png"));
		uiRoot->addChild(image1);

		Engine::update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_Image.Basic1.png")));
		uiRoot->removeChild(image1);
	}
}

