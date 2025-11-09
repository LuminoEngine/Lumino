#include "Internal.hpp"
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include "../Audio/AudioManager.hpp"
#include <LuminoEngine/Engine/Engine.hpp>

namespace ln {

//==============================================================================
//
// NOTE: 結局 Engine クラスは復活させることにした。
//   モジュールを小分けにするにしても、統合的な初期化処理が無いと初期化のクローンコードが増え、かなり使いづらい。
//
// NOTE: Naming
//    再利用可能性をより重視して再設計している今、 Engine という名前が果たして良いのかわからない。
//    が、Core という名前よりは汎用的なことまでしていると思うし、他に良い名前が思い浮かばない。
//    とりあえず filament が engine という名前を使っているので、それに倣うことにする。
//

MaybeResult_deprecated Engine::initialize(const EngineOptions& options) {
    RuntimeModuleSettings engineOptions;
    engineOptions.windowSystem = options.platform.windowSystem;
    engineOptions.graphicsAPI = options.graphics.graphicsAPI;
    if (!EngineInstance::initialize(engineOptions)) {
        return LN_MAKE_ERROR_deprecated();
    }
    return LN_MAKE_SUCCESS();
}

void Engine::terminate() {
    detail::AudioManager2::terminate();
    EngineInstance::terminate();
}

void Engine::mountAssetDirectory(const StringView& path) {
    EngineInstance::instance()->assetManager()->addAssetDirectory(path);
}

void Engine::mountAssetArchive(const StringView& filePath, const StringView& password) {
    EngineInstance::instance()->assetManager()->mountAssetArchive(filePath, password);
}

EngineInstance* Engine::getEngineContext_() {
    return EngineInstance::instance();
}

} // namespace ln
