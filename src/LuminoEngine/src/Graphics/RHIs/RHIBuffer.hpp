#pragma once
#include "RHIObject.hpp"

namespace ln {
namespace detail {

enum class DeviceResourceType
{
	VertexBuffer,
	IndexBuffer,
	UniformBuffer,
};

class IGraphicsRHIBuffer
	: public IGraphicsDeviceObject
{
public:
	virtual DeviceResourceType resourceType() const = 0;

protected:
	virtual ~IGraphicsRHIBuffer();
};


class IVertexBuffer
	: public IGraphicsRHIBuffer
{
public:
	virtual DeviceResourceType resourceType() const { return DeviceResourceType::VertexBuffer; }
	virtual size_t getBytesSize() = 0;
	virtual GraphicsResourceUsage usage() const = 0;
	//virtual void* map() = 0;	// write only. 初期化用. dynamic に書き換えたい場合は CommandList のデータ転送を使用すること.
	//virtual void unmap() = 0;

protected:
	IVertexBuffer();
	virtual ~IVertexBuffer();
};

class IIndexBuffer
	: public IGraphicsRHIBuffer
{
public:
	virtual DeviceResourceType resourceType() const { return DeviceResourceType::IndexBuffer; }
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

class RHIBuffer
	: public RHIObject
{
public:
	RHIBuffer();

private:
};

} // namespace detail
} // namespace ln

