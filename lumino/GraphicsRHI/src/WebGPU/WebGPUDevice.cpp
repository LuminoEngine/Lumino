#include <Windows.h>
#include <LuminoPlatform/PlatformSupport.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPUSwapChain.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPUCommandList.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPURenderPass.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPUDevice.hpp>

namespace ln {
namespace detail {

//==============================================================================
// DX12Device

WebGPUDevice::WebGPUDevice()
    : m_device(nullptr) {
}

bool WebGPUDevice::init(const Settings& settings) {
    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;
    m_instance = wgpuCreateInstance(&desc);
    if (!m_instance) {
        LN_LOG_ERROR("wgpuCreateInstance failed.");
        return false;
    }

    // Select adapter
    {
        auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* pUserData) {
            WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(pUserData);
            self->m_adapters.push_back({ status, adapter });
        };
        WGPURequestAdapterOptions adapterOptions = {};
        adapterOptions.nextInChain = nullptr;
        adapterOptions.compatibleSurface = nullptr;
        adapterOptions.powerPreference = WGPUPowerPreference_Undefined;
        adapterOptions.forceFallbackAdapter = false;
        wgpuInstanceRequestAdapter(
            m_instance,
            &adapterOptions,
            onAdapterRequestEnded,
            this);

		if (m_adapters.empty()) {
            LN_LOG_ERROR("Adapter not found.");
            return false;
        }

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
    }

    // Request device.
    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.nextInChain = nullptr;
    deviceDesc.label = "Lumino device";
    deviceDesc.requiredFeaturesCount = 0;
    deviceDesc.requiredFeatures = nullptr;
    deviceDesc.requiredLimits = nullptr;
    deviceDesc.defaultQueue.nextInChain = nullptr;
    deviceDesc.defaultQueue.label = "Lumino default queue";
    if (!requestDevice(m_adapters[0].adapter, deviceDesc)) {
        return false;
    }
	
    // Setup debug log.
	if (settings.debugMode) {
        auto onDeviceError = [](WGPUErrorType type, char const* message, void*) {
            LN_LOG_ERROR("WebGPU({}): {}", type, message);
        };
        wgpuDeviceSetUncapturedErrorCallback(m_device, onDeviceError, nullptr);
    }

    // Prepare Queue.
    m_queue = wgpuDeviceGetQueue(m_device);

	
    return true;
}

void WebGPUDevice::dispose() {

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

void WebGPUDevice::onGetCaps(GraphicsDeviceProperty* outCaps) {
    LN_NOTIMPLEMENTED();
}

Ref<ISwapChain> WebGPUDevice::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) {
    auto ptr = makeRef<WebGPUSwapChain>();
    if (!ptr->init(this, window, backbufferSize)) {
        return nullptr;
    }
    return ptr;
}

Ref<ICommandList> WebGPUDevice::onCreateCommandList() {
    auto ptr = makeRef<WebGPUCommandList>();
    if (!ptr->init(this)) {
        return nullptr;
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
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IVertexDeclaration> WebGPUDevice::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> WebGPUDevice::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> WebGPUDevice::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> WebGPUDevice::onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    LN_NOTIMPLEMENTED();
    return nullptr;
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
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<ISamplerState> WebGPUDevice::onCreateSamplerState(const SamplerStateData& desc) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IShaderPass> WebGPUDevice::onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> WebGPUDevice::onCreateUniformBuffer(uint32_t size) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IDescriptorPool> WebGPUDevice::onCreateDescriptorPool(IShaderPass* shaderPass) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void WebGPUDevice::onQueueSubmit(ICommandList* context, RHIResource* affectRendreTarget) {
    WebGPUCommandList* rhiCommandList = static_cast<WebGPUCommandList*>(context);

    WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
    cmdBufferDescriptor.nextInChain = nullptr;
    cmdBufferDescriptor.label = nullptr;
    WGPUCommandBuffer command = wgpuCommandEncoderFinish(rhiCommandList->commandEncoder(), &cmdBufferDescriptor);
    wgpuQueueSubmit(m_queue, 1, &command);

#if 0    // 20230302 時点のバージョンではこ wgpuQueueOnSubmittedWorkDone は export されておらず使えなかった
	rhiCommandList->onSubmitted();
	auto callback = [](WGPUQueueWorkDoneStatus status, void* userdata) {
        WebGPUCommandList* rhiCommandList = reinterpret_cast<WebGPUCommandList*>(userdata);
        rhiCommandList->onSubmittedWorkDone();
    };
    wgpuQueueOnSubmittedWorkDone(m_queue, callback, rhiCommandList);
#endif
}

void WebGPUDevice::onQueuePresent(ISwapChain* swapChain) {
    static_cast<WebGPUSwapChain*>(swapChain)->present();
}

Result<> WebGPUDevice::requestDevice(WGPUAdapter adapter, const WGPUDeviceDescriptor& descriptor) {

    auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* userdata) {
        WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(userdata);
        if (status == WGPURequestDeviceStatus_Success) {
            self->m_device = device;
        }
    };

    wgpuAdapterRequestDevice(
        adapter,
        &descriptor,
        onDeviceRequestEnded,
        this);

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
    chainedStruct1.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;

    WGPUSurfaceDescriptorFromWindowsHWND hwndDesc;
    hwndDesc.chain = chainedStruct1;
    hwndDesc.hinstance = hInstance;
    hwndDesc.hwnd = hWnd;

    WGPUSurfaceDescriptor desc = {};
    desc.label = nullptr;
    desc.nextInChain = reinterpret_cast<const WGPUChainedStruct*>(&hwndDesc);

    return wgpuInstanceCreateSurface(m_instance, &desc);
}

} // namespace detail
} // namespace ln
