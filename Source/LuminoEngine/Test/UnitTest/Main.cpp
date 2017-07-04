#include "TestConfig.h"
#include <Lumino/UI/UIFrameWindow.h>
#include "../../../Source/EngineManager.h"
#include "../../../Source/Graphics/GraphicsManager.h"
#include "../../../Source/UI/UIManager.h"

//------------------------------------------------------------------------------
void TestEnv::SetUp()
{
}

//------------------------------------------------------------------------------
void TestEnv::TearDown()
{
}

//------------------------------------------------------------------------------
void TestEnv::saveScreenShot(const TCHAR* filePath)
{
	EngineManager::Instance->getGraphicsManager()->getMainSwapChain()->getBackBuffer()->lock()->save(filePath);
	EngineManager::Instance->getGraphicsManager()->getMainSwapChain()->getBackBuffer()->unlock();
}

//------------------------------------------------------------------------------
bool TestEnv::EqualsScreenShot(const TCHAR* filePath, int passRate)
{
	bool r = TestEnv::EqualsBitmapFile(EngineManager::Instance->getGraphicsManager()->getMainSwapChain()->getBackBuffer()->lock(), filePath, passRate);
	EngineManager::Instance->getGraphicsManager()->getMainSwapChain()->getBackBuffer()->unlock();
	return r;
}

//------------------------------------------------------------------------------
//bool TestEnv::equalsTexture(Texture* texture, const TCHAR* filePath)
//{
//	bool r = TestEnv::EqualsBitmapFile(texture->lock(), filePath, 90);
//	texture->unlock();
//	return r;
//}

//------------------------------------------------------------------------------
Color32 MixPixels(Bitmap* bmp, int x, int y)
{
	const Color32& c = bmp->getPixel(x, y);
	int r = c.r; int g = c.g; int b = c.b; int a = c.a;
	int count = 1;

	if (y > 0) {
		if (x > 0) {
			const Color32& c = bmp->getPixel(x - 1, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		{
			const Color32& c = bmp->getPixel(x, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->getSize().width - 1) {
			const Color32& c = bmp->getPixel(x + 1, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}
	{
		if (x > 0) {
			const Color32& c = bmp->getPixel(x - 1, y);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->getSize().width - 1) {
			const Color32& c = bmp->getPixel(x + 1, y);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}
	if (y < bmp->getSize().height - 1) {
		if (x > 0) {
			const Color32& c = bmp->getPixel(x - 1, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		{
			const Color32& c = bmp->getPixel(x, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->getSize().width - 1) {
			const Color32& c = bmp->getPixel(x + 1, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}

	return Color32(r / count, g / count, b / count, a / count);
}

bool TestEnv::EqualsBitmapFile(Bitmap* bmp1, const TCHAR* filePath, int passRate)
{
	Bitmap bmp2(filePath);

	bool ignoreAlpha = true;

	int colorRange = 255 - (255 * passRate / 100);
	int pass = 0;
	int fail = 0;

	for (int y = 0; y < bmp1->getSize().height; ++y)
	{
		for (int x = 0; x < bmp1->getSize().width; ++x)
		{
			Color32 c1 = MixPixels(bmp1, x, y);
			Color32 c2 = MixPixels(&bmp2, x, y);
			if (abs(c1.r - c2.r) <= colorRange &&
				abs(c1.g - c2.g) <= colorRange &&
				abs(c1.b - c2.b) <= colorRange &&
				(ignoreAlpha || abs(c1.a - c2.a) <= colorRange))
			{
				++pass;
			}
			else
			{
				++fail;
			}
		}
	}

	int thr = ((bmp1->getSize().height * bmp1->getSize().width) * passRate / 100);
	return pass >= thr;
}

bool TestEnv::CheckScreenShot(const TCHAR* filePath, int passRate, bool save)
{
	if (save)
	{
		saveScreenShot(filePath);
		return true;
	}
	else
	{
		return EqualsScreenShot(filePath, passRate);
	}
}
//------------------------------------------------------------------------------
void TestEnv::WaitRendering()
{
	Application::getMainWindow()->getSwapChain()->WaitForPresent();
}










//------------------------------------------------------------------------------
void EngineInitalize()
{
	Engine::initialize();

	// テストしやすいように固定フレームレートにする
	Engine::setFrameUpdateMode(FrameUpdateMode::Fixed);

	RawFont::registerFontFile(LN_LOCALFILE("../../../../Tools/VLGothic/VL-Gothic-Regular.ttf"));
	RawFont::getDefaultFont()->setName(_T("VL Gothic"));

	// 背景はグレーにしておくと加算合成のテストとか、いろいろ都合がよい
	UIViewport::getMain()->setViewBackgroundColor(Color32::Gray);



	{
		auto buttonNormalBrush = TextureBrush::create(detail::UIManager::getInstance()->getDefaultSkinTexture());
		buttonNormalBrush->setSourceRect(0, 0, 32, 32);
		buttonNormalBrush->getBorderThickness(8, 8, 8, 8);
		buttonNormalBrush->getImageDrawMode(BrushImageDrawMode::BoxFrame);
		buttonNormalBrush->setWrapMode(BrushWrapMode::Stretch);

		auto* res = detail::UIManager::getInstance()->getDefaultStyleTable();
		auto* style = res->getStyle(_T("UIButton"));
		// base
		{
			auto* props = style->getPropertyTable();
			props->background = RefPtr<Brush>::staticCast(buttonNormalBrush);
			props->borderThickness = ThicknessF(0);
		}
	}
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
		"--gtest_filter=Test_Scene_Sprite.DrawCallCount"
	};
	argc = sizeof(testArgs) / sizeof(char*);
	testing::InitGoogleTest(&argc, (char**)testArgs);
#else
	testing::InitGoogleTest(&argc, argv);
#endif
	::testing::AddGlobalTestEnvironment(new TestEnv());

	{
		Logger::initialize(_T("test_log.txt"));

		int scale = 1;
		EngineSettings::setMainWindowSize(SizeI(160 * scale, 120 * scale));
		EngineSettings::setMainBackBufferSize(SizeI(160 * scale, 120 * scale));
		EngineSettings::SetGraphicsRenderingType(GraphicsRenderingType::Threaded);//GraphicsRenderingType::Immediate);//
		detail::EngineSettings::instance.defaultSkinFilePath = LN_LOCALFILE("UI/Data/Skin.png");
	}

	{
		EngineSettings::SetGraphicsAPI(GraphicsAPI::DirectX9);

		EngineInitalize();
		int r = RUN_ALL_TESTS();
		Engine::terminate();
		if (r != 0) return r;
	}
	//{
	//	EngineSettings::SetGraphicsAPI(GraphicsAPI::OpenGL);

	//	EngineInitalize();
	//	int r = RUN_ALL_TESTS();
	//	Engine::terminate();
	//	if (r != 0) return r;
	//}

	//RUN_ALL_TESTS();
	//return RUN_ALL_TESTS();
	return 0;
}
