#include "Internal.hpp"
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/RHIModule.hpp>
//#include <LuminoEngine/Bitmap/BitmapRenderingContext.hpp>
//#include <LuminoEngine/GPU/VertexBuffer.hpp>
//#include <LuminoEngine/GPU/VertexLayout.hpp>
//#include <LuminoEngine/GPU/RenderPass.hpp>
//#include <LuminoEngine/GPU/Texture.hpp>
//#include <LuminoEngine/GPU/OpenGLGraphicsContext.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Graphics/Font/detail/FontManager.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "../Audio/AudioManager.hpp"
//#include <LuminoEngine/Rendering/CommandList.hpp>
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

MaybeResult Engine::initialize(const EngineOptions& options) {
    RuntimeModuleSettings engineOptions;
    engineOptions.windowSystem = options.platform.windowSystem;
    engineOptions.graphicsAPI = options.graphics.graphicsAPI;
    if (!EngineManager::initialize(engineOptions)) {
        return LN_MAKE_ERROR();
    }

    if (options.graphics.enabled) {

        {
            detail::FontManager::Settings settings;
            settings.assetManager = detail::AssetManager::instance();
            if (!detail::FontManager::initialize(settings)) {
                return err();
            }
        }

        {
            detail::RenderingManager::Settings settings;
            settings.graphicsManager = EngineManager::instance()->graphicsManager();
            settings.fontManager = detail::FontManager::instance();
            if (!detail::RenderingManager::initialize(settings)) {
                return err();
            }
        }
    }

    
    if (options.audio.enabled) {
        detail::AudioManager2::Settings settings;
        settings.assetManager = detail::AssetManager::instance();
        auto result = detail::AudioManager2::initialize(settings);
        if (!result) {
            return result;
        }
    }
    return LN_MAKE_SUCCESS();
}

void Engine::terminate() {
    detail::AudioManager2::terminate();
    detail::RenderingManager::terminate();
    detail::FontManager::terminate();
    EngineManager::terminate();
}

void Engine::mountAssetDirectory(const StringView& path) {
    EngineManager::instance()->assetManager()->addAssetDirectory(path);
}

void Engine::mountAssetArchive(const StringView& filePath, const StringView& password) {
    EngineManager::instance()->assetManager()->mountAssetArchive(filePath, password);
}

EngineManager* Engine::getEngineContext_() {
    return EngineManager::instance();
}

} // namespace ln
