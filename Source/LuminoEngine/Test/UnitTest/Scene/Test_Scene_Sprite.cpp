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
		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));

		auto sprite1 = Sprite2D::create(tex);
		sprite1->setPosition(0, 0);

		auto sprite2 = Sprite2D::create(tex);
		sprite2->setPosition(32, 0);
		sprite2->setOpacity(0.5);
		sprite2->setBlendMode(BlendMode::Alpha);

		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.Basic1.png")));

		sprite1->removeFromWorld();
		sprite2->removeFromWorld();
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, BlendMode)
{
	auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
	auto sprite1 = Sprite2D::create(tex);
	auto sprite2 = Sprite2D::create(tex);
	auto sprite3 = Sprite2D::create(tex);
	auto sprite4 = Sprite2D::create(tex);
	auto sprite5 = Sprite2D::create(tex);

	sprite1->setPosition(0, 0);
	sprite1->setBlendMode(BlendMode::Normal);

	sprite2->setPosition(32, 0);
	sprite2->setBlendMode(BlendMode::Alpha);

	sprite3->setPosition(64, 0);
	sprite3->setBlendMode(BlendMode::Add);

	sprite4->setPosition(96, 0);
	sprite4->setBlendMode(BlendMode::Subtract);

	sprite5->setPosition(128, 0);
	sprite5->setBlendMode(BlendMode::Multiply);

	Engine::update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.BlendMode1.png")));
	Engine::getWorld2D()->RemoveAllObjects();
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Anchor)
{
	// <Test> 原点の指定
	{
		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));

		// 左上原点
		auto sprite1 = Sprite2D::create(tex);
		sprite1->setPosition(0, 0);
		sprite1->setAnchorPoint(0, 0);

		// 中央原点
		auto sprite2 = Sprite2D::create(tex);
		sprite2->setPosition(32, 32);
		sprite2->setAnchorPoint(Vector2(0.5, 0.5));

		// 右下原点
		auto sprite3 = Sprite2D::create(tex);
		sprite3->setPosition(64, 64);
		sprite3->setAnchorPoint(1, 1);

		// 中央下原点
		auto sprite4 = Sprite2D::create(tex);
		sprite4->setPosition(32, 120);
		sprite4->setAnchorPoint(0.5, 1);

		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.Anchor.png")));
		Engine::getWorld2D()->RemoveAllObjects();
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, DrawCallCount)
{
	// <Test> ステートが同一であれば1度のドローコールにまとめられる。
	{
		// 1度書いて、初期状態のときの描画数を覚えておく
		Engine::update();
		TestEnv::WaitRendering();
		int defaultCount = EngineDiag::getGraphicsDeviceDrawCount();

		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite2D::create(tex);
		auto sprite2 = Sprite2D::create(tex);
		auto sprite3 = Sprite2D::create(tex);

		Engine::update();
		TestEnv::WaitRendering();
		ASSERT_EQ(defaultCount + 1, EngineDiag::getGraphicsDeviceDrawCount());
		Engine::getWorld2D()->RemoveAllObjects();
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Issues_Volkoff)
{
	// <Issues> デフォルトのサンプラステートの繰り返しモードは Repert になる。
	{
		auto sprite1 = Sprite2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
		sprite1->setSourceRect(32, 0, 32, 32);
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_1.png")));
		Engine::getWorld2D()->RemoveAllObjects();
	}
	// <Issues> 2D では Z ソートの基準がカメラ位置からの直線距離ではなく、スクリーンからの距離でなければならない。
	{
		auto tex1 = Texture2D::create(32, 32);
		auto tex2 = Texture2D::create(32, 32);
		tex1->clear(Color32::Red);
		tex2->clear(Color32::Blue);
		auto s1 = Sprite2D::create(tex1);
		auto s2 = Sprite2D::create(tex2);
		s1->setPosition(10, 20, 100);
		s2->setPosition(15, 25, 100);	// スクリーンが Z 平面に平行なら、Z が同じときはあとから作ったものが常に手前になる。
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_2.png")));
		Engine::getWorld2D()->RemoveAllObjects();
	}
	// <Issues> 2D では Z ソートの基準がカメラ位置からの直線距離ではなく、スクリーンからの距離でなければならない。

	// <Issues> テクスチャを変更できること。
	{
		auto tex1 = Texture2D::create(32, 32);
		auto tex2 = Texture2D::create(32, 32);
		tex1->clear(Color32::Green);
		tex2->clear(Color32::Blue);
		auto s1 = Sprite2D::create(tex1);
		Engine::update();			// 1度描く
		s1->setTexture(tex2);			// 次にテクスチャを変更する
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_3.png")));
		Engine::getWorld2D()->RemoveAllObjects();
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
		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite3DComponent::create(1, 1, tex);
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite3D.Basic1.png")));
	}
	// <Test> 不透明度の設定
	{
		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite3DComponent::create(1, 1, tex);
		sprite1->setPosition(-1, 0, 0);
		sprite1->setOpacity(0.25f);
		auto sprite2 = Sprite3DComponent::create(1, 1, tex);
		sprite2->setPosition(0, 0, 0);
		sprite2->setOpacity(0.5f);
		auto sprite3 = Sprite3DComponent::create(1, 1, tex);
		sprite3->setPosition(1, 0, 0);
		sprite3->setOpacity(0.75f);
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite3D.Basic2.png"), 95));
	}
	// <Test> 普通の描画
	// <Test> 不透明度の設定
	//{
	//	Engine::update();
	//	int defaultCount = EngineDiag::getGraphicsDeviceDrawCount();

	//	auto tex1 = Texture2D::create(32, 32);
	//	auto tex2 = Texture2D::create(32, 32);
	//	auto sprite1 = Sprite3DComponent::create(1, 1, tex1);
	//	auto sprite2 = Sprite3DComponent::create(1, 1, tex2);
	//	auto sprite3 = Sprite3DComponent::create(1, 1, tex1);
	//	sprite1->setPosition(0, 0);
	//	sprite2->setPosition(10, 10);
	//	sprite3->setPosition(20, 20);

	//	Engine::update();
	//	int defaultCount2 = EngineDiag::getGraphicsDeviceDrawCount();
	//	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite3D.Basic1.png"), 99, true));
	//}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite3D, ViewFrustumCulling)
{
	// <Test> 視錐台化リング
	{
		Engine::update();
		TestEnv::WaitRendering();
		int count1 = EngineDiag::getGraphicsDeviceDrawCount();

		auto tex1 = Texture2D::create(32, 32);
		auto sprite1 = Sprite3DComponent::create(1, 1, tex1);
		Engine::update();
		TestEnv::WaitRendering();
		int count2= EngineDiag::getGraphicsDeviceDrawCount();
		ASSERT_EQ(count1 + 1, count2);

		sprite1->setPosition(-100, 0);
		Engine::update();
		TestEnv::WaitRendering();
		int count3 = EngineDiag::getGraphicsDeviceDrawCount();
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
		auto text = TextBlock2DComponent::create(_T("Lumino"));
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_TextBlock2D.Basic1.png")));
	}
	{
		auto text1 = TextBlock2DComponent::create();
		text1->setText(_T("upper left"));
		text1->setPosition(0, 0, 0);
		text1->setAnchorPoint(0, 0);

		auto text2 = TextBlock2DComponent::create();
		text2->setText(_T("upper right"));
		text2->setPosition(160, 0, 0);
		text2->setAnchorPoint(1, 0);

		auto text3 = TextBlock2DComponent::create();
		text3->setText(_T("lower left"));
		text3->setPosition(0, 120, 0);
		text3->setAnchorPoint(0, 1);

		auto text4 = TextBlock2DComponent::create();
		text4->setText(_T("lower right"));
		text4->setPosition(160, 120, 0);
		text4->setAnchorPoint(1, 1);

		auto text5 = TextBlock2DComponent::create();
		text5->setText(_T("center"));
		text5->setPosition(80, 60, 0);
		text5->setAnchorPoint(0.5, 0.5);

		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_TextBlock2D.Basic2.png"), 99, true));
	}
}
#endif
