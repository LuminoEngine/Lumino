#pragma once
#include <webgpu/webgpu.h>
#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsDeviceContext.hpp>

#ifdef __EMSCRIPTEN__
#define LN_WEBGPU_LEGACY 1
#define LN_WEBGPU_STRING_VIEW const char*
#define LN_WEBGPU_MAKE_STRING_VIEW(literal) literal
#define LN_WEBGPU_USERDATA_PARAMS(x) void* x
#define WGPUTexelCopyTextureInfo WGPUImageCopyTexture
#define WGPUTexelCopyBufferLayout WGPUTextureDataLayout

#ifdef WGPU_RENDER_PIPELINE_DESCRIPTOR_INIT
// もし Dawn のヘッダが採用されたら、以下も含めてそちらを使うよう対応する
#error "WGPU_RENDER_PIPELINE_DESCRIPTOR_INIT already defined"
#endif
#define WGPU_RENDER_PIPELINE_DESCRIPTOR_INIT {}
#define WGPU_TEXTURE_DESCRIPTOR_INIT {}
#define WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT {}
#define WGPU_DEVICE_DESCRIPTOR_INIT {}
#define WGPU_STENCIL_FACE_STATE_INIT {}
#define WGPU_QUEUE_WORK_DONE_CALLBACK_INFO_INIT {}
#define WGPU_FUTURE_WAIT_INFO_INIT {}
#define WGPU_BIND_GROUP_LAYOUT_ENTRY_INIT {}
#define WGPU_SURFACE_CAPABILITIES_INIT {}
#define WGPU_SURFACE_CONFIGURATION_INIT {}
#define WGPU_VERTEX_ATTRIBUTE_INIT {}
#define WGPU_BIND_GROUP_ENTRY_INIT {}
#define WGPU_BIND_GROUP_DESCRIPTOR_INIT {}
#define WGPU_TEXEL_COPY_TEXTURE_INFO_INIT {}
#define WGPU_TEXEL_COPY_BUFFER_LAYOUT_INIT {}
#define WGPU_SAMPLER_DESCRIPTOR_INIT {}
#define WGPU_FRAGMENT_STATE_INIT {}
#define WGPU_BLEND_STATE_INIT {}
#define WGPU_COLOR_TARGET_STATE_INIT {}
#define WGPU_DEPTH_STENCIL_STATE_INIT {}
#define WGPU_RENDER_PASS_DEPTH_STENCIL_ATTACHMENT_INIT {}

#else
#define LN_WEBGPU_STRING_VIEW WGPUStringView
#define LN_WEBGPU_MAKE_STRING_VIEW(literal) {literal, sizeof(literal) - 1}
#define LN_WEBGPU_USERDATA_PARAMS(x) void *x, void *userdata2
#endif

namespace ln {
namespace detail {
	
class WebGPUDevice;
class WebGPUShaderPass;
class WebGPURenderTarget;
class WebGPUBindGroupCache;
class WebGPUDescriptorPool;
class WebGPUDescriptor;
class WebGPUSingleFrameAllocatorPageManager;
class WebGPUSingleFrameAllocator;

} // namespace detail
} // namespace ln
