#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUHelper {
public:
    static TextureFormat WGPUTextureFormatToTextureFormat(WGPUTextureFormat value);
};

} // namespace detail
} // namespace ln
