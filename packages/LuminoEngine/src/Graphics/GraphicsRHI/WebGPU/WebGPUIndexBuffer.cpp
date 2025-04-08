#include <LuminoEngine/Graphics/GraphicsRHI/RHIHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUIndexBuffer.hpp>

namespace ln {
namespace detail {

WebGPUIndexBuffer::WebGPUIndexBuffer()
    : m_device(nullptr)
    , m_nativeBuffer(nullptr) {
}

MaybeResult_deprecated WebGPUIndexBuffer::init(
    WebGPUDevice* device, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount,
    const void* initialData) {
    if (!RHIResource::initAsIndexBuffer(usage, format, indexCount)) return LN_MAKE_ERROR_deprecated();
    m_device = device;

    if (format == IndexBufferFormat::UInt32) {
        m_indexFormat = WGPUIndexFormat_Uint32;
    }
    else if (format == IndexBufferFormat::UInt16) {
        m_indexFormat = WGPUIndexFormat_Uint16;
    }
    else {
        return LN_MAKE_ERROR_deprecated("Invalid index buffer format.");
    }

    uint64_t memorySize = RHIHelper::getIndexStride(format) * indexCount;

    WGPUBufferDescriptor bufferDesc = WGPU_BUFFER_DESCRIPTOR_INIT;
    bufferDesc.usage = WGPUBufferUsage_Index | WGPUBufferUsage_CopyDst;
    bufferDesc.size = memorySize;
    bufferDesc.mappedAtCreation = 0;
    m_nativeBuffer = wgpuDeviceCreateBuffer(device->wgpuDevice(), &bufferDesc);
    if (!m_nativeBuffer) {
        return LN_MAKE_ERROR_deprecated("wgpuDeviceCreateBuffer() failed.");
    }

    if (initialData) {
        wgpuQueueWriteBuffer(device->wgpuQueue(), m_nativeBuffer, 0, initialData, memorySize);
    }

    return LN_MAKE_SUCCESS();
}

void WebGPUIndexBuffer::onDestroy() {
    if (m_nativeBuffer) {
        wgpuBufferRelease(m_nativeBuffer);
        m_nativeBuffer = nullptr;
    }
    RHIResource::onDestroy();
}

void* WebGPUIndexBuffer::map() {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void WebGPUIndexBuffer::unmap() {
    LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln
