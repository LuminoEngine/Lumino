#include <LuminoEngine/GraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoEngine/GraphicsRHI/RHIProfiler.hpp>
#include <LuminoEngine/GraphicsRHI/RHIHelper.hpp>
#include <LuminoEngine/GraphicsRHI/RHIResource.hpp>

namespace ln {
namespace detail {

//==============================================================================
// RHIResource

RHIResource::RHIResource()
    : m_type(RHIResourceType::Unknown)
    , m_usage(GraphicsResourceUsage::Static)
    , m_memorySize(0)
    , m_extentSize{ 0, 0, 0 }
    , m_textureFormat(TextureFormat::Unknown)
    , m_mipmap(false)
    , m_multisample(false) {
    LN_LOG_VERBOSE("RHIResource [0x{:x}] constructed.", (intptr_t)this);
}

RHIResource::~RHIResource() {
    if (IGraphicsDevice* d = device()) {
        switch (m_type) {
            case RHIResourceType::VertexBuffer:
                d->profiler()->removeVertexBuffer(this);
                break;
            case RHIResourceType::IndexBuffer:
                d->profiler()->removeIndexBuffer(this);
                break;
            case RHIResourceType::UniformBuffer:
                d->profiler()->removeUniformBuffer(this);
                break;
            case RHIResourceType::Texture2D:
                d->profiler()->removeTexture2D(this);
                break;
            case RHIResourceType::RenderTarget:
                d->renderPassCache()->invalidateRenderTarget(static_cast<RHIResource*>(this));
                d->profiler()->removeRenderTarget(this);
                break;
            case RHIResourceType::DepthBuffer:
                d->renderPassCache()->invalidateDepthBuffer(static_cast<RHIResource*>(this));
                d->profiler()->removeDepthBuffer(this);
                break;
            default:
                LN_UNREACHABLE();
                break;
        }
    }
}

Result<> RHIResource::initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize) {
    m_type = RHIResourceType::VertexBuffer;
    m_usage = usage;
    m_memorySize = memorySize;
    return ok();
}

Result<> RHIResource::initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount) {
    m_type = RHIResourceType::IndexBuffer;
    m_usage = usage;

    int stride = 0;
    if (format == IndexBufferFormat::UInt16) {
        stride = 2;
    }
    else if (format == IndexBufferFormat::UInt32) {
        stride = 4;
    }
    else {
        LN_UNREACHABLE();
        return err();
    }
    m_memorySize = stride * indexCount;

    return ok();
}

Result<> RHIResource::initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize) {
    // 今のところ UniformBuffer は Dynamic (複数 DrawCall で共有しない) 前提
    // TODO: OpenGL の Streaming みたいにもうひとつ用意して区別したほうがいいかも
    if (LN_REQUIRE(usage == GraphicsResourceUsage::Dynamic)) return err();
    m_type = RHIResourceType::UniformBuffer;
    m_usage = usage;
    m_memorySize = memorySize;
    return ok();
}

Result<> RHIResource::initAsTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat format, bool mipmap) {
    //if (LN_REQUIRE(format == TextureFormat::Unknown)) return err();
    m_type = RHIResourceType::Texture2D;
    m_usage = usage;
    m_extentSize.width = width;
    m_extentSize.height = height;
    m_extentSize.depth = 1;
    m_memorySize = width * height * RHIHelper::getPixelSize(format);
    m_textureFormat = format;
    m_mipmap = mipmap;
    return ok();
}

Result<> RHIResource::initAsTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, int32_t depth, TextureFormat format) {
    m_type = RHIResourceType::Texture3D;
    m_usage = usage;
    m_extentSize.width = width;
    m_extentSize.height = height;
    m_extentSize.depth = depth;
    m_memorySize = width * height * depth * RHIHelper::getPixelSize(format);
    m_textureFormat = format;
    return ok();
}

Result<> RHIResource::initAsRenderTarget(uint32_t width, uint32_t height, TextureFormat format, bool mipmap, bool multisample) {
    //if (LN_REQUIRE(format == TextureFormat::Unknown)) return err();
    m_type = RHIResourceType::RenderTarget;
    m_usage = GraphicsResourceUsage::Static;
    m_extentSize.width = width;
    m_extentSize.height = height;
    m_extentSize.depth = 1;
    m_memorySize = width * height * RHIHelper::getPixelSize(format);
    m_textureFormat = format;
    m_mipmap = mipmap;
    m_multisample = multisample;
    return ok();
}

Result<> RHIResource::initAsDepthBuffer(uint32_t width, uint32_t height, bool multisample) {
    if (LN_REQUIRE(m_type == RHIResourceType::Unknown)) return err();
    if (LN_REQUIRE(width > 0)) return err();
    if (LN_REQUIRE(height > 0)) return err();
    m_type = RHIResourceType::DepthBuffer;
    m_usage = GraphicsResourceUsage::Static;
    m_extentSize.width = width;
    m_extentSize.height = height;
    m_extentSize.depth = 1;
    m_memorySize = width * height * 4;  // TODO: format
    m_multisample = multisample;
    return ok();
}

void* RHIResource::map() {
    LN_UNREACHABLE();
    return nullptr;
}

void RHIResource::unmap() {
    LN_UNREACHABLE();
}

RHIRef<RHIBitmap> RHIResource::readData() {
    LN_UNREACHABLE();
    return nullptr;
}

} // namespace detail
} // namespace ln
