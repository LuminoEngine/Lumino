#include <TestConfig.h>


class IntegrateTest_Scene_Sprite : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};


//-----------------------------------------------------------------------------
TEST_F(IntegrateTest_Scene_Sprite, Basic)
{
	auto sprite1 = Sprite2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));

	Engine::UpdateFrame();
	TestEnv::SaveScreenShot(LN_TEMPFILE("test.png"));

	//while (true)
	//{
	//	if (!Engine::UpdateFrame()) {
	//		break;
	//	}
	//}


#if 0


	//SceneGraphManager::ConfigData c;
	//c.FileManager = &FileManager::GetInstance();
	//c.PhysicsManager = TestEnv::PhysicsManager;
	//c.GraphicsManager = TestEnv::Manager;
	//RefPtr<SceneGraphManager> sceneGraph(LN_NEW SceneGraphManager(c));

	SceneGraphManager* sceneGraph = TestEnv::Application->GetSceneGraphManager();
	sceneGraph->CreateMMDSceneGraph();

	ByteBuffer code(FileSystem::ReadAllBytes(LOCALFILE("../../../src/Scene/Resource/SSNoLighting.fx")));
	RefPtr<Shader> shader(Shader::Create((char*)code.GetData(), code.GetSize()));
	MMEShaderErrorInfo errInfo;
	RefPtr<MMEShader> mmeShader(MMEShaderBuilder::Create(sceneGraph, shader, &errInfo));


	RefPtr<Texture> tex(Texture::Create(LOCALFILE("TestData/Test1.png")));
	//RefPtr<Texture> tex(Texture::Create(_T("D:/GameProjects/Chronicles/t2.png")));
	RefPtr<Sprite> sprite(LN_NEW Sprite());
	sprite->Create(SpriteCoord_RZ, sceneGraph);
	sprite->SetSize(SizeF(1, 1));
	sprite->SetTexture(tex);
	sprite->SetShader(mmeShader);

	sceneGraph->Get3DRootNode()->AddChild(sprite);


	Renderer* r = TestEnv::Renderer;
	SwapChain* swap = TestEnv::MainSwapChain;
	while (TestEnv::Platform->DoEvents())
	{
		if (TestEnv::Platform->GetMainWindow()->GetSize() != swap->GetBackBuffer()->GetSize())
		{
			swap->Resize(TestEnv::Platform->GetMainWindow()->GetSize());
		}


		Renderer* r = TestEnv::BeginRendering();

		sceneGraph->UpdateFrame(0.01f);
		sceneGraph->GetMMDViewPane()->Render(swap->GetBackBuffer());

		TestEnv::EndRendering();
		::Sleep(10);
	}

	sceneGraph->ReleaseMMDSceneGraph();
#endif
	//TestEnv::SaveScreenShot(LOCALFILE("test.png"));
	//ASSERT_TRUE(TestEnv::EqualsScreenShot(LOCALFILE("TestData/Test_Graphics_IndexBuffer.BasicTriangle.png")));
}


//-----------------------------------------------------------------------------
TEST_F(IntegrateTest_Scene_Sprite, BlendMode)
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

	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color::Gray);

	Engine::UpdateFrame();
	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/IntegrateTest_Scene_Sprite.BlendMode.png")));
}
