#include <TestConfig.h>
#include <Lumino/Scene/TextBlock.h>

//==============================================================================
class Test_Scene_Sprite : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Basic)
{
	// <Test> 普通の描画
	// <Test> 不透明度の設定
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));

		auto sprite1 = Sprite2DComponent::Create(tex);
		sprite1->SetPosition(0, 0);

		auto sprite2 = Sprite2DComponent::Create(tex);
		sprite2->SetPosition(32, 0);
		sprite2->SetOpacity(0.5);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.Basic1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, BlendMode)
{
	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
	auto sprite1 = Sprite2DComponent::Create(tex);
	auto sprite2 = Sprite2DComponent::Create(tex);
	auto sprite3 = Sprite2DComponent::Create(tex);
	auto sprite4 = Sprite2DComponent::Create(tex);
	auto sprite5 = Sprite2DComponent::Create(tex);

	sprite1->SetPosition(0, 0);
	sprite1->SetBlendMode(BlendMode::Normal);

	sprite2->SetPosition(32, 0);
	sprite2->SetBlendMode(BlendMode::Alpha);

	sprite3->SetPosition(64, 0);
	sprite3->SetBlendMode(BlendMode::Add);

	sprite4->SetPosition(96, 0);
	sprite4->SetBlendMode(BlendMode::Subtract);

	sprite5->SetPosition(128, 0);
	sprite5->SetBlendMode(BlendMode::Multiply);

	Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.BlendMode1.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Anchor)
{
	// <Test> 原点の指定
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));

		// 左上原点
		auto sprite1 = Sprite2DComponent::Create(tex);
		sprite1->SetPosition(0, 0);
		sprite1->SetAnchorPoint(0, 0);

		// 中央原点
		auto sprite2 = Sprite2DComponent::Create(tex);
		sprite2->SetPosition(32, 32);
		sprite2->SetAnchorPoint(Vector2(0.5, 0.5));

		// 右下原点
		auto sprite3 = Sprite2DComponent::Create(tex);
		sprite3->SetPosition(64, 64);
		sprite3->SetAnchorPoint(1, 1);

		// 中央下原点
		auto sprite4 = Sprite2DComponent::Create(tex);
		sprite4->SetPosition(32, 120);
		sprite4->SetAnchorPoint(0.5, 1);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.Anchor.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, DrawCallCount)
{
	// <Test> ステートが同一であれば1度のドローコールにまとめられる。
	{
		// 1度書いて、初期状態のときの描画数を覚えておく
		Engine::Update();
		TestEnv::WaitRendering();
		int defaultCount = EngineDiag::GetGraphicsDeviceDrawCount();

		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite2DComponent::Create(tex);
		auto sprite2 = Sprite2DComponent::Create(tex);
		auto sprite3 = Sprite2DComponent::Create(tex);

		Engine::Update();
		TestEnv::WaitRendering();
		ASSERT_EQ(defaultCount + 1, EngineDiag::GetGraphicsDeviceDrawCount());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Issues_Volkoff)
{
	// <Issues> デフォルトのサンプラステートの繰り返しモードは Repert になる。
	{
		auto sprite1 = Sprite2DComponent::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		sprite1->SetTextureRect(32, 0, 32, 32);
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_1.png")));
	}
	// <Issues> 2D では Z ソートの基準がカメラ位置からの直線距離ではなく、スクリーンからの距離でなければならない。
	{
		auto tex1 = Texture2D::Create(32, 32);
		auto tex2 = Texture2D::Create(32, 32);
		tex1->Clear(Color32::Red);
		tex2->Clear(Color32::Blue);
		auto s1 = Sprite2DComponent::Create(tex1);
		auto s2 = Sprite2DComponent::Create(tex2);
		s1->SetPosition(10, 20, 100);
		s2->SetPosition(15, 25, 100);	// スクリーンが Z 平面に平行なら、Z が同じときはあとから作ったものが常に手前になる。
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_2.png")));
	}
	// <Issues> 2D では Z ソートの基準がカメラ位置からの直線距離ではなく、スクリーンからの距離でなければならない。

	// <Issues> テクスチャを変更できること。
	{
		auto tex1 = Texture2D::Create(32, 32);
		auto tex2 = Texture2D::Create(32, 32);
		tex1->Clear(Color32::Green);
		tex2->Clear(Color32::Blue);
		auto s1 = Sprite2DComponent::Create(tex1);
		Engine::Update();			// 1度描く
		s1->SetTexture(tex2);			// 次にテクスチャを変更する
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_3.png")));
	}
}





//==============================================================================
class Test_Scene_Sprite3D : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite3D, Basic)
{
	// <Test> 普通の描画
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite3DComponent::Create(1, 1, tex);
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite3D.Basic1.png")));
	}
	// <Test> 不透明度の設定
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite3DComponent::Create(1, 1, tex);
		sprite1->SetPosition(-1, 0, 0);
		sprite1->SetOpacity(0.25f);
		auto sprite2 = Sprite3DComponent::Create(1, 1, tex);
		sprite2->SetPosition(0, 0, 0);
		sprite2->SetOpacity(0.5f);
		auto sprite3 = Sprite3DComponent::Create(1, 1, tex);
		sprite3->SetPosition(1, 0, 0);
		sprite3->SetOpacity(0.75f);
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite3D.Basic2.png"), 95));
	}
	// <Test> 普通の描画
	// <Test> 不透明度の設定
	//{
	//	Engine::Update();
	//	int defaultCount = EngineDiag::GetGraphicsDeviceDrawCount();

	//	auto tex1 = Texture2D::Create(32, 32);
	//	auto tex2 = Texture2D::Create(32, 32);
	//	auto sprite1 = Sprite3DComponent::Create(1, 1, tex1);
	//	auto sprite2 = Sprite3DComponent::Create(1, 1, tex2);
	//	auto sprite3 = Sprite3DComponent::Create(1, 1, tex1);
	//	sprite1->SetPosition(0, 0);
	//	sprite2->SetPosition(10, 10);
	//	sprite3->SetPosition(20, 20);

	//	Engine::Update();
	//	int defaultCount2 = EngineDiag::GetGraphicsDeviceDrawCount();
	//	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite3D.Basic1.png"), 99, true));
	//}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite3D, ViewFrustumCulling)
{
	// <Test> 視錐台化リング
	{
		Engine::Update();
		TestEnv::WaitRendering();
		int count1 = EngineDiag::GetGraphicsDeviceDrawCount();

		auto tex1 = Texture2D::Create(32, 32);
		auto sprite1 = Sprite3DComponent::Create(1, 1, tex1);
		Engine::Update();
		TestEnv::WaitRendering();
		int count2= EngineDiag::GetGraphicsDeviceDrawCount();
		ASSERT_EQ(count1 + 1, count2);

		sprite1->SetPosition(-100, 0);
		Engine::Update();
		TestEnv::WaitRendering();
		int count3 = EngineDiag::GetGraphicsDeviceDrawCount();
		ASSERT_EQ(count1, count3);	// 完全に範囲外なので描画されない
	}
}

