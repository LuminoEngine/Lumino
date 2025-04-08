#include <LuminoEngine/Graphics/GraphicsRHI/RHIHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderTarget.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPURenderTarget

WebGPURenderTarget::WebGPURenderTarget()
    : m_rhiDevice(nullptr)
    , m_nativeTexture(nullptr)
    , m_nativeTextureView(nullptr)
    , m_nativeFormat(WGPUTextureFormat_Undefined) {
}

Result_deprecated<> WebGPURenderTarget::initForSwapChainWrapper(
    WebGPUDevice* rhiDevice,
    int width,
    int height,
    TextureFormat format, WGPUTextureFormat nativeFormat) {
    if (!RHIResource::initAsRenderTarget(width, height, format, false, false)) return err();
    m_rhiDevice = rhiDevice;
    m_nativeFormat = nativeFormat;
    return ok();
}

void WebGPURenderTarget::onDestroy() {
    m_nativeTexture = nullptr;
    m_nativeTextureView = nullptr;
}

void WebGPURenderTarget::wrapTextureView(
    WGPUTexture nativeTexture, WGPUTextureView nativeTextureView) {
    m_nativeTexture = nativeTexture;
    m_nativeTextureView = nativeTextureView;
}

// We define a function that hides implementation-specific variants of device polling:
void wgpuPollEvents([[maybe_unused]] WGPUDevice device, [[maybe_unused]] bool yieldToWebBrowser) {
#if defined(WEBGPU_BACKEND_DAWN)
    wgpuDeviceTick(device);
#elif defined(WEBGPU_BACKEND_WGPU)
    wgpuDevicePoll(device, false, nullptr);
#elif defined(WEBGPU_BACKEND_EMSCRIPTEN)
    if (yieldToWebBrowser) {
        emscripten_sleep(100);
    }
#endif
}

