#include "pch.hpp"
#include <LuminoBase.hpp>
#include <lumino_core/CoreInstance.hpp>
#include <lumino_api/lumino.h>

LNResult LNInstance_Initialize() {
    auto result = ln::CoreInstance::initialize(ln::CoreInstance::Settings{});
    if (!result) return LN_ERROR_UNKNOWN;
    return LN_OK;
}

void LNInstance_Terminate() {
    ln::CoreInstance::terminate();
}
