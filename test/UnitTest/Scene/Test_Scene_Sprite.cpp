#include <TestConfig.h>

class Test_Scene_Sprite : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Basic)
{
	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
	auto sprite1 = Sprite2D::Create(tex);
	sprite1->SetSize(SizeF(32, 32));
	Engine::UpdateFrame();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Scene_Sprite.Basic.png")));
}

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
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Scene_Sprite.BlendMode.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, DrawCallCount)
{
	// <Test> 
	{
		// 1度書いて、初期状態のときの描画数を覚えておく
		Engine::UpdateFrame();
		int defaultCount = EngineDiag::GetGraphicsDeviceDrawCount();

		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite2D::Create(tex);
		auto sprite2 = Sprite2D::Create(tex);
		auto sprite3 = Sprite2D::Create(tex);

		Engine::UpdateFrame();
		int defaultCount3 = EngineDiag::GetGraphicsDeviceDrawCount();
		ASSERT_EQ(defaultCount + 1, EngineDiag::GetGraphicsDeviceDrawCount());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Issues_Volkoff)
{
	// <Issues> デフォルトのサンプラステートの繰り返しモードは Repert になる。
	{
		auto sprite1 = Sprite2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		sprite1->SetSrcRect(32, 0, 32, 32);
		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Scene_Sprite.Issues_Volkoff_1.png")));
	}
	// <Issues> 2D では Z ソートの基準がカメラ位置からの直線距離ではなく、スクリーンからの距離でなければならない。
	{
		auto tex1 = Texture2D::Create(32, 32);
		auto tex2 = Texture2D::Create(32, 32);
		tex1->Clear(Color32::Red);
		tex2->Clear(Color32::Blue);
		auto s1 = Sprite2D::Create(tex1);
		auto s2 = Sprite2D::Create(tex2);
		s1->SetPosition(10, 20, 100);
		s2->SetPosition(15, 25, 100);	// スクリーンが Z 平面に平行なら、Z が同じときはあとから作ったものが常に手前になる。
		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Scene_Sprite.Issues_Volkoff_2.png")));
	}
	// <Issues> 2D では Z ソートの基準がカメラ位置からの直線距離ではなく、スクリーンからの距離でなければならない。
	{
		auto tex1 = Texture2D::Create(32, 32);
		auto tex2 = Texture2D::Create(32, 32);
		tex1->Clear(Color32::Red);
		tex2->Clear(Color32::Blue);
		auto s1 = Sprite2D::Create(tex1);
		Engine::UpdateFrame();			// 1度描く
		s1->SetTexture(tex2);			// 次にテクスチャを変更する
		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Scene_Sprite.Issues_Volkoff_3.png")));
	}
}
