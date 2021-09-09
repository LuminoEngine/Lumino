#pragma once
#include "RHIObject.hpp"

namespace ln {
namespace detail {

enum class RHIResourceType : uint8_t
{
	Unknown,
	VertexBuffer,
	IndexBuffer,
	UniformBuffer,
	Texture2D,
	RenderTarget,
};

class RHIResource
	: public RHIDeviceObject
{
public:
	RHIResourceType resourceType() const { return m_type; }
	GraphicsResourceUsage usage() const { return m_usage; }
	uint64_t memorySize() const { return m_memorySize; }
	RHISizeI extentSize() const { return m_extentSize; }
	TextureFormat textureFormat() const { return m_textureFormat; }

	// map/unmap は Usage=Streaming のみ許可。
	// 今は UniformBuffer が Usage にかかわらずそのような実装になっているのみ。
	virtual void* map();
	virtual void unmap();

	// RenderTarget のみサポート
	virtual RHIRef<RHIBitmap> readData();
	bool isMultisample() const { return m_msaa; }

	bool mipmap() const { return m_mipmap; }

protected:
	RHIResource();
	virtual ~RHIResource();
	bool initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize);
	bool initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount);
	bool initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize);
	bool initAsTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat format, bool mipmap);
	bool initAsRenderTarget(uint32_t width, uint32_t height, TextureFormat format, bool mipmap, bool msaa);

private:
	RHIResourceType m_type;
	GraphicsResourceUsage m_usage;
	uint64_t m_memorySize;
	RHISizeI m_extentSize;
	TextureFormat m_textureFormat;
	bool m_mipmap;
	bool m_msaa;
};

} // namespace detail
} // namespace ln

