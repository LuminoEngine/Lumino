#pragma once
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <string>

namespace ln {

/** stb_image で画像ファイルを読み込み、RHI テクスチャを作成する。 */
class TextureLoader {
public:
#ifndef __EMSCRIPTEN__
    /** 画像ファイル (PNG, JPG, BMP, TGA など) を読み込み、RGBA8 テクスチャを作成する。 */
    static Result<Ref<rhi::Texture>> loadFromFile(rhi::Device* device, const std::string& path);
#endif

    /** メモリ上の画像を読み込む。 */
    static Result<Ref<rhi::Texture>> loadFromMemory(rhi::Device* device, const void* data, size_t size);

    /** 1x1 の白テクスチャを作成する (既定値として便利)。 */
    static Result<Ref<rhi::Texture>> createWhiteTexture(rhi::Device* device);
};

} // namespace ln
