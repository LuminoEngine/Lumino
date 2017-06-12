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
		レイアウトルートに直接 SetContent() しただけの場合は、左上に表示される、で正しい。(WPF)
		WPF の TextBlock を同様にデフォルト状態で配置すると、
		- Widht,Height は NaN
		- ActualWidth,ActualHeight はウィンドウいっぱい
		となる。
	*/

	auto uiRoot = Engine::GetMainWindow();
	auto textBlock1 = UITextBlock::create();
	textBlock1->setText(_T("TextBlock"));
	uiRoot->AddChild(textBlock1);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_TextBlock.Basic.png")));
	uiRoot->RemoveChild(textBlock1);
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
		auto uiRoot = Engine::GetMainWindow();
		auto image1 = UIImage::create(LN_LOCALFILE("../Graphics/TestData/Sprite2.png"));
		uiRoot->AddChild(image1);

		Engine::Update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_Image.Basic1.png")));
		uiRoot->RemoveChild(image1);
	}
}

