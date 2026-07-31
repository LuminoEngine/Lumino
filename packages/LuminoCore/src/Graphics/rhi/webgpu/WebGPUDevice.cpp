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
#include <cstring>

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

    auto beginResult = initAsyncBegin(desc);
    if (!beginResult) {
        return LN_FORWARD_ERROR(beginResult);
    }

    // 完了までポンプする。
    for (;;) {
        AsyncInitStatus st = pumpAsyncInit();
        if (st == AsyncInitStatus::Ready) {
            LN_LOG_INFO("WebGPUDevice::init: end (success)");
            return LN_MAKE_SUCCESS();
        }
        if (st == AsyncInitStatus::Failed) {
            return LN_MAKE_ERROR("WebGPUDevice::init failed.");
        }
#if defined(__EMSCRIPTEN__)
        // ブラウザではコールバックがイベントループ経由で解決されるため、
        // ASYNCIFY で制御を返しながら待つ。このため本関数は ASYNCIFY 済みの
        // エントリポイント (LNInstance_Initialize) からのみ呼び出せる。
        emscripten_sleep(10);
#endif
    }
}

VoidResult WebGPUDevice::initAsyncBegin(const DeviceDesc& desc) {
    (void)desc;

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
        WGPUInstanceDescriptor instDesc = WGPU_INSTANCE_DESCRIPTOR_INIT;
        instDesc.nextInChain = nullptr;
        instDesc.capabilities.timedWaitAnyEnable = 1;
        instDesc.capabilities.timedWaitAnyMaxCount = 8;
        m_instance = wgpuCreateInstance(&instDesc);
#endif
        if (!m_instance) {
            m_initPhase = InitPhase::Failed;
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
    // ネイティブ Dawn の WGPUCallbackMode_AllowSpontaneous モードではコールバックは
    // 同期的に呼ばれるため、呼び出し直後に結果を参照できる。
    // ブラウザではイベントループ経由で解決されるため、pumpAsyncInit で完了を確認する。
    {
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
            if (status != WGPURequestAdapterStatus_Success) {
                LN_LOG_ERROR("[WebGPU] RequestAdapter failed: %.*s", (int)message.length, message.data);
            }
        };
        callbackInfo.userdata1 = this;

        WGPUFuture _ = wgpuInstanceRequestAdapter(m_instance, &adapterOptions, callbackInfo);
        (void)_;
    }

    m_initPhase = InitPhase::WaitingAdapter;
    return LN_MAKE_SUCCESS();
}

Device::AsyncInitStatus WebGPUDevice::pumpAsyncInit() {
    if (m_initPhase == InitPhase::Ready) {
        return AsyncInitStatus::Ready;
    }
    if (m_initPhase == InitPhase::Failed || m_initPhase == InitPhase::NotStarted) {
        return AsyncInitStatus::Failed;
    }

#if !defined(__EMSCRIPTEN__)
    // ネイティブでは明示的にイベントを処理してコールバックを発火させる。
    // (AllowSpontaneous では通常同期的に発火するため、これは保険である)
    if (m_instance) {
        wgpuInstanceProcessEvents(m_instance);
    }
#endif

    if (m_initPhase == InitPhase::WaitingAdapter) {
        if (m_adapters.empty()) {
            return AsyncInitStatus::Pending;
        }
        if (m_adapters[0].status != WGPURequestAdapterStatus_Success || !m_adapters[0].adapter) {
            LN_LOG_ERROR("WebGPUDevice: adapter request failed.");
            m_initPhase = InitPhase::Failed;
            return AsyncInitStatus::Failed;
        }
        m_adapter = m_adapters[0].adapter;

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

        requestDeviceFromAdapter();
        m_initPhase = InitPhase::WaitingDevice;
    }

    if (m_initPhase == InitPhase::WaitingDevice) {
        if (!m_deviceReq.done) {
            return AsyncInitStatus::Pending;
        }
        if (m_deviceReq.status != WGPURequestDeviceStatus_Success || !m_deviceReq.device) {
            LN_LOG_ERROR("wgpuAdapterRequestDevice failed.");
            m_initPhase = InitPhase::Failed;
            return AsyncInitStatus::Failed;
        }
        m_device = m_deviceReq.device;

        // デフォルトキューを取得する
        m_queue = wgpuDeviceGetQueue(m_device);
        if (!m_queue) {
            LN_LOG_ERROR("wgpuDeviceGetQueue failed.");
            m_initPhase = InitPhase::Failed;
            return AsyncInitStatus::Failed;
        }

        m_initPhase = InitPhase::Ready;
        return AsyncInitStatus::Ready;
    }

    return AsyncInitStatus::Pending;
}

