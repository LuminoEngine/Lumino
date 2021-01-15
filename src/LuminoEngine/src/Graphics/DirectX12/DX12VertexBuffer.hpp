
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

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

private:
	DX12Device* m_device;
	GraphicsResourceUsage m_usage;
	size_t m_size;
	ComPtr<ID3D12Resource> m_vertexBuffer;
};

} // namespace detail
} // namespace ln
