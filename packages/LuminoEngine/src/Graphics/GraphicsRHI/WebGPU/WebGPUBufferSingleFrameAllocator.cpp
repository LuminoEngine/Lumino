#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUBufferSingleFrameAllocator.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPUSingleFrameAllocatorPage

WebGPUSingleFrameAllocatorPage::~WebGPUSingleFrameAllocatorPage() {
    if (m_nativeBuffer) {
        wgpuBufferRelease(m_nativeBuffer);
        m_nativeBuffer = nullptr;
    }
}

bool WebGPUSingleFrameAllocatorPage::init(WebGPUDevice* device, uint64_t size) {
    WGPUBufferDescriptor bufferDesc = WGPU_BUFFER_DESCRIPTOR_INIT;
    bufferDesc.usage = WGPUBufferUsage_CopySrc | WGPUBufferUsage_CopyDst;
    bufferDesc.size = size;
    bufferDesc.mappedAtCreation = 0;
    m_nativeBuffer = wgpuDeviceCreateBuffer(device->wgpuDevice(), &bufferDesc);
    if (!m_nativeBuffer) {
        LN_LOG_ERROR("wgpuDeviceCreateBuffer() failed.");
        return false;
    }

    return true;
}

//==============================================================================
// WebGPUSingleFrameAllocatorPageManager

WebGPUSingleFrameAllocatorPageManager::WebGPUSingleFrameAllocatorPageManager(
    WebGPUDevice* device, size_t pageSize)
    : LinearAllocatorPageManager(pageSize)
    , m_device(device) {
}

Ref<AbstractLinearAllocatorPage> WebGPUSingleFrameAllocatorPageManager::onCreateNewPage(
    size_t size) {
    auto page = makeRef<WebGPUSingleFrameAllocatorPage>();
    if (!page->init(m_device, size)) {
        return nullptr;
    }
    return page;
}

//==============================================================================
// WebGPUSingleFrameAllocator

WebGPUSingleFrameAllocator::WebGPUSingleFrameAllocator(
    WebGPUSingleFrameAllocatorPageManager* manager)
    : AbstractLinearAllocator(manager) {
}

WebGPUSingleFrameBufferInfo WebGPUSingleFrameAllocator::allocate(size_t size, size_t alignment) {
    WebGPUSingleFrameBufferInfo info = { nullptr, 0 };

    AbstractLinearAllocatorPage* page;
    size_t offset;
    if (allocateCore(size, alignment, &page, &offset)) {
        auto* page2 = static_cast<WebGPUSingleFrameAllocatorPage*>(page);
        info.nativeBuffer = page2->nativeBuffer();
        info.offset = offset;
        return info;
    }
    else {
        return info;
    }
}

} // namespace detail
} // namespace ln
