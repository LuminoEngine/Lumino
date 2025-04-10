#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUUniformBuffer.hpp>

namespace ln {
namespace detail {

WebGPUUniformBuffer::WebGPUUniformBuffer()
    : m_device(nullptr)
    , m_nativeBuffer(nullptr) {
}

MaybeResult_deprecated WebGPUUniformBuffer::init(WebGPUDevice* device, uint32_t size) {
    if (!RHIResource::initAsUniformBuffer(GraphicsResourceUsage::Dynamic, size)) return LN_MAKE_ERROR_deprecated();
    m_device = device;

    WGPUBufferDescriptor bufferDesc = {};
    bufferDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    bufferDesc.size = size;
    bufferDesc.mappedAtCreation = 0;
    m_nativeBuffer = wgpuDeviceCreateBuffer(device->wgpuDevice(), &bufferDesc);
    if (!m_nativeBuffer) {
        return LN_MAKE_ERROR_deprecated("wgpuDeviceCreateBuffer() failed.");
    }

    //float currentTime = 1.0f;
    //   wgpuQueueWriteBuffer(queue, uniformBuffer, 0, &currentTime, sizeof(float));

    return LN_MAKE_SUCCESS();
}

void WebGPUUniformBuffer::onDestroy() {
    if (m_nativeBuffer) {
        wgpuBufferRelease(m_nativeBuffer);
        m_nativeBuffer = nullptr;
    }
    RHIResource::onDestroy();
}

void* WebGPUUniformBuffer::map() {
    m_mappedData.resize(memorySize());
    return m_mappedData.data();

    /*

    auto onBuffer2Mapped =
        [](WGPUMapAsyncStatus status, struct WGPUStringView message, void* userdata1, void* userdata2) {
        std::cout << "Buffer 2 mapped with status " << status << std::endl;
    };
    WGPUBufferMapCallbackInfo callbackInfo = WGPU_BUFFER_MAP_CALLBACK_INFO_INIT;
    callbackInfo.nextInChain = nullptr;
    callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    callbackInfo.callback = onBuffer2Mapped;
    callbackInfo.userdata1 = this;
    callbackInfo.userdata2 = nullptr;
    wgpuBufferMapAsync(m_nativeBuffer, WGPUMapMode_Write, 0, memorySize(), callbackInfo);

    printf("");
    return nullptr;
    */
}

void WebGPUUniformBuffer::unmap() {
    //wgpuBufferUnmap(m_nativeBuffer);
    wgpuQueueWriteBuffer(m_device->wgpuQueue(), m_nativeBuffer, 0, m_mappedData.data(), m_mappedData.size());
}

} // namespace detail
} // namespace ln
