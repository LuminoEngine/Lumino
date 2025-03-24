#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Platform/detail/GLFWPlatformWindow.hpp>
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/SurfaceContext.hpp>
#include <LuminoEngine/Rendering/RenderingPipeline/FlatRenderingPipeline.hpp>
#include "../../LuminoEngine/src/Platform/GLFWPlatformWindowManager.hpp"
#include "TestEnv.hpp"

ln::Ref<ln::PlatformWindow> TestEnv::mainWindow;
LNHandle TestEnv::surfaceContext = LN_NULL_HANDLE;
LNHandle TestEnv::viewPoint = LN_NULL_HANDLE;

void TestEnv::initialize() {
    ln::FileSystem::createDirectory(TestEnv::getTempPath(U""));

    ln::EngineOptions options;
    options.platform.title = U"Test";
    options.platform.width = 320;
    options.platform.height = 240;
    options.platform.windowSystem = ln::WindowSystem::GLFWWithOpenGL;
    ln::Engine::initialize(options);

    ln::WindowCreationSettings windowOptions;
    windowOptions.title = U"Test";
    windowOptions.clientWidth = 320;
    windowOptions.clientHeight = 240;
    mainWindow = ln::EngineInstance::instance()->platformManager()->createWindow(windowOptions);

    LNGLGraphicsContext_CreateFromCurrentGL(320, 240, &surfaceContext);
    LNGraphicsViewPoint_Create(&viewPoint);
    LNGraphicsViewPoint_SetupPerspective2D(viewPoint, 0, 0, 0, 320, 240, -500, 500);

    TestEnv::present();
}

void TestEnv::terminate() {
    LNObject_Release(viewPoint);
    LNObject_Release(surfaceContext);
    ln::Engine::terminate();
}

void TestEnv::present() {
    ln::PlatformWindow* window1 = mainWindow;
    ln::detail::GLFWPlatformWindow* window2 = static_cast<ln::detail::GLFWPlatformWindow*>(window1);

    GLFWwindow* glfwWindow = window2->glfwWindow();
    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

ln::Path TestEnv::getTestDataPath(const ln::Path& localPath) {
    return ln::Path(ASSETS_DIR, localPath);
}

ln::Path TestEnv::getTempPath(const ln::Path& localPath) {
    return ln::Path(LN_LOCALFILE("tmp"), localPath);
}

bool TestEnv::checkScreenShot(const ln::Path& filePath, int passRate, bool save) {
    auto* m = ln::detail::RuntimeManager::instance();
    ln::SurfaceContext* sc = static_cast<ln::SurfaceContext*>(m->getObjectEntry(surfaceContext)->object);
    ln::GraphicsContext* context = sc->context;
    return ln::GraphicsTestHelper::checkScreenShot(
        TestEnv::getTestDataPath(filePath), context, context->currentBackbuffer(), passRate, save);
}

ln::ByteBuffer TestEnv::compileShader(const ln::Path& filePath) {
#ifdef _WIN32
#ifdef _DEBUG
    ln::Path tool = ln::Path(CMAKE_BINARY_DIR, U"packages/lumino-cli/Debug/lumino.exe");
#else
    ln::Path tool = ln::Path(CMAKE_BINARY_DIR, U"packages/lumino-cli/Release/lumino.exe");
#endif
#else
#error "Not implemented"
#endif
    auto outputFilePath = TestEnv::getTempPath(filePath.fileName().replaceExtension(U".lcfx"));
    ln::Process2::exec(ln::format(U"{} fxc \"{}\" \"{}\"", tool, filePath, outputFilePath));
    return ln::FileSystem::readAllBytes(outputFilePath).unwrap();
}
