#pragma once
#include "Common.hpp"
#include "RHIObject.hpp"

namespace ln {
namespace detail {

enum class RHIResourceType : uint8_t {
    Unknown,
    VertexBuffer,
    IndexBuffer,
    UniformBuffer,
    Texture2D,
    Texture3D,
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
    // NOTE: map/unmap は必要なの？（0.10 では UniformBuffer のみ map/unmap 可能としている）
    //   バッファの置き場所に依る最適化の話。
    //   まず、バッファは基本的に DEVICE_LOCAL(GPU側) とするのがよい。GPU が一番高速にアクセスできるのはここである。
    //   また制約として、
    //   - DEVICE_LOCAL なバッファは CPU から直接アクセスできない。(Map できない)
    //   - HOST_VISIBLE なバッファを DEVICE_LOCAL に変更することはできない。
    //   このため、 DEVICE_LOCAL にデータを転送する StagingBuffer という仕掛けが必要になる。
    //   当然ながら、 StagingBuffer の分だけリソースを消費することになる。
    //   簡単にまとめると次のようになる。
    //   - Static(一度しか変更されないようなバッファ) : DEVICE_LOCAL
    //   - Dynamic(毎フレーム変更されるバッファ) : HOST_VISIBLE


    // RenderTarget のみサポート
    virtual RHIRef<RHIBitmap> readData();
    bool isMultisample() const { return m_multisample; }

    bool mipmap() const { return m_mipmap; }

protected:
    RHIResource();
    virtual ~RHIResource();
    Result_deprecated<> initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize);
    Result_deprecated<> initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount);
    Result_deprecated<> initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize);
    Result_deprecated<> initAsTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat format, bool mipmap);
    Result_deprecated<> initAsTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, int32_t depth, TextureFormat format);
    Result_deprecated<> initAsRenderTarget(uint32_t width, uint32_t height, TextureFormat format, bool mipmap, bool multisample);
    Result_deprecated<> initAsDepthBuffer(uint32_t width, uint32_t height, bool multisample);

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
