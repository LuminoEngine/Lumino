#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUShaderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUVertexLayout.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderTarget.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDepthBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUPipeline.hpp>

namespace ln {
namespace detail {

WGPUBlendFactor toWGPUBlendFactor(BlendFactor value) {
    switch (value) {
        case BlendFactor::Zero:
            return WGPUBlendFactor_Zero;
        case BlendFactor::One:
            return WGPUBlendFactor_One;
        case BlendFactor::SourceColor:
            return WGPUBlendFactor_Src;
        case BlendFactor::InverseSourceColor:
            return WGPUBlendFactor_OneMinusSrc;
        case BlendFactor::SourceAlpha:
            return WGPUBlendFactor_SrcAlpha;
        case BlendFactor::InverseSourceAlpha:
            return WGPUBlendFactor_OneMinusSrcAlpha;
        case BlendFactor::DestinationColor:
            return WGPUBlendFactor_Dst;
        case BlendFactor::InverseDestinationColor:
            return WGPUBlendFactor_OneMinusDst;
        case BlendFactor::DestinationAlpha:
            return WGPUBlendFactor_DstAlpha;
        case BlendFactor::InverseDestinationAlpha:
            return WGPUBlendFactor_OneMinusDstAlpha;
        default:
            assert(false);
            return WGPUBlendFactor_Zero;
    }
}
WGPUBlendOperation toWGPUBlendOperation(BlendOp value) {
    switch (value) {
        case BlendOp::Add:
            return WGPUBlendOperation_Add;
        case BlendOp::Subtract:
            return WGPUBlendOperation_Subtract;
        case BlendOp::ReverseSubtract:
            return WGPUBlendOperation_ReverseSubtract;
        case BlendOp::Min:
            return WGPUBlendOperation_Min;
        case BlendOp::Max:
            return WGPUBlendOperation_Max;
        default:
            assert(false);
            return WGPUBlendOperation_Add;
    }
}

WGPUCompareFunction toWGPUCompareFunction(ComparisonFunc value) {
    switch (value) {
        case ComparisonFunc::Never:
            return WGPUCompareFunction_Never;
        case ComparisonFunc::Less:
            return WGPUCompareFunction_Less;
            break;
        case ComparisonFunc::LessEqual:
            return WGPUCompareFunction_LessEqual;
            break;
        case ComparisonFunc::Greater:
            return WGPUCompareFunction_Greater;
            break;
        case ComparisonFunc::GreaterEqual:
            return WGPUCompareFunction_GreaterEqual;
            break;
        case ComparisonFunc::Equal:
            return WGPUCompareFunction_Equal;
            break;
        case ComparisonFunc::NotEqual:
            return WGPUCompareFunction_NotEqual;
            break;
        case ComparisonFunc::Always:
            return WGPUCompareFunction_Always;
            break;
        default:
            LN_UNREACHABLE();
            return WGPUCompareFunction_Never;
    }
}

WGPUStencilOperation toWGPUStencilOperation(StencilOp value) {
    switch (value) {
        case StencilOp::Keep:
            return WGPUStencilOperation_Keep;
        case StencilOp::Replace:
            return WGPUStencilOperation_Replace;
        default:
            LN_UNREACHABLE();
            return WGPUStencilOperation_Keep;
    }
}

WGPUStencilFaceState toWGPUStencilFaceState(const StencilOpDesc& value) {
    WGPUStencilFaceState result = WGPU_STENCIL_FACE_STATE_INIT;
    result.compare = toWGPUCompareFunction(value.stencilFunc);
    result.failOp = toWGPUStencilOperation(value.stencilFailOp);
    result.depthFailOp = toWGPUStencilOperation(value.stencilDepthFailOp);
    result.passOp = toWGPUStencilOperation(value.stencilPassOp);
    return result;
}

//==============================================================================
// WebGPUPipeline

WebGPUPipeline::WebGPUPipeline()
    : m_wgpuDevice(nullptr)
    , m_nativePipeline(nullptr) {
}

WebGPUPipeline::~WebGPUPipeline() {
}

MaybeResult_deprecated WebGPUPipeline::init(WebGPUDevice* wgpuDevice, const DevicePipelineStateDesc& state) {
    m_wgpuDevice = wgpuDevice;
    WGPUDevice nativeDevice = wgpuDevice->wgpuDevice();
    WebGPUShaderPass* shaderPass = static_cast<WebGPUShaderPass*>(state.shaderPass);
    WebGPURenderPass* renderPass = static_cast<WebGPURenderPass*>(state.renderPass);
    WebGPUVertexLayout* vertexLayout = static_cast<WebGPUVertexLayout*>(state.vertexDeclaration);

    WebGPUPipelineVertexLayout pipelineVertexLayout;
    auto result1 = vertexLayout->createPipelineVertexLayout(shaderPass, &pipelineVertexLayout);
    if (!result1) return result1;
    
    WGPURenderPipelineDescriptor pipelineDesc = WGPU_RENDER_PIPELINE_DESCRIPTOR_INIT;
    pipelineDesc.nextInChain = nullptr;

    // WGPUVertexState
    pipelineDesc.vertex.module = shaderPass->nativeVertShaderModule();
#ifdef LN_WEBGPU_LEGACY
    pipelineDesc.vertex.entryPoint = shaderPass->vertEntryPointName().c_str();
#else
    pipelineDesc.vertex.entryPoint.data = shaderPass->vertEntryPointName().c_str();
    pipelineDesc.vertex.entryPoint.length = shaderPass->vertEntryPointName().length();
#endif
    pipelineDesc.vertex.constantCount = 0;
    pipelineDesc.vertex.constants = nullptr;
    pipelineDesc.vertex.bufferCount = pipelineVertexLayout.bufferLayouts.size();
    pipelineDesc.vertex.buffers = pipelineVertexLayout.bufferLayouts.data();

    // WGPUPrimitiveState
    switch (state.topology) {
        case PrimitiveTopology::TriangleList:
            pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
            break;
        case PrimitiveTopology::TriangleStrip:
            pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleStrip;
            break;
        case PrimitiveTopology::LineList:
            pipelineDesc.primitive.topology = WGPUPrimitiveTopology_LineList;
            break;
        case PrimitiveTopology::LineStrip:
            pipelineDesc.primitive.topology = WGPUPrimitiveTopology_LineStrip;
            break;
        case PrimitiveTopology::PointList:
            pipelineDesc.primitive.topology = WGPUPrimitiveTopology_PointList;
            break;
        default:
            return LN_MAKE_ERROR_deprecated("Unsupported topology %d", state.topology);
    }
    pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
    pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
    switch (state.rasterizerState.cullMode) {
        case CullMode::None:
            pipelineDesc.primitive.cullMode = WGPUCullMode_None;
            break;
        case CullMode::Front:
            pipelineDesc.primitive.cullMode = WGPUCullMode_Front;
            break;
        case CullMode::Back:
            pipelineDesc.primitive.cullMode = WGPUCullMode_Back;
            break;
        default:
            return LN_MAKE_ERROR_deprecated("Unsupported cull mode %d", state.rasterizerState.cullMode);
    }
#ifndef LN_WEBGPU_LEGACY
    pipelineDesc.primitive.unclippedDepth = 0;
#endif

    // WGPUFragmentState
    std::array<WGPUColorTargetState, MaxMultiRenderTargets> colorTargetStates;
    std::array<WGPUBlendState, MaxMultiRenderTargets> blendStates;
    WGPUFragmentState fragmentState = WGPU_FRAGMENT_STATE_INIT;
    {
        fragmentState.module = shaderPass->nativeFragShaderModule();
#ifdef LN_WEBGPU_LEGACY
        fragmentState.entryPoint = shaderPass->fragEntryPointName().c_str();
#else
        fragmentState.entryPoint.data = shaderPass->fragEntryPointName().c_str();
        fragmentState.entryPoint.length = shaderPass->fragEntryPointName().length();
#endif
        fragmentState.constantCount = 0;
        fragmentState.constants = nullptr;

        int targetCount = renderPass->getAvailableRenderTargetCount();
        for (int i = 0; i < targetCount; i++) {
            WebGPURenderTarget* renderTarget = static_cast<WebGPURenderTarget*>(
                renderPass->m_renderTargets[i]);
            const RenderTargetBlendDesc& desc = state.blendState.independentBlendEnable
                ? state.blendState.renderTargets[0]
                : state.blendState.renderTargets[i];
            WGPUBlendState& blendState = blendStates[i];
            blendState = WGPU_BLEND_STATE_INIT;
            blendState.color.srcFactor = toWGPUBlendFactor(desc.sourceBlend);
            blendState.color.dstFactor = toWGPUBlendFactor(desc.destinationBlend);
            blendState.color.operation = toWGPUBlendOperation(desc.blendOp);
            blendState.alpha.srcFactor = toWGPUBlendFactor(desc.sourceBlendAlpha);
            blendState.alpha.dstFactor = toWGPUBlendFactor(desc.destinationBlendAlpha);
            blendState.alpha.operation = toWGPUBlendOperation(desc.blendOpAlpha);

            WGPUColorTargetState& colorTargetState = colorTargetStates[i];
            colorTargetState = WGPU_COLOR_TARGET_STATE_INIT;
            colorTargetState.format = renderTarget->nativeFormat();
            colorTargetState.blend = &blendState;
            colorTargetState.writeMask =
                WGPUColorWriteMask_All; // We could write to only some of the color channels.
        }

        fragmentState.targetCount = targetCount;
        fragmentState.targets = colorTargetStates.data();
        pipelineDesc.fragment = &fragmentState;
    }

    // WGPUDepthStencilState
    WGPUDepthStencilState depthStencilState = WGPU_DEPTH_STENCIL_STATE_INIT;
    if (renderPass->m_depthBuffer) {
        WebGPUDepthBuffer* depthBuffer = static_cast<WebGPUDepthBuffer*>(renderPass->m_depthBuffer);
        depthStencilState.format = depthBuffer->nativeFormat();

#ifdef LN_WEBGPU_LEGACY
        depthStencilState.depthWriteEnabled = state.depthStencilState.depthWriteEnabled ? 1
                                                                                        : 0;
#else
        depthStencilState.depthWriteEnabled = state.depthStencilState.depthWriteEnabled ? WGPUOptionalBool_True
                                                                                        : WGPUOptionalBool_False;
#endif

        depthStencilState.depthCompare = toWGPUCompareFunction(state.depthStencilState.depthTestFunc);
        depthStencilState.stencilFront = toWGPUStencilFaceState(state.depthStencilState.frontFace);
        depthStencilState.stencilBack = toWGPUStencilFaceState(state.depthStencilState.backFace);
        depthStencilState.stencilReadMask = 0xFFFFFFFF;
        depthStencilState.stencilWriteMask = 0xFFFFFFFF;
        depthStencilState.depthBias = 0;
        depthStencilState.depthBiasSlopeScale = 0.0f;
        depthStencilState.depthBiasClamp = 0.0f;

        pipelineDesc.depthStencil = &depthStencilState;
    }
    else {
        pipelineDesc.depthStencil = nullptr;
    }

	// WGPUMultisampleState
    pipelineDesc.multisample.count = 1; // Samples per pixel
    pipelineDesc.multisample.mask = ~0u; // Default value for the mask, meaning "all bits on"
    pipelineDesc.multisample.alphaToCoverageEnabled = false; // Default value as well (irrelevant for count = 1 anyways)

    // WGPUPipelineLayout
    pipelineDesc.layout = shaderPass->pipelineLayout();

    m_nativePipeline = wgpuDeviceCreateRenderPipeline(nativeDevice, &pipelineDesc);
    if (!m_nativePipeline) {
        return LN_MAKE_ERROR_deprecated("Failed wgpuDeviceCreateRenderPipeline");
    }
    return LN_MAKE_SUCCESS();
}

void WebGPUPipeline::onDestroy() {
    if (m_nativePipeline) {
        wgpuRenderPipelineRelease(m_nativePipeline);
        m_nativePipeline = nullptr;
    }
    IPipeline::onDestroy();
}

} // namespace detail
} // namespace ln
