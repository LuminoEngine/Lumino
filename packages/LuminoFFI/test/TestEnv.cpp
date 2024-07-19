#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoEngine/Platform/PlatformModule.hpp>
#include <LuminoEngine/Platform/Platform.hpp>
#include <LuminoEngine/RHIModule.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderingPipeline/FlatRenderingPipeline.hpp>
#include "TestEnv.hpp"

void TestEnv::initialize() {
    auto graphicsAPI = ln::GraphicsAPI::OpenGL;
    auto windowSystem = ln::WindowSystem::Native;
    ln::detail::GraphicsManager::selectDefaultSystem(&graphicsAPI, &windowSystem);
    ln::RuntimeModule::initialize();
    ln::PlatformModule::initialize({ { U"Test", 320, 240 }, windowSystem });
    ln::GraphicsModule::initialize({ graphicsAPI });
}

void TestEnv::terminate() {
    ln::GraphicsModule::terminate();
    ln::PlatformModule::terminate();
    ln::RuntimeModule::terminate();
}
