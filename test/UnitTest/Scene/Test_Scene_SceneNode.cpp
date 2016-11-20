#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>

class Test_Scene_SceneNode : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SceneNode, Visible)
{
	// <Test> Visible プロパティが false の場合は、ドローコール自体呼び出されないこと。
	{
		// 1度書いて、初期状態のときの描画数を覚えておく
		Engine::Update();
		//int defaultCount = EngineDiag::GetVisualNodeDrawCount();
		int defaultCount = Engine::GetDefaultSceneGraph2D()->GetRenderingProfiler().GetLastFrameData()->nodeDrawCount;

		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite2D::Create(tex);
		auto sprite2 = Sprite2D::Create(tex);

		sprite1->SetVisible(false);

		Engine::Update();
		ASSERT_EQ(defaultCount + 1, Engine::GetDefaultSceneGraph2D()->GetRenderingProfiler().GetLastFrameData()->nodeDrawCount);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SceneNode, DepthTest)
{
	// <Test> デフォルトでは深度テスト&深度書き込みは有効。
	{
		auto tex1 = Texture2D::Create(32, 32);
		auto tex2 = Texture2D::Create(32, 32);
		tex1->Clear(Color32::Red);
		tex2->Clear(Color32::Blue);

		auto box1 = StaticMesh::CreateBox(Vector3(1, 2, 3));
		auto box2 = StaticMesh::CreateBox(Vector3(2, 1, 1));
		box1->GetMaterials()->GetAt(0)->SetMaterialTexture(tex1);
		box2->GetMaterials()->GetAt(0)->SetMaterialTexture(tex2);

		Engine::Update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Scene_SceneNode.DepthTest.png")));
	}
}

//------------------------------------------------------------------------------
//TEST_F(Test_Scene_SceneNode, MaterialUtilities)
//{
//
//	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
//	auto sprite1 = Sprite2D::Create(tex);
//	//auto sprite2 = Sprite2D::Create(tex);
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
