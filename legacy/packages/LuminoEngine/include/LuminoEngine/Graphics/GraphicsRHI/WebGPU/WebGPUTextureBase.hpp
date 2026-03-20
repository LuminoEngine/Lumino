#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUTextureBase : public RHIResource {
public:
    virtual WGPUTexture nativeTexture() const = 0;
    virtual WGPUTextureView nativeTextureView() const = 0;
};

} // namespace detail
} // namespace ln
