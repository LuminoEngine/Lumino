#include <LuminoEngine/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/GraphicsRHI/WebGPU/WebGPURenderPass.hpp>
#include <LuminoEngine/GraphicsRHI/WebGPU/WebGPUCommandList.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPUCommandList

WebGPUCommandList::WebGPUCommandList()
    : m_rhiDevice(nullptr)
    , m_commandEncoder(nullptr)
    , m_renderPassEncoder(nullptr)
    , m_working()
    //, m_isRecording(false)
{
}

Result<> WebGPUCommandList::init(WebGPUDevice* rhiDevice) {
    m_rhiDevice = rhiDevice;
    return ok();
}

void WebGPUCommandList::onDestroy() {
    ICommandList::onDestroy();
}

void WebGPUCommandList::onSubmitted() {
    m_working.lock();
}

void WebGPUCommandList::onSubmittedWorkDone() {
    m_working.unlock();
}

void WebGPUCommandList::onSaveExternalRenderState() {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onRestoreExternalRenderState() {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onBeginCommandRecoding() {
    //if (LN_ASSERT(!m_isRecording)) return;
	
    WGPUCommandEncoderDescriptor encoderDesc = {};
    encoderDesc.nextInChain = nullptr;
    encoderDesc.label = nullptr;
    m_commandEncoder = wgpuDeviceCreateCommandEncoder(m_rhiDevice->wgpuDevice(), &encoderDesc);
    //m_isRecording = true;
}

void WebGPUCommandList::onEndCommandRecoding() {
    //if (LN_ASSERT(m_isRecording)) return;
    //m_isRecording = false;
}

void WebGPUCommandList::onBeginRenderPass(IRenderPass* renderPass) {
    auto rhiRenderPass = static_cast<WebGPURenderPass*>(renderPass);
    m_renderPassEncoder = wgpuCommandEncoderBeginRenderPass(m_commandEncoder, rhiRenderPass->resolve());
}

void WebGPUCommandList::onEndRenderPass(IRenderPass* renderPass) {
    wgpuRenderPassEncoderEnd(m_renderPassEncoder);
    m_renderPassEncoder = nullptr;
}

void WebGPUCommandList::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onSetSubData(RHIResource* resource, size_t offset, const void* data, size_t length) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onDispatch(const GraphicsContextState& state, IPipeline* basePipeline, int groupCountX, int groupCountY, int groupCountZ) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount, int instanceCount) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::onDrawExtension(INativeGraphicsExtension* extension) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::wait() {
    m_working.wait();
}

} // namespace detail
} // namespace ln
