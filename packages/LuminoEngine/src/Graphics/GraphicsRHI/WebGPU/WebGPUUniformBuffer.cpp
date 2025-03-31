#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUUniformBuffer.hpp>

namespace ln {
namespace detail {

WebGPUUniformBuffer::WebGPUUniformBuffer()
    : m_device(nullptr)
    , m_nativeUniformBuffer(nullptr) {
}

MaybeResult WebGPUUniformBuffer::init(WebGPUDevice* device, uint32_t size) {
    if (!RHIResource::initAsUniformBuffer(GraphicsResourceUsage::Dynamic, size)) return LN_MAKE_ERROR();
    m_device = device;

    WGPUBufferDescriptor bufferDesc = WGPU_BUFFER_DESCRIPTOR_INIT;
    bufferDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
    bufferDesc.size = size;
    bufferDesc.mappedAtCreation = 0;
    m_nativeUniformBuffer = wgpuDeviceCreateBuffer(device->wgpuDevice(), &bufferDesc);
    if (!m_nativeUniformBuffer) {
        return LN_MAKE_ERROR("wgpuDeviceCreateBuffer() failed.");
    }

    //float currentTime = 1.0f;
    //   wgpuQueueWriteBuffer(queue, uniformBuffer, 0, &currentTime, sizeof(float));

    return LN_MAKE_SUCCESS();
}

void WebGPUUniformBuffer::onDestroy() {
    if (m_nativeUniformBuffer) {
        wgpuBufferRelease(m_nativeUniformBuffer);
        m_nativeUniformBuffer = nullptr;
    }
    RHIResource::onDestroy();
}

} // namespace detail
} // namespace ln
