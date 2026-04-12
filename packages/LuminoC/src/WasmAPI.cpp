// Phase 1 web wrapper.
// A minimal subset of the LuminoC C-API compiled only for the Emscripten build.
// At Phase 1 it exposes LNInstance_Initialize / LNInstance_Terminate which
// drive the real ln::CoreInstance, so we can verify that the init / terminate
// path (and its log output) flows all the way to the browser console.
//
// The full LuminoAPI.cpp pulls in GraphicsModule / GraphicsContext / Texture /
// Mesh / ... and cannot be compiled for the web until Phase 2/4. This file is
// intentionally self-contained so we can link it into the wasm executable
// alongside a small slice of LuminoBase + LuminoCore.

#if defined(__EMSCRIPTEN__)

#include <cstdio>
#include <LuminoBase.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Runtime/ObjectRegistry.hpp>
#include <LuminoCore/Platform/Window.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoC/lumino.h>

namespace {

// Route LuminoBase's Logger callback to stdout so Emscripten forwards it to
// the browser's console.log (easier to spot than stderr on the web).
void luminoLogToStdout(ln::LogLevel level, const char* file, int line,
                       const char* /*func*/, const char* message) {
    const char* levelStr = "?";
    switch (level) {
        case ln::LogLevel::Trace:   levelStr = "T"; break;
        case ln::LogLevel::Debug:   levelStr = "D"; break;
        case ln::LogLevel::Verbose: levelStr = "V"; break;
        case ln::LogLevel::Info:    levelStr = "I"; break;
        case ln::LogLevel::Warning: levelStr = "W"; break;
        case ln::LogLevel::Error:   levelStr = "E"; break;
        case ln::LogLevel::Fatal:   levelStr = "F"; break;
        default: break;
    }
    std::printf("[Lumino][%s] %s:%d %s\n", levelStr, file ? file : "?", line, message ? message : "");
}

bool s_loggerInstalled = false;

void ensureLoggerInstalled() {
    if (s_loggerInstalled) return;
    ln::Logger::setLevel(ln::LogLevel::Trace);
    ln::Logger::setCallback(&luminoLogToStdout);
    s_loggerInstalled = true;
}

ln::rhi::Backend toRhiBackend(LNGraphicsBackend b) {
    switch (b) {
        case LN_GRAPHICS_BACKEND_VULKAN: return ln::rhi::Backend::Vulkan;
        case LN_GRAPHICS_BACKEND_WEBGPU: return ln::rhi::Backend::WebGPU;
        case LN_GRAPHICS_BACKEND_DEFAULT:
        default:                         return ln::rhi::Backend::WebGPU;
    }
}

} // anonymous namespace

extern "C" {

LUMINO_API LNResult LNInstance_Initialize(const LNInstanceInitializeSettings* settings) {
    ensureLoggerInstalled();
    std::printf("LNInstance_Initialize: called\n");

    ln::CoreInstance::Settings s = {};
    if (settings) {
        s.enableValidation = settings->enableValidation != LN_FALSE;
        s.preferredBackend = toRhiBackend(settings->preferredBackend);
    } else {
        s.preferredBackend = ln::rhi::Backend::WebGPU;
    }

    auto result = ln::CoreInstance::initialize(s);
    if (!result) {
        std::printf("LNInstance_Initialize: FAILED\n");
        return LN_ERROR_UNKNOWN;
    }
    std::printf("LNInstance_Initialize: success\n");
    return LN_OK;
}

LUMINO_API void LNInstance_Terminate() {
    ensureLoggerInstalled();
    std::printf("LNInstance_Terminate: called\n");
    ln::CoreInstance::terminate();
    std::printf("LNInstance_Terminate: success\n");
}

//------------------------------------------------------------------------------
// Handle helpers (mirrors LuminoAPI.cpp's anonymous namespace)
//------------------------------------------------------------------------------

static LNHandle wrapObjectFromCreate(ln::Object* object) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_NULL_HANDLE;
    LNHandle handle = instance->objectRegistry()->registerObject(object);
    object->addRef();
    return handle;
}

static LNHandle wrapObjectFromGet(ln::Object* object) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_NULL_HANDLE;
    return instance->objectRegistry()->registerObject(object);
}

//------------------------------------------------------------------------------
// LNObject
//------------------------------------------------------------------------------

LUMINO_API LNResult LNObject_Release(LNHandle handle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    if (handle == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;
    if (!instance->objectRegistry()->release(handle)) {
        return LN_ERROR_INVALID_HANDLE;
    }
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNWindow
//------------------------------------------------------------------------------

LUMINO_API LNResult LNWindow_CreateFromCanvas(
    const char* canvasSelector,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
    if (!canvasSelector || !outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    ln::platform::WindowDesc winDesc;
    winDesc.canvasSelector = canvasSelector;
    winDesc.width = width;
    winDesc.height = height;

    ln::GraphicsContextDesc gfxDesc;
    auto windowResult = ln::platform::PlatformWindow::create(
        nullptr, winDesc, gfxDesc);
    if (!windowResult) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromCreate(windowResult->get());
    return LN_OK;
}

LUMINO_API LNResult LNWindow_Create(
    const char* title,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
    // Web ビルドでは GLFW ベースの Window は作成できない。
    (void)title;
    (void)width;
    (void)height;
    if (outHandle) *outHandle = LN_NULL_HANDLE;
    return LN_ERROR_UNKNOWN;
}

LUMINO_API LNResult LNWindow_GetGraphicsContext(LNHandle window, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    ln::GraphicsContext* ctx = obj->graphicsContext();
    if (!ctx) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromGet(ctx);
    return LN_OK;
}

LUMINO_API LNResult LNWindow_ProcessEvents(LNHandle handle, LNBool* outQuit) {
    if (!outQuit) return LN_ERROR_INVALID_ARGUMENT;
    *outQuit = LN_FALSE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(handle);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    *outQuit = obj->processEvents() ? LN_FALSE : LN_TRUE;
    return LN_OK;
}

} // extern "C"

#endif // __EMSCRIPTEN__
