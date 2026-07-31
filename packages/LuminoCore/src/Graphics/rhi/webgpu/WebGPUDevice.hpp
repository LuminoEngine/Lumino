#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

#include <webgpu/webgpu.h>
#include <string>
#include <unordered_map>

namespace ln::rhi::webgpu {

// ------ WebGPUDevice ----------------------------------------------------------------------------------------------------------------

class WebGPUDevice final : public Device {
public:
    WebGPUDevice();

    /** ブロッキング初期化。initAsyncBegin + pumpAsyncInit の完了待ちループ。
        Web では ASYNCIFY (emscripten_sleep) を使うため、ASYNCIFY 済みの
        エントリポイント (LNInstance_Initialize) からのみ呼び出せる。 */
    VoidResult init(const DeviceDesc& desc);

    /** 非ブロッキング初期化を開始する (インスタンス作成 + アダプタ要求)。
        以後 pumpAsyncInit() で進行させる。デバイスロスト自動復旧用。 */
    VoidResult initAsyncBegin(const DeviceDesc& desc);

    /** 非ブロッキング初期化を 1 ステップ進める。 */
    AsyncInitStatus pumpAsyncInit() override;

    bool isValid() const { return m_device != nullptr; }

    // RHI interface
    DeviceLimits deviceLimits() const override;
    Result<Ref<SwapChain>> createSwapChain(const SwapChainDesc& desc) override;
    Result<Ref<Buffer>> createBuffer(const BufferDesc& desc) override;
    Result<Ref<Texture>> createTexture(const TextureDesc& desc) override;
    Result<Ref<TextureView>> createTextureView(Texture* texture) override;
    Result<Ref<Sampler>> createSampler(const SamplerDesc& desc) override;
    Result<Ref<ShaderModule>> createShaderModule(const ShaderModuleDesc& desc) override;
    Result<Ref<PipelineLayout>> createPipelineLayout(const PipelineLayoutDesc& desc) override;
    Result<Ref<RenderPipeline>> createRenderPipeline(const RenderPipelineDesc& desc) override;
    VoidResult writeBuffer(Buffer* dst, uint64_t dstOffset, const void* data, uint64_t size) override;
    Result<std::vector<uint8_t>> readbackTexture(TextureView* view) override;
    void waitIdle() override;
    Backend backend() const override { return Backend::WebGPU; }
    void debugSimulateDeviceLost(bool deep) override;

    // Internal accessors
    WGPUInstance wgpuInstance() const { return m_instance; }
    WGPUAdapter wgpuAdapter() const { return m_adapter; }
    WGPUDevice wgpuDevice() const { return m_device; }
    WGPUQueue wgpuQueue() const { return m_queue; }

    /** デバイスロストコールバックから参照する。擬似ロスト (deep) 中は
        Destroyed 理由のコールバックもロスト扱いにするためのフラグ。 */
    bool isSimulatingDeviceLost() const { return m_simulatingDeviceLost; }

private:
    struct WebGPUAdapterEntry {
        WGPURequestAdapterStatus status;
        WGPUAdapter adapter;
    };

    /** 非ブロッキング初期化の進行フェーズ。 */
    enum class InitPhase {
        NotStarted,
        WaitingAdapter,
        WaitingDevice,
        Ready,
        Failed,
    };

    /** デバイス要求コールバックの結果格納先。非同期完了に備えメンバで保持する。 */
    struct DeviceRequest {
        WGPUDevice device = nullptr;
        WGPURequestDeviceStatus status = {};
        bool done = false;
    };

    void finalize() override;

    /** アダプタ確定後にデバイスを要求する (pumpAsyncInit の一部)。 */
    void requestDeviceFromAdapter();

    /**
     * キャプチャされなかったエラーを (間引きつつ) ログ出力する。
     *
     * 無効なパイプラインは描画のたびに同じエラーを報告するため、そのまま出力すると
     * 毎フレーム同じ行が流れて最初の原因がログから埋もれてしまう。
     * そのため同一メッセージは先頭 kMaxLogPerMessage 件までに制限する。
     */
    void logUncapturedError(WGPUErrorType type, const std::string& message);

    /** 同一メッセージをログ出力する上限。これを超えた分は抑制する。 */
    static constexpr uint32_t kMaxLogPerMessage = 3;

    /** 種類の異なるメッセージを記録する上限 (メモリの無制限な増加を防ぐ)。 */
    static constexpr size_t kMaxTrackedMessages = 64;

    WGPUInstance m_instance = nullptr;
    WGPUAdapter  m_adapter  = nullptr;
    WGPUDevice   m_device   = nullptr;
    WGPUQueue    m_queue    = nullptr;
    std::vector<WebGPUAdapterEntry> m_adapters;
    InitPhase m_initPhase = InitPhase::NotStarted;
    DeviceRequest m_deviceReq;
    bool m_simulatingDeviceLost = false;

    /** キャプチャされなかったエラーのメッセージごとの発生回数。 */
    std::unordered_map<std::string, uint32_t> m_uncapturedErrorCounts;

#ifdef _WIN32
    HMODULE m_hD3DCompilerDLL = nullptr;
#endif
};

} // namespace ln::rhi::webgpu
