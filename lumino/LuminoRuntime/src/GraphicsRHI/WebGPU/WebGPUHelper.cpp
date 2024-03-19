#include <LuminoEngine/GraphicsRHI/WebGPU/WebGPUHelper.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPUHelper
	
static const std::pair<TextureFormat, WGPUTextureFormat> s_textureFormatConversionTable[] = {
    { TextureFormat::Unknown, WGPUTextureFormat_Undefined },
    { TextureFormat::RGBA8, WGPUTextureFormat_RGBA32Uint },
    { TextureFormat::RGB8, WGPUTextureFormat_RGBA8Unorm }, // TODO: remove
    { TextureFormat::RGBA16F, WGPUTextureFormat_RGBA16Float },
    { TextureFormat::RGBA32F, WGPUTextureFormat_RGBA32Float },
    { TextureFormat::R16F, WGPUTextureFormat_R16Float },
    { TextureFormat::R32F, WGPUTextureFormat_R32Float },
    { TextureFormat::R32S, WGPUTextureFormat_R32Sint },
};

TextureFormat WebGPUHelper::WGPUTextureFormatToTextureFormat(WGPUTextureFormat value) {
    for (auto& i : s_textureFormatConversionTable) {
        if (i.second == value) {
            return i.first;
        }
    }
    return TextureFormat::Unknown;
}


} // namespace detail
} // namespace ln
