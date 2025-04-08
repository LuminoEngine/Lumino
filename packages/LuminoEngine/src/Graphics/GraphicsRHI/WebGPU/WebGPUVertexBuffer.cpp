#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUVertexBuffer.hpp>

namespace ln {
namespace detail {

WebGPUVertexBuffer::WebGPUVertexBuffer()
    : m_device(nullptr)
    , m_nativeBuffer(nullptr) {
}

MaybeResult_deprecated WebGPUVertexBuffer::init(
    WebGPUDevice* device, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) {
    if (!RHIResource::initAsVertexBuffer(usage, bufferSize)) return LN_MAKE_ERROR_deprecated();
    m_device = device;

    WGPUBufferDescriptor bufferDesc = WGPU_BUFFER_DESCRIPTOR_INIT;
    bufferDesc.usage = WGPUBufferUsage_Vertex | WGPUBufferUsage_CopyDst;
    bufferDesc.size = bufferSize;
    bufferDesc.mappedAtCreation = 0;
    m_nativeBuffer = wgpuDeviceCreateBuffer(device->wgpuDevice(), &bufferDesc);
    if (!m_nativeBuffer) {
        return LN_MAKE_ERROR_deprecated("wgpuDeviceCreateBuffer() failed.");
    }

    if (initialData) {
        wgpuQueueWriteBuffer(device->wgpuQueue(), m_nativeBuffer, 0, initialData, bufferSize);
    }

    return LN_MAKE_SUCCESS();
}

void WebGPUVertexBuffer::onDestroy() {
    if (m_nativeBuffer) {
        wgpuBufferRelease(m_nativeBuffer);
        m_nativeBuffer = nullptr;
    }
    RHIResource::onDestroy();
}

void* WebGPUVertexBuffer::map() {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void WebGPUVertexBuffer::unmap() {
    LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln
