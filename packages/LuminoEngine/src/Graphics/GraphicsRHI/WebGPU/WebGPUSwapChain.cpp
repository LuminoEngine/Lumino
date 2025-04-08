#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderTarget.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUSwapChain.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPUSwapChain

WebGPUSwapChain::WebGPUSwapChain()
    : m_device(nullptr)
    , m_wgpuSurface(nullptr)
    , m_format(TextureFormat::Unknown)
    , m_width(0)
    , m_height(0)
    , m_imageIndex(0)
    , m_currentRenderTargets{} {
}

Result_deprecated<> WebGPUSwapChain::init(WebGPUDevice* device, PlatformWindow* window, const SizeI& backbufferSize) {
    m_device = device;
    m_wgpuSurface = m_device->getWGPUSurface(window);
    m_width = backbufferSize.width;
    m_height = backbufferSize.height;

    WGPUDevice wgpuDevice = m_device->wgpuDevice();
    WGPUAdapter wgpuAdapter = m_device->wgpuAdapter();

    // Querying Surface Capabilities
    // https://webgpu-native.github.io/webgpu-headers/Surfaces.html#Surface-Capabilities
    WGPUTextureFormat preferredFormat = WGPUTextureFormat_Undefined;
    bool supportsMailbox = false;
    {
        WGPUSurfaceCapabilities capabilities = WGPU_SURFACE_CAPABILITIES_INIT;
        WGPUStatus result = wgpuSurfaceGetCapabilities(m_wgpuSurface, wgpuAdapter, &capabilities);
        if (result != WGPUStatus_Success) {
            return LN_MAKE_ERROR_deprecated("wgpuSurfaceGetCapabilities failed. %d", result);
        }
        preferredFormat = capabilities.formats[0];
        bool supportsMailbox = false;
        for (size_t i = 0; i < capabilities.presentModeCount; i++) {
            if (capabilities.presentModes[i] == WGPUPresentMode_Mailbox) supportsMailbox = true;
        }
        wgpuSurfaceCapabilitiesFreeMembers(capabilities);
    }

    // Configure the surface
    WGPUSurfaceConfiguration config = WGPU_SURFACE_CONFIGURATION_INIT;
    config.nextInChain = nullptr;
    config.width = m_width;
    config.height = m_height;
    config.usage = WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_CopySrc; // CopySrc is for screen capture
    config.format = preferredFormat;
    config.viewFormatCount = 0;
    config.viewFormats = nullptr;
    config.device = wgpuDevice;
    config.presentMode = supportsMailbox ? WGPUPresentMode_Mailbox : WGPUPresentMode_Fifo,
    config.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpuSurfaceConfigure(m_wgpuSurface, &config);

    //WGPUSwapChainDescriptor swapChainDesc = {};
    //swapChainDesc.width = backbufferSize.width;
    //swapChainDesc.height = backbufferSize.height;
    //swapChainDesc.usage = WGPUTextureUsage_RenderAttachment;

    //WGPUTextureFormat swapChainFormat = wgpuSurfaceGetPreferredFormat(m_wgpuSurface, m_device->wgpuAdapter());
    //swapChainDesc.format = swapChainFormat;
    //m_format = WebGPUHelper::WGPUTextureFormatToTextureFormat(swapChainFormat);
	
    //swapChainDesc.presentMode = WGPUPresentMode_Fifo;
    //m_wgpuSwapChain = wgpuDeviceCreateSwapChain(m_device->wgpuDevice(), m_wgpuSurface, &swapChainDesc);
    //if (!m_wgpuSwapChain) {
    //    LN_LOG_ERROR("wgpuDeviceCreateSwapChain failed.");
    //    return err();
    //}


    for (int i = 0; i < BackbufferCount; i++) {
        auto renderTarget = makeRef<WebGPURenderTarget>();
        if (!renderTarget->initForSwapChainWrapper(m_device, m_width, m_height, m_format, preferredFormat)) {
            return err();
        }
        m_currentRenderTargets[i] = renderTarget;
    }
	
	//

    // https://hackmd.io/@webgpu/HJdib9rOD#GPUFence--gt-GPUQueueonSubmittedWorkDone
	
    return ok();
}

