#pragma once
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <string>

namespace ln {

/** Loads image files into RHI textures via stb_image. */
class TextureLoader {
public:
#ifndef __EMSCRIPTEN__
    /** Load an image file (PNG, JPG, BMP, TGA, etc.) and create an RGBA8 texture. */
    static Result<Ref<rhi::Texture>> loadFromFile(rhi::Device* device, const std::string& path);
#endif

    /** Load an image from memory. */
    static Result<Ref<rhi::Texture>> loadFromMemory(rhi::Device* device, const void* data, size_t size);

    /** Create a 1x1 white texture (useful as default). */
    static Result<Ref<rhi::Texture>> createWhiteTexture(rhi::Device* device);
};

} // namespace ln