#if 1

//==============================================================================
class Test_Scene_TextBlock2D : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_TextBlock2D, Basic)
{
	{
		auto text = TextBlock2DComponent::Create(_T("Lumino"));
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_TextBlock2D.Basic1.png")));
	}
	{
		auto text1 = TextBlock2DComponent::Create();
		text1->SetText(_T("upper left"));
		text1->SetPosition(0, 0, 0);
		text1->SetAnchorPoint(0, 0);

		auto text2 = TextBlock2DComponent::Create();
		text2->SetText(_T("upper right"));
		text2->SetPosition(160, 0, 0);
		text2->SetAnchorPoint(1, 0);

		auto text3 = TextBlock2DComponent::Create();
		text3->SetText(_T("lower left"));
		text3->SetPosition(0, 120, 0);
		text3->SetAnchorPoint(0, 1);

		auto text4 = TextBlock2DComponent::Create();
		text4->SetText(_T("lower right"));
		text4->SetPosition(160, 120, 0);
		text4->SetAnchorPoint(1, 1);

		auto text5 = TextBlock2DComponent::Create();
		text5->SetText(_T("center"));
		text5->SetPosition(80, 60, 0);
		text5->SetAnchorPoint(0.5, 0.5);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_TextBlock2D.Basic2.png"), 99, true));
	}
}
#endif
