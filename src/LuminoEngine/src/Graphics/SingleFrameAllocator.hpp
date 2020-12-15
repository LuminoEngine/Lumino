
#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
    
class SingleFrameUniformBufferAllocatorPage
	: public AbstractLinearAllocatorPage
{
public:
	virtual ~SingleFrameUniformBufferAllocatorPage();
	bool init(IGraphicsDevice* device, uint32_t size);
	IUniformBuffer* buffer() const { return m_buffer.get(); }

private:
	Ref<IUniformBuffer> m_buffer;
};

class SingleFrameUniformBufferAllocatorPageManager
	: public LinearAllocatorPageManager
{
public:
	SingleFrameUniformBufferAllocatorPageManager(IGraphicsDevice* device, size_t pageSize);

protected:
	Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) override;

private:
	IGraphicsDevice* m_device;
};

class SingleFrameUniformBufferAllocator
	: public AbstractLinearAllocator
{
public:
	SingleFrameUniformBufferAllocator(SingleFrameUniformBufferAllocatorPageManager* manager);
	UniformBufferView allocate(size_t size, size_t alignment = 64);

protected:
};

} // namespace detail
} // namespace ln
