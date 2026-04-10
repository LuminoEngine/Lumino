#include "WebGPUDevice.hpp"
#include "WebGPUSwapChain.hpp"
#include "WebGPUTexture.hpp"
#include "WebGPUTextureView.hpp"
#include "WebGPUBuffer.hpp"
#include "WebGPUShaderModule.hpp"
#include "WebGPUPipelineLayout.hpp"
#include "WebGPUHelpers.hpp"

namespace ln::rhi::webgpu {

// ------ WebGPUDevice ----------------------------------------------------------------------------------------------------------------

WebGPUDevice::WebGPUDevice() = default;

VoidResult WebGPUDevice::init(const DeviceDesc& desc) {
    // WebGPU インスタンスを生成する
    {
        WGPUInstanceFeatureName features[] = {
            WGPUInstanceFeatureName_TimedWaitAny
        };
        WGPUInstanceLimits limits = {};
        limits.nextInChain = nullptr;
        limits.timedWaitAnyMaxCount = 8;

        WGPUInstanceDescriptor instDesc = {};
        instDesc.nextInChain = nullptr;
        instDesc.requiredFeatureCount = sizeof(features) / sizeof(features[0]);
        instDesc.requiredFeatures = features;
        instDesc.requiredLimits = &limits;
        m_instance = wgpuCreateInstance(&instDesc);
        if (!m_instance) {
            return LN_MAKE_ERROR("wgpuCreateInstance failed.");
        }
    }

#ifdef _WIN32
    // Windows 環境の場合、D3DCompiler DLL をロードしておく。
    // 先にこうしておかないと、wgpuInstanceRequestAdapter で次のようなエラーが発生することがあった。
    // - Error: DynamicLib.Open: d3dcompiler_47.dll Windows Error: 87
    // - [WebGPU] RequestDevice failed: DynamicLib.Open: dxil.dll Windows Error: 87
    //m_hD3DCompilerDLL = ::LoadLibraryW(D3DCOMPILER_DLL_W);

#endif

    // アダプターをリクエストする
    // Dawn の WGPUCallbackMode_AllowSpontaneous モードではコールバックは同期的に呼ばれるため、
    // 呼び出し直後に結果を参照できる。
    {
        //struct AdapterRequest {
        //    WGPUAdapter adapter = nullptr;
        //    WGPURequestAdapterStatus status = {};
        //} adapterReq;

        WGPURequestAdapterOptions adapterOptions = WGPU_REQUEST_ADAPTER_OPTIONS_INIT;
        adapterOptions.nextInChain = nullptr;
        adapterOptions.featureLevel = WGPUFeatureLevel_Undefined;
        adapterOptions.powerPreference = WGPUPowerPreference_HighPerformance;
        adapterOptions.forceFallbackAdapter = false;
        adapterOptions.backendType = WGPUBackendType_Vulkan;
        adapterOptions.compatibleSurface = nullptr;

        WGPURequestAdapterCallbackInfo callbackInfo = WGPU_REQUEST_ADAPTER_CALLBACK_INFO_INIT;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                                   WGPUStringView message,
                                   void* userdata1,
                                   void*) {
            WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(userdata1);
            self->m_adapters.push_back({status, adapter});
            //auto* req = static_cast<AdapterRequest*>(userdata1);
            //req->status = status;
            //req->adapter = adapter;
            if (status != WGPURequestAdapterStatus_Success) {
                std::cerr << "[WebGPU] RequestAdapter failed: "
                          << std::string(message.data, message.length) << "\n";
            }
        };
        callbackInfo.userdata1 = this;

        WGPUFuture _ = wgpuInstanceRequestAdapter(m_instance, &adapterOptions, callbackInfo);

        //if (adapterReq.status != WGPURequestAdapterStatus_Success || !adapterReq.adapter) {
        //    return LN_MAKE_ERROR("wgpuInstanceRequestAdapter failed.");
        //}
        m_adapter = m_adapters[0].adapter; //adapterReq.adapter;
    }

