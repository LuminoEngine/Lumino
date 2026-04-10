#include "WebGPUBuffer.hpp"
#include "WebGPUDevice.hpp"

namespace ln::rhi::webgpu {

WebGPUBuffer::WebGPUBuffer() = default;

VoidResult WebGPUBuffer::init(WebGPUDevice* device, const BufferDesc& desc) {
    m_device = device;
    m_size = desc.size;

    WGPUBufferUsage usage = WGPUBufferUsage_None;
    if (desc.usage & BufferUsage::Vertex)  usage |= WGPUBufferUsage_Vertex;
    if (desc.usage & BufferUsage::Index)   usage |= WGPUBufferUsage_Index;
    if (desc.usage & BufferUsage::Uniform) usage |= WGPUBufferUsage_Uniform;
    if (desc.usage & BufferUsage::Storage) usage |= WGPUBufferUsage_Storage;
    if (desc.usage & BufferUsage::CopySrc) usage |= WGPUBufferUsage_CopySrc;
    if (desc.usage & BufferUsage::CopyDst) usage |= WGPUBufferUsage_CopyDst;
    // Always allow CopyDst so we can upload via wgpuQueueWriteBuffer.
    usage |= WGPUBufferUsage_CopyDst;

    WGPUBufferDescriptor bufferDesc = WGPU_BUFFER_DESCRIPTOR_INIT;
    bufferDesc.label = {"LuminoBuffer", WGPU_STRLEN};
    bufferDesc.usage = usage;
    bufferDesc.size = desc.size;
    bufferDesc.mappedAtCreation = false;

    m_buffer = wgpuDeviceCreateBuffer(m_device->wgpuDevice(), &bufferDesc);
    if (!m_buffer) {
        return LN_MAKE_ERROR("wgpuDeviceCreateBuffer failed.");
    }

    if (desc.initialData && desc.size > 0) {
        wgpuQueueWriteBuffer(m_device->wgpuQueue(), m_buffer, 0,
                             desc.initialData, desc.size);
    }

    return LN_MAKE_SUCCESS();
}

void WebGPUBuffer::finalize() {
    if (m_buffer) {
        wgpuBufferRelease(m_buffer);
        m_buffer = nullptr;
    }
    Buffer::finalize();
}

void* WebGPUBuffer::map() {
    // WebGPU's wgpuBufferMapAsync is asynchronous and incompatible with
    // the synchronous map() interface. Use a CPU shadow buffer instead;
    // unmap() flushes it via wgpuQueueWriteBuffer.
    if (m_shadow.size() != m_size) {
        m_shadow.resize(static_cast<size_t>(m_size));
    }
    return m_shadow.data();
}

void WebGPUBuffer::unmap() {
    if (!m_shadow.empty() && m_buffer) {
        wgpuQueueWriteBuffer(m_device->wgpuQueue(), m_buffer, 0,
                             m_shadow.data(), m_shadow.size());
    }
}

} // namespace ln::rhi::webgpu
