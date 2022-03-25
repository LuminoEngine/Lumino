#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/RuntimeModule.hpp>

namespace ln {

//==============================================================================
// Runtime

Result RuntimeModule::initialize(const RuntimeModuleSettings& settings) {
    if (EngineContext2::initialize(settings))
        return ok();
    else
        return err();
}

void RuntimeModule::terminate() {
    EngineContext2::terminate();
}

void RuntimeModule::mountAssetDirectory(const StringView& path) {
    EngineContext2::instance()->assetManager()->addAssetDirectory(path);
}

void RuntimeModule::mountAssetArchive(const StringView& filePath, const StringView& password) {
    EngineContext2::instance()->assetManager()->mountAssetArchive(filePath, password);
}

} // namespace ln