void WebGPUSwapChain::onDestroy() {
    for (auto& i : m_currentRenderTargets) {
        i->destroy();
    }
    m_currentRenderTargets = {};
    m_wgpuSurface = nullptr;
}

uint32_t WebGPUSwapChain::getBackbufferCount() {
    return BackbufferCount;
}

void WebGPUSwapChain::acquireNextImage(int* outImageIndex) {
    *outImageIndex = m_imageIndex;
    
	// Get the surface texture
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(m_wgpuSurface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal) {
        LN_LOG_ERROR("wgpuSurfaceGetCurrentTexture failed.");
        return;
    }
    
	// Create a view for this surface texture
    WGPUTextureViewDescriptor viewDescriptor = WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT;
    viewDescriptor.nextInChain = nullptr;
    viewDescriptor.label = { "Surface texture view", 20 };
    viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
    viewDescriptor.dimension = WGPUTextureViewDimension_2D;
    viewDescriptor.baseMipLevel = 0;
    viewDescriptor.mipLevelCount = 1;
    viewDescriptor.baseArrayLayer = 0;
    viewDescriptor.arrayLayerCount = 1;
    viewDescriptor.aspect = WGPUTextureAspect_All;
    WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

    // https://github.com/eliemichel/LearnWebGPU-Code/blob/step025/main.cpp
#ifndef WEBGPU_BACKEND_WGPU
    // We no longer need the texture, only its view
    // (NB: with wgpu-native, surface textures must not be manually released)
    //wgpuTextureRelease(surfaceTexture.texture);
#endif // WEBGPU_BACKEND_WGPU

    WebGPURenderTarget* renderTarget = m_currentRenderTargets[m_imageIndex];
    renderTarget->wrapTextureView(surfaceTexture.texture, targetView);
}

RHIResource* WebGPUSwapChain::getRenderTarget(int imageIndex) const {
    return m_currentRenderTargets[imageIndex];
}

Result_deprecated<> WebGPUSwapChain::resizeBackbuffer(uint32_t width, uint32_t height) {
    LN_NOTIMPLEMENTED();
    return ok();
}

