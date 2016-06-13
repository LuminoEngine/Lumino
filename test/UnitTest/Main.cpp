#include "TestConfig.h"
#include "../../../src/EngineManager.h"

//------------------------------------------------------------------------------
//GTEST_API_ int main(int argc, char **argv)
//{
//
//#ifdef _WIN32
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif
//	setlocale(LC_ALL, "");
//
//
//#if 0	// 部分的にテストを実行したりする
//	char* testArgs[] = {
//		argv[0],
//		"--gtest_filter=Test_Scene_Sprite.*"
//		//"--gtest_filter=Test_Imaging_Bitmap.BitBlt"
//		//"--gtest_filter=Test_Graphics_Texture.Lock"
//	};
//	argc = sizeof(testArgs) / sizeof(char*);
//	testing::InitGoogleTest(&argc, (char**)testArgs);
//#else
//	testing::InitGoogleTest(&argc, argv);
//#endif
//	::testing::AddGlobalTestEnvironment(new TestEnv());
//	return RUN_ALL_TESTS();
//}

#if 0
Platform::PlatformManager*	TestEnv::Platform = NULL;
Physics::PhysicsManager*	TestEnv::PhysicsManager = NULL;
GraphicsManagerPtr			TestEnv::Manager;
Lumino::Renderer*			TestEnv::Renderer = NULL;
SwapChain*		TestEnv::MainSwapChain = NULL;
SceneGraphManager*			TestEnv::MMDSceneGraph = NULL;

//------------------------------------------------------------------------------
Lumino::Renderer* TestEnv::BeginRendering()
{
	Renderer* r = TestEnv::Renderer;
	SwapChain* swap = TestEnv::MainSwapChain;
	r->SetRenderTarget(0, swap->GetBackBuffer());
	r->SetDepthBuffer(swap->GetBackBufferDepth());
	r->Clear(ClearFlags::All, ColorF::White, 1.0f);
	return r;
}

//------------------------------------------------------------------------------
void TestEnv::EndRendering()
{
	TestEnv::MainSwapChain->Present();
}

//------------------------------------------------------------------------------
PathName TestEnv::MakeScreenShotPath(const char* fileName)
{
	PathName path(_T("TestData"));
	path.Append(Manager->GetGraphicsAPI().ToString());
	path.Append(fileName);
	return path;
}




//------------------------------------------------------------------------------
RefPtr<Shader> TestEnv::CreateShader(const TCHAR* filePath)
{
	ByteBuffer code = FileSystem::ReadAllBytes(filePath);
	RefPtr<Shader> shader(Shader::Create((char*)code.GetData(), code.GetSize()));
	return shader;
}
#endif

//------------------------------------------------------------------------------
void TestEnv::SetUp()
{
	Logger::Initialize(_T("test_log.txt"));

	EngineSettings settings;
	EngineSettings::SetMainWindowSize(Size(160, 120));
	EngineSettings::SetMainBackBufferSize(Size(160, 120));
	EngineSettings::SetGraphicsAPI(GraphicsAPI::DirectX9);
	EngineSettings::SetGraphicsRenderingType(GraphicsRenderingType::Immediate);
	//settings.graphicsAPI = GraphicsAPI::DirectX9; //GraphicsAPI::OpenGL;//
	//settings.renderingType = GraphicsRenderingType::Immediate; //RenderingType::Deferred;//
	Engine::Initialize();

	Font::RegisterFontFile(LN_LOCALFILE("../../tools/VLGothic/VL-Gothic-Regular.ttf"));
	Font::GetDefaultFont()->SetName(_T("VL Gothic"));

	// 背景はグレーにしておくと加算合成のテストとか、いろいろ都合がよい
	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color::Gray);
}

//------------------------------------------------------------------------------
void TestEnv::TearDown()
{
	Engine::Terminate();
}

//------------------------------------------------------------------------------
void TestEnv::SaveScreenShot(const TCHAR* filePath)
{
	EngineManager::Instance->GetGraphicsManager()->GetMainSwapChain()->GetBackBuffer()->Lock()->Save(filePath);
	EngineManager::Instance->GetGraphicsManager()->GetMainSwapChain()->GetBackBuffer()->Unlock();
}

//------------------------------------------------------------------------------
bool TestEnv::EqualsScreenShot(const TCHAR* filePath)
{
	bool r = TestEnv::EqualsBitmapFile(EngineManager::Instance->GetGraphicsManager()->GetMainSwapChain()->GetBackBuffer()->Lock(), filePath);
	EngineManager::Instance->GetGraphicsManager()->GetMainSwapChain()->GetBackBuffer()->Unlock();
	return r;
}

//------------------------------------------------------------------------------
bool TestEnv::EqualsTexture(Texture* texture, const TCHAR* filePath)
{
	bool r = TestEnv::EqualsBitmapFile(texture->Lock(), filePath);
	texture->Unlock();
	return r;
}

//------------------------------------------------------------------------------
Color MixPixels(Bitmap* bmp, int x, int y)
{
	const Color& c = bmp->GetPixel(x, y);
	int r = c.r; int g = c.g; int b = c.b; int a = c.a;
	int count = 1;

	if (y > 0) {
		if (x > 0) {
			const Color& c = bmp->GetPixel(x - 1, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		{
			const Color& c = bmp->GetPixel(x, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->GetSize().width - 1) {
			const Color& c = bmp->GetPixel(x + 1, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}
	{
		if (x > 0) {
			const Color& c = bmp->GetPixel(x - 1, y);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->GetSize().width - 1) {
			const Color& c = bmp->GetPixel(x + 1, y);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}
	if (y < bmp->GetSize().height - 1) {
		if (x > 0) {
			const Color& c = bmp->GetPixel(x - 1, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		{
			const Color& c = bmp->GetPixel(x, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->GetSize().width - 1) {
			const Color& c = bmp->GetPixel(x + 1, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}

	return Color(r / count, g / count, b / count, a / count);
}

bool TestEnv::EqualsBitmapFile(Bitmap* bmp1, const TCHAR* filePath)
{
	Bitmap bmp2(filePath);

	int passRate = 90;
	bool ignoreAlpha = true;

	int colorRange = 255 - (255 * passRate / 100);
	int pass = 0;

	for (int y = 0; y < bmp1->GetSize().height; ++y)
	{
		for (int x = 0; x < bmp1->GetSize().width; ++x)
		{
			Color c1 = MixPixels(bmp1, x, y);
			Color c2 = MixPixels(&bmp2, x, y);
			if (abs(c1.r - c2.r) <= colorRange &&
				abs(c1.g - c2.g) <= colorRange &&
				abs(c1.b - c2.b) <= colorRange &&
				(ignoreAlpha || abs(c1.a - c2.a) <= colorRange))
			{
				++pass;
			}
		}
	}

	return pass >= ((bmp1->GetSize().height * bmp1->GetSize().width) * passRate / 100);
}

//------------------------------------------------------------------------------
GTEST_API_ int main(int argc, char **argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");

#if 1	// 部分的にテストを実行したりする
	char* testArgs[] = {
		argv[0],
		"--gtest_filter=Test_UI_TextBlock.*"
	};
	argc = sizeof(testArgs) / sizeof(char*);
	testing::InitGoogleTest(&argc, (char**)testArgs);
#else
	testing::InitGoogleTest(&argc, argv);
#endif
	::testing::AddGlobalTestEnvironment(new TestEnv());
	return RUN_ALL_TESTS();
}
