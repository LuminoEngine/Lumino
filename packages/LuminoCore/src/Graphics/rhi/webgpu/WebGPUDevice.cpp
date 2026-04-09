#include <iostream>
#include <dawn/native/DawnNative.h>
#include <dawn/dawn_proc_table.h>

#include "WebGPUDevice.hpp"

namespace ln::rhi::webgpu {

// ------ WebGPUDevice ----------------------------------------------------------------------------------------------------------------

WebGPUDevice::WebGPUDevice() = default;

VoidResult WebGPUDevice::init(const DeviceDesc& desc) {
    // Dawn の C API はデフォルトで関数ポインタ経由のディスパッチを使う。
    // スタティックリンク時はネイティブの proc テーブルを明示的に登録する必要がある。
    //dawnProcSetProcs(&dawn::native::GetProcs());

    // WebGPU インスタンスを生成する
    {
        // https://qiita.com/lriki/items/1ecdf377a6b2b57ba331
        WGPUInstanceFeatureName features[] = {
            WGPUInstanceFeatureName_TimedWaitAny
        };
        WGPUInstanceLimits limits = {};
        limits.nextInChain = nullptr;
        limits.timedWaitAnyMaxCount = 8;

        WGPUInstanceDescriptor desc = {};
        desc.nextInChain = nullptr;
        desc.requiredFeatureCount = sizeof(features) / sizeof(features[0]);
        desc.requiredFeatures = features;
        desc.requiredLimits = &limits;
        m_instance = wgpuCreateInstance(&desc);
        if (!m_instance) {
            return LN_MAKE_ERROR("wgpuCreateInstance failed.");
        }
    }

    
    // WGPURequestAdapterOptions adapterOptions = {};
    //adapterOptions.nextInChain = nullptr;
    ////adapterOptions.featureLevel = WGPUFeatureLevel_Compatibility;
    //adapterOptions.compatibleSurface = nullptr;
    //adapterOptions.powerPreference = WGPUPowerPreference_Undefined;
    //adapterOptions.forceFallbackAdapter = false;
    //auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status,
    //                            WGPUAdapter adapter,
    //                            char const* message,
    //                            void* userdata) {
    //    WebGPUDevice* self = reinterpret_cast<WebGPUDevice*>(userdata);
    //    //self->m_adapters.push_back({status, adapter});
    //};
    //wgpuInstanceRequestAdapter(
    //    m_instance,
    //    &adapterOptions,
    //    onAdapterRequestEnded,
    //    this);

 #if 0
    // アダプターをリクエストする
    // Dawn のネイティブ実装ではコールバックは同期的に呼ばれるため、
    // 呼び出し直後に結果を参照できる。
    struct AdapterRequest {
        WGPUAdapter adapter = nullptr;
        WGPURequestAdapterStatus status = WGPURequestAdapterStatus_Unknown;
    } adapterReq;

    WGPURequestAdapterOptions adapterOptions = {};
    adapterOptions.powerPreference = WGPUPowerPreference_HighPerformance;

    wgpuInstanceRequestAdapter(
        m_instance,
        &adapterOptions,
        [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* userdata) {
            auto* req = static_cast<AdapterRequest*>(userdata);
            req->status = status;
            req->adapter = adapter;
            if (status != WGPURequestAdapterStatus_Success) {
                std::cerr << "[WebGPU] RequestAdapter failed: "
                          << (message ? message : "(no message)") << "\n";
            }
        },
        &adapterReq);

    if (adapterReq.status != WGPURequestAdapterStatus_Success || !adapterReq.adapter) {
        wgpuInstanceRelease(m_instance);
        m_instance = nullptr;
        return LN_MAKE_ERROR("wgpuInstanceRequestAdapter failed.");
    }
    m_adapter = adapterReq.adapter;

    // アダプター情報をログ出力する
    WGPUAdapterProperties props = {};
    wgpuAdapterGetProperties(m_adapter, &props);
    std::cout << "[WebGPU] Adapter: " << (props.name ? props.name : "unknown")
              << " / " << (props.driverDescription ? props.driverDescription : "")
              << "\n";

    // デバイスをリクエストする
    struct DeviceRequest {
        WGPUDevice device = nullptr;
        WGPURequestDeviceStatus status = WGPURequestDeviceStatus_Unknown;
    } deviceReq;

    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.label = "LuminoDevice";

    wgpuAdapterRequestDevice(
        m_adapter,
        &deviceDesc,
        [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* userdata) {
            auto* req = static_cast<DeviceRequest*>(userdata);
            req->status = status;
            req->device = device;
            if (status != WGPURequestDeviceStatus_Success) {
                std::cerr << "[WebGPU] RequestDevice failed: "
                          << (message ? message : "(no message)") << "\n";
            }
        },
        &deviceReq);

    if (deviceReq.status != WGPURequestDeviceStatus_Success || !deviceReq.device) {
        wgpuAdapterRelease(m_adapter);
        m_adapter = nullptr;
        wgpuInstanceRelease(m_instance);
        m_instance = nullptr;
        return LN_MAKE_ERROR("wgpuAdapterRequestDevice failed.");
    }
    m_device = deviceReq.device;

    // デバイスエラーコールバックを登録する
    wgpuDeviceSetUncapturedErrorCallback(
        m_device,
        [](WGPUErrorType type, char const* message, void*) {
            std::cerr << "[WebGPU] Uncaptured error [type=" << static_cast<int>(type) << "]: "
                      << (message ? message : "(no message)") << "\n";
#if defined(_MSC_VER) && defined(_DEBUG)
            __debugbreak();
#endif
        },
        nullptr);

    // デフォルトキューを取得する
    m_queue = wgpuDeviceGetQueue(m_device);
    if (!m_queue) {
        return LN_MAKE_ERROR("wgpuDeviceGetQueue failed.");
    }

    std::cout << "[WebGPU] Device initialized successfully.\n";
#endif
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

Result<Ref<SwapChain>> WebGPUDevice::createSwapChain(const SwapChainDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU SwapChain not yet implemented."});
}

Result<Ref<Buffer>> WebGPUDevice::createBuffer(const BufferDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU Buffer not yet implemented."});
}

Result<Ref<Texture>> WebGPUDevice::createTexture(const TextureDesc&) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU Texture not yet implemented."});
}

Result<Ref<TextureView>> WebGPUDevice::createTextureView(Texture*) {
    return tl::unexpected(Error{ErrorCode::NotSupported, "WebGPU TextureView not yet implemented."});
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