void WebGPUSwapChain::present() {

    #if 0
    {
        WGPUInstance nativeInstance = m_device->nativeInstance();
        WGPUDevice nativeDevice = m_device->wgpuDevice();
        WGPUQueue nativeQueue = m_device->wgpuQueue();
        uint32_t pixelSize = 4; //RHIHelper::getPixelSize(textureFormat());
        uint32_t width = 320;   //extentSize().width;
        uint32_t height = 240;  //extentSize().height;
        uint32_t size = width * height * pixelSize;
        #if 0

        auto onDone = [](WGPUQueueWorkDoneStatus status, void* userdata1, void* userdata) {
            printf("Done!!!");
        };
        WGPUQueueWorkDoneCallbackInfo callbackInfo2 = WGPU_QUEUE_WORK_DONE_CALLBACK_INFO_INIT;
        callbackInfo2.mode = WGPUCallbackMode_WaitAnyOnly;
        callbackInfo2.callback = onDone;
        WGPUFuture f2 = wgpuQueueOnSubmittedWorkDone(nativeQueue, callbackInfo2);

        WGPUFutureWaitInfo waitInfo2 = WGPU_FUTURE_WAIT_INFO_INIT;
        waitInfo2.future = f2;
        waitInfo2.completed = 0;
        auto r12 = wgpuInstanceWaitAny(m_device->nativeInstance(), 1, &waitInfo2, 2000); //

        ::_sleep(1000); // TODO: remove
        #endif

        RHIRef<RHIBitmap> bitmap = makeRHIRef<RHIBitmap>();
        if (!bitmap->init(pixelSize, width, height)) {
            //return nullptr;
        }


        // Transfer Buffer
        WGPUBufferDescriptor bufferDesc = WGPU_BUFFER_DESCRIPTOR_INIT;
        bufferDesc.usage = WGPUBufferUsage_MapRead | WGPUBufferUsage_CopyDst;
        bufferDesc.size = size;
        bufferDesc.mappedAtCreation = 0;
        WGPUBuffer nativeBuffer = wgpuDeviceCreateBuffer(nativeDevice, &bufferDesc);
        if (!nativeBuffer) {
            LN_MAKE_ERROR("wgpuDeviceCreateBuffer() failed."); // TODO:
            //return nullptr;
        }

        WGPUTexelCopyTextureInfo sourceInfo = WGPU_TEXEL_COPY_TEXTURE_INFO_INIT;
        sourceInfo.texture = m_currentRenderTargets[m_imageIndex]->nativeTexture(); //m_nativeTexture;
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
        wgpuCommandEncoderInsertDebugMarker(commandEncoder, { "Do one thing", 10 });
        wgpuCommandEncoderCopyTextureToBuffer(commandEncoder, &sourceInfo, &destInfo, &copySize);
        WGPUCommandBuffer commandBuffer = wgpuCommandEncoderFinish(commandEncoder, nullptr);
        wgpuCommandEncoderRelease(commandEncoder);
        wgpuQueueSubmit(nativeQueue, 1, &commandBuffer);
        wgpuCommandBufferRelease(commandBuffer);

        // Queue を待機する
        auto onDone = [](WGPUQueueWorkDoneStatus status, void* userdata1, void* userdata) {
            printf("Done!!!");
        };
        WGPUQueueWorkDoneCallbackInfo callbackInfo1 = WGPU_QUEUE_WORK_DONE_CALLBACK_INFO_INIT;
        callbackInfo1.mode = WGPUCallbackMode_WaitAnyOnly;
        callbackInfo1.callback = onDone;
        WGPUFuture future1 = wgpuQueueOnSubmittedWorkDone(nativeQueue, callbackInfo1);
        WGPUFutureWaitInfo waitInfo1 = WGPU_FUTURE_WAIT_INFO_INIT;
        waitInfo1.future = future1;
        waitInfo1.completed = 0;
        auto ss = wgpuInstanceWaitAny(nativeInstance, 1, &waitInfo1, 2000);

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
            const uint8_t* mapping = (uint8_t*)
                wgpuBufferGetConstMappedRange(context->buffer, 0, context->size);
            LN_ASSERT(mapping);

            wgpuBufferUnmap(context->buffer);
        };
        WGPUBufferMapCallbackInfo callbackInfo = WGPU_BUFFER_MAP_CALLBACK_INFO_INIT;
        callbackInfo.mode = WGPUCallbackMode_WaitAnyOnly;
        callbackInfo.callback = onBuffer2Mapped;
        callbackInfo.userdata1 = &context;
        WGPUFuture f = wgpuBufferMapAsync(nativeBuffer, WGPUMapMode_Read, 0, size, callbackInfo);

        // wgpuInstanceProcessEvents
        WGPUFutureWaitInfo waitInfo = WGPU_FUTURE_WAIT_INFO_INIT;
        waitInfo.future = f;
        waitInfo.completed = 0;
        auto r1 = wgpuInstanceWaitAny(m_device->nativeInstance(), 1, &waitInfo, 2000); //

        //
        while (!context.ready) {
            //  ^^^^^^^^^^^^^ Use context.ready here instead of ready
            wgpuDeviceTick(nativeDevice);
        }

        wgpuBufferDestroy(nativeBuffer);
        wgpuBufferRelease(nativeBuffer);
        //return nullptr;
    }
    #endif




    wgpuSurfacePresent(m_wgpuSurface);

#if defined(WEBGPU_BACKEND_DAWN)
    wgpuDeviceTick(m_device->wgpuDevice());
#elif defined(WEBGPU_BACKEND_WGPU)
    wgpuDevicePoll(m_device->wgpuDevice(), false, nullptr);
#endif

    m_imageIndex = (m_imageIndex + 1) % BackbufferCount;
}

} // namespace detail
} // namespace ln
