#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <LuminoCore.hpp>
#include <LuminoEngine/Engine/Engine.hpp>
#include <LuminoEngine/Platform/PlatformModule.hpp>
#include <LuminoEngine/Platform/Platform.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#ifdef LUMINO_USE_WEBGPU
#include <LuminoGraphicsRHI/WebGPU/WebGPUDevice.hpp>
#endif
#include <LuminoEngine/Graphics/GraphicsRHI/ShaderCompiler/detail/ShaderManager.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDeviceContext.hpp>
#include "../../LuminoEngine/src/Graphics/GraphicsRHI/ShaderCompiler/UnifiedShaderCompiler.hpp"
using namespace ln;

SizeI g_viewSize;
Ref<PlatformWindow> g_window;
Ref<detail::IGraphicsDevice> g_device;
Ref<detail::ISwapChain> g_swapChain;
std::vector<Ref<detail::ICommandList>> g_commandLists;
std::vector<Ref<detail::IRenderPass>> g_renderPasses;

void init() {
    EngineOptions options;
    options.graphics.enabled = false;
    Engine::initialize(options);

    ln::WindowCreationSettings windowOptions;
    windowOptions.title = U"Test";
    windowOptions.clientHeight = 160;
    windowOptions.clientWidth = 120;
    auto mainWindow = ln::EngineManager::instance()->platformManager()->createWindow(windowOptions);

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
        settings.mainWindow = mainWindow;
        settings.debugMode = true;
        bool dummy = false;
        g_device = *detail::VulkanDevice::create(settings, &dummy);
    }

    // Create SwapChain.
    mainWindow->getSize(&g_viewSize);
    g_swapChain = g_device->createSwapChain(mainWindow, g_viewSize);

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

bool utils_processEvents() {
    ln::detail::PlatformManager* manager = ln::EngineManager::instance()->platformManager();
    manager->processSystemEventQueue();
    return !manager->shouldQuit();
}

bool utils_shouldQuit() {
    ln::detail::PlatformManager* manager = ln::EngineManager::instance()->platformManager();
    return manager->shouldQuit();
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

    Engine::terminate();
}

void mainLoop() {
    utils_processEvents();

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
    while (!utils_shouldQuit()) {
        mainLoop();
        Thread::sleep(16);
    }
#endif
    cleanup();
    return 0;
}
