
#if defined(__ANDROID__)
#include "Internal.hpp"
#include <LuminoEngine/Engine/Engine.hpp>
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Platform/AndroidPlatformInterface.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include "../Engine/EngineManager.hpp"

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

namespace ln {

static ln::Application* g_app = nullptr;

void AndroidPlatformInterface::initialize(int viewWidth, int viewHeight)
{
    ln::GlobalLogger::addLogcatAdapter();
    g_app = ::LuminoCreateApplicationInstance();
    ln::detail::ApplicationHelper::initialize(g_app);
    ln::detail::SwapChainHelper::setBackendBufferSize(ln::Engine::mainWindow()->swapChain(), viewWidth, viewHeight);
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

} // namespace ln
#endif
