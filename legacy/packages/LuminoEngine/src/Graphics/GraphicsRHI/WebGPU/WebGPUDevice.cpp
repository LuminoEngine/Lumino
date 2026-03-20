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
#ifdef LN_WEBGPU_LEGACY
    // emsdk/upstream/emscripten/system/lib/webgpu/webgpu.cpp を見ると、
    // > assert(descriptor == nullptr); // descriptor not implemented yet
    // という assert があるので、引数は nullptr にしている。
    m_instance = wgpuCreateInstance(nullptr);
#else
    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;
    desc.capabilities.timedWaitAnyEnable = 1;
    desc.capabilities.timedWaitAnyMaxCount = 8;
    m_instance = wgpuCreateInstance(&desc);
#endif
    if (!m_instance) {
        LN_LOG_ERROR("wgpuCreateInstance failed.");
        return false;
    }

    // Select adapter
    {
        WGPURequestAdapterOptions adapterOptions = {};
        adapterOptions.nextInChain = nullptr;
        //adapterOptions.featureLevel = WGPUFeatureLevel_Compatibility;
        adapterOptions.compatibleSurface = nullptr;
        adapterOptions.powerPreference = WGPUPowerPreference_Undefined;
        adapterOptions.forceFallbackAdapter = false;
#ifdef LN_WEBGPU_LEGACY
        auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status,
                                        WGPUAdapter adapter,
                                        char const* message, void* userdata) {
            WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(userdata);
            self->m_adapters.push_back({status, adapter});
        };
        wgpuInstanceRequestAdapter(m_instance, &adapterOptions, onAdapterRequestEnded, this);
#else
        auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status,
                                        WGPUAdapter adapter,
                                        LN_WEBGPU_STRING_VIEW message,
                                        LN_WEBGPU_USERDATA_PARAMS(userdata)) {
            WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(userdata);
            self->m_adapters.push_back({status, adapter});
        };
        WGPURequestAdapterCallbackInfo callbackInfo = {};
        callbackInfo.nextInChain = nullptr;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.callback = onAdapterRequestEnded;
        callbackInfo.userdata1 = this;
        WGPUFuture future = wgpuInstanceRequestAdapter(m_instance, &adapterOptions, callbackInfo);
#endif

#ifdef __EMSCRIPTEN__
        while (m_adapters.empty()) {
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

        WGPUDeviceDescriptor deviceDesc = WGPU_DEVICE_DESCRIPTOR_INIT;
        deviceDesc.nextInChain = nullptr;
        deviceDesc.label = LN_WEBGPU_MAKE_STRING_VIEW("Lumino device");
        deviceDesc.requiredFeatureCount = 0;
        deviceDesc.requiredFeatures = nullptr;
        deviceDesc.requiredLimits = nullptr;
        deviceDesc.defaultQueue.nextInChain = nullptr;
        deviceDesc.defaultQueue.label = LN_WEBGPU_MAKE_STRING_VIEW("Lumino default queue");
        // TODO: これの設定はしておいた方がよさそう
        //deviceDesc.deviceLostCallbackInfo = WGPU_DEVICE_LOST_CALLBACK_INFO_INIT;
        //deviceDesc.uncapturedErrorCallbackInfo = WGPU_UNCAPTURED_ERROR_CALLBACK_INFO_INIT;

        // Setup debug log.
#ifndef LN_WEBGPU_LEGACY
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
#endif

        if (!requestDevice(adapter, deviceDesc)) {
            return false;
        }

#ifdef LN_WEBGPU_LEGACY
        auto onDeviceError = [](WGPUErrorType type, char const* message, void* /* pUserData */) {
            std::cout << "Uncaptured device error: type " << type;
            if (message) std::cout << " (" << message << ")";
            std::cout << std::endl;
        };
        wgpuDeviceSetUncapturedErrorCallback(m_device, onDeviceError, nullptr /* pUserData */);
#endif
    }

    // Diag
    {
#ifdef LN_WEBGPU_LEGACY
        WGPUSupportedLimits adapterSupportedLimits = {};
        WGPUSupportedLimits deviceSupportedLimits = {};
        wgpuAdapterGetLimits(adapter, &adapterSupportedLimits);
        wgpuDeviceGetLimits(m_device, &deviceSupportedLimits);
        const WGPULimits& adapterLimits = adapterSupportedLimits.limits;
        const WGPULimits& deviceLimits = deviceSupportedLimits.limits;
#else
        WGPULimits adapterLimits = {};
        WGPULimits deviceLimits = {};
        wgpuAdapterGetLimits(adapter, &adapterLimits);
        wgpuDeviceGetLimits(m_device, &deviceLimits);
#endif
        LN_LOG_INFO("Adapter Supported limits:");
        LN_LOG_INFO("  maxVertexAttributes: {}", adapterLimits.maxVertexAttributes);
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

Result<Ref<ISwapChain>> WebGPUDevice::onCreateSwapChain(const SwapChainCreateInfo& createInfo) {
    return makeRefWithInit<WebGPUSwapChain>(this, createInfo);
}

Result<Ref<ICommandList>> WebGPUDevice::onCreateCommandList() {
    auto ptr = makeRef<WebGPUCommandList>();
    if (!ptr->init(this)) {
        return LN_MAKE_ERROR();
    }
    return ptr;
}

Result<Ref<IRenderPass>> WebGPUDevice::onCreateRenderPass(const RenderPassCreateInfo& createInfo) {
    auto ptr = makeRef<WebGPURenderPass>();
    auto result = ptr->init(
        this,
        createInfo.buffers,
        createInfo.clearFlags,
        createInfo.clearColor,
        createInfo.clearDepth,
        createInfo.clearStencil);
    if (!result) {
        return LN_TO_ERROR(result);
    }
    return ptr;
}

Result<Ref<IPipeline>> WebGPUDevice::onCreatePipeline(const DevicePipelineCreateInfo& createInfo) {
    auto ptr = makeRef<WebGPUPipeline>();
    auto result = ptr->init(this, createInfo);
    if (!result) {
        return LN_TO_ERROR(result);
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

Result<Ref<ISamplerState>> WebGPUDevice::onCreateSamplerState(const SamplerStateData& desc) {
    return makeRefWithInit<WebGPUSamplerState>(this, desc);
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

#ifdef LN_WEBGPU_LEGACY
    auto onDeviceRequestEnded =
        [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* pUserData) {
            WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(pUserData);
            if (status == WGPURequestDeviceStatus_Success) {
                self->m_device = device;
            }
            else {
                std::cout << "Could not get WebGPU device: " << message << std::endl;
            }
        };
    wgpuAdapterRequestDevice(adapter, &descriptor, onDeviceRequestEnded, this);
#else
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
    #endif

#ifdef __EMSCRIPTEN__
    while (!m_device) {
        emscripten_sleep(100);
    }
#endif // __EMSCRIPTEN__

	if (!m_device) {
        LN_LOG_ERROR("Device not found.");
        return err();
    }

    return ok();
}

} // namespace detail
} // namespace ln
