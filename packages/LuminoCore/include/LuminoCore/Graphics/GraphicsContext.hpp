#pragma once
#include <memory>
#include <chrono>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoCore/Graphics/PipelineCache.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#if !defined(__EMSCRIPTEN__)
#include <LuminoCore/Graphics/DebugPrint.hpp>
#endif
#include <LuminoCore/Platform/Window.hpp>

namespace ln {
class GraphicsModule;
class Texture;

/** GraphicsContext (スワップチェーン + 深度バッファ) を作成するための記述子。 */
struct GraphicsContextDesc {
    //rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8UnormSrgb;
    bool vsync = false;
};

/** GraphicsContext::beginFrame() が返す、フレームごとの描画ターゲット。 */
struct FramebufferInfo {
    Ref<Texture> colorTexture;
    Ref<Texture> depthTexture;
};

/**
 * RHI デバイス、スワップチェーン、深度バッファの管理をまとめたグラフィックスコンテキスト。
 *
 * PlatformWindow が作成し、PlatformWindow::graphicsContext() でアクセスします。
 *
 * 複数の作成経路を予定しています:
 *   - デスクトップウィンドウ: createForWindow() がウィンドウ用のデバイスとスワップチェーンを構築します。
 *   - ブラウザ WebGPU:        createFromCanvas() (将来)。
 *   - 外部への組み込み:       createExternal() (将来)。
 *
 * 作成後は、作成経路にかかわらずレンダリングコードは同一です。
 */
class GraphicsContext : public Object {
public:

    ~GraphicsContext() override;

    /** 既存のウィンドウ用のグラフィックスコンテキストを作成します。 */
    static Result<Ref<GraphicsContext>> createForWindow(
        GraphicsModule* module,
        platform::PlatformWindow* window,
        const GraphicsContextDesc& desc);

    /** HTML canvas からグラフィックスコンテキストを作成します (Web のみ)。 */
    static Result<Ref<GraphicsContext>> createForCanvas(
        GraphicsModule* module,
        const std::string& canvasSelector,
        uint32_t width,
        uint32_t height,
        const GraphicsContextDesc& desc);

    // 将来の作成経路
    // static Result<Ref<GraphicsContext>> createExternal(const ExternalContextDesc& desc);

    /** 次のバックバッファを取得し、フレームのターゲットを準備します。
     * width/height で現在の描画先サイズを指定します。
     * 前フレームと異なる場合、SwapChain と深度バッファを自動的にリサイズします。
     */
    Result<const FramebufferInfo*> beginFrame(uint32_t width, uint32_t height);
    const FramebufferInfo* currentFramebuffer() const;

    /**
     * デバイスロスト自動復旧: GPU リソース (SwapChain / 深度バッファ /
     * PipelineCache / Renderer / DebugPrint) を解放します。
     * GraphicsModule::pumpRecovery から呼ばれます。複数回呼んでも安全です。
     */
    void teardownForDeviceRecovery();

    /**
     * デバイスロスト自動復旧: 新しいデバイス上で GPU リソースを再構築します。
     * GraphicsModule::pumpRecovery から呼ばれます。
     */
    Result<void> rebuildAfterDeviceRecovery();

    /** 現在のフレームをプレゼントします。 */
    void endFrame();

    /**
     * 現在のフレームのバックバッファをキャプチャするよう要求します。
     * endFrame() の前に呼び出してください。
     *
     * スワップチェーンイメージは present() の後に acquire 解除されると
     * レイアウト遷移を含む一切の使用ができなくなる
     * (VUID UNASSIGNED-non-acquired-swapchain-image-used) ため、
     * 実際の読み戻しは present() 直前 (endFrame() 内) に行います。
     * この要求フラグが立っているフレームだけ読み戻すことで、
     * 毎フレームの読み戻しコストを回避します。
     */
    void requestCaptureBackbuffer();

    /**
     * requestCaptureBackbuffer() で要求し endFrame() で読み戻された
     * バックバッファ内容を確定させます。endFrame() の後に呼び出してください。
     * captureBuffer() で取得できるピクセルデータは、次のキャプチャまで有効です。
     * 事前に requestCaptureBackbuffer() を呼んでいない場合はエラーを返します。
     */
    Result<void> captureBackbuffer();

