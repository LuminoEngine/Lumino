#include <Windows.h>
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUBindGroupCache.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUBufferSingleFrameAllocator.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUSwapChain.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUCommandList.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDepthBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUVertexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUIndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUUniformBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUTexture2D.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUSamplerState.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUShaderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUVertexLayout.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUPipeline.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDescriptorPool.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>

namespace ln {
namespace detail {

//==============================================================================
// DX12Device

WebGPUDevice::WebGPUDevice()
    : m_device(nullptr)
    , m_bindGroupCache(std::make_unique<WebGPUBindGroupCache>(this)) {
}

bool WebGPUDevice::init(const Settings& settings) {
    // https://developer.chrome.com/blog/new-in-webgpu-128?hl=ja#dawn_updates
    //WGPUInstanceCapabilities capabilities = WGPU_INSTANCE_CAPABILITIES_INIT;
    //capabilities.timedWaitAnyEnable = 1;
    //capabilities.timedWaitAnyMaxCount = 0;
    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;
    desc.capabilities.timedWaitAnyEnable = 1;
    desc.capabilities.timedWaitAnyMaxCount = 8;
    m_instance = wgpuCreateInstance(&desc);
    if (!m_instance) {
        LN_LOG_ERROR("wgpuCreateInstance failed.");
        return false;
    }

    // Select adapter
    {
        auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status,
                                        WGPUAdapter adapter,
                                        WGPUStringView message,
                                        void* userdata1,
                                        void* userdata2) {
            WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(userdata1);
            self->m_adapters.push_back({ status, adapter });
        };
        WGPURequestAdapterOptions adapterOptions = {};
        adapterOptions.nextInChain = nullptr;
        adapterOptions.compatibleSurface = nullptr;
        adapterOptions.powerPreference = WGPUPowerPreference_Undefined;
        adapterOptions.forceFallbackAdapter = false;
        WGPURequestAdapterCallbackInfo callbackInfo = {};
        callbackInfo.nextInChain = nullptr;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.callback = onAdapterRequestEnded;
        callbackInfo.userdata1 = this;
        WGPUFuture future = wgpuInstanceRequestAdapter(
            m_instance,
            &adapterOptions, 
            callbackInfo);

#ifdef __EMSCRIPTEN__
        while (!userData.requestEnded) {
            emscripten_sleep(100);
        }
#endif // __EMSCRIPTEN__

		if (m_adapters.empty()) {
            LN_LOG_ERROR("Adapter not found.");
            return false;
        }

#if 0 // アダプタ情報の取得は wgpuAdapterGetPropertiesVk などバックエンド固有の関数になったようなので、対応するまでコメントアウト
		if (Logger::shouldLog(LogLevel::Verbose)) {
            for (const auto& i : m_adapters) {
                WGPUAdapterProperties properties = {};
                wgpuAdapterGetProperties(i.adapter, &properties);
                LN_LOG_VERBOSE("Adapter properties:");
                LN_LOG_VERBOSE("  nextInChain: {}", (intptr_t)properties.nextInChain);
                LN_LOG_VERBOSE("  vendorID: {}", properties.vendorID);
                LN_LOG_VERBOSE("  deviceID: {}", properties.deviceID);
                LN_LOG_VERBOSE("  name: {}", properties.name);
                LN_LOG_VERBOSE("  driverDescription: {}", properties.driverDescription);
                LN_LOG_VERBOSE("  adapterType: {}", properties.adapterType);
                LN_LOG_VERBOSE("  backendType: {}", properties.backendType);

                size_t featureCount = wgpuAdapterEnumerateFeatures(i.adapter, nullptr);
                std::vector<WGPUFeatureName> features;
                features.resize(featureCount);
                wgpuAdapterEnumerateFeatures(i.adapter, features.data());
                LN_LOG_VERBOSE("Adapter features:");
                for (const auto& f : features) {
                    LN_LOG_VERBOSE("  {}", f);
                }
            }
        }
#endif
    }

    WGPUAdapter adapter = m_adapters[0].adapter;

