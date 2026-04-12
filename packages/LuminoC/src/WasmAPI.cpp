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

} // extern "C"

#endif // __EMSCRIPTEN__
