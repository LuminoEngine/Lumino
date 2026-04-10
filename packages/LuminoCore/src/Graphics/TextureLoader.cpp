#include <LuminoCore/Graphics/TextureLoader.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#include <stb_image.h>

#include <fstream>
#include <vector>

namespace ln {

static Result<Ref<rhi::Texture>> createTextureFromRGBA(rhi::Device* device, const u8* pixels, u32 w, u32 h) {
    rhi::TextureDesc desc;
    desc.width = w;
    desc.height = h;
    desc.format = rhi::TextureFormat::RGBA8Unorm;
    desc.usage = rhi::TextureUsage::Sampled | rhi::TextureUsage::CopyDst;
    desc.initialData = pixels;
    return device->createTexture(desc);
}

Result<Ref<rhi::Texture>> TextureLoader::loadFromFile(rhi::Device* device, const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        return LN_MAKE_ERROR("Failed to open file: %s", path.c_str());
    }
    auto fileSize = file.tellg();
    file.seekg(0);
    std::vector<u8> buf(static_cast<size_t>(fileSize));
    file.read(reinterpret_cast<char*>(buf.data()), fileSize);
    return loadFromMemory(device, buf.data(), buf.size());
}

Result<Ref<rhi::Texture>> TextureLoader::loadFromMemory(rhi::Device* device, const void* data, size_t size) {
    int w, h, channels;
    u8* pixels = stbi_load_from_memory(
        static_cast<const stbi_uc*>(data), static_cast<int>(size),
        &w, &h, &channels, 4);
    if (!pixels) {
        return LN_MAKE_ERROR("stb_image decode failed");
    }
    auto result = createTextureFromRGBA(device, pixels, static_cast<u32>(w), static_cast<u32>(h));
    stbi_image_free(pixels);
    return result;
}

Result<Ref<rhi::Texture>> TextureLoader::createWhiteTexture(rhi::Device* device) {
    u8 white[4] = {255, 255, 255, 255};
    return createTextureFromRGBA(device, white, 1, 1);
}

} // namespace ln
