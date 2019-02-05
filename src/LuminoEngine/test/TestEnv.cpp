#include "Common.hpp"
#include "../src/Engine/EngineManager.hpp"
#include "TestEnv.hpp"

String TestEnv::LuminoCLI;

void TestEnv::setup()
{
	GlobalLogger::addStdErrAdapter();
	EngineSettings::setMainWindowSize(160, 120);
	EngineSettings::setMainBackBufferSize(160, 120);
	EngineSettings::setGraphicsAPI(GraphicsAPI::Vulkan);
    //EngineSettings::setEngineFeatures(EngineFeature::Experimental);
    detail::EngineDomain::engineManager()->init();

    if (0)  // Experimental
    {
        Font::registerFontFile(LN_LOCALFILE("../../../tools/VLGothic/VL-Gothic-Regular.ttf"));
        Engine::mainCamera()->setBackgroundColor(Color(0.5, 0.5, 0.5, 1.0));

        Engine::mainAmbientLight()->setColor(Color::White);
        Engine::mainAmbientLight()->setIntensity(0.5);
        Engine::mainDirectionalLight()->setColor(Color::White);
        Engine::mainDirectionalLight()->setIntensity(0.5);
        Engine::mainDirectionalLight()->setPosition(10, 10, -10);
        Engine::mainDirectionalLight()->lookAt(Vector3(0, 0, 0));
    }

#ifdef LN_OS_WIN32
	LuminoCLI = Path::combine(Path(ln::Environment::executablePath()).parent().parent().parent().parent(), u"tools", u"LuminoCLI", u"Debug", u"lumino-cli.exe");
#else
#endif
}

void TestEnv::teardown()
{
    detail::EngineDomain::release();
}

void TestEnv::updateFrame()
{
    detail::EngineDomain::engineManager()->updateFrame();
    detail::EngineDomain::engineManager()->renderFrame();
    detail::EngineDomain::engineManager()->presentFrame();
}

void TestEnv::resetGraphicsContext(GraphicsContext* context)
{
	context->setColorBuffer(0, Engine::mainWindow()->swapChain()->colorBuffer());
	context->setDepthBuffer(Engine::mainWindow()->swapChain()->depthBuffer());
}

Ref<Bitmap2D> TestEnv::capture()
{
	return Engine::mainWindow()->swapChain()->colorBuffer()->readData();
}

void TestEnv::saveScreenShot(const Char* filePath)
{
	Engine::mainWindow()->swapChain()->colorBuffer()->readData()->save(filePath);
}

bool TestEnv::equalsScreenShot(const Char* filePath, int passRate)
{
	bool r = TestEnv::equalsBitmapFile(Engine::mainWindow()->swapChain()->colorBuffer()->readData(), filePath, passRate);
	return r;
}

static Color32 mixPixels(Bitmap2D* bmp, int x, int y)
{
	const Color32& c = bmp->getPixel32(x, y);
	int r = c.r; int g = c.g; int b = c.b; int a = c.a;
	int count = 1;

	if (y > 0) {
		if (x > 0) {
			const Color32& c = bmp->getPixel32(x - 1, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		{
			const Color32& c = bmp->getPixel32(x, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->width() - 1) {
			const Color32& c = bmp->getPixel32(x + 1, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}
	{
		if (x > 0) {
			const Color32& c = bmp->getPixel32(x - 1, y);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->width() - 1) {
			const Color32& c = bmp->getPixel32(x + 1, y);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}
	if (y < bmp->height() - 1) {
		if (x > 0) {
			const Color32& c = bmp->getPixel32(x - 1, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		{
			const Color32& c = bmp->getPixel32(x, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->width() - 1) {
			const Color32& c = bmp->getPixel32(x + 1, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}

	return Color32(r / count, g / count, b / count, a / count);
}

bool TestEnv::equalsBitmapFile(Bitmap2D* bmp1, const Char* filePath, int passRate)
{
	auto bmp2 = newObject<Bitmap2D>();
	bmp2->load(filePath);

	bool ignoreAlpha = true;

	int colorRange = 255 - (255 * passRate / 100);
	int pass = 0;
	int fail = 0;

	for (int y = 0; y < bmp1->height(); ++y)
	{
		for (int x = 0; x < bmp1->width(); ++x)
		{
			Color32 c1 = mixPixels(bmp1, x, y);
			Color32 c2 = mixPixels(bmp2, x, y);
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

	int thr = ((bmp1->height() * bmp1->width()) * passRate / 100);
	return pass >= thr;
}

bool TestEnv::checkScreenShot(const Char* filePath, int passRate, bool save)
{
	if (save)
	{
		saveScreenShot(filePath);
		return true;
	}
	else
	{
		saveScreenShot(LN_ASSETFILE("Result/0.png"));
		return equalsScreenShot(filePath, passRate);
	}
}

void TestEnv::waitRendering()
{
	Engine::mainWindow()->swapChain()->wait();
}

