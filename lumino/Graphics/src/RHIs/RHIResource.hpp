#pragma once
#include <LuminoGraphics/Common.hpp>
#include "RHIObject.hpp"

namespace ln {
namespace detail {

enum class RHIResourceType : uint8_t {
    Unknown,
    VertexBuffer,
    IndexBuffer,
    UniformBuffer,
    Texture2D,
    RenderTarget,
    DepthBuffer,
};

class RHIResource
    : public RHIDeviceObject {
public:
    RHIResourceType resourceType() const { return m_type; }
    GraphicsResourceUsage usage() const { return m_usage; }
    uint64_t memorySize() const { return m_memorySize; }
    const RHIExtent3D& extentSize() const { return m_extentSize; }
    TextureFormat textureFormat() const { return m_textureFormat; }

    // map/unmap は Usage=Streaming のみ許可。
    // 今は UniformBuffer が Usage にかかわらずそのような実装になっているのみ。
    virtual void* map();
    virtual void unmap();

    // RenderTarget のみサポート
    virtual RHIRef<RHIBitmap> readData();
    bool isMultisample() const { return m_multisample; }

    bool mipmap() const { return m_mipmap; }

protected:
    RHIResource();
    virtual ~RHIResource();
    Result initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize);
    Result initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount);
    Result initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize);
    Result initAsTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat format, bool mipmap);
    Result initAsRenderTarget(uint32_t width, uint32_t height, TextureFormat format, bool mipmap, bool multisample);
    Result initAsDepthBuffer(uint32_t width, uint32_t height, bool multisample);

private:
    RHIResourceType m_type;
    GraphicsResourceUsage m_usage;
    uint64_t m_memorySize;
    RHIExtent3D m_extentSize;
    TextureFormat m_textureFormat;
    bool m_mipmap;
    bool m_multisample;
};

} // namespace detail
} // namespace ln
