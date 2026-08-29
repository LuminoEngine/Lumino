#pragma once

/**
 * @file rhi.hpp
 * WebGPU-style Rendering Hardware Interface for Lumino.
 * 
 * Design principles:
 *   - Resources are immutable after creation.
 *   - Pipeline state is pre-built.
 *   - Commands are encoded then submitted (two-phase).
 *   - Vulkan complexities (descriptor pools, pipeline caches, framebuffers)
 *     are hidden inside the backend.
 */

#include <LuminoBase/Types.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoBase/math/Math.hpp>
#include <LuminoBase/math/Matrix4x4.hpp>
#include <LuminoBase/SmallVector.hpp>
#include "RHIObject.hpp"

#include <atomic>
#include <string>
#include <vector>

namespace ln::rhi {
static const int kMaxMultiRenderTargets = 8;
static const int kMaxBindGroupEntries = 16;

// ------ Forward declarations ------
class Device;
class SwapChain;
class Buffer;
class Texture;
class TextureView;
class Sampler;
class ShaderModule;
class BindGroupLayout;
class BindGroup;
class PipelineLayout;
class RenderPipeline;
class CommandBuffer;
class RenderPass;

// ------ Enums ------------------------------------------------------------------------------------------------------------------------------

enum class Backend {
    Vulkan,
    WebGPU,
};

enum class TextureFormat {
    Undefined,
    BGRA8Unorm,
    BGRA8UnormSrgb,
    RGBA8Unorm,
    RGBA8UnormSrgb,
    Depth24Stencil8,
    Depth32Float,
    R8Unorm,
    RG8Unorm,
    RGBA16Float,
    RGBA32Float,
};

enum class BufferUsage : uint32_t {
    Vertex  = 0x01,
    Index   = 0x02,
    Uniform = 0x04,
    Storage = 0x08,
    CopySrc = 0x10,
    CopyDst = 0x20,
};
inline BufferUsage operator|(BufferUsage a, BufferUsage b) {
    return static_cast<BufferUsage>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
inline bool operator&(BufferUsage a, BufferUsage b) {
    return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
}

enum class TextureUsage : uint32_t {
    Sampled      = 0x01,
    Storage      = 0x02,
    RenderTarget = 0x04,
    CopySrc      = 0x08,
    CopyDst      = 0x10,
    DepthStencil = 0x20,  // depth/stencil attachment
};
inline TextureUsage operator|(TextureUsage a, TextureUsage b) {
    return static_cast<TextureUsage>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

enum class ShaderStage : uint32_t {
    Vertex   = 0x01,
    Fragment = 0x02,
    Compute  = 0x04,
};
inline ShaderStage operator|(ShaderStage a, ShaderStage b) {
    return static_cast<ShaderStage>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

enum class VertexFormat {
    Float32x2,
    Float32x3,
    Float32x4,
    Uint8x4Norm,
    Sint16x2,
    Sint16x4,
};

enum class IndexFormat {
    Uint16,
    Uint32,
};

enum class PrimitiveTopology {
    TriangleList,
    TriangleStrip,
    LineList,
    LineStrip,
    PointList,
};

enum class CullMode {
    None,
    Front,
    Back,
};

enum class FrontFace {
    CCW,
    CW,
};

enum class BlendFactor {
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstColor,
    OneMinusDstColor,
    DstAlpha,
    OneMinusDstAlpha,
};

enum class BlendOp {
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max,
};

enum class CompareFunction {
    Never,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    NotEqual,
    Always,
};

enum class StencilOp {
    Keep,
    Zero,
    Replace,
    IncrementClamp,
    DecrementClamp,
    Invert,
};

enum class LoadOp {
    Load,
    Clear,
    DontCare,
};

enum class StoreOp {
    Store,
    DontCare,
};

enum class BindingType {
    UniformBuffer,
    StorageBuffer,
    SampledTexture,
    StorageTexture,
    Sampler,
};

enum class FilterMode {
    Nearest,
    Linear,
};

enum class AddressMode {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
};

// ------ Descriptors ------------------------------------------------------------------------------------------------------------------

struct BufferDesc {
    uint64_t size = 0;
    BufferUsage usage = BufferUsage::Vertex;
    const void* initialData = nullptr;
    bool mappable = false; ///< If true, buffer is host-visible even for Vertex/Index usage.
};

struct TextureDesc {
    uint32_t width = 1;
    uint32_t height = 1;
    uint32_t depthOrArrayLayers = 1;
    uint32_t mipLevels = 1;
    TextureFormat format = TextureFormat::RGBA8Unorm;
    TextureUsage usage = TextureUsage::Sampled;
    const void* initialData = nullptr; ///< If non-null, pixel data uploaded via staging buffer.
};

// 既定値は ln::SamplerState (Material.hpp) と揃えてある。
// UV が範囲外に出たときに反対側の端から回り込む Repeat は、気付きにくい描画不具合
// (ポストエフェクトの光漏れ、シャドウマップの回り込み) の原因になるため、
// 既定は ClampToEdge とし、タイリングが必要な場合に明示的に Repeat を指定する。
struct SamplerDesc {
    FilterMode magFilter = FilterMode::Linear;
    FilterMode minFilter = FilterMode::Linear;
    AddressMode addressU = AddressMode::ClampToEdge;
    AddressMode addressV = AddressMode::ClampToEdge;
    AddressMode addressW = AddressMode::ClampToEdge;
    uint32_t maxAnisotropy = 1;
};

enum class ShaderCodeFormat {
    SPIRV,
    WGSL,
};

struct ShaderModuleDesc {
    ShaderCodeFormat format = ShaderCodeFormat::SPIRV;
    const void* code = nullptr;
    size_t codeSizeBytes = 0;

    /**
     * デバッグ用の識別名 (例: "Unlit.slang:fsMain")。
     *
     * WebGPU バックエンドではそのままオブジェクトのラベルになる。
     * ブラウザ / Dawn は不正なシェーダのエラーを
     * `[Invalid ShaderModule "<label>"]` の形で報告するため、
     * ここに名前を入れておくと「どのシェーダか」がログから分かる。
     */
    std::string debugName;
};

struct VertexAttribute {
    uint32_t location = 0;
    VertexFormat format = VertexFormat::Float32x3;
    uint32_t offset = 0;
};

struct VertexBufferLayout {
    uint32_t stride = 0;
    std::vector<VertexAttribute> attributes;
    bool perInstance = false;
};

struct BlendState {
    bool enabled = false;
    BlendFactor srcColor = BlendFactor::One;
    BlendFactor dstColor = BlendFactor::Zero;
    BlendOp colorOp = BlendOp::Add;
    BlendFactor srcAlpha = BlendFactor::One;
    BlendFactor dstAlpha = BlendFactor::Zero;
    BlendOp alphaOp = BlendOp::Add;
    bool colorWriteEnabled = true;
};

struct StencilFaceState {
    CompareFunction compare = CompareFunction::Always;
    StencilOp failOp      = StencilOp::Keep;
    StencilOp depthFailOp = StencilOp::Keep;
    StencilOp passOp      = StencilOp::Keep;
};

struct DepthStencilState {
    bool depthTestEnable = false;
    bool depthWriteEnable = false;
    CompareFunction depthCompare = CompareFunction::Less;
    bool stencilTestEnable = false;
    StencilFaceState stencilFront;
    StencilFaceState stencilBack;
    uint32_t stencilReadMask  = 0xFF;
    uint32_t stencilWriteMask = 0xFF;
};

struct BindGroupLayoutEntry {
    uint32_t binding = 0;
    ShaderStage visibility = ShaderStage::Vertex;
    BindingType type = BindingType::UniformBuffer;
    bool hasDynamicOffset = false;  ///< Only valid for UniformBuffer/StorageBuffer.
};

struct BindGroupLayoutDesc {
    std::vector<BindGroupLayoutEntry> entries;
};

struct BindGroupEntry {
    uint32_t binding = 0;
    Buffer* buffer = nullptr;          // for uniform/storage
    uint64_t offset = 0;
    uint64_t size = 0;
    TextureView* textureView = nullptr; // for sampled texture
    Sampler* sampler = nullptr;         // for sampler
};

struct BindGroupDesc {
    BindGroupLayout* layout = nullptr;
    std::vector<BindGroupEntry> entries;
};

struct PipelineLayoutDesc {
    std::vector<BindGroupLayoutDesc> setLayouts;
};

struct RenderPassLayoutDesc {
    SmallVector<TextureFormat, kMaxMultiRenderTargets> colorFormats;
    TextureFormat depthStencilFormat = TextureFormat::Undefined;
    uint32_t sampleCount = 1;
};

struct RenderPipelineDesc {
    PipelineLayout* layout = nullptr;
    ShaderModule* vertexShader = nullptr;
    std::string vertexEntry = "main";
    ShaderModule* fragmentShader = nullptr;
    std::string fragmentEntry = "main";
    std::vector<VertexBufferLayout> vertexBuffers;
    PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    CullMode cullMode = CullMode::None;
    FrontFace frontFace = FrontFace::CCW;
    RenderPass* renderPass = nullptr;
    std::vector<BlendState> blendStates;
    DepthStencilState depthStencil;

    /**
     * デバッグ用の識別名 (例: "Unlit.slang:Forward")。
     * @see ShaderModuleDesc::debugName
     */
    std::string debugName;
};

struct ColorAttachment {
    TextureView* view = nullptr;
    LoadOp loadOp = LoadOp::Clear;
    StoreOp storeOp = StoreOp::Store;
    Color clearColor = {0, 0, 0, 1};
};

struct DepthStencilAttachment {
    TextureView* view = nullptr;
    LoadOp depthLoadOp = LoadOp::Clear;
    StoreOp depthStoreOp = StoreOp::Store;
    float clearDepth = 1.0f;
    LoadOp stencilLoadOp = LoadOp::Clear;
    StoreOp stencilStoreOp = StoreOp::Store;
    uint32_t clearStencil = 0;
};

struct RenderPassDesc {
    SmallVector<ColorAttachment, kMaxMultiRenderTargets> colorAttachments;
    DepthStencilAttachment* depthStencilAttachment = nullptr;
};

struct SwapChainDesc {
    void* nativeWindowHandle = nullptr;
    uint32_t width = 1280;
    uint32_t height = 720;
    bool vsync = true;
};

struct DeviceDesc {
    Backend backend = Backend::Vulkan;
    bool enableValidation = false;
};

// ------ Resource Interfaces --------------------------------------------------------------------------------------------------

class Buffer : public RHIObject {
public:
    virtual ~Buffer() = default;
    virtual uint64_t size() const = 0;
    // バッファへの書き込みは Device::writeBuffer() に一本化している。
    // CPU マップは Vulkan 固有の実装手段であり (WebGPU の mapAsync は非同期、WebGL2 は仕様上
    // マップ自体が存在しない)、RHI の共通契約にはしない。
};

class Texture : public RHIObject {
public:
    virtual ~Texture() = default;
    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;
    virtual TextureFormat format() const = 0;
};

class TextureView : public RHIObject {
    // NOTE: なぜ TextureView も公開しているのか？
    //   legacy では次のように、 Texture が内部に View も持っていた。
    //   ```
    //   class VulkanTexture : public Texture {
    //         VkImage m_image;
    //         VkImageView m_view;
    //   }
    //   ```
    //   こうすると RHI を使う側のコードはシンプルになるが、次のようなことが難しくなる可能性がある。
    //   - Mipmap 生成
    //   - Cubemap 描画
    //   - Cascaded shadow map など高度な描画
    //   完全に隠すのは少しためらわれるのと、 Texture にデフォルトの view を取得するような仕組みにしつつ、
    //   Material などで隠蔽してみることにする。

 public:
    virtual ~TextureView() = default;
    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;
};

class Sampler : public RHIObject {
public:
    virtual ~Sampler() = default;
};

class ShaderModule : public RHIObject {
public:
    virtual ~ShaderModule() = default;
};

class BindGroupLayout : public RHIObject {
public:
    virtual ~BindGroupLayout() = default;
};

class BindGroup : public RHIObject {
public:
    virtual ~BindGroup() = default;
};

class PipelineLayout : public RHIObject {
public:
    virtual ~PipelineLayout() = default;

    /** Create a BindGroup for the specified descriptor set index. */
    virtual Result<Ref<BindGroup>> createBindGroup(
        uint32_t setIndex, const std::vector<BindGroupEntry>& entries) = 0;
};

class RenderPipeline : public RHIObject {
public:
    virtual ~RenderPipeline() = default;
};

// ------ RenderPass ----------------------------------------------------------------------------------------------------------------

class RenderPass : public RHIObject {
public:
    virtual ~RenderPass() = default;
    virtual const RenderPassLayoutDesc& layoutDesc() const = 0;

    // Encoding methods
    virtual void setPipeline(RenderPipeline* pipeline) = 0;
    virtual void setVertexBuffer(uint32_t slot, Buffer* buffer, uint64_t offset = 0) = 0;
    virtual void setIndexBuffer(Buffer* buffer, IndexFormat format, uint64_t offset = 0) = 0;
    virtual void setBindGroup(uint32_t index, BindGroup* group) = 0;
    virtual void setBindGroup(uint32_t index, BindGroup* group,
                              const uint32_t* dynamicOffsets, uint32_t dynamicOffsetCount) = 0;
    virtual void setViewport(float x, float y, float w, float h, float minDepth = 0, float maxDepth = 1) = 0;
    virtual void setScissorRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
    virtual void setStencilReference(uint32_t reference) = 0;
    virtual void draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0) = 0;
    virtual void drawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0, int32_t baseVertex = 0, uint32_t firstInstance = 0) = 0;
    virtual void end() = 0;
};

// ------ Command Encoding --------------------------------------------------------------------------------------------------------

class CommandBuffer : public RHIObject {
public:
    virtual ~CommandBuffer() = default;
    virtual RenderPass* beginRenderPass(const RenderPassDesc& desc) = 0;
    virtual void submit() = 0;
};

// ------ SwapChain ----------------------------------------------------------------------------------------------------------------------

class SwapChain : public RHIObject {
public:
    virtual ~SwapChain() = default;
    /** Acquire the next framebuffer texture view. Must be called once per frame. */
    virtual TextureView* acquireNextTexture() = 0;
    /** Present the current frame. */
    virtual void present() = 0;
    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;

    /** サーフェスを新しいサイズで再設定します。サイズが同一の場合は何もしません。 */
    virtual VoidResult resize(uint32_t width, uint32_t height) = 0;

    /** インフライトフレーム数を返します (SwapChain イメージ数)。 */
    virtual uint32_t maxFramesInFlight() const = 0;

    virtual uint32_t currentFrame() const = 0;

    /** 現在のフレームの CommandBuffer を取得します。InFrightFrame ごとにインスタンスは変わります。 */
    virtual CommandBuffer* getCurrentCommandBuffer() = 0;
};

// ------ Device Limits -----------------------------------------------------------------------------------------------------------

struct DeviceLimits {
    uint32_t minUniformBufferOffsetAlignment = 256;
    uint32_t maxUniformBufferRange = 65536;
};

// ------ Device (Factory) --------------------------------------------------------------------------------------------------------

class Device : public RHIObject {
public:
    /** beginCreateAsync で開始した非同期初期化の進行状態。 */
    enum class AsyncInitStatus {
        Pending,
        Ready,
        Failed,
    };

    virtual ~Device() = default;

    /** Create a device with the given backend. */
    static Result<Ref<Device>> create(const DeviceDesc& desc);

    /**
     * 非ブロッキングのデバイス作成を開始する (デバイスロスト自動復旧用)。
     * 返されたデバイスは pumpAsyncInit() が Ready を返すまで使用してはならない。
     * Web (Emscripten) の WebGPU ではアダプタ/デバイス要求がブラウザのイベント
     * ループ経由で解決されるため、毎フレーム pumpAsyncInit() を呼んで進行させる。
     * 同期的に初期化できるバックエンド (Vulkan / ネイティブ WebGPU) では
     * この呼び出し内で初期化が完了し、最初の pumpAsyncInit() が Ready を返す。
     */
    static Result<Ref<Device>> beginCreateAsync(const DeviceDesc& desc);

    /** beginCreateAsync で開始した初期化を 1 ステップ進める。
        同期的に初期化されるバックエンドの既定実装は常に Ready を返す。 */
    virtual AsyncInitStatus pumpAsyncInit() { return AsyncInitStatus::Ready; }

    /** Query device limits. */
    virtual DeviceLimits deviceLimits() const = 0;

    // Resource creation
    virtual Result<Ref<SwapChain>> createSwapChain(const SwapChainDesc& desc) = 0;
    virtual Result<Ref<Buffer>> createBuffer(const BufferDesc& desc) = 0;
    virtual Result<Ref<Texture>> createTexture(const TextureDesc& desc) = 0;
    virtual Result<Ref<TextureView>> createTextureView(Texture* texture) = 0;
    virtual Result<Ref<Sampler>> createSampler(const SamplerDesc& desc) = 0;
    virtual Result<Ref<ShaderModule>> createShaderModule(const ShaderModuleDesc& desc) = 0;
    virtual Result<Ref<PipelineLayout>> createPipelineLayout(const PipelineLayoutDesc& desc) = 0;
    virtual Result<Ref<RenderPipeline>> createRenderPipeline(const RenderPipelineDesc& desc) = 0;

    /** Write data to a buffer. Works on all backends including those without map/unmap support. */
    virtual VoidResult writeBuffer(Buffer* dst, uint64_t dstOffset, const void* data, uint64_t size) = 0;

    /**
     * テクスチャビューの内容をCPU側のピクセルバッファに読み戻します。
     * この関数は主にテストで使用するもので、実行速度は速くありません。
     * 通常のゲームループで使用することは想定されていません。
     */
    virtual Result<std::vector<uint8_t>> readbackTexture(TextureView* view) = 0;

    /** Wait for the device to become idle. */
    virtual void waitIdle() = 0;

    /** Get the backend type of this device. */
    virtual Backend backend() const = 0;

    // ------ Device lost ------

    /** デバイスロスト状態か。バックエンドの検知イベントにより true になる。
        一度 true になったらこの Device インスタンスの寿命中は false に戻らない
        (復旧は新しい Device インスタンスで行う)。 */
    bool isDeviceLost() const { return m_deviceLost.load(std::memory_order_acquire); }

    /** ロスト理由 (ログ・テレメトリ用)。未ロスト時は空文字列。 */
    const std::string& deviceLostReason() const { return m_deviceLostReason; }

    /** バックエンド実装がロスト検知時に呼ぶ。
        フラグを立てる以外の副作用を持たない (解放や再作成をここから始めてはならない)。
        WebGPU のコールバックは他の API 呼び出し中に spontaneous に発火しうるため、
        スレッドセーフに実装している。 */
    void markDeviceLost(const char* reason) {
        // 最初の呼び出しだけが理由を書き込む。理由の書き込み完了後にフラグを公開する。
        if (m_deviceLostGuard.exchange(true, std::memory_order_acq_rel)) return;
        m_deviceLostReason = reason ? reason : "";
        m_deviceLost.store(true, std::memory_order_release);
    }

    /** テスト用: デバイスロストをシミュレートする。
        deep=true の場合、バックエンドによっては実デバイスを破棄して後続 API の
        エラー挙動まで再現する (WebGPU)。既定実装はフラグを立てるのみ。 */
    virtual void debugSimulateDeviceLost(bool deep) {
        (void)deep;
        markDeviceLost("simulated");
    }

private:
    std::atomic<bool> m_deviceLostGuard{false};
    std::atomic<bool> m_deviceLost{false};
    std::string m_deviceLostReason;
};

} // namespace ln::rhi
