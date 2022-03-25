﻿#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Runtime/Runtime.hpp>

namespace ln {

//==============================================================================
// Runtime

Result Runtime::initialize(const RuntimeModuleSettings& settings) {
    if (EngineContext2::initialize(settings))
        return ok();
    else
        return err();
}

void Runtime::terminate() {
    EngineContext2::terminate();
}

void Runtime::mountAssetDirectory(const StringView& path) {
    EngineContext2::instance()->assetManager()->addAssetDirectory(path);
}

void Runtime::mountAssetArchive(const StringView& filePath, const StringView& password) {
    EngineContext2::instance()->assetManager()->mountAssetArchive(filePath, password);
}

} // namespace ln
