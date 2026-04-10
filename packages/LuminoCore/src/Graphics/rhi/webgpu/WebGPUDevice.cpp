#include <iostream>
#include <Windows.h>
#include <d3dcompiler.h>
#include "WebGPUDevice.hpp"
#include "WebGPUSwapChain.hpp"
#include "WebGPUTexture.hpp"
#include "WebGPUTextureView.hpp"
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

    // Error: DynamicLib.Open: d3dcompiler_47.dll Windows Error: 87
    HMODULE m_hD3DCompilerDLL = ::LoadLibraryW(D3DCOMPILER_DLL_W);
    if (m_hD3DCompilerDLL) {
        //D3DCompile2 = reinterpret_cast<PFN_D3DCompile2>(
        //    ::GetProcAddress(m_hD3DCompilerDLL, "D3DCompile2"));
    }

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
        adapterOptions.compatibleSurface = nullptr;
        adapterOptions.powerPreference = WGPUPowerPreference_HighPerformance;
        adapterOptions.forceFallbackAdapter = false;

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
        deviceDesc.uncapturedErrorCallbackInfo.callback =
            [](WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void*, void*) {
                std::cerr << "[WebGPU] Uncaptured error [type=" << static_cast<int>(type) << "]: "
                          << std::string(message.data, message.length) << "\n";
#if defined(_MSC_VER) && defined(_DEBUG)
                __debugbreak();
#endif
            };

        // デバイスロストコールバック
        deviceDesc.deviceLostCallbackInfo.callback =
            [](WGPUDevice const*, WGPUDeviceLostReason reason, WGPUStringView message, void*, void*) {
                std::cerr << "[WebGPU] Device lost [reason=" << static_cast<int>(reason) << "]: "
                          << std::string(message.data, message.length) << "\n";
            };

        WGPURequestDeviceCallbackInfo callbackInfo = WGPU_REQUEST_DEVICE_CALLBACK_INFO_INIT;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                                   WGPUStringView message, void* userdata1, void*) {
            auto* req = static_cast<DeviceRequest*>(userdata1);
            req->status = status;
            req->device = device;
            if (status != WGPURequestDeviceStatus_Success) {
                std::cerr << "[WebGPU] RequestDevice failed: "
                          << std::string(message.data, message.length) << "\n";
            }
        };
        callbackInfo.userdata1 = &deviceReq;

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

Result<Ref<Buffer>> WebGPUDevice::createBuffer(const BufferDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU Buffer not yet implemented."});
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

Result<Ref<ShaderModule>> WebGPUDevice::createShaderModule(const ShaderModuleDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU ShaderModule not yet implemented."});
}

Result<Ref<PipelineLayout>> WebGPUDevice::createPipelineLayout(const PipelineLayoutDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU PipelineLayout not yet implemented."});
}

Result<Ref<RenderPipeline>> WebGPUDevice::createRenderPipeline(const RenderPipelineDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU RenderPipeline not yet implemented."});
}

VoidResult WebGPUDevice::writeBuffer(Buffer*, u64, const void*, u64) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU writeBuffer not yet implemented."});
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
