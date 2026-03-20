#pragma once
#include "Common.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>

namespace ln {
namespace detail {

struct WebGPUSingleFrameBufferInfo {
    WGPUBuffer nativeBuffer;
    uint32_t offset;
};

class WebGPUSingleFrameAllocatorPage : public AbstractLinearAllocatorPage {
public:
    virtual ~WebGPUSingleFrameAllocatorPage();
    bool init( WebGPUDevice* device, uint64_t size);
    WGPUBuffer nativeBuffer() const { return m_nativeBuffer; }

private:
    WGPUBuffer m_nativeBuffer;
};

class WebGPUSingleFrameAllocatorPageManager : public LinearAllocatorPageManager {
public:
    WebGPUSingleFrameAllocatorPageManager(WebGPUDevice* device, uint64_t pageSize);

protected:
    Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) override;

private:
    WebGPUDevice* m_device;
};

class WebGPUSingleFrameAllocator : public AbstractLinearAllocator {
public:
    WebGPUSingleFrameAllocator(WebGPUSingleFrameAllocatorPageManager* manager);
    WebGPUSingleFrameBufferInfo allocate(size_t size, size_t alignment = 64);
};


} // namespace detail
} // namespace ln
