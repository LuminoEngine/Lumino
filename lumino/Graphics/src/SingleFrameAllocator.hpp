
#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoGraphics/ConstantBuffer.hpp>
#include "RHIs/GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
    
class SingleFrameUniformBufferAllocatorPage
	: public AbstractLinearAllocatorPage
{
public:
	virtual ~SingleFrameUniformBufferAllocatorPage();
	bool init(uint32_t size);
	ConstantBuffer* buffer() const { return m_buffer.get(); }

private:
	Ref<ConstantBuffer> m_buffer;
};

class SingleFrameUniformBufferAllocatorPageManager
	: public LinearAllocatorPageManager
{
public:
	SingleFrameUniformBufferAllocatorPageManager(size_t pageSize);

protected:
	Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) override;

private:
};

class SingleFrameUniformBufferAllocator
	: public AbstractLinearAllocator
{
public:
	SingleFrameUniformBufferAllocator(SingleFrameUniformBufferAllocatorPageManager* manager);
	ConstantBufferView allocate(size_t size, size_t alignment);
	void unmap();

protected:
};

} // namespace detail
} // namespace ln
