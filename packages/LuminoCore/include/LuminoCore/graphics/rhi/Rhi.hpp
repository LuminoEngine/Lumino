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
#include "RHIObject.hpp"

#include <string>
#include <vector>

namespace ln::rhi {

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
class RenderPassEncoder;

// ------ Enums ------------------------------------------------------------------------------------------------------------------------------

enum class Backend {
    Vulkan,
    WebGPU,
};

enum class TextureFormat {
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

enum class BufferUsage : u32 {
    Vertex  = 0x01,
    Index   = 0x02,
    Uniform = 0x04,
    Storage = 0x08,
    CopySrc = 0x10,
    CopyDst = 0x20,
};
inline BufferUsage operator|(BufferUsage a, BufferUsage b) {
    return static_cast<BufferUsage>(static_cast<u32>(a) | static_cast<u32>(b));
}
inline bool operator&(BufferUsage a, BufferUsage b) {
    return (static_cast<u32>(a) & static_cast<u32>(b)) != 0;
}

enum class TextureUsage : u32 {
    Sampled      = 0x01,
    Storage      = 0x02,
    RenderTarget = 0x04,
    CopySrc      = 0x08,
    CopyDst      = 0x10,
    DepthStencil = 0x20,  // depth/stencil attachment
};
inline TextureUsage operator|(TextureUsage a, TextureUsage b) {
    return static_cast<TextureUsage>(static_cast<u32>(a) | static_cast<u32>(b));
}

enum class ShaderStage : u32 {
    Vertex   = 0x01,
    Fragment = 0x02,
    Compute  = 0x04,
};
inline ShaderStage operator|(ShaderStage a, ShaderStage b) {
    return static_cast<ShaderStage>(static_cast<u32>(a) | static_cast<u32>(b));
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
    SrcAlpha,
    OneMinusSrcAlpha,
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
    u64 size = 0;
    BufferUsage usage = BufferUsage::Vertex;
    const void* initialData = nullptr;
};

struct TextureDesc {
    u32 width = 1;
    u32 height = 1;
    u32 depthOrArrayLayers = 1;
    u32 mipLevels = 1;
    TextureFormat format = TextureFormat::RGBA8Unorm;
    TextureUsage usage = TextureUsage::Sampled;
    const void* initialData = nullptr; ///< If non-null, pixel data uploaded via staging buffer.
};

struct SamplerDesc {
    FilterMode magFilter = FilterMode::Linear;
    FilterMode minFilter = FilterMode::Linear;
    AddressMode addressU = AddressMode::Repeat;
    AddressMode addressV = AddressMode::Repeat;
    AddressMode addressW = AddressMode::Repeat;
    u32 maxAnisotropy = 1;
};

struct ShaderModuleDesc {
    const u32* spirvCode = nullptr;
    size_t spirvSizeBytes = 0;
};

struct VertexAttribute {
    u32 location = 0;
    VertexFormat format = VertexFormat::Float32x3;
    u32 offset = 0;
};

struct VertexBufferLayout {
    u32 stride = 0;
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
};

struct DepthStencilState {
    bool depthTestEnable = false;
    bool depthWriteEnable = false;
    CompareFunction depthCompare = CompareFunction::Less;
};

struct BindGroupLayoutEntry {
    u32 binding = 0;
    ShaderStage visibility = ShaderStage::Vertex;
    BindingType type = BindingType::UniformBuffer;
    bool hasDynamicOffset = false;  ///< Only valid for UniformBuffer/StorageBuffer.
};

struct BindGroupLayoutDesc {
    std::vector<BindGroupLayoutEntry> entries;
};

struct BindGroupEntry {
    u32 binding = 0;
    Buffer* buffer = nullptr;          // for uniform/storage
    u64 offset = 0;
    u64 size = 0;
    TextureView* textureView = nullptr; // for sampled texture
    Sampler* sampler = nullptr;         // for sampler
};

struct BindGroupDesc {
    BindGroupLayout* layout = nullptr;
    std::vector<BindGroupEntry> entries;
};

struct PipelineLayoutDesc {
    std::vector<BindGroupLayout*> bindGroupLayouts;
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
    std::vector<TextureFormat> colorFormats;
    std::vector<BlendState> blendStates;
    TextureFormat depthStencilFormat = TextureFormat::Depth24Stencil8;
    DepthStencilState depthStencil;
    u32 sampleCount = 1;
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
    f32 clearDepth = 1.0f;
};

struct RenderPassDesc {
    std::vector<ColorAttachment> colorAttachments;
    DepthStencilAttachment* depthStencilAttachment = nullptr;
};

struct SwapChainDesc {
    void* nativeWindowHandle = nullptr;
    u32 width = 1280;
    u32 height = 720;
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
    virtual u64 size() const = 0;
    /** Map the buffer for CPU write. Returns nullptr if not mappable. */
    virtual void* map() = 0;
    virtual void unmap() = 0;
};

class Texture : public RHIObject {
public:
    virtual ~Texture() = default;
    virtual u32 width() const = 0;
    virtual u32 height() const = 0;
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
};

class RenderPipeline : public RHIObject {
public:
    virtual ~RenderPipeline() = default;
};

// ------ Command Encoding --------------------------------------------------------------------------------------------------------

class RenderPassEncoder {
public:
    virtual ~RenderPassEncoder() = default;
    virtual void setPipeline(RenderPipeline* pipeline) = 0;
    virtual void setVertexBuffer(u32 slot, Buffer* buffer, u64 offset = 0) = 0;
    virtual void setIndexBuffer(Buffer* buffer, IndexFormat format, u64 offset = 0) = 0;
    virtual void setBindGroup(u32 index, BindGroup* group) = 0;
    virtual void setBindGroup(u32 index, BindGroup* group,
                              const u32* dynamicOffsets, u32 dynamicOffsetCount) = 0;
    virtual void setViewport(f32 x, f32 y, f32 w, f32 h, f32 minDepth = 0, f32 maxDepth = 1) = 0;
    virtual void setScissorRect(u32 x, u32 y, u32 w, u32 h) = 0;
    virtual void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0) = 0;
    virtual void drawIndexed(u32 indexCount, u32 instanceCount = 1, u32 firstIndex = 0, i32 baseVertex = 0, u32 firstInstance = 0) = 0;
    virtual void end() = 0;
};

class CommandBuffer : public RHIObject {
public:
    virtual ~CommandBuffer() = default;
    virtual RenderPassEncoder* beginRenderPass(const RenderPassDesc& desc) = 0;
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
    virtual u32 width() const = 0;
    virtual u32 height() const = 0;

