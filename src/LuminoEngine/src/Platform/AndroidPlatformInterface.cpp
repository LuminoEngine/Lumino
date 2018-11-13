
#if defined(__ANDROID__)
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Platform/AndroidPlatformInterface.hpp>

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

namespace ln {

static ln::Application* g_app = nullptr;

void AndroidPlatformInterface::initialize(int viewWidth, int viewHeight)
{
    ln::GlobalLogger::addLogcatAdapter();
    g_app = ::LuminoCreateApplicationInstance();
    ln::detail::ApplicationHelper::initialize(g_app);
    ln::detail::SwapChainHelper::setBackendBufferSize(ln::Engine::mainWindow()->swapChain(), width, height);
}

void AndroidPlatformInterface::finalize()
{
    ln::detail::ApplicationHelper::finalize(g_app);
    ln::RefObjectHelper::release(g_app);
    g_app = nullptr;
}

void AndroidPlatformInterface::updateFrame()
{
    ln::detail::ApplicationHelper::processTick(g_app);
}

void AndroidPlatformInterface::addAssetArchive(const ln::String& fileFullPath)
{
}

} // namespace ln
#endif
