#include "WebGPUDevice.hpp"
#include "WebGPUSwapChain.hpp"
#include "WebGPUTexture.hpp"
#include "WebGPUTextureView.hpp"
#include "WebGPUBuffer.hpp"
#include "WebGPUShaderModule.hpp"
#include "WebGPUPipelineLayout.hpp"
#include "WebGPUSampler.hpp"
#include "WebGPURenderPipeline.hpp"
#include "WebGPUHelpers.hpp"
#include <LuminoBase/Logger.hpp>

// TODO: あとで LuminoCore を wasm へリンクするように修正する。
// そうすればこれは pch に移動できるはず。
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

namespace ln::rhi::webgpu {

// ------ WebGPUDevice ----------------------------------------------------------------------------------------------------------------

WebGPUDevice::WebGPUDevice() = default;

VoidResult WebGPUDevice::init(const DeviceDesc& desc) {
    LN_LOG_INFO("WebGPUDevice::init: begin");

    // WebGPU インスタンスを生成する
    {
#if LN_WEBGPU_DAWN_LATEST
        WGPUInstanceFeatureName features[] = {WGPUInstanceFeatureName_TimedWaitAny};
        WGPUInstanceLimits limits = {};
        limits.nextInChain = nullptr;
        limits.timedWaitAnyMaxCount = 8;

        WGPUInstanceDescriptor instDesc = {};
        instDesc.nextInChain = nullptr;
        instDesc.requiredFeatureCount = sizeof(features) / sizeof(features[0]);
        instDesc.requiredFeatures = features;
        instDesc.requiredLimits = &limits;
        m_instance = wgpuCreateInstance(&instDesc);
#else
        WGPUInstanceDescriptor desc = WGPU_INSTANCE_DESCRIPTOR_INIT;
        desc.nextInChain = nullptr;
        desc.capabilities.timedWaitAnyEnable = 1;
        desc.capabilities.timedWaitAnyMaxCount = 8;
        m_instance = wgpuCreateInstance(&desc);
#endif
        if (!m_instance) {
            return LN_MAKE_ERROR("wgpuCreateInstance failed.");
        }
    }

#ifdef _WIN32
    // Windows 環境の場合、D3DCompiler DLL をロードしておく。
    // 先にこうしておかないと、wgpuInstanceRequestAdapter で次のようなエラーが発生することがあった。
    // - Error: DynamicLib.Open: d3dcompiler_47.dll Windows Error: 87
    // - [WebGPU] RequestDevice failed: DynamicLib.Open: dxil.dll Windows Error: 87
    m_hD3DCompilerDLL = ::LoadLibraryW(D3DCOMPILER_DLL_W);

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
        // 2026/4/16時点では WGPUPowerPreference_HighPerformance を指定すると Chrome で次のように Warning するので Undefined にしておく。
        // The powerPreference option is currently ignored when calling requestAdapter() on Windows. See https://crbug.com/369219127
        adapterOptions.powerPreference = WGPUPowerPreference_Undefined;
        adapterOptions.forceFallbackAdapter = false;
        adapterOptions.backendType = WGPUBackendType_Undefined; // デバッグとかで必要なら WGPUBackendType_Vulkan とかにしてみよう。
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
                LN_LOG_ERROR("[WebGPU] RequestAdapter failed: %.*s", (int)message.length, message.data);
            }
        };
        callbackInfo.userdata1 = this;

        WGPUFuture _ = wgpuInstanceRequestAdapter(m_instance, &adapterOptions, callbackInfo);

        //if (adapterReq.status != WGPURequestAdapterStatus_Success || !adapterReq.adapter) {
        //    return LN_MAKE_ERROR("wgpuInstanceRequestAdapter failed.");
        //}

        // Wait WGPUFuture
#ifdef __EMSCRIPTEN__
        while (m_adapters.empty()) {
            emscripten_sleep(100);
        }
