#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoEngine/Platform/PlatformModule.hpp>
#include <LuminoEngine/Platform/Platform.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Platform/detail/GLFWPlatformWindow.hpp>
#include <LuminoEngine/RHIModule.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderingPipeline/FlatRenderingPipeline.hpp>
#include "../../LuminoEngine/src/Platform/GLFWPlatformWindowManager.hpp"
#include "TestEnv.hpp"

LNHandle TestEnv::graphicsContext = LN_NULL_HANDLE;
LNHandle TestEnv::viewPoint = LN_NULL_HANDLE;

void TestEnv::initialize() {
    ln::Engine::initialize();
    ln::PlatformModule::initialize({ { U"Test", 320, 240 }, ln::WindowSystem::GLFWWithOpenGL });

    LNGLGraphicsContext_CreateFromCurrentGL(320, 240, &graphicsContext);
    LNGraphicsViewPoint_Create(&viewPoint);
    LNGraphicsViewPoint_SetupPerspective2D(viewPoint, 0, 0, 0, 320, 240, -500, 500);

    TestEnv::present();
}

void TestEnv::terminate() {
    LNObject_Release(viewPoint);
    LNObject_Release(graphicsContext);
    ln::PlatformModule::terminate();
    ln::Engine::terminate();
}

void TestEnv::present() {
    ln::PlatformWindow* window1 = ln::detail::PlatformManager::instance()->mainWindow();
    ln::detail::GLFWPlatformWindow* window2 = static_cast<ln::detail::GLFWPlatformWindow*>(window1);

    GLFWwindow* glfwWindow = window2->glfwWindow();
    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

ln::Path TestEnv::getTestDataPath(ln::Path localPath) {
    return ln::Path(TEST_DATA_DIR, localPath);
}

bool TestEnv::checkScreenShot(const ln::Path& filePath, int passRate, bool save) {
    auto context = static_cast<ln::GraphicsContext*>(ln::Runtime::getObject(graphicsContext));
    return ln::GraphicsTestHelper::checkScreenShot(
        TestEnv::getTestDataPath(filePath), context, context->currentBackbuffer(), passRate, save);
}