RHIRef<RHIBitmap> WebGPURenderTarget::readData() {
    WGPUDevice nativeDevice = m_rhiDevice->wgpuDevice();
    WGPUQueue nativeQueue = m_rhiDevice->wgpuQueue();
    uint32_t pixelSize = 4; //RHIHelper::getPixelSize(textureFormat());
    uint32_t width = extentSize().width;
    uint32_t height = extentSize().height;
    uint32_t size = width * height * pixelSize;

    RHIRef<RHIBitmap> bitmap = makeRHIRef<RHIBitmap>();
    if (!bitmap->init(pixelSize, width, height)) {
        return nullptr;
    }

    ::_sleep(1000); // TODO: remove

    // Transfer Buffer
    WGPUBufferDescriptor bufferDesc = WGPU_BUFFER_DESCRIPTOR_INIT;
    bufferDesc.usage = WGPUBufferUsage_MapRead | WGPUBufferUsage_CopyDst;
    bufferDesc.size = size;
    bufferDesc.mappedAtCreation = 0;
    WGPUBuffer nativeBuffer = wgpuDeviceCreateBuffer(nativeDevice, &bufferDesc);
    if (!nativeBuffer) {
        LN_MAKE_ERROR("wgpuDeviceCreateBuffer() failed.");  // TODO:
        return nullptr;
    }

    WGPUTexelCopyTextureInfo sourceInfo = WGPU_TEXEL_COPY_TEXTURE_INFO_INIT;
    sourceInfo.texture = m_nativeTexture;
    sourceInfo.mipLevel = 0;
    sourceInfo.origin = { 0, 0, 0 };
    sourceInfo.aspect = WGPUTextureAspect_All;
    WGPUTexelCopyBufferInfo destInfo = WGPU_TEXEL_COPY_BUFFER_INFO_INIT;
    destInfo.buffer = nativeBuffer;
    destInfo.layout.offset = 0;
    destInfo.layout.bytesPerRow = width * pixelSize;
    destInfo.layout.rowsPerImage = height;
    WGPUExtent3D copySize = { width, height, 1 };
    WGPUCommandEncoder commandEncoder = wgpuDeviceCreateCommandEncoder(nativeDevice, nullptr);
    wgpuCommandEncoderInsertDebugMarker(commandEncoder, { "Do one thing",10 });
    wgpuCommandEncoderCopyTextureToBuffer(
        commandEncoder,
        &sourceInfo,
        &destInfo,
        &copySize);
    WGPUCommandBuffer commandBuffer = wgpuCommandEncoderFinish(commandEncoder, nullptr);
    wgpuCommandEncoderRelease(commandEncoder);
    wgpuQueueSubmit(nativeQueue, 1, &commandBuffer);
    wgpuCommandBufferRelease(commandBuffer);

    ::_sleep(1000); // TODO: remove
    struct Context {
        bool ready;
        WGPUBuffer buffer;
        size_t size;
    };
    Context context = { false, nativeBuffer, static_cast<size_t>(size) };
	auto onBuffer2Mapped = [](WGPUMapAsyncStatus status,
                              struct WGPUStringView message,
                              void* userdata1,
                              void* userdata2) {
        Context* context = reinterpret_cast<Context*>(userdata1);
        context->ready = true;
        if (status != WGPUMapAsyncStatus_Success) return;
        uint8_t const* mapping = (uint8_t*)wgpuBufferGetConstMappedRange(
            context->buffer,
            0, context->size);
        LN_ASSERT(mapping);

        wgpuBufferUnmap(context->buffer);
    
        };
    WGPUBufferMapCallbackInfo callbackInfo = WGPU_BUFFER_MAP_CALLBACK_INFO_INIT;
        callbackInfo.mode = WGPUCallbackMode_WaitAnyOnly;
    callbackInfo.callback = onBuffer2Mapped;
    callbackInfo.userdata1 = &context;
    auto f = wgpuBufferMapAsync(nativeBuffer, WGPUMapMode_Read, 0, size, callbackInfo);

    // wgpuInstanceProcessEvents 
    WGPUFutureWaitInfo waitInfo = WGPU_FUTURE_WAIT_INFO_INIT;
    waitInfo.future = f;
    waitInfo.completed = 0;
    auto r1 = wgpuInstanceWaitAny(m_rhiDevice->nativeInstance(), 1, &waitInfo, 2000); //

    //wgpuQueueOnSubmittedWorkDone
    while (!context.ready) {
        //  ^^^^^^^^^^^^^ Use context.ready here instead of ready
        wgpuPollEvents(nativeDevice, true /* yieldToBrowser */);
    }

    /*
    [Error] WebGPU(2): Destroyed texture [Texture "of [Surface]"] used in a submit.
 - While calling [Queue "Lumino default queue"].Submit([[CommandBuffer]])*/


    wgpuBufferDestroy(nativeBuffer);
    wgpuBufferRelease(nativeBuffer);
    return nullptr;
}

/*

static void read_buffer_map_cb(WGPUBufferMapAsyncStatus status, void* user_data)
{
  UNUSED_VAR(user_data);

  if (status == WGPUBufferMapAsyncStatus_Success) {
    int32_t w = offscreen_rendering.pixel_data.buffer_dimensions.width;
    int32_t h = offscreen_rendering.pixel_data.buffer_dimensions.height;
    int32_t channels_num = 4;

    size_t pixels_size     = w * h * channels_num;
    uint8_t* pixels        = (uint8_t*)malloc(pixels_size);
    uint8_t const* mapping = (uint8_t*)wgpuBufferGetConstMappedRange(
      offscreen_rendering.pixel_data.buffer.buffer, 0,
      sizeof(offscreen_rendering.pixel_data.buffer.size));
    ASSERT(mapping);
    memcpy(pixels, mapping, pixels_size);
    stbi_write_png(screenshot_filename, w, h, channels_num, pixels,
                   w * sizeof(int));
    wgpuBufferUnmap(offscreen_rendering.pixel_data.buffer.buffer);
    free(pixels);

    offscreen_rendering.pixel_data.buffer_mapped = false;
    screenshot_requested                         = false;
    screenshot_saved                             = true;
  }
}
*/
} // namespace detail
} // namespace ln
