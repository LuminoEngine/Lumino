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
#include <LuminoEngine/Platform/PlatformModule.hpp>
#include <LuminoEngine/Font/detail/FontManager.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
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
    if (!EngineContext2::initialize(RuntimeModuleSettings{})) {
        return LN_MAKE_ERROR();
    }

    if (options.platform.enabled) {
        PlatformModuleSettings opt;
        opt.mainWindowSettings.title = options.platform.title;
        opt.mainWindowSettings.clientWidth = options.platform.width;
        opt.mainWindowSettings.clientHeight = options.platform.height;
        opt.windowSystem = options.platform.windowSystem;
        PlatformModule::initialize(opt);
    }

    if (options.graphics.enabled) {
        if (!GraphicsModule::initialize({ options.graphics.graphicsAPI })) {
            return err();
        }

        {
            detail::FontManager::Settings settings;
            settings.assetManager = detail::AssetManager::instance();
            if (!detail::FontManager::initialize(settings)) {
                return err();
            }
        }

        {
            detail::RenderingManager::Settings settings;
            settings.graphicsManager = detail::GraphicsManager::instance();
            settings.fontManager = detail::FontManager::instance();
            if (!detail::RenderingManager::initialize(settings)) {
                return err();
            }
        }
    }
    return ok();
}

void Engine::terminate() {
    detail::RenderingManager::terminate();
    detail::FontManager::terminate();
    GraphicsModule::terminate();
    PlatformModule::terminate();
    EngineContext2::terminate();
}

void Engine::mountAssetDirectory(const StringView& path) {
    EngineContext2::instance()->assetManager()->addAssetDirectory(path);
}

void Engine::mountAssetArchive(const StringView& filePath, const StringView& password) {
    EngineContext2::instance()->assetManager()->mountAssetArchive(filePath, password);
}

EngineContext2* Engine::getEngineContext_() {
    return EngineContext2::instance();
}

} // namespace ln