    // アダプター情報をログ出力する
    {
        WGPUAdapterInfo info = WGPU_ADAPTER_INFO_INIT;
        wgpuAdapterGetInfo(m_adapter, &info);
        std::cout << "[WebGPU] Adapter: "
                  << std::string(info.device.data, info.device.length)
                  << " / " << std::string(info.description.data, info.description.length)
                  << "\n";
    }

    // デバイスをリクエストする
    {
        struct DeviceRequest {
            WGPUDevice device = nullptr;
            WGPURequestDeviceStatus status = {};
        } deviceReq;

        WGPUDeviceDescriptor deviceDesc = WGPU_DEVICE_DESCRIPTOR_INIT;
        deviceDesc.label = {"LuminoDevice", WGPU_STRLEN};

        // エラーコールバック
        auto onUncapturedError =
            [](WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void*, void*) {
                std::cerr << "[WebGPU] Uncaptured error [type=" << static_cast<int>(type)
                          << "]: " << std::string(message.data, message.length) << "\n";
#if defined(_MSC_VER) && defined(_DEBUG)
                __debugbreak();
#endif
            };
        deviceDesc.uncapturedErrorCallbackInfo.callback = onUncapturedError;



        // デバイスロストコールバック
        // mode を AllowSpontaneous にしないとコールバックが EventManager にキューイングされ、
        // Instance 破棄時に CallbackCancelled に書き換えられてしまう。
        deviceDesc.deviceLostCallbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        deviceDesc.deviceLostCallbackInfo.callback =
            [](WGPUDevice const*, WGPUDeviceLostReason reason, WGPUStringView message, void*, void*) {
                if (reason == WGPUDeviceLostReason_Destroyed) {
                    return; // 正常な破棄時は無視する
                }
                std::cerr << "[WebGPU] Device lost [reason=" << static_cast<int>(reason) << "]: "
                          << std::string(message.data, message.length) << "\n";
            };

        auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status,
                                       WGPUDevice device,
                                       WGPUStringView message,
                                       void* userdata1,
                                       void* userdata2) {
            auto* req = static_cast<DeviceRequest*>(userdata1);
            req->status = status;
            req->device = device;
            if (status != WGPURequestDeviceStatus_Success) {
                std::cerr << "[WebGPU] RequestDevice failed: "
                          << std::string(message.data, message.length) << "\n";
            }
        };


        WGPURequestDeviceCallbackInfo callbackInfo = WGPU_REQUEST_DEVICE_CALLBACK_INFO_INIT;
        callbackInfo.nextInChain = nullptr;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        //callbackInfo.userdata1 = this;
        callbackInfo.userdata1 = &deviceReq;
        callbackInfo.userdata2 = nullptr;
        callbackInfo.callback = onDeviceRequestEnded;

        wgpuAdapterRequestDevice(m_adapter, &deviceDesc, callbackInfo);

        if (deviceReq.status != WGPURequestDeviceStatus_Success || !deviceReq.device) {
            return LN_MAKE_ERROR("wgpuAdapterRequestDevice failed.");
        }
        m_device = deviceReq.device;
    }

    // デフォルトキューを取得する
    m_queue = wgpuDeviceGetQueue(m_device);
    if (!m_queue) {
        return LN_MAKE_ERROR("wgpuDeviceGetQueue failed.");
    }

    std::cout << "[WebGPU] Device initialized successfully.\n";
    return LN_MAKE_SUCCESS();
}

void WebGPUDevice::finalize() {
    if (m_queue) {
        wgpuQueueRelease(m_queue);
        m_queue = nullptr;
    }
    if (m_device) {
        wgpuDeviceDestroy(m_device); // Destroyed 理由でコールバックを即発火させてから解放する
        wgpuDeviceRelease(m_device);
        m_device = nullptr;
    }
    if (m_adapter) {
        wgpuAdapterRelease(m_adapter);
        m_adapter = nullptr;
    }
    if (m_instance) {
        wgpuInstanceRelease(m_instance);
        m_instance = nullptr;
    }
#ifdef _WIN32
    if (m_hD3DCompilerDLL) {
        ::FreeLibrary(m_hD3DCompilerDLL);
        m_hD3DCompilerDLL = nullptr;
    }
#endif
    Device::finalize();
}

