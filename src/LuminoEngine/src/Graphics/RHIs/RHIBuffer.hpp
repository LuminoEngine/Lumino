#pragma once
#include "RHIObject.hpp"

namespace ln {
namespace detail {

enum class RHIBufferType
{
	VertexBuffer,
	IndexBuffer,
	UniformBuffer,
};

class RHIBuffer
	: public IGraphicsDeviceObject
{
public:
	virtual RHIBufferType resourceType() const = 0;

protected:
	virtual ~RHIBuffer();
};


class IVertexBuffer
	: public RHIBuffer
{
public:
	virtual RHIBufferType resourceType() const { return RHIBufferType::VertexBuffer; }
	virtual size_t getBytesSize() = 0;
	virtual GraphicsResourceUsage usage() const = 0;
	//virtual void* map() = 0;	// write only. 初期化用. dynamic に書き換えたい場合は CommandList のデータ転送を使用すること.
	//virtual void unmap() = 0;

protected:
	IVertexBuffer();
	virtual ~IVertexBuffer();
};

class IIndexBuffer
	: public RHIBuffer
{
public:
	virtual RHIBufferType resourceType() const { return RHIBufferType::IndexBuffer; }
	virtual size_t getBytesSize() = 0;
	virtual GraphicsResourceUsage usage() const = 0;
	//virtual void* map() = 0;	// write only. 初期化用. dynamic に書き換えたい場合は CommandList のデータ転送を使用すること.
	//virtual void unmap() = 0;

protected:
	IIndexBuffer();
	virtual ~IIndexBuffer();
};

class IUniformBuffer
	: public IGraphicsDeviceObject
{
public:
	virtual void* map() = 0;
	virtual void unmap() = 0;

protected:
	virtual ~IUniformBuffer();
};

} // namespace detail
} // namespace ln

