#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>

class Test_Scene_SceneNode : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#if 0
//------------------------------------------------------------------------------
TEST_F(Test_Scene_SceneNode, Visible)
{
	// <Test> Visible プロパティが false の場合は、ドローコール自体呼び出されないこと。
	{
		// 1度書いて、初期状態のときの描画数を覚えておく
		Engine::Update();
		//int defaultCount = EngineDiag::GetVisualNodeDrawCount();
		int defaultCount = Engine::GetWorld2D()->GetRenderingProfiler().GetLastFrameData()->nodeDrawCount;

		auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite2DComponent::create(tex);
		auto sprite2 = Sprite2DComponent::create(tex);

		sprite1->SetVisible(false);

		Engine::Update();
		ASSERT_EQ(defaultCount + 1, Engine::GetWorld2D()->GetRenderingProfiler().GetLastFrameData()->nodeDrawCount);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SceneNode, DepthTest)
{
	auto tex1 = Texture2D::create(32, 32);
	auto tex2 = Texture2D::create(32, 32);
	tex1->Clear(Color32::Red);
	tex2->Clear(Color32::Blue);

	// <Test> デフォルトでは深度テスト&深度書き込みは有効。
	{
		auto box1 = StaticMeshComponent::CreateBox(Vector3(1, 2, 3));
		auto box2 = StaticMeshComponent::CreateBox(Vector3(2, 1, 1));
		box1->GetMaterials()->GetAt(0)->SetMaterialTexture(tex1);
		box2->GetMaterials()->GetAt(0)->SetMaterialTexture(tex2);
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_SceneNode.DepthTest1.png")));
	}
	// <Test> 深度テスト無効
	{
		auto s1 = Sprite3DComponent::create(5, 5, tex1);
		auto s2 = Sprite3DComponent::create(5, 5, tex2);
		s1->SetAngles(0, Math::PI / 4, 0);
		s2->SetAngles(0, -Math::PI / 4, 0);
		s2->SetDepthTestEnabled(false);
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_SceneNode.DepthTest2.png"), 95));
	}
	// <Test> 深度書き込み無効
	{
		auto s1 = Sprite3DComponent::create(5, 5, tex1);
		auto s2 = Sprite3DComponent::create(5, 5, tex2);
		s1->SetAngles(0, Math::PI / 4, 0);
		s1->SetDepthWriteEnabled(false);
		s2->SetAngles(0, -Math::PI / 4, 0);
		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_SceneNode.DepthTest2.png"), 95));	// 結果は↑と同じ
	}
}
#endif

//------------------------------------------------------------------------------
//TEST_F(Test_Scene_SceneNode, MaterialUtilities)
//{
//
//	auto tex = Texture2D::create(LN_LOCALFILE("TestData/Sprite1.png"));
//	auto sprite1 = Sprite2DComponent::create(tex);
//	//auto sprite2 = Sprite2DComponent::create(tex);
//
//	sprite1->SetOpacity(0.25f);
//
//	//sprite2->SetPosition(10, 10);
//
//	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color32::Gray);
//	Engine::UpdateFrame();
//	int defaultCount2 = EngineDiag::GetVisualNodeDrawCount();
//
//
//
//	//ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/IntegrateTest_Scene_Sprite.BlendMode.png")));
//	TestEnv::SaveScreenShot(LN_LOCALFILE("test.png"));
//}
//