    // Request device.
    {
        WGPULimits limits = WGPU_LIMITS_INIT;
        //limits.maxBufferSize = 2097152;

        WGPUDeviceDescriptor deviceDesc = WGPU_DEVICE_DESCRIPTOR_INIT;
        deviceDesc.nextInChain = nullptr;
        deviceDesc.label = { "Lumino device", 13 };
        deviceDesc.requiredFeatureCount = 0;
        deviceDesc.requiredFeatures = nullptr;
        deviceDesc.requiredLimits = &limits;
        deviceDesc.defaultQueue.nextInChain = nullptr;
        deviceDesc.defaultQueue.label = { "Lumino default queue", 20 };
        deviceDesc.deviceLostCallbackInfo = WGPU_DEVICE_LOST_CALLBACK_INFO_INIT;
        deviceDesc.uncapturedErrorCallbackInfo = WGPU_UNCAPTURED_ERROR_CALLBACK_INFO_INIT;

        // Setup debug log.
        if (settings.debugMode) {
            auto onDeviceError = [](WGPUDevice const* device, WGPUErrorType type, WGPUStringView message, void*, void*) {
                LN_LOG_ERROR("WebGPU({}): {}", static_cast<int>(type), std::string_view(message.data, message.length));
            };
            WGPUUncapturedErrorCallbackInfo callbackInfo = WGPU_UNCAPTURED_ERROR_CALLBACK_INFO_INIT;
            callbackInfo.nextInChain = nullptr;
            callbackInfo.callback = onDeviceError;
            callbackInfo.userdata1 = nullptr;
            callbackInfo.userdata2 = nullptr;
            deviceDesc.uncapturedErrorCallbackInfo = callbackInfo;
        }

        if (!requestDevice(adapter, deviceDesc)) {
            return false;
        }
    }

    // Diag
    {
        WGPULimits adapterLimits = {};
        WGPULimits deviceLimits = {};
        wgpuAdapterGetLimits(adapter, &adapterLimits);
        LN_LOG_INFO("Adapter Supported limits:");
        LN_LOG_INFO("  maxVertexAttributes: {}", adapterLimits.maxVertexAttributes);
        wgpuDeviceGetLimits(m_device, &deviceLimits);
        LN_LOG_INFO("Device Supported limits:");
        LN_LOG_INFO("  maxVertexAttributes: {}", deviceLimits.maxVertexAttributes);
    }

	
    // Prepare Queue.
    m_queue = wgpuDeviceGetQueue(m_device);

    const size_t PageSize = 0x200000; // 2MB
    m_transferBufferSingleFrameAllocator = makeRef<WebGPUSingleFrameAllocatorPageManager>(
        this,
        PageSize);
    	
    return true;
}

void WebGPUDevice::dispose() {
    if (m_transferBufferSingleFrameAllocator) {
        m_transferBufferSingleFrameAllocator = nullptr;
    }
	if (m_device) {
        wgpuDeviceDestroy(m_device);
        m_device = nullptr;
    }
	
    // wgpu-native 等のサンプルを見ると、特に破棄関数はなさそう
    m_instance = nullptr;
}

INativeGraphicsInterface* WebGPUDevice::getNativeInterface() const {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void WebGPUDevice::onGetDeviceProperties(GraphicsDeviceProperties* outCaps) {
    outCaps->graphicsAPI = LN_GRAPHICS_BACKEND_WEBGPU;
    outCaps->requestedShaderTriple.target = "wgsl";
    outCaps->requestedShaderTriple.version = 1;
    outCaps->requestedShaderTriple.option = "";
    outCaps->imageLayoytVFlip = false;
    outCaps->uniformBufferOffsetAlignment = 256;
    outCaps->shaderTarget = kokage::ShaderTarget_WGSL;
}

Result<Ref<ISwapChain>> WebGPUDevice::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) {
    auto ptr = makeRef<WebGPUSwapChain>();
    if (!ptr->init(this, window, backbufferSize)) {
        return LN_MAKE_ERROR();
    }
    return ptr;
}

Result<Ref<ICommandList>> WebGPUDevice::onCreateCommandList() {
    auto ptr = makeRef<WebGPUCommandList>();
    if (!ptr->init(this)) {
        return LN_MAKE_ERROR();
    }
    return ptr;
}

Ref<IRenderPass> WebGPUDevice::onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    auto ptr = makeRef<WebGPURenderPass>();
    if (!ptr->init(this, buffers, clearFlags, clearColor, clearDepth, clearStencil)) {
        return nullptr;
    }
    return ptr;
}

Ref<IPipeline> WebGPUDevice::onCreatePipeline(const DevicePipelineStateDesc& state) {
    auto ptr = makeRef<WebGPUPipeline>();
    if (!ptr->init(this, state)) {
        return nullptr;
    }
    return ptr;
}