    /** 現在のフレームの CommandBuffer を取得します。InFrightFrame ごとにインスタンスは変わります。 */
    virtual CommandBuffer* getCurrentCommandBuffer() = 0;
};

// ------ Device Limits -----------------------------------------------------------------------------------------------------------

struct DeviceLimits {
    u32 minUniformBufferOffsetAlignment = 256;
    u32 maxUniformBufferRange = 65536;
};

// ------ Device (Factory) --------------------------------------------------------------------------------------------------------

class Device : public RHIObject {
public:
    virtual ~Device() = default;

    /** Create a device with the given backend. */
    static Result<Ref<Device>> create(const DeviceDesc& desc);

    /** Query device limits. */
    virtual DeviceLimits deviceLimits() const = 0;

    // Resource creation
    virtual Result<Ref<SwapChain>> createSwapChain(const SwapChainDesc& desc) = 0;
    virtual Result<Ref<Buffer>> createBuffer(const BufferDesc& desc) = 0;
    virtual Result<Ref<Texture>> createTexture(const TextureDesc& desc) = 0;
    virtual Result<Ref<TextureView>> createTextureView(Texture* texture) = 0;
    virtual Result<Ref<Sampler>> createSampler(const SamplerDesc& desc) = 0;
    virtual Result<Ref<ShaderModule>> createShaderModule(const ShaderModuleDesc& desc) = 0;
    virtual Result<Ref<BindGroupLayout>> createBindGroupLayout(const BindGroupLayoutDesc& desc) = 0;
    virtual Result<Ref<BindGroup>> createBindGroup(const BindGroupDesc& desc) = 0;
    virtual Result<Ref<PipelineLayout>> createPipelineLayout(const PipelineLayoutDesc& desc) = 0;
    virtual Result<Ref<RenderPipeline>> createRenderPipeline(const RenderPipelineDesc& desc) = 0;

    /** Wait for the device to become idle. */
    virtual void waitIdle() = 0;
};

} // namespace ln::rhi
