
#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/GPU/ConstantBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsDeviceContext.hpp>

namespace ln {
namespace detail {

class SingleFrameUniformBufferAllocatorPage
    : public AbstractLinearAllocatorPage {
public:
    virtual ~SingleFrameUniformBufferAllocatorPage();
    bool init(GraphicsContext* context, uint32_t size);
    ConstantBuffer* buffer() const { return m_buffer.get(); }

private:
    Ref<ConstantBuffer> m_buffer;
};

class SingleFrameUniformBufferAllocatorPageManager
    : public LinearAllocatorPageManager {
public:
    SingleFrameUniformBufferAllocatorPageManager(GraphicsContext* context, size_t pageSize);

protected:
    Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) override;

private:
    GraphicsContext* m_context;
};

class SingleFrameUniformBufferAllocator
    : public AbstractLinearAllocator {
public:
    SingleFrameUniformBufferAllocator(SingleFrameUniformBufferAllocatorPageManager* manager);
    ConstantBufferView allocate(size_t size, size_t alignment);
    void unmap();

protected:
};

} // namespace detail
} // namespace ln