    /** 最後の captureBackbuffer() 呼び出しでキャプチャしたピクセルデータにアクセスします。 */
    const std::vector<uint8_t>& captureBuffer() const { return m_captureBuffer; }

    void waitIdle();

    /** RHI デバイスを所有する GraphicsModule。 */
    GraphicsModule* module() const { return m_module; }

    /** 下層の RHI デバイス (CoreInstance が所有)。 */
    rhi::Device* device() const;

    /** このコンテキストのパイプラインキャッシュ。 */
    PipelineCache* pipelineCache() const { return m_pipelineCache.get(); }

    /** スワップチェーンのカラーフォーマット。 */
    rhi::TextureFormat colorFormat() const { return m_colorFormat; }

    /** このコンテキストが管理するデプスフォーマット。 */
    rhi::TextureFormat depthFormat() const { return m_depthFormat; }

    /** 現在のフレームバッファの幅 (ピクセル)。 */
    uint32_t width() const { return m_width; }

    /** 現在のフレームバッファの高さ (ピクセル)。 */
    uint32_t height() const { return m_height; }

    /** インフライトフレーム数 (スワップチェーンから取得)。 */
    uint32_t maxFramesInFlight() const;

    rhi::CommandBuffer* currentCommandBuffer() const;

    /** このコンテキストが所有する Renderer。 */
    Renderer* renderer() const { return m_renderer.get(); }

#if !defined(__EMSCRIPTEN__)
    /** このコンテキストが所有するデバッグ表示ヘルパー。初回使用時に遅延生成されます。 */
    DebugPrint* debugPrint() { return m_debugPrint.get(); }
#endif

    /**
     * 現在のフレームのオーバーレイ描画用にデバッグ文字列をキューへ追加します。
     * DebugPrint の初期化前に呼び出しても安全です - 利用できない場合は無視されます。
     */
    void debugPrintText(const char* str);

    /** DebugPrint サブシステムを初期化 (または再初期化) します。 */
    Result<void> initDebugPrint();

    /** 直前に完了したフレームのフレーム時間 (ミリ秒)。 */
    float lastFrameTimeMs() const { return m_lastFrameTimeMs; }

    /** 秒間フレーム数 (直前に完了したフレームから算出)。 */
    float fps() const { return m_fps; }

    // フレームスコープの一時状態 (BeginFrame~EndFrame 間有効)
    rhi::CommandBuffer*        m_currentCmd         = nullptr;
    rhi::RenderPass*           m_currentPass        = nullptr;

private:

    GraphicsContext();

    // present() 直前に現在のバックバッファを m_captureBuffer へ読み戻す。
    Result<void> captureBackbufferInternal();

    GraphicsModule* m_module;
    platform::PlatformWindow* m_window = nullptr; // 非所有
    Ref<rhi::SwapChain> m_swapChain;
    std::vector<FramebufferInfo> m_framebuffers; // SwapChain Image (InFlightFrame) ごとに1つ

#if !defined(__EMSCRIPTEN__)
    Ref<DebugPrint> m_debugPrint;
#endif

    // フレーム時間の計測
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point m_frameBeginTime = {};
    bool              m_firstFrame     = true;
    float             m_lastFrameTimeMs = 0.0f;
    float             m_fps             = 0.0f;
    bool                 m_captureRequested = false; // 今フレームのキャプチャ要求
    bool                 m_captureValid     = false; // m_captureBuffer に有効な内容があるか
    std::vector<uint8_t> m_captureBuffer;
    rhi::TextureFormat m_colorFormat = rhi::TextureFormat::BGRA8Unorm;
    rhi::TextureFormat m_depthFormat = rhi::TextureFormat::Depth24Stencil8;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::unique_ptr<PipelineCache> m_pipelineCache;
    Ref<Renderer> m_renderer;

    // デバイスロスト復旧時に SwapChain を作り直すための生成情報
    std::string m_canvasSelector; // Web のみ。デスクトップでは空
    bool m_vsync = false;
};

} // namespace ln
