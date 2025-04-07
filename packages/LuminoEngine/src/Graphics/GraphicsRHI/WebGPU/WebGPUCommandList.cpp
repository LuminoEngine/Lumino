#include <LuminoEngine/Graphics/GraphicsRHI/RHIHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUPipeline.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUVertexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUIndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDescriptor.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUCommandList.hpp>

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
	
    WGPUCommandEncoderDescriptor encoderDesc = WGPU_COMMAND_ENCODER_DESCRIPTOR_INIT;
    encoderDesc.nextInChain = nullptr;
    encoderDesc.label = WGPU_STRING_VIEW_INIT;
    m_commandEncoder = wgpuDeviceCreateCommandEncoder(m_rhiDevice->wgpuDevice(), &encoderDesc);
    //m_isRecording = true;
    
    //wgpuCommandEncoderRelease(encoder);
}

void WebGPUCommandList::onEndCommandRecoding() {
    //if (LN_ASSERT(m_isRecording)) return;
    //m_isRecording = false;
}

void WebGPUCommandList::onBeginRenderPass(IRenderPass* renderPass) {
    auto rhiRenderPass = static_cast<WebGPURenderPass*>(renderPass);
    m_renderPassEncoder = wgpuCommandEncoderBeginRenderPass(m_commandEncoderm_commandEncoder, rhiRenderPass->resolve());
}

void WebGPUCommandList::onEndRenderPass(IRenderPass* renderPass) {
    wgpuRenderPassEncoderEnd(m_renderPassEncoder);
    wgpuRenderPassEncoderRelease(m_renderPassEncoder);
    m_renderPassEncoder = nullptr;
}

void WebGPUCommandList::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) {
    if (!m_renderPassEncoder) {
        LN_UNREACHABLE();
        return;
    }

    if (stateDirtyFlags & GraphicsContextStateDirtyFlags_RegionRects) {
        // TODO:
        //wgpuRenderPassEncoderSetViewport

    }

    
    if (submitSource == GraphicsContextSubmitSource_Draw) {
        // Pipeline
        auto* wgpuPipeline = static_cast<WebGPUPipeline*>(pipeline);
        wgpuRenderPassEncoderSetPipeline(m_renderPassEncoder, wgpuPipeline->nativePipeline());
    
        // VertexBuffer
        int vbCount = 0;
        for (int i = 0; i < state.primitive.vertexBuffers.size(); i++) {
            if (state.primitive.vertexBuffers[i]) {
                auto* vertexBuffer = static_cast<WebGPUVertexBuffer*>(state.primitive.vertexBuffers[i]);
                wgpuRenderPassEncoderSetVertexBuffer(
                    m_renderPassEncoder,
                    i,
                    vertexBuffer->nativeBuffer(),
                    0,
                    vertexBuffer->memorySize());
            }
        }

        // IndexBuffer
        if (state.primitive.indexBuffer) {
            auto* indexBuffer = static_cast<WebGPUIndexBuffer*>(state.primitive.indexBuffer);
            wgpuRenderPassEncoderSetIndexBuffer(
                m_renderPassEncoder,
                indexBuffer->nativeBuffer(),
                indexBuffer->indexFormat(),
                0,
                indexBuffer->memorySize());
        }

        // BindGroup
        if (state.descriptor) {
            auto* descriptor = static_cast<WebGPUDescriptor*>(state.descriptor);
            wgpuRenderPassEncoderSetBindGroup(
                m_renderPassEncoder,
                0,
                descriptor->nativeBindGroup(),
                0,
                nullptr);
        }
    }
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
    wgpuRenderPassEncoderDrawIndexed(
        m_renderPassEncoder,
        RHIHelper::getPrimitiveVertexCount(primitive, primitiveCount),
        instanceCount,
        startIndex,
        vertexOffset,
        0);
}


void WebGPUCommandList::onDrawExtension(INativeGraphicsExtension* extension) {
    LN_NOTIMPLEMENTED();
}

void WebGPUCommandList::wait() {
    m_working.wait();
}

} // namespace detail
} // namespace ln
