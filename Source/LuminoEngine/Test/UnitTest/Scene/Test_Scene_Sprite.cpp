#include <TestConfig.h>

//==============================================================================
//# スプライトのテスト
class Test_Scene_Sprite2D : public LuminoGraphicsTest {};

//------------------------------------------------------------------------------
//## 画像を表示する
TEST_F(Test_Scene_Sprite2D, Basic)
{
	//- [ ] 普通の描画
	//- [ ] 不透明度の設定
	{
		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = TestEnv::addWorld2D(Sprite2D::create(tex));
		sprite1->setPosition(0, 0);

		auto sprite2 = TestEnv::addWorld2D(Sprite2D::create(tex));
		sprite2->setPosition(32, 0);
		sprite2->setOpacity(0.5);
		sprite2->setBlendMode(BlendMode::Alpha);

		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite2D.Basic1.png")));
		LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## スプライトのブレンドモードを変更できる
TEST_F(Test_Scene_Sprite2D, BlendMode)
{
	//- [ ] Normal
	//- [ ] Alpha
	//- [ ] Add
	//- [ ] Subtract
	//- [ ] Multiply
	auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
	auto sprite1 = TestEnv::addWorld2D(Sprite2D::create(tex));
	auto sprite2 = TestEnv::addWorld2D(Sprite2D::create(tex));
	auto sprite3 = TestEnv::addWorld2D(Sprite2D::create(tex));
	auto sprite4 = TestEnv::addWorld2D(Sprite2D::create(tex));
	auto sprite5 = TestEnv::addWorld2D(Sprite2D::create(tex));

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
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite2D.BlendMode1.png")));
	LN_TEST_CLEAN_SCENE;
}

//------------------------------------------------------------------------------
//## スプライトの Anchor を変更できる
TEST_F(Test_Scene_Sprite2D, Anchor)
{
	auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));

	//- [ ] 左上原点
	auto sprite1 = TestEnv::addWorld2D(Sprite2D::create(tex));
	sprite1->setPosition(0, 0);
	sprite1->setAnchorPoint(0, 0);

	//- [ ] 中央原点
	auto sprite2 = TestEnv::addWorld2D(Sprite2D::create(tex));
	sprite2->setPosition(32, 32);
	sprite2->setAnchorPoint(Vector2(0.5, 0.5));

	//- [ ] 右下原点
	auto sprite3 = TestEnv::addWorld2D(Sprite2D::create(tex));
	sprite3->setPosition(64, 64);
	sprite3->setAnchorPoint(1, 1);

	//- [ ] 中央下原点
	auto sprite4 = TestEnv::addWorld2D(Sprite2D::create(tex));
	sprite4->setPosition(32, 120);
	sprite4->setAnchorPoint(0.5, 1);

	Engine::update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite2D.Anchor.png")));
	LN_TEST_CLEAN_SCENE;
}

//------------------------------------------------------------------------------
//TEST_F(Test_Scene_Sprite2D, DrawCallCount)
//{
//	// <Test> ステートが同一であれば1度のドローコールにまとめられる。
//	{
//		// 1度書いて、初期状態のときの描画数を覚えておく
//		Engine::update();
//		TestEnv::WaitRendering();
//		int defaultCount = EngineDiag::getGraphicsDeviceDrawCount();
//
//		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
//		auto sprite1 = TestEnv::addWorld2D(Sprite2D::create(tex));
//		auto sprite2 = TestEnv::addWorld2D(Sprite2D::create(tex));
//		auto sprite3 = TestEnv::addWorld2D(Sprite2D::create(tex));
//
//		Engine::update();
//		TestEnv::WaitRendering();
//		ASSERT_EQ(defaultCount + 1, EngineDiag::getGraphicsDeviceDrawCount());
//		LN_TEST_CLEAN_SCENE;
//	}
//}

