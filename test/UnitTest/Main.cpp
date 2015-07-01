#include "TestConfig.h"

Platform::PlatformManager*	TestEnv::Application = NULL;
Physics::PhysicsManager*	TestEnv::PhysicsManager = NULL;
GraphicsManager*			TestEnv::Manager = NULL;
Graphics::Renderer*			TestEnv::Renderer = NULL;
Graphics::SwapChain*		TestEnv::MainSwapChain = NULL;
SceneGraphManager*			TestEnv::MMDSceneGraph = NULL;


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TestEnv::SaveScreenShot(const TCHAR* filePath)
{
	MainSwapChain->GetBackBuffer()->Lock()->Save(filePath);
	MainSwapChain->GetBackBuffer()->Unlock();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool TestEnv::EqualsScreenShot(const TCHAR* filePath)
{
	bool r = TestEnv::EqualsBitmapFile(MainSwapChain->GetBackBuffer()->Lock(), filePath);
	MainSwapChain->GetBackBuffer()->Unlock();
	return r;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool TestEnv::EqualsBitmapFile(Imaging::Bitmap* bmp1, const TCHAR* filePath)
{
	Imaging::Bitmap bmp2(filePath);
	bmp1->ConvertToDownFlow();
	bmp2.ConvertToDownFlow();
	return bmp1->Equals(&bmp2);
}

//-----------------------------------------------------------------------------
// プログラム開始時の初期化処理
//-----------------------------------------------------------------------------
void TestEnv::SetUp()
{
	Logger::Initialize(_T("test_log.txt"));

	Platform::ApplicationSettings s;
	s.MainWindowSettings.ClientSize.Set(160, 120);
	Application = LN_NEW Platform::PlatformManager(s);

	PhysicsManager = LN_NEW Physics::PhysicsManager(Physics::SimulationType_Sync);


	Graphics::GraphicsManagerConfigData gmcd;
	gmcd.MainWindow = Application->GetMainWindow();
	gmcd.FileManager = &FileManager::GetInstance();
	Manager = LN_NEW Graphics::GraphicsManager(gmcd);

	Renderer = Manager->GetRenderer();
	MainSwapChain = Manager->GetMainSwapChain();
	//Device = Manager->GetGraphicsDevice()->GetDeviceObject();


	// SceneGraph
	SceneGraphManager::ConfigData c;
	c.FileManager = &FileManager::GetInstance();
	c.PhysicsManager = PhysicsManager;
	c.GraphicsManager = TestEnv::Manager;
	MMDSceneGraph = LN_NEW SceneGraphManager(c);
	MMDSceneGraph->CreateMMDSceneGraph();

}

//-----------------------------------------------------------------------------
// プログラム終了時の終了処理
//-----------------------------------------------------------------------------
void TestEnv::TearDown()
{
	if (MMDSceneGraph) {
		MMDSceneGraph->ReleaseMMDSceneGraph();
		LN_SAFE_RELEASE(MMDSceneGraph);
	}
	
	LN_SAFE_RELEASE(Manager);
	LN_SAFE_RELEASE(PhysicsManager);
	LN_SAFE_RELEASE(Application);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GTEST_API_ int main(int argc, char **argv)
{

#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");


#if 1	// 部分的にテストを実行したりする
	char* testArgs[] = {
		argv[0],
		"--gtest_filter=Test_Graphics_TextRenderer.*"
		//"--gtest_filter=Test_Imaging_Bitmap.BitBlt"
		//"--gtest_filter=Test_Graphics_Texture.Lock"
	};
	argc = sizeof(testArgs) / sizeof(char*);
	testing::InitGoogleTest(&argc, (char**)testArgs);
#else
	testing::InitGoogleTest(&argc, argv);
#endif
	::testing::AddGlobalTestEnvironment(new TestEnv());
	return RUN_ALL_TESTS();
}
