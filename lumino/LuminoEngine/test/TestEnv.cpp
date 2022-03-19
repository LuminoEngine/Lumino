#include "Common.hpp"
#include "../src/Engine/EngineManager.hpp"
#include "../src/Scene/SceneManager.hpp"
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include "TestEnv.hpp"

String TestEnv::LuminoCLI;
Ref<DepthBuffer> TestEnv::depthBuffer;
//RenderTargetTexture* TestEnv::lastBackBuffer;

void TestEnv::setup()
{
    EngineFeature feature = EngineFeature::Experimental;//EngineFeature::Public; //

	//Logger::addStdErrAdapter();
	EngineSettings::setMainWindowSize(160, 120);
	//EngineSettings::setMainBackBufferSize(160, 120);
	EngineSettings::setGraphicsAPI(GraphicsAPI::OpenGL);//GraphicsAPI::DirectX12);//GraphicsAPI::Vulkan);//
    EngineSettings::setPriorityGPUName(_TT("Microsoft Basic Render Driver"));
	EngineSettings::setGraphicsDebugEnabled(true);
    EngineSettings::setEngineFeatures(feature);
	EngineSettings::addAssetDirectory(LN_LOCALFILE(_TT("Assets")));
    //EngineSettings::setAssetStorageAccessPriority(AssetStorageAccessPriority::AllowLocalDirectory);
	EngineContext::current()->initializeEngineManager();
	detail::EngineDomain::engineManager()->initializeAllManagers();
    detail::EngineDomain::engineManager()->sceneManager()->autoAddingToActiveWorld = true;
	//detail::RuntimeManager::initialize(detail::RuntimeManager::s_globalSettings);


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

	const auto backbufferSize = Engine::mainWindow()->swapChain()->backbufferSize();
	depthBuffer = DepthBuffer::create(backbufferSize.width, backbufferSize.height);

#ifdef LN_OS_WIN32
	LuminoCLI = Path::combine(Path(ln::Environment::executablePath()).parent().parent().parent().parent(), _TT("tools"), _TT("LuminoCLI"), _TT("Debug"), _TT("lumino-cli.exe"));
#else
#endif
	LN_TEST_CLEAN_SCENE;
}

void TestEnv::teardown()
{
	depthBuffer = nullptr;
	//detail::RuntimeManager::terminate();
    detail::EngineDomain::release();
}

void TestEnv::updateFrame()
{
    detail::EngineDomain::engineManager()->updateFrame();
    detail::EngineDomain::engineManager()->presentFrame();
}

void TestEnv::resetScene()
{
	Engine::world()->removeAllObjects();
	Engine::mainUIView()->removeAllChildren();
#ifdef LN_COORD_RH
	Engine::mainCamera()->setPosition(0, 0, 10);
#else
	Engine::mainCamera()->setPosition(0, 0, -10);
#endif
	Engine::mainCamera()->lookAt(Vector3(0, 0, 0));
	Engine::mainLight()->lookAt(0, -1, 0);
	Engine::mainLight()->setEnabled(true);
	Engine::mainLight()->setSkyColor(Color::Black);
	Engine::mainLight()->setGroundColor(Color::Black);
	Engine::mainLight()->setAmbientColor(Color(0.5, 0.5, 0.5));
	Engine::mainLight()->setColor(Color::White);
	Engine::mainLight()->setIntensity(1.0f);
}

GraphicsContext* TestEnv::graphicsContext()
{
	return Engine::graphicsContext();
}

SwapChain* TestEnv::mainWindowSwapChain()
{
	return Engine::mainWindow()->swapChain();
}

void TestEnv::resetGraphicsContext(GraphicsContext* context)
{
	context->resetState();
	//context->setRenderTarget(0, Engine::mainWindow()->swapChain()->currentBackbuffer());
	//context->setDepthBuffer(depthBuffer);
}

GraphicsContext* TestEnv::beginFrame()
{
	auto ctx = TestEnv::mainWindowSwapChain()->beginFrame2();
	return ctx;
}

void TestEnv::endFrame()
{
	TestEnv::mainWindowSwapChain()->endFrame();
}

RenderPass* TestEnv::renderPass()
{
	return TestEnv::mainWindowSwapChain()->currentRenderPass();
}

Ref<Bitmap2D> TestEnv::capture(RenderTargetTexture* renderTarget)
{
	if (renderTarget)
		return detail::TextureInternal::readData(renderTarget, nullptr);
	else
		return detail::TextureInternal::readData(TestEnv::mainWindowSwapChain()->currentBackbuffer(), TestEnv::graphicsContext());
}

void TestEnv::saveScreenShot(const Path& filePath, RenderTargetTexture* renderTarget)
{
    capture(renderTarget)->save(filePath);
}

bool TestEnv::equalsScreenShot(const Path& filePath, RenderTargetTexture* renderTarget, int passRate)
{
	bool r = TestEnv::equalsBitmapFile(capture(renderTarget), filePath, passRate);
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

bool TestEnv::equalsBitmapFile(Bitmap2D* bmp1, const Path& filePath, int passRate)
{
	auto bmp2 = makeObject<Bitmap2D>();
	bmp2->load(filePath);

	bool ignoreAlpha = true;

	int colorRange = 255 - (255 * passRate / 100);
	int pass = 0;
	int fail = 0;

	for (int y = 0; y < bmp1->height(); ++y)
	{
		for (int x = 0; x < bmp1->width(); ++x)
		{
            auto c1 = mixPixels(bmp1, x, y);
            auto c2 = mixPixels(bmp2, x, y);
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

bool TestEnv::checkScreenShot(const Path& filePath, RenderTargetTexture* renderTarget, int passRate, bool save)
{
	if (save)
	{
		saveScreenShot(filePath, renderTarget);
		return true;
	}
	else
	{
        bool result = equalsScreenShot(filePath, renderTarget, passRate);
        if (!result) {
            saveScreenShot(LN_ASSETFILE("Result/0-latest-failer.png"), renderTarget);
        }
        return result;
	}
}

void TestEnv::waitRendering()
{
}