#endif // __EMSCRIPTEN__

        if (m_adapters.empty()) {
            return LN_MAKE_ERROR("Adapter not found.");
        }

        //printf("m_adapters %d\n", (int)m_adapters.size());
        m_adapter = m_adapters[0].adapter; //adapterReq.adapter;
    }

    // アダプター情報をログ出力する
    {
        WGPUAdapterInfo info = WGPU_ADAPTER_INFO_INIT;
        WGPUStatus status = wgpuAdapterGetInfo(m_adapter, &info);
        if (status != WGPUStatus_Success) {
            LN_LOG_ERROR("wgpuAdapterGetInfo failed: %d", static_cast<int>(status));
        }
        LN_LOG_INFO(
            "WebGPU Adapter: %s / %s / %s",
            std::string(info.device.data, info.device.length).c_str(),
            std::string(info.vendor.data, info.vendor.length).c_str(),
            std::string(info.description.data, info.description.length).c_str());
    }

    // デバイスをリクエストする
    {
        struct DeviceRequest {
            WGPUDevice device = nullptr;
            WGPURequestDeviceStatus status = {};
        } deviceReq;

        WGPUDeviceDescriptor deviceDesc = WGPU_DEVICE_DESCRIPTOR_INIT;
        deviceDesc.label = {"LuminoDevice", WGPU_STRLEN};

        // RGBA32Float 等の float32 テクスチャをフィルタリング可能にする feature を要求する。
        // これがないと float32 テクスチャのバインドグループレイアウトで
        // UnfilterableFloat vs Float のバリデーションエラーが発生する。
        // もしこれが使えない場合は TextureFormt で WGPUTextureSampleType_UnfilterableFloat を検討する必要があるかも。
        std::vector<WGPUFeatureName> requiredFeatures;
        if (wgpuAdapterHasFeature(m_adapter, WGPUFeatureName_Float32Filterable)) {
            requiredFeatures.push_back(WGPUFeatureName_Float32Filterable);
            LN_LOG_INFO("WebGPUDevice: enabling Float32Filterable feature.");
        } else {
            LN_LOG_WARNING("WebGPUDevice: Float32Filterable not supported by adapter.");
        }
        if (!requiredFeatures.empty()) {
            deviceDesc.requiredFeatureCount = requiredFeatures.size();
            deviceDesc.requiredFeatures = requiredFeatures.data();
        }

        // エラーコールバック
        auto onUncapturedError =
            [](WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void*, void*) {
            LN_LOG_ERROR("[WebGPU] Uncaptured error [type=%d]: %s", static_cast<int>(type),
                std::string(message.data, message.length).c_str());
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
                LN_LOG_ERROR(
                    "[WebGPU] Device lost [reason=%d]: %s",
                    static_cast<int>(reason),
                    std::string(message.data, message.length).c_str());
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
                LN_LOG_ERROR(
                    "[WebGPU] RequestDevice failed: %s",
                    std::string(message.data, message.length).c_str());
            }
        };

        WGPURequestDeviceCallbackInfo callbackInfo = WGPU_REQUEST_DEVICE_CALLBACK_INFO_INIT;
        callbackInfo.nextInChain = nullptr;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.callback = onDeviceRequestEnded;
        //callbackInfo.userdata1 = this;
        callbackInfo.userdata1 = &deviceReq;
        callbackInfo.userdata2 = nullptr;
        WGPUFuture _ = wgpuAdapterRequestDevice(m_adapter, &deviceDesc, callbackInfo);

        // Wait WGPUFuture
#ifdef __EMSCRIPTEN__
        while (deviceReq.device == nullptr) {
            emscripten_sleep(100);
        }
#endif // __EMSCRIPTEN__

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

    LN_LOG_INFO("WebGPUDevice::init: end (success)");
    return LN_MAKE_SUCCESS();
}

void WebGPUDevice::finalize() {
    LN_LOG_INFO("WebGPUDevice::finalize: begin");
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
    LN_LOG_INFO("WebGPUDevice::finalize: end");
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
        return LN_FORWARD_ERROR(result);
    }
    return Ref<SwapChain>(sc);
}

Result<Ref<Buffer>> WebGPUDevice::createBuffer(const BufferDesc& desc) {
    auto buf = Ref<WebGPUBuffer>::adopt(new WebGPUBuffer());
    auto result = buf->init(this, desc);
    if (!result) {
        return LN_FORWARD_ERROR(result);
    }
    return Ref<Buffer>(buf);
}

Result<Ref<Texture>> WebGPUDevice::createTexture(const TextureDesc& desc) {
    auto tex = Ref<WebGPUTexture>::adopt(new WebGPUTexture());
    auto result = tex->init(this, desc);
    if (!result) {
        return LN_FORWARD_ERROR(result);
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
        return LN_FORWARD_ERROR(result);
    }
    return Ref<TextureView>(view);
}

Result<Ref<Sampler>> WebGPUDevice::createSampler(const SamplerDesc& desc) {
    auto s = Ref<WebGPUSampler>::adopt(new WebGPUSampler());
    auto result = s->init(this, desc);
    if (!result) {
        return LN_FORWARD_ERROR(result);
    }
    return Ref<Sampler>(s);
}

Result<Ref<ShaderModule>> WebGPUDevice::createShaderModule(const ShaderModuleDesc& desc) {
    auto sm = Ref<WebGPUShaderModule>::adopt(new WebGPUShaderModule());
    auto result = sm->init(this, desc);
    if (!result) {
        return LN_FORWARD_ERROR(result);
    }
    return Ref<ShaderModule>(sm);
}

Result<Ref<PipelineLayout>> WebGPUDevice::createPipelineLayout(const PipelineLayoutDesc& desc) {
    auto pl = Ref<WebGPUPipelineLayout>::adopt(new WebGPUPipelineLayout());
    auto result = pl->init(this, desc);
    if (!result) {
        return LN_FORWARD_ERROR(result);
    }
    return Ref<PipelineLayout>(pl);
}

Result<Ref<RenderPipeline>> WebGPUDevice::createRenderPipeline(const RenderPipelineDesc& desc) {
    auto rp = Ref<WebGPURenderPipeline>::adopt(new WebGPURenderPipeline());
    auto result = rp->init(this, desc);
    if (!result) {
        return LN_FORWARD_ERROR(result);
    }
    return Ref<RenderPipeline>(rp);
}

VoidResult WebGPUDevice::writeBuffer(Buffer* dst, uint64_t dstOffset, const void* data, uint64_t size) {
    auto* wgpuBuf = static_cast<WebGPUBuffer*>(dst);
    wgpuQueueWriteBuffer(m_queue, wgpuBuf->handle(), dstOffset, data, size);
    return LN_MAKE_SUCCESS();
}

Result<std::vector<uint8_t>> WebGPUDevice::readbackTexture(TextureView*) {
    return LN_MAKE_ERROR("WebGPU readbackTexture not yet implemented.");
}

void WebGPUDevice::waitIdle() {
#if defined(__EMSCRIPTEN__)
    // FIXME: wgpuDeviceTick は使えないので、もし Web 上でバックバッファをキャプチャしたいとかなったら別の手段を考える。
#else
    if (m_device) {
        wgpuDeviceTick(m_device);
    }
#endif
}

} // namespace ln::rhi::webgpu