void WebGPUDevice::requestDeviceFromAdapter() {
    m_deviceReq = DeviceRequest{};

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
    // 同じエラーが毎フレーム報告されてもログが埋もれないよう、
    // 出力は logUncapturedError で間引く。
    auto onUncapturedError =
        [](WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void* userdata1, void*) {
            auto* self = reinterpret_cast<WebGPUDevice*>(userdata1);
            // WGPUStringView の length は WGPU_STRLEN のとき「NUL 終端」を意味する。
            const size_t length = !message.data ? 0
                : (message.length == WGPU_STRLEN ? strlen(message.data) : message.length);
            std::string text(message.data ? message.data : "", length);
            if (self) {
                self->logUncapturedError(type, text);
            }
            else {
                LN_LOG_ERROR("[WebGPU] Uncaptured error [type=%d]: %s",
                    static_cast<int>(type), text.c_str());
            }
        };
    deviceDesc.uncapturedErrorCallbackInfo.callback = onUncapturedError;
    deviceDesc.uncapturedErrorCallbackInfo.userdata1 = this;

    // デバイスロストコールバック
    // mode を AllowSpontaneous にしないとコールバックが EventManager にキューイングされ、
    // Instance 破棄時に CallbackCancelled に書き換えられてしまう。
    // AllowSpontaneous では他の wgpu API 呼び出し中に発火しうるため、
    // コールバック内はフラグを立てる (markDeviceLost) 以外の処理を行ってはならない。
    deviceDesc.deviceLostCallbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceDesc.deviceLostCallbackInfo.userdata1 = this;
    deviceDesc.deviceLostCallbackInfo.callback =
        [](WGPUDevice const*, WGPUDeviceLostReason reason, WGPUStringView message, void* userdata1, void*) {
            auto* self = reinterpret_cast<WebGPUDevice*>(userdata1);
            if (reason == WGPUDeviceLostReason_Destroyed &&
                !(self && self->isSimulatingDeviceLost())) {
                return; // 正常な破棄時は無視する (擬似ロスト中の destroy はロスト扱い)
            }
            LN_LOG_ERROR(
                "[WebGPU] Device lost [reason=%d]: %s",
                static_cast<int>(reason),
                std::string(message.data, message.length).c_str());
            if (self) {
                self->markDeviceLost("wgpu device lost callback");
            }
        };

    auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status,
                                   WGPUDevice device,
                                   WGPUStringView message,
                                   void* userdata1,
                                   void*) {
        auto* self = reinterpret_cast<WebGPUDevice*>(userdata1);
        self->m_deviceReq.status = status;
        self->m_deviceReq.device = device;
        self->m_deviceReq.done = true;
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
    callbackInfo.userdata1 = this;
    callbackInfo.userdata2 = nullptr;
    WGPUFuture _ = wgpuAdapterRequestDevice(m_adapter, &deviceDesc, callbackInfo);
    (void)_;
}