//------------------------------------------------------------------------------
//## 不具合のリグレッションテスト
TEST_F(Test_Scene_Sprite2D, Issues_Volkoff)
{
	//- [ ] デフォルトのサンプラステートの繰り返しモードは Repert になる。
	{
		auto sprite1 = TestEnv::addWorld2D(Sprite2D::create(LN_LOCALFILE("TestData/Sprite1.png")));
		sprite1->setSourceRect(32, 0, 32, 32);
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite2D.Issues_Volkoff_1.png")));
		LN_TEST_CLEAN_SCENE;
	}

	//- [ ] 2D では Z ソートの基準がカメラ位置からの直線距離ではなく、スクリーンからの距離でなければならない。
	{
		auto tex1 = Texture2D::create(32, 32);
		auto tex2 = Texture2D::create(32, 32);
		tex1->clear(Color32::Red);
		tex2->clear(Color32::Green);
		auto s1 = TestEnv::addWorld2D(Sprite2D::create(tex1));
		auto s2 = TestEnv::addWorld2D(Sprite2D::create(tex2));
		s1->setPosition(10, 20, 100);
		s2->setPosition(15, 25, 100);	// スクリーンが Z 平面に平行なら、Z が同じときはあとから作ったものが常に手前になる。
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite2D.Issues_Volkoff_2.png")));
		LN_TEST_CLEAN_SCENE;
	}

	//- [ ] テクスチャを変更できること。
	{
		auto tex1 = Texture2D::create(32, 32);
		auto tex2 = Texture2D::create(32, 32);
		tex1->clear(Color32::Green);
		tex2->clear(Color32::Blue);
		auto s1 = TestEnv::addWorld2D(Sprite2D::create(tex1));
		Engine::update();			// 1度描く
		s1->setTexture(tex2);			// 次にテクスチャを変更する
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite2D.Issues_Volkoff_3.png")));
		LN_TEST_CLEAN_SCENE;
	}
}


//==============================================================================
//# スプライト (3D) のテスト
class Test_Scene_Sprite3D : public LuminoGraphicsTest {};

//------------------------------------------------------------------------------
//## 画像を表示する
TEST_F(Test_Scene_Sprite3D, Basic)
{
	ScopedCameraPosition camera(0, 0, -10);
	//- [ ] 普通の描画
	{
		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = TestEnv::addWorld3D(Sprite3D::create(5, 5, tex));
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite3D.Basic1.png"), 95));
		LN_TEST_CLEAN_SCENE;
	}
	//- [ ] 不透明度の設定
	{
		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = TestEnv::addWorld3D(Sprite3D::create(5, 5, tex));
		sprite1->setPosition(-5, 0, 0);
		sprite1->setOpacity(0.25f);
		auto sprite2 = TestEnv::addWorld3D(Sprite3D::create(5, 5, tex));
		sprite2->setPosition(0, 0, 0);
		sprite2->setOpacity(0.5f);
		auto sprite3 = TestEnv::addWorld3D(Sprite3D::create(5, 5, tex));
		sprite3->setPosition(5, 0, 0);
		sprite3->setOpacity(0.75f);
		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite3D.Basic2.png"), 95));
		LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## スプライトの視錐台カリングのテスト
TEST_F(Test_Scene_Sprite3D, ViewFrustumCulling)
{
	//- [ ] 視界外はドローコールが無くなる
	{
		Engine::update();
		TestEnv::WaitRendering();
		int count1 = EngineDiag::getGraphicsDeviceDrawCount();

		auto tex1 = Texture2D::create(32, 32);
		auto sprite1 = TestEnv::addWorld3D(Sprite3D::create(1, 1, tex1));
		Engine::update();
		TestEnv::WaitRendering();
		int count2= EngineDiag::getGraphicsDeviceDrawCount();
		ASSERT_EQ(true, count1 < count2);	// 少なくとも何も無い状態での update() よりも多いこと (DepthPrepass など、Scene 側の都合で複数回描かれることもある)

		sprite1->setPosition(-100, 0);
		Engine::update();
		TestEnv::WaitRendering();
		int count3 = EngineDiag::getGraphicsDeviceDrawCount();
		ASSERT_EQ(count1, count3);	// 完全に範囲外なので描画されない
	}
}

//------------------------------------------------------------------------------
//## 不具合のリグレッションテスト
TEST_F(Test_Scene_Sprite3D, Issues_HC1)
{
	//- [ ] スプライトの BillboardType が ToScreen、カメラ位置が Z+ にあるとき、Sprite3D の向きが反転してしまう問題の修正。
	{
		Vector3 pos(0, 0, 10);
		ScopedCameraPosition cp(pos + Vector3(0, 2, -8), pos);

		auto sprite1 = TestEnv::addWorld3D(Sprite3D::create(7, 5, Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"))));
		sprite1->setPosition(pos);
		sprite1->getSpriteComponent()->setBillboardType(BillboardType::ToScreen);

		Engine::update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestResult/Test_Scene_Sprite3D.Issues_HC1_1.png"), 90));
		LN_TEST_CLEAN_SCENE;
	}
}
