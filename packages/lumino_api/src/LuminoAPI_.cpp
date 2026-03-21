#include "pch.hpp"
#include <LuminoBase.hpp>
#include <lumino_core/CoreInstance.hpp>
#include <lumino_core/runtime/ObjectRegistry.hpp>
#include <lumino_core/graphics/Texture2D.hpp>
#include <lumino_api/lumino.h>

LNResult LNInstance_Initialize() {
    auto result = ln::CoreInstance::initialize(ln::CoreInstance::Settings{});
    if (!result) return LN_ERROR_UNKNOWN;
    return LN_OK;
}

void LNInstance_Terminate() {
    ln::CoreInstance::terminate();
}

LNResult LNObject_Release(LNHandle handle) {
    auto* inst = ln::CoreInstance::instance();
    if (!inst) return LN_RUNTIME_UNINITIALIZED;
    if (handle == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;

    if (!inst->objectRegistry()->release(handle))
        return LN_ERROR_INVALID_HANDLE;

    return LN_OK;
}

LNResult LNTexture2D_Create(
    uint32_t width,
    uint32_t height,
    uint32_t format,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* inst = ln::CoreInstance::instance();
    if (!inst) return LN_RUNTIME_UNINITIALIZED;

    auto texture = ln::Ref<ln::Texture2D>::adopt(LN_NEW ln::Texture2D(width, height, format));
    LNHandle handle = inst->objectRegistry()->registerObject(std::move(texture));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}
