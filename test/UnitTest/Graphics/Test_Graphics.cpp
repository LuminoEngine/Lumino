#include <TestConfig.h>

//==============================================================================
class Test_Graphics_Color : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Constructor)
{
	Color32 c1;
	ASSERT_EQ(Color32(0, 0, 0, 0), c1);
	Color32 c2(1, 2, 3);
	ASSERT_EQ(Color32(1, 2, 3, 255), c2);
	Color32 c3(1, 2, 3, 4);
	ASSERT_EQ(Color32(1, 2, 3, 4), c3);
}


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Operators)
{
	Color32 c1(1, 2, 3, 4);
	Color32 c2(1, 2, 3, 4);
	Color32 c3(1, 2, 3, 5);
	ASSERT_EQ(true, c1 == c2);
	ASSERT_EQ(false, c1 == c3);
	ASSERT_EQ(true, c1 != c3);
	ASSERT_EQ(false, c1 != c2);
}

//==============================================================================
class Test_Graphics_Viewport : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Viewport, Basic)
{
	const SizeI& size = Viewport::GetMainViewport()->GetSize();
	ASSERT_EQ(160, size.width);
	ASSERT_EQ(120, size.height);
}

//==============================================================================
class Test_Graphics_Rendering : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, Basic)
{

	//if (Engine::BeginRendering())
	//{
	//	Engine::Render();

	//	auto* r = Engine::GetDefault2DLayer()->GetRenderer();
	//	r->Clear(ClearFlags::Color, Color::Red);
	//	auto tex = Texture2D::Create(LN_LOCALFILE("../Scene/TestData/Sprite1.png"));
	//	r->DrawSprite2D(SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color::White);

	//	Engine::EndRendering();
	//}

	//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Basic1.png"), 99, true));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, Clear)
{
	// <Test> 2D シーンのクリア
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto* r1 = Engine::GetDefault2DLayer()->GetRenderer();
			r1->Clear(ClearFlags::Color, Color::Blue);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear1.png")));
	}
	// <Test> 3D シーンのクリア
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto* r1 = Engine::GetDefault3DLayer()->GetRenderer();
			r1->Clear(ClearFlags::Color, Color::Red);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear2.png")));
	}
	// <Test> 2D、3D シーンのクリアの組み合わせ。2D が手前になる
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Clear(ClearFlags::Color, Color::Blue);
			Engine::GetDefault3DLayer()->GetRenderer()->Clear(ClearFlags::Color, Color::Red);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear3.png")));
	}
}
