
#include "Internal.hpp"
#include "GraphicsDeviceContext.hpp"
#include "RHIProfiler.hpp"
#include "RHIResource.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// RHIResource

RHIResource::RHIResource()
	: m_type(RHIResourceType::Unknown)
	, m_usage(GraphicsResourceUsage::Static)
	, m_memorySize(0)
	, m_extentSize{0, 0}
	, m_textureFormat(TextureFormat::Unknown)
	, m_mipmap(false)
	, m_msaa(false)
{
	LN_LOG_VERBOSE << "RHIResource [0x" << this << "] constructed.";
}

RHIResource::~RHIResource()
{
	if (IGraphicsDevice* d = device()) {
		switch (m_type)
		{
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
			d->renderPassCache()->invalidate(static_cast<RHIResource*>(this));
			d->profiler()->removeRenderTarget(this);
			break;
		default:
			break;
		}
	}
}

bool RHIResource::initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize)
{
	m_type = RHIResourceType::VertexBuffer;
	m_usage = usage;
	m_memorySize = memorySize;
	return true;
}

bool RHIResource::initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount)
{
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
		return false;
	}
	m_memorySize = stride * indexCount;

	return true;
}

bool RHIResource::initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize)
{
	// 今のところ UniformBuffer は Dynamic (複数 DrawCall で共有しない) 前提
	// TODO: OpenGL の Streaming みたいにもうひとつ用意して区別したほうがいいかも
	if (LN_REQUIRE(usage == GraphicsResourceUsage::Dynamic)) return false;
	m_type = RHIResourceType::UniformBuffer;
	m_usage = usage;
	m_memorySize = memorySize;
	return true;
}

bool RHIResource::initAsTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat format, bool mipmap)
{
	//if (LN_REQUIRE(format == TextureFormat::Unknown)) return false;
	m_type = RHIResourceType::Texture2D;
	m_usage = usage;
	m_extentSize.width = width;
	m_extentSize.height = height;
	m_memorySize = width * height * GraphicsHelper::getPixelSize(format);
	m_textureFormat = format;
	m_mipmap = mipmap;
	return true;
}

bool RHIResource::initAsRenderTarget(uint32_t width, uint32_t height, TextureFormat format, bool mipmap, bool msaa)
{
	//if (LN_REQUIRE(format == TextureFormat::Unknown)) return false;
	m_type = RHIResourceType::RenderTarget;
	m_usage = GraphicsResourceUsage::Static;
	m_extentSize.width = width;
	m_extentSize.height = height;
	m_memorySize = width * height * GraphicsHelper::getPixelSize(format);
	m_textureFormat = format;
	m_mipmap = mipmap;
	m_msaa = msaa;
	return true;
}

void* RHIResource::map()
{
	LN_UNREACHABLE();
	return nullptr;
}

void RHIResource::unmap()
{
	LN_UNREACHABLE();
}

RHIRef<RHIBitmap> RHIResource::readData()
{
	LN_UNREACHABLE();
	return nullptr;
}

} // namespace detail
} // namespace ln

