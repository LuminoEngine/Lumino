#include "Common.hpp"
#include "../src/Engine/EngineManager.hpp"
#include "TestEnv.hpp"

String TestEnv::LuminoCLI;
Ref<DepthBuffer> TestEnv::depthBuffer;

void TestEnv::setup()
{
    EngineFeature feature = EngineFeature::Experimental;//EngineFeature::Public; //

	GlobalLogger::addStdErrAdapter();
	EngineSettings::setMainWindowSize(160, 120);
	EngineSettings::setMainBackBufferSize(160, 120);
	EngineSettings::setGraphicsAPI(GraphicsAPI::OpenGL);//GraphicsAPI::Vulkan);//
    EngineSettings::setEngineFeatures(feature);
	EngineSettings::addAssetDirectory(LN_LOCALFILE(u"Assets"));
    detail::EngineDomain::engineManager()->init();

    if (feature == EngineFeature::Experimental)  // Experimental
    {
        Font::registerFontFromFile(LN_LOCALFILE("../../../tools/VLGothic/VL-Gothic-Regular.ttf"));
        Engine::mainCamera()->setBackgroundColor(Color(0.5, 0.5, 0.5, 1.0));

        //Engine::mainAmbientLight()->setColor(Color::White);
        //Engine::mainAmbientLight()->setIntensity(0.5);
        //Engine::mainDirectionalLight()->setColor(Color::White);
        //Engine::mainDirectionalLight()->setIntensity(0.5);
        //Engine::mainDirectionalLight()->setPosition(10, 10, -10);
        //Engine::mainDirectionalLight()->lookAt(Vector3(0, 0, 0));
    }

	RenderTargetTexture* backbuffer = Engine::mainWindow()->swapChain()->backbuffer();
	depthBuffer = DepthBuffer::create(backbuffer->width(), backbuffer->height());

#ifdef LN_OS_WIN32
	LuminoCLI = Path::combine(Path(ln::Environment::executablePath()).parent().parent().parent().parent(), u"tools", u"LuminoCLI", u"Debug", u"lumino-cli.exe");
#else
#endif
}

void TestEnv::teardown()
{
	depthBuffer = nullptr;
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
	context->resetState();
	context->setRenderTarget(0, Engine::mainWindow()->swapChain()->backbuffer());
	context->setDepthBuffer(depthBuffer);
}

Ref<Bitmap2D> TestEnv::capture()
{
	return detail::TextureHelper::readData(Engine::mainWindow()->swapChain()->backbuffer());
}

void TestEnv::saveScreenShot(const Char* filePath)
{
    capture()->save(filePath);
}

bool TestEnv::equalsScreenShot(const Char* filePath, int passRate)
{
	bool r = TestEnv::equalsBitmapFile(capture(), filePath, passRate);
	return r;
}

static ColorI mixPixels(Bitmap2D* bmp, int x, int y)
{
	const ColorI& c = bmp->getPixel32(x, y);
	int r = c.r; int g = c.g; int b = c.b; int a = c.a;
	int count = 1;

	if (y > 0) {
		if (x > 0) {
			const ColorI& c = bmp->getPixel32(x - 1, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		{
			const ColorI& c = bmp->getPixel32(x, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->width() - 1) {
			const ColorI& c = bmp->getPixel32(x + 1, y - 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}
	{
		if (x > 0) {
			const ColorI& c = bmp->getPixel32(x - 1, y);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->width() - 1) {
			const ColorI& c = bmp->getPixel32(x + 1, y);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}
	if (y < bmp->height() - 1) {
		if (x > 0) {
			const ColorI& c = bmp->getPixel32(x - 1, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		{
			const ColorI& c = bmp->getPixel32(x, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
		if (x < bmp->width() - 1) {
			const ColorI& c = bmp->getPixel32(x + 1, y + 1);
			r += c.r; g += c.g; b += c.b; a += c.a; ++count;
		}
	}

	return ColorI(r / count, g / count, b / count, a / count);
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
			ColorI c1 = mixPixels(bmp1, x, y);
			ColorI c2 = mixPixels(bmp2, x, y);
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
}

