#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <LuminoCore.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#ifdef LUMINO_USE_WEBGPU
#include <LuminoGraphicsRHI/WebGPU/WebGPUDevice.hpp>
#endif
#include <LuminoGraphicsRHI/ShaderCompiler/detail/ShaderManager.hpp>
#include <LuminoGraphicsRHI/Vulkan/VulkanDeviceContext.hpp>
#include "../../src/ShaderCompiler/UnifiedShaderCompiler.hpp"
using namespace ln;

SizeI g_viewSize;
Ref<PlatformWindow> g_window;
Ref<detail::IGraphicsDevice> g_device;
Ref<detail::ISwapChain> g_swapChain;
std::vector<Ref<detail::ICommandList>> g_commandLists;
std::vector<Ref<detail::IRenderPass>> g_renderPasses;

void init() {
    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Example", 640, 480 }, WindowSystem::Native });

    auto window = Platform::mainWindow();

#ifdef LUMINO_USE_WEBGPU
    {
        detail::WebGPUDevice::Settings settings;
        settings.debugMode = true;
        auto device = makeRef<detail::WebGPUDevice>();
        device->init(settings);
        g_device = device;
    }
#endif
    if (!g_device) {
        detail::VulkanDevice::Settings settings;
        settings.mainWindow = window;
        settings.debugMode = true;
        bool dummy = false;
        auto device = *detail::VulkanDevice::create(settings, &dummy);
        device->refreshCaps();
        g_device = device;
    }

    // Create SwapChain.
    window->getSize(&g_viewSize);
    g_swapChain = g_device->createSwapChain(window, g_viewSize);

    // Create frame resources.
    for (int i = 0; i < g_swapChain->getBackbufferCount(); i++) {
        detail::DeviceFramebufferState info;
        info.renderTargets[0] = g_swapChain->getRenderTarget(i);
        Ref<detail::IRenderPass> renderPass = g_device->createRenderPass(info, ClearFlags::All, Color::parse(U"#98d98e"), 1.0, 0);
        g_renderPasses.push_back(renderPass);

        Ref<detail::ICommandList> commandList = g_device->createCommandList();
        g_commandLists.push_back(commandList);
    }
}

void cleanup() {
    for (const Ref<detail::ICommandList>& i : g_commandLists) {
        i->destroy();
    }
    g_commandLists.clear();

    for (const Ref<detail::IRenderPass>& i : g_renderPasses) {
        i->destroy();
    }
    g_renderPasses.clear();

    g_swapChain->destroy();
    g_swapChain = nullptr;

    g_device->dispose();
    g_device = nullptr;

    PlatformModule::terminate();
    RuntimeModule::terminate();
}

void mainLoop() {
    Platform::processEvents();

    int imageIndex = 0;
    g_swapChain->acquireNextImage(&imageIndex);

    detail::ICommandList* commandList = g_commandLists[imageIndex];
    commandList->wait();

    commandList->begin();
    commandList->beginRenderPass(g_renderPasses[imageIndex]);
    commandList->endRenderPass(g_renderPasses[imageIndex]);
    commandList->end();

    g_device->queueSubmit(commandList, g_swapChain->getRenderTarget(imageIndex));
    g_device->queuePresent(g_swapChain);
}

int main() {
    init();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 60, true);
#else
    while (!Platform::shouldQuit()) {
        mainLoop();
        Thread::sleep(16);
    }
#endif
    cleanup();
    return 0;
}