void WebGPUDevice::logUncapturedError(WGPUErrorType type, const std::string& message) {
    // 無効なシェーダやパイプラインは描画のたびに同じエラーを報告する。
    // 全部出すと最初の (本当の原因を含む) エラーが流れてしまうため、
    // 同一メッセージは先頭数件だけ出力する。
    uint32_t count = 0;
    if (m_uncapturedErrorCounts.size() < kMaxTrackedMessages) {
        count = ++m_uncapturedErrorCounts[message];
    }
    else {
        auto it = m_uncapturedErrorCounts.find(message);
        if (it != m_uncapturedErrorCounts.end()) {
            count = ++it->second;
        }
        else {
            // 記録枠を使い切っている場合は抑制せずに出力する
            // (メッセージが多様すぎる異常時に、逆に隠してしまわないようにする)。
            count = 1;
        }
    }

    if (count > kMaxLogPerMessage) {
        return;
    }

    LN_LOG_ERROR("[WebGPU] Uncaptured error [type=%d]: %s",
        static_cast<int>(type), message.c_str());

    if (count == kMaxLogPerMessage) {
        LN_LOG_ERROR(
            "[WebGPU] The error above has been reported %u times; "
            "further occurrences of the same message are suppressed.",
            count);
    }

#if defined(_MSC_VER) && defined(_DEBUG)
    // 毎フレーム break すると操作できなくなるため、抑制されない間だけ break する。
    __debugbreak();
#endif
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

Result<std::vector<uint8_t>> WebGPUDevice::readbackTexture(TextureView* view) {
    auto* wgpuView = static_cast<WebGPUTextureView*>(view);
    if (!wgpuView) {
        return LN_MAKE_ERROR("Invalid TextureView for readback.");
    }
    WGPUTexture srcTexture = wgpuView->sourceTexture();
    if (!srcTexture) {
        return LN_MAKE_ERROR("TextureView has no source WGPUTexture for readback.");
    }

    const uint32_t width = wgpuView->width();
    const uint32_t height = wgpuView->height();
    // バックバッファは BGRA8/RGBA8 を想定 (4 bytes/pixel)。
    // 返すデータはスワップチェーンの生バイト列で、RGBA への並べ替えは
    // GraphicsContext 側で行われる (Vulkan 実装と同じ規約)。
    const uint32_t bytesPerPixel = 4;

    // WebGPU の texture->buffer コピーは bytesPerRow が 256 の倍数である必要がある。
    const uint32_t unpaddedBytesPerRow = width * bytesPerPixel;
    const uint32_t kAlign = 256;
    const uint32_t paddedBytesPerRow = (unpaddedBytesPerRow + kAlign - 1) / kAlign * kAlign;
    const uint64_t bufferSize = static_cast<uint64_t>(paddedBytesPerRow) * height;

    // CPU から読み出せるステージングバッファを作成する。
    WGPUBufferDescriptor bufDesc = WGPU_BUFFER_DESCRIPTOR_INIT;
    bufDesc.size = bufferSize;
    bufDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
    bufDesc.mappedAtCreation = false;
    WGPUBuffer staging = wgpuDeviceCreateBuffer(m_device, &bufDesc);
    if (!staging) {
        return LN_MAKE_ERROR("Failed to create readback staging buffer.");
    }

    // テクスチャ → バッファのコピーをエンコードして送信する。
    WGPUCommandEncoderDescriptor encDesc = WGPU_COMMAND_ENCODER_DESCRIPTOR_INIT;
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(m_device, &encDesc);

    WGPUTexelCopyTextureInfo src = WGPU_TEXEL_COPY_TEXTURE_INFO_INIT;
    src.texture = srcTexture;
    src.mipLevel = 0;
    src.origin = {0, 0, 0};
    src.aspect = WGPUTextureAspect_All;

    WGPUTexelCopyBufferInfo dst = WGPU_TEXEL_COPY_BUFFER_INFO_INIT;
    dst.buffer = staging;
    dst.layout.offset = 0;
    dst.layout.bytesPerRow = paddedBytesPerRow;
    dst.layout.rowsPerImage = height;

    WGPUExtent3D extent = {width, height, 1};
    wgpuCommandEncoderCopyTextureToBuffer(encoder, &src, &dst, &extent);

    WGPUCommandBufferDescriptor cmdDesc = WGPU_COMMAND_BUFFER_DESCRIPTOR_INIT;
    WGPUCommandBuffer cmd = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    wgpuQueueSubmit(m_queue, 1, &cmd);
    wgpuCommandBufferRelease(cmd);
    wgpuCommandEncoderRelease(encoder);

#if defined(__EMSCRIPTEN__)
    // Web 上ではマップ完了を同期的に待てないため未対応。
    wgpuBufferRelease(staging);
    return LN_MAKE_ERROR("readbackTexture is not supported on Emscripten.");
#else
    // ステージングバッファをマップし、コールバックが発火するまでデバイスを回す。
    struct MapState {
        bool done = false;
        WGPUMapAsyncStatus status = WGPUMapAsyncStatus_Force32;
    } mapState;

    WGPUBufferMapCallbackInfo mapCbInfo = WGPU_BUFFER_MAP_CALLBACK_INFO_INIT;
    mapCbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    mapCbInfo.callback = [](WGPUMapAsyncStatus status, WGPUStringView message,
                            void* userdata1, void*) {
        auto* st = static_cast<MapState*>(userdata1);
        st->status = status;
        st->done = true;
        if (status != WGPUMapAsyncStatus_Success) {
            LN_LOG_ERROR("[WebGPU] Buffer map for readback failed: %.*s",
                         static_cast<int>(message.length), message.data);
        }
    };
    mapCbInfo.userdata1 = &mapState;
    wgpuBufferMapAsync(staging, WGPUMapMode_Read, 0, bufferSize, mapCbInfo);

    while (!mapState.done) {
        wgpuDeviceTick(m_device);
    }

    if (mapState.status != WGPUMapAsyncStatus_Success) {
        wgpuBufferRelease(staging);
        return LN_MAKE_ERROR("wgpuBufferMapAsync failed.");
    }

    const uint8_t* mapped = static_cast<const uint8_t*>(
        wgpuBufferGetConstMappedRange(staging, 0, bufferSize));
    if (!mapped) {
        wgpuBufferUnmap(staging);
        wgpuBufferRelease(staging);
        return LN_MAKE_ERROR("wgpuBufferGetConstMappedRange returned null.");
    }

    // 行パディングを取り除きながらコピーする。
    std::vector<uint8_t> pixels(static_cast<size_t>(unpaddedBytesPerRow) * height);
    for (uint32_t y = 0; y < height; ++y) {
        std::memcpy(pixels.data() + static_cast<size_t>(y) * unpaddedBytesPerRow,
                    mapped + static_cast<size_t>(y) * paddedBytesPerRow,
                    unpaddedBytesPerRow);
    }

    wgpuBufferUnmap(staging);
    wgpuBufferRelease(staging);
    return pixels;
#endif
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

void WebGPUDevice::debugSimulateDeviceLost(bool deep) {
    m_simulatingDeviceLost = true;
    markDeviceLost("simulated");
    if (deep && m_device) {
        // 実際にデバイスを破棄し、後続 API 呼び出しのエラー挙動まで再現する。
        // Destroyed 理由のロストコールバックが発火するが、m_simulatingDeviceLost が
        // 立っているためロスト扱いとして処理される。
        wgpuDeviceDestroy(m_device);
    }
}

} // namespace ln::rhi::webgpu