// ------ Stub implementations (将来実装予定) -----------------------------------------------------------------------------------------

DeviceLimits WebGPUDevice::deviceLimits() const {
    // TODO: WGPUSupportedLimits からクエリする
    return DeviceLimits{};
}

Result<Ref<SwapChain>> WebGPUDevice::createSwapChain(const SwapChainDesc& desc) {
    auto sc = Ref<WebGPUSwapChain>::adopt(new WebGPUSwapChain());
    auto result = sc->init(this, desc);
    if (!result) {
        return tl::unexpected(result.error());
    }
    return Ref<SwapChain>(sc);
}

Result<Ref<Buffer>> WebGPUDevice::createBuffer(const BufferDesc& desc) {
    auto buf = Ref<WebGPUBuffer>::adopt(new WebGPUBuffer());
    auto result = buf->init(this, desc);
    if (!result) {
        return tl::unexpected(result.error());
    }
    return Ref<Buffer>(buf);
}

Result<Ref<Texture>> WebGPUDevice::createTexture(const TextureDesc& desc) {
    auto tex = Ref<WebGPUTexture>::adopt(new WebGPUTexture());
    auto result = tex->init(this, desc);
    if (!result) {
        return tl::unexpected(result.error());
    }
    return Ref<Texture>(tex);
}

Result<Ref<TextureView>> WebGPUDevice::createTextureView(Texture* texture) {
    auto* webgpuTex = static_cast<WebGPUTexture*>(texture);
    auto view = Ref<WebGPUTextureView>::adopt(new WebGPUTextureView());
    WGPUTextureFormat fmt = toWGPUTextureFormat(webgpuTex->format());
    WGPUTextureAspect aspect = WGPUTextureAspect_All;
    // Depth/stencil textures need DepthOnly aspect for shader binding,
    // but All is fine for render attachment usage.
    auto result = view->init(this, webgpuTex->handle(), fmt, aspect,
                             webgpuTex->width(), webgpuTex->height());
    if (!result) {
        return tl::unexpected(result.error());
    }
    return Ref<TextureView>(view);
}

Result<Ref<Sampler>> WebGPUDevice::createSampler(const SamplerDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU Sampler not yet implemented."});
}

Result<Ref<ShaderModule>> WebGPUDevice::createShaderModule(const ShaderModuleDesc& desc) {
    auto sm = Ref<WebGPUShaderModule>::adopt(new WebGPUShaderModule());
    auto result = sm->init(this, desc);
    if (!result) {
        return tl::unexpected(result.error());
    }
    return Ref<ShaderModule>(sm);
}

Result<Ref<PipelineLayout>> WebGPUDevice::createPipelineLayout(const PipelineLayoutDesc& desc) {
    auto pl = Ref<WebGPUPipelineLayout>::adopt(new WebGPUPipelineLayout());
    auto result = pl->init(this, desc);
    if (!result) {
        return tl::unexpected(result.error());
    }
    return Ref<PipelineLayout>(pl);
}

Result<Ref<RenderPipeline>> WebGPUDevice::createRenderPipeline(const RenderPipelineDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU RenderPipeline not yet implemented."});
}

VoidResult WebGPUDevice::writeBuffer(Buffer* dst, u64 dstOffset, const void* data, u64 size) {
    auto* wgpuBuf = static_cast<WebGPUBuffer*>(dst);
    wgpuQueueWriteBuffer(m_queue, wgpuBuf->handle(), dstOffset, data, size);
    return LN_MAKE_SUCCESS();
}

Result<std::vector<uint8_t>> WebGPUDevice::readbackTexture(TextureView*) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU readbackTexture not yet implemented."});
}

void WebGPUDevice::waitIdle() {
    if (m_device) {
        wgpuDeviceTick(m_device);
    }
}

} // namespace ln::rhi::webgpu
