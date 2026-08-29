#include "WebGPURenderPipeline.hpp"
#include "WebGPUDevice.hpp"
#include "WebGPUShaderModule.hpp"
#include "WebGPUPipelineLayout.hpp"
#include "WebGPUHelpers.hpp"
#include <array>
#include <vector>

namespace ln::rhi::webgpu {

WebGPURenderPipeline::WebGPURenderPipeline() = default;

VoidResult WebGPURenderPipeline::init(WebGPUDevice* device, const RenderPipelineDesc& desc) {
    m_device = device;

    if (!desc.vertexShader || !desc.fragmentShader) {
        return LN_MAKE_ERROR("RenderPipelineDesc requires vertex and fragment shaders.");
    }

    // ラベルは WebGPU のエラーメッセージ (`[Invalid RenderPipeline "..."]`) に現れるため、
    // どのシェーダのどのパスなのかが分かる名前を入れる。
    m_debugName = desc.debugName.empty() ? std::string("LuminoPipeline") : desc.debugName;

    WGPURenderPipelineDescriptor pipelineDesc = WGPU_RENDER_PIPELINE_DESCRIPTOR_INIT;
    pipelineDesc.label = {m_debugName.c_str(), m_debugName.size()};

    // レイアウト
    pipelineDesc.layout = desc.layout
        ? static_cast<WebGPUPipelineLayout*>(desc.layout)->handle()
        : nullptr;

    // ---- Vertex state ----
    auto* vsModule = static_cast<WebGPUShaderModule*>(desc.vertexShader);
    pipelineDesc.vertex.module = vsModule->handle();
    pipelineDesc.vertex.entryPoint = {desc.vertexEntry.c_str(), desc.vertexEntry.size()};
    pipelineDesc.vertex.constantCount = 0;
    pipelineDesc.vertex.constants = nullptr;

    // 頂点バッファレイアウト - 属性の格納領域は wgpuDeviceCreateRenderPipeline() より長く生存させる必要がある。
    std::vector<std::vector<WGPUVertexAttribute>> attributeStorage(desc.vertexBuffers.size());
    std::vector<WGPUVertexBufferLayout> bufferLayouts(desc.vertexBuffers.size());
    for (size_t i = 0; i < desc.vertexBuffers.size(); ++i) {
        const auto& vb = desc.vertexBuffers[i];
        auto& attrs = attributeStorage[i];
        attrs.reserve(vb.attributes.size());
        for (const auto& a : vb.attributes) {
            WGPUVertexAttribute wa{};
            wa.format = toWGPUVertexFormat(a.format);
            wa.offset = a.offset;
            wa.shaderLocation = a.location;
            attrs.push_back(wa);
        }
        bufferLayouts[i] = WGPU_VERTEX_BUFFER_LAYOUT_INIT;
        bufferLayouts[i].arrayStride = vb.stride;
        bufferLayouts[i].stepMode = vb.perInstance
            ? WGPUVertexStepMode_Instance
            : WGPUVertexStepMode_Vertex;
        bufferLayouts[i].attributeCount = attrs.size();
        bufferLayouts[i].attributes = attrs.data();
    }
    pipelineDesc.vertex.bufferCount = bufferLayouts.size();
    pipelineDesc.vertex.buffers = bufferLayouts.data();

    // ---- Primitive state ----
    pipelineDesc.primitive.topology = toWGPUPrimitiveTopology(desc.topology);
    pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
    pipelineDesc.primitive.frontFace = toWGPUFrontFace(desc.frontFace);
    pipelineDesc.primitive.cullMode = toWGPUCullMode(desc.cullMode);
    pipelineDesc.primitive.unclippedDepth = false;

    // ---- Fragment state ----
    const auto& renderPassLayout = desc.renderPassLayout;

    std::vector<WGPUBlendState> blendStates(renderPassLayout.colorFormats.size());
    std::vector<WGPUColorTargetState> colorTargets(renderPassLayout.colorFormats.size());
    for (size_t i = 0; i < renderPassLayout.colorFormats.size(); ++i) {
        // このターゲットの BlendState を選ぶ (無ければインデックス 0、それも無ければデフォルト)。
        BlendState bs;
        if (i < desc.blendStates.size()) {
            bs = desc.blendStates[i];
        } else if (!desc.blendStates.empty()) {
            bs = desc.blendStates[0];
        }

        blendStates[i] = WGPU_BLEND_STATE_INIT;
        blendStates[i].color.srcFactor = toWGPUBlendFactor(bs.srcColor);
        blendStates[i].color.dstFactor = toWGPUBlendFactor(bs.dstColor);
        blendStates[i].color.operation = toWGPUBlendOperation(bs.colorOp);
        blendStates[i].alpha.srcFactor = toWGPUBlendFactor(bs.srcAlpha);
        blendStates[i].alpha.dstFactor = toWGPUBlendFactor(bs.dstAlpha);
        blendStates[i].alpha.operation = toWGPUBlendOperation(bs.alphaOp);

        colorTargets[i] = WGPU_COLOR_TARGET_STATE_INIT;
        colorTargets[i].format = toWGPUTextureFormat(renderPassLayout.colorFormats[i]);
        colorTargets[i].blend = bs.enabled ? &blendStates[i] : nullptr;
        colorTargets[i].writeMask = bs.colorWriteEnabled
            ? WGPUColorWriteMask_All
            : WGPUColorWriteMask_None;
    }

    auto* fsModule = static_cast<WebGPUShaderModule*>(desc.fragmentShader);
    WGPUFragmentState fragmentState = WGPU_FRAGMENT_STATE_INIT;
    fragmentState.module = fsModule->handle();
    fragmentState.entryPoint = {desc.fragmentEntry.c_str(), desc.fragmentEntry.size()};
    fragmentState.constantCount = 0;
    fragmentState.constants = nullptr;
    fragmentState.targetCount = colorTargets.size();
    fragmentState.targets = colorTargets.data();
    pipelineDesc.fragment = &fragmentState;

    // ---- Depth-stencil state ----
    WGPUDepthStencilState depthStencilState = WGPU_DEPTH_STENCIL_STATE_INIT;
    if (renderPassLayout.depthStencilFormat != TextureFormat::Undefined) {
        depthStencilState.format = toWGPUTextureFormat(renderPassLayout.depthStencilFormat);
        depthStencilState.depthWriteEnabled = desc.depthStencil.depthWriteEnable
            ? WGPUOptionalBool_True
            : WGPUOptionalBool_False;
        depthStencilState.depthCompare = desc.depthStencil.depthTestEnable
            ? toWGPUCompareFunction(desc.depthStencil.depthCompare)
            : WGPUCompareFunction_Always;

        depthStencilState.stencilFront.compare = toWGPUCompareFunction(desc.depthStencil.stencilFront.compare);
        depthStencilState.stencilFront.failOp = toWGPUStencilOperation(desc.depthStencil.stencilFront.failOp);
        depthStencilState.stencilFront.depthFailOp = toWGPUStencilOperation(desc.depthStencil.stencilFront.depthFailOp);
        depthStencilState.stencilFront.passOp = toWGPUStencilOperation(desc.depthStencil.stencilFront.passOp);

        depthStencilState.stencilBack.compare = toWGPUCompareFunction(desc.depthStencil.stencilBack.compare);
        depthStencilState.stencilBack.failOp = toWGPUStencilOperation(desc.depthStencil.stencilBack.failOp);
        depthStencilState.stencilBack.depthFailOp = toWGPUStencilOperation(desc.depthStencil.stencilBack.depthFailOp);
        depthStencilState.stencilBack.passOp = toWGPUStencilOperation(desc.depthStencil.stencilBack.passOp);

        depthStencilState.stencilReadMask = desc.depthStencil.stencilReadMask;
        depthStencilState.stencilWriteMask = desc.depthStencil.stencilWriteMask;
        depthStencilState.depthBias = 0;
        depthStencilState.depthBiasSlopeScale = 0.0f;
        depthStencilState.depthBiasClamp = 0.0f;

        pipelineDesc.depthStencil = &depthStencilState;
    } else {
        pipelineDesc.depthStencil = nullptr;
    }

    // ---- Multisample state ----
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = ~0u;
    pipelineDesc.multisample.alphaToCoverageEnabled = false;

    m_pipeline = wgpuDeviceCreateRenderPipeline(m_device->wgpuDevice(), &pipelineDesc);
    if (!m_pipeline) {
        return LN_MAKE_ERROR("wgpuDeviceCreateRenderPipeline failed. (%s)", m_debugName.c_str());
    }
    return LN_MAKE_SUCCESS();
}

void WebGPURenderPipeline::finalize() {
    if (m_pipeline) {
        wgpuRenderPipelineRelease(m_pipeline);
        m_pipeline = nullptr;
    }
    RenderPipeline::finalize();
}

} // namespace ln::rhi::webgpu