Ref<IVertexDeclaration> WebGPUDevice::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) {
    auto ptr = makeRef<WebGPUVertexLayout>();
    if (!ptr->init(this, elements, elementsCount)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> WebGPUDevice::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) {
    auto ptr = makeRef<WebGPUVertexBuffer>();
    if (!ptr->init(this, usage, bufferSize, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> WebGPUDevice::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) {
    auto ptr = makeRef<WebGPUIndexBuffer>();
    if (!ptr->init(
            this,
            usage,
            format,
            indexCount,
            initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> WebGPUDevice::onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    auto ptr = makeRef<WebGPUTexture2D>();
    if (!ptr->init(
            this,
            usage,
            width,
            height,
            requestFormat,
            mipmap,
            initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> WebGPUDevice::onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> WebGPUDevice::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> WebGPUDevice::onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> WebGPUDevice::onCreateDepthBuffer(uint32_t width, uint32_t height) {
    auto ptr = makeRef<WebGPUDepthBuffer>();
    if (!ptr->init(this, width, height)) {
        return nullptr;
    }
    return ptr;
}

Ref<ISamplerState> WebGPUDevice::onCreateSamplerState(const SamplerStateData& desc) {
    auto ptr = makeRef<WebGPUSamplerState>();
    if (!ptr->init(this, desc)) {
        return nullptr;
    }
    return ptr;
}

Ref<IShaderPass> WebGPUDevice::onCreateShaderPass(
    const ShaderPassCreateInfo& createInfo,
    const ShaderPassCreateInfo2* createInfo2OrNull,
    ShaderCompilationDiag* diag) {
    assert(createInfo2OrNull);
    auto ptr = makeRef<WebGPUShaderPass>();
    if (!ptr->init(this, *createInfo2OrNull)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> WebGPUDevice::onCreateUniformBuffer(uint32_t size) {
    auto ptr = makeRef<WebGPUUniformBuffer>();
    if (!ptr->init(this, size)) {
        return nullptr;
    }
    return ptr;
}

Ref<IDescriptorPool> WebGPUDevice::onCreateDescriptorPool(IShaderPass* shaderPass) {
    auto ptr = makeRef<WebGPUDescriptorPool>();
    if (!ptr->init(this, static_cast<WebGPUShaderPass*>(shaderPass))) {
        return nullptr;
    }
    return ptr;
}

void WebGPUDevice::onQueueSubmit(ICommandList* context, RHIResource* affectRendreTarget) {
    WebGPUCommandList* rhiCommandList = static_cast<WebGPUCommandList*>(context);
    WGPUCommandBuffer commandBuffer = rhiCommandList->lastFinishedCommandBuffer();
    if (!commandBuffer) {
        // 一度も begin していない場合
    }
    else {
        wgpuQueueSubmit(m_queue, 1, &commandBuffer);
#if 0 // 20230302 時点のバージョンではこ wgpuQueueOnSubmittedWorkDone は export されておらず使えなかった
	rhiCommandList->onSubmitted();
	auto callback = [](WGPUQueueWorkDoneStatus status, void* userdata) {
        WebGPUCommandList* rhiCommandList = reinterpret_cast<WebGPUCommandList*>(userdata);
        rhiCommandList->onSubmittedWorkDone();
    };
    wgpuQueueOnSubmittedWorkDone(m_queue, callback, rhiCommandList);
#endif
    }

}

void WebGPUDevice::onQueuePresent(ISwapChain* swapChain) {
    static_cast<WebGPUSwapChain*>(swapChain)->present();
}

Result_deprecated<> WebGPUDevice::requestDevice(WGPUAdapter adapter, const WGPUDeviceDescriptor& descriptor) {

    auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status,
                                   WGPUDevice device,
                                   WGPUStringView message,
                                   void* userdata1,
                                   void* userdata2) {
        WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(userdata1);
        if (status == WGPURequestDeviceStatus_Success) {
            self->m_device = device;
        }
    };

    WGPURequestDeviceCallbackInfo callbackInfo = WGPU_REQUEST_DEVICE_CALLBACK_INFO_INIT;
    callbackInfo.nextInChain = nullptr;
    callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    callbackInfo.callback = onDeviceRequestEnded;
    callbackInfo.userdata1 = this;
    callbackInfo.userdata2 = nullptr;
    wgpuAdapterRequestDevice(adapter, &descriptor, callbackInfo);

#ifdef __EMSCRIPTEN__
    while (!userData.requestEnded) {
        emscripten_sleep(100);
    }
#endif // __EMSCRIPTEN__

	if (!m_device) {
        LN_LOG_ERROR("Device not found.");
        return err();
    }

    return ok();
}

// https://github.com/eliemichel/glfw3webgpu/blob/main/glfw3webgpu.c
WGPUSurface WebGPUDevice::getWGPUSurface(PlatformWindow* window) const {
    HWND hWnd = reinterpret_cast<HWND>(PlatformSupport::getWin32WindowHandle(window));
    HINSTANCE hInstance = ::GetModuleHandle(NULL);

    WGPUChainedStruct chainedStruct1 = {};
    chainedStruct1.next = nullptr;
#ifdef WEBGPU_BACKEND_DAWN
    chainedStruct1.sType = WGPUSType_SurfaceSourceWindowsHWND;
#else
    chainedStruct1.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;
#endif

    WGPUSurfaceDescriptorFromWindowsHWND hwndDesc = {};
    hwndDesc.chain = chainedStruct1;
    hwndDesc.hinstance = hInstance;
    hwndDesc.hwnd = hWnd;

    WGPUSurfaceDescriptor desc = {};
    desc.nextInChain = &hwndDesc.chain;

    return wgpuInstanceCreateSurface(m_instance, &desc);
}

} // namespace detail
} // namespace ln
