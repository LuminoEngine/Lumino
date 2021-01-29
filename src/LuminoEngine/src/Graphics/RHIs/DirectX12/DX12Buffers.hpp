﻿
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

class DX12Buffer
	: public RHIObject
{
public:
	DX12Buffer();
	bool init(DX12Device* device, size_t size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState);
	void dispose();
	ID3D12Resource* dxResource() const { return m_dxResource.Get(); }
	size_t size() const { return m_size; }
	void* map();
	void unmap();
	void resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState);

private:
	ComPtr<ID3D12Resource> m_dxResource;
	size_t m_size;
	D3D12_RESOURCE_STATES m_resourceState;
	bool m_mapped;
};

class DX12VertexBuffer
	: public IVertexBuffer
{
public:
	DX12VertexBuffer();
	bool init(DX12Device* device, GraphicsResourceUsage usage, size_t size, const void* initialData);
	void dispose() override;
	size_t getBytesSize() override;
	GraphicsResourceUsage usage() const override;
	void* map() override;
	void unmap() override;

	const RHIPtr<DX12Buffer>& buffer() const { return m_buffer; }
	ID3D12Resource* dxResource() const { return m_buffer->dxResource(); }

private:
	DX12Device* m_device;
	GraphicsResourceUsage m_usage;
	RHIPtr<DX12Buffer> m_buffer;
	//size_t m_size;
	//ComPtr<ID3D12Resource> m_vertexBuffer;
};

class DX12IndexBuffer
	: public IIndexBuffer
{
public:
	DX12IndexBuffer();
	Result init(DX12Device* device, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
	void dispose() override;
	size_t getBytesSize() override;
	GraphicsResourceUsage usage() const override;
	void* map() override;
	void unmap() override;

	const RHIPtr<DX12Buffer>& buffer() const { return m_buffer; }
	ID3D12Resource* dxResource() const { return m_buffer->dxResource(); }
	DXGI_FORMAT indexFormat() const { return m_indexFormat; }

protected:
	DX12Device* m_device;
	GraphicsResourceUsage m_usage;
	RHIPtr<DX12Buffer> m_buffer;
	DXGI_FORMAT m_indexFormat;
};

} // namespace detail
} // namespace ln