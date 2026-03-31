#include <lumino_core/graphics/ForwardRenderer.hpp>
#include <lumino_core/graphics/GraphicsContext.hpp>
#include <lumino_core/graphics/PipelineCache.hpp>
#include <lumino_shader/UnifiedShader2.hpp>
#include <lumino_shader/UnifiedShaderSerializer2.hpp>
#include <cstring>

// Precompiled BasicLit shader data for extracting reflection info.
static const unsigned char s_basicLitShaderData[] = {
#include "shaders/BasicLit.lcsh.inl"
};

namespace ln {

// ------ Helpers (same as Material.cpp — local linkage) --------------------------------------------

static rhi::BindingType mapElementKind(shader::ParameterBlockElementKind kind) {
    switch (kind) {
    case shader::ParameterBlockElementKind_ConstantBuffer: return rhi::BindingType::UniformBuffer;
    case shader::ParameterBlockElementKind_Texture:        return rhi::BindingType::SampledTexture;
    case shader::ParameterBlockElementKind_SamplerState:   return rhi::BindingType::Sampler;
    case shader::ParameterBlockElementKind_StorageBuffer:  return rhi::BindingType::StorageBuffer;
    default: return rhi::BindingType::UniformBuffer;
    }
}

static rhi::BindGroupLayoutDesc buildBindGroupLayoutFromReflection(
    const shader::ParameterBlockLayout2& block,
    const shader::TargetBindingLayout2& mergedBindings) {
    rhi::BindGroupLayoutDesc desc;
    for (const auto& binding : mergedBindings.bindings) {
        if (binding.setIndex != block.setIndex) continue;
        rhi::BindGroupLayoutEntry entry;
        entry.binding = binding.bindingIndex;
        entry.type = mapElementKind(binding.kind);
        entry.visibility = static_cast<rhi::ShaderStage>(binding.used);
        desc.entries.push_back(entry);
    }
    return desc;
}

static const shader::ParameterBlockLayout2* findParameterBlock(
    const shader::UnifiedShader2* shader, const std::string& name) {
    for (const auto& block : shader->parameterBlocks()) {
        if (block.name == name) return &block;
    }
    return nullptr;
}

static int16_t findConstantBufferSize(const shader::ParameterBlockLayout2& block) {
    for (const auto& elem : block.elements) {
        if (elem.kind == shader::ParameterBlockElementKind_ConstantBuffer) {
            return elem.constantBufferSize;
        }
    }
    return -1;
}

// ------ ForwardRenderer ----------------------------------------------------------------------------------------------------------

Result<Ref<ForwardRenderer>> ForwardRenderer::create(
    rhi::Device* device,
    rhi::TextureFormat colorFormat,
    rhi::TextureFormat depthFormat) {

    // Load BasicLit shader to extract reflection info for BindGroupLayouts.
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(
        s_basicLitShaderData, sizeof(s_basicLitShaderData));
    if (!loadResult) return tl::make_unexpected(loadResult.error());
    auto unifiedShader = std::move(*loadResult);

    // Find the Forward pass to get merged binding layout.
    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No shader passes found"});
    }
    auto* globalPass = globalPasses[0].get();
    auto targetPassId = globalPass->getTargetShaderPassId(shader::ShaderTarget_SPIRV);
    auto* targetPass = unifiedShader->targetShaderPass(targetPassId);
    if (!targetPass) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No SPIRV target pass"});
    }

    // Find ParameterBlocks by name.
    auto* viewBlock = findParameterBlock(unifiedShader.get(), "viewData");
    auto* materialBlock = findParameterBlock(unifiedShader.get(), "materialData");
    auto* objectBlock = findParameterBlock(unifiedShader.get(), "objectData");
    if (!viewBlock || !materialBlock || !objectBlock) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError,
            "Missing required ParameterBlocks (viewData, materialData, objectData)"});
    }

    auto renderer = Ref<ForwardRenderer>::adopt(new ForwardRenderer());
    renderer->m_colorFormat = colorFormat;
    renderer->m_depthFormat = depthFormat;

    // Get CB sizes from reflection.
    int16_t viewCBSize = findConstantBufferSize(*viewBlock);
    int16_t objectCBSize = findConstantBufferSize(*objectBlock);
    if (viewCBSize <= 0 || objectCBSize <= 0) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError,
            "Invalid constant buffer sizes in reflection"});
    }
    renderer->m_viewUBOSize = static_cast<u64>(viewCBSize);
    renderer->m_objectUBOSize = static_cast<u64>(objectCBSize);

    // ---- Set 0: View BindGroupLayout (from "viewData" reflection) ----
    auto viewBGLDesc = buildBindGroupLayoutFromReflection(*viewBlock, targetPass->bindingLayout);
    auto viewBGLResult = device->createBindGroupLayout(viewBGLDesc);
    if (!viewBGLResult) return tl::make_unexpected(viewBGLResult.error());
    renderer->m_viewBindGroupLayout = std::move(*viewBGLResult);

    // ---- Set 1: Material BindGroupLayout (from "materialData" reflection) ----
    auto matBGLDesc = buildBindGroupLayoutFromReflection(*materialBlock, targetPass->bindingLayout);
    Ref<rhi::BindGroupLayout> matBGL;
    {
        auto r = device->createBindGroupLayout(matBGLDesc);
        if (!r) return tl::make_unexpected(r.error());
        matBGL = std::move(*r);
    }

    // ---- Set 2: Object BindGroupLayout (from "objectData" reflection, dynamic UBO) ----
    auto objBGLDesc = buildBindGroupLayoutFromReflection(*objectBlock, targetPass->bindingLayout);
    for (auto& entry : objBGLDesc.entries) {
        if (entry.type == rhi::BindingType::UniformBuffer) {
            entry.hasDynamicOffset = true;
        }
    }
    auto objBGLResult = device->createBindGroupLayout(objBGLDesc);
    if (!objBGLResult) return tl::make_unexpected(objBGLResult.error());
    renderer->m_objectBindGroupLayout = std::move(*objBGLResult);

    // ---- PipelineLayout (Set 0, Set 1, Set 2) ----
    rhi::PipelineLayoutDesc plDesc;
    plDesc.bindGroupLayouts = {
        renderer->m_viewBindGroupLayout.get(),
        matBGL.get(),
        renderer->m_objectBindGroupLayout.get(),
    };
    auto plResult = device->createPipelineLayout(plDesc);
    if (!plResult) return tl::make_unexpected(plResult.error());
    renderer->m_pipelineLayout = std::move(*plResult);

    // ---- Per-view UBO ----
    rhi::BufferDesc viewBufDesc;
    viewBufDesc.size = renderer->m_viewUBOSize;
    viewBufDesc.usage = rhi::BufferUsage::Uniform;
    auto viewBufResult = device->createBuffer(viewBufDesc);
    if (!viewBufResult) return tl::make_unexpected(viewBufResult.error());
    renderer->m_viewUBO = std::move(*viewBufResult);

    // ---- Per-view BindGroup ----
    rhi::BindGroupDesc viewBGDesc;
    viewBGDesc.layout = renderer->m_viewBindGroupLayout.get();
    viewBGDesc.entries = {
        {0, renderer->m_viewUBO.get(), 0, renderer->m_viewUBOSize, nullptr, nullptr},
    };
    auto viewBGResult = device->createBindGroup(viewBGDesc);
    if (!viewBGResult) return tl::make_unexpected(viewBGResult.error());
    renderer->m_viewBindGroup = std::move(*viewBGResult);

    // ---- Dynamic UBO Allocator for per-object data ----
    auto allocResult = DynamicUniformAllocator::create(
        device, renderer->m_objectBindGroupLayout.get(), 0,
        static_cast<u32>(renderer->m_objectUBOSize));
    if (!allocResult) return tl::make_unexpected(allocResult.error());
    renderer->m_objectAllocator = std::move(*allocResult);

    return renderer;
}

Result<Ref<ForwardRenderer>> ForwardRenderer::create(GraphicsContext* ctx) {
    auto result = create(ctx->device(), ctx->colorFormat(), ctx->depthFormat());
    if (!result) return tl::make_unexpected(result.error());
    (*result)->m_ctx = ctx;
    return result;
}

// ------ Multi-pass API ---------------------------------------------------------------------------------------------------------------

void ForwardRenderer::beginFrame() {
    m_objectAllocator->beginFrame(m_frameCounter++);
    m_currentCmd = m_ctx->device()->getCommandBuffer();
}

void ForwardRenderer::endFrame() {
    m_currentCmd->submit();
    m_currentCmd = nullptr;
}

void ForwardRenderer::beginRenderPass(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Color& clearColor) {

    rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments = {{colorTarget, rhi::LoadOp::Clear, rhi::StoreOp::Store, clearColor}};

    rhi::DepthStencilAttachment depthAttachment;
    if (depthTarget) {
        depthAttachment.view = depthTarget;
        depthAttachment.depthLoadOp = rhi::LoadOp::Clear;
        depthAttachment.depthStoreOp = rhi::StoreOp::Store;
        depthAttachment.clearDepth = 1.0f;
        rpDesc.depthStencilAttachment = &depthAttachment;
    }

    m_currentPass = m_currentCmd->beginRenderPass(rpDesc);

    // Reset deferred bind group state for the new pass.
    for (u32 i = 0; i < kMaxBindGroupSets; ++i) {
        m_passBindGroups[i] = nullptr;
        m_passBindGroupDirty[i] = false;
    }
}

void ForwardRenderer::endRenderPass() {
    m_currentPass->end();
    m_currentPass = nullptr;
}

void ForwardRenderer::setPassBindGroup(u32 setIndex, rhi::BindGroup* bindGroup) {
    if (setIndex < kMaxBindGroupSets) {
        m_passBindGroups[setIndex] = bindGroup;
        m_passBindGroupDirty[setIndex] = true;
    }
}

Result<void> ForwardRenderer::drawMesh(Mesh* mesh, const Transform& transform) {
    m_currentPass->setVertexBuffer(0, mesh->vertexBuffer());
    m_currentPass->setIndexBuffer(mesh->indexBuffer(), rhi::IndexFormat::Uint32);

    for (auto& sub : mesh->submeshes()) {
        Material* mat = nullptr;
        if (sub.materialIndex < mesh->materials().size()) {
            mat = mesh->materials()[sub.materialIndex].get();
        }
        if (!mat && !mesh->materials().empty()) {
            mat = mesh->materials()[0].get();
        }

        if (mat) {
            auto result = drawSubmesh(mesh, mat, transform, sub);
            if (!result) return result;
        }
    }
    return {};
}

Result<void> ForwardRenderer::drawMesh(Mesh* mesh, const Transform& transform, Material* material) {
    m_currentPass->setVertexBuffer(0, mesh->vertexBuffer());
    m_currentPass->setIndexBuffer(mesh->indexBuffer(), rhi::IndexFormat::Uint32);

    for (auto& sub : mesh->submeshes()) {
        auto result = drawSubmesh(mesh, material, transform, sub);
        if (!result) {
            return result;
        }
    }
    return {};
}

void ForwardRenderer::flushPassBindGroups() {
    for (u32 i = 0; i < kMaxBindGroupSets; ++i) {
        if (m_passBindGroupDirty[i] && m_passBindGroups[i]) {
            m_currentPass->setBindGroup(i, m_passBindGroups[i]);
            m_passBindGroupDirty[i] = false;
        }
    }
}

Result<void> ForwardRenderer::drawSubmesh(
    Mesh* mesh, Material* mat, const Transform& transform, const SubMesh& sub) {

    auto* pipelineCache = m_ctx->pipelineCache();

    // Sub-allocate from the dynamic UBO and write object params.
    auto alloc = m_objectAllocator->allocate();
    {
        Matrix4x4 worldMatrix = transform.matrix();
        Matrix4x4 normalMatrix = transform.normalMatrix();
        ObjectParamsUBO objParams{};
        std::memcpy(objParams.world, worldMatrix.m, sizeof(f32) * 16);
        std::memcpy(objParams.normalMatrix, normalMatrix.m, sizeof(f32) * 16);
        std::memcpy(alloc.cpuPtr, &objParams, sizeof(objParams));
    }

    PipelineCacheKey key;
    key.vertexShader    = mat->vertexShader();
    key.fragmentShader  = mat->fragmentShader();
    key.vertexEntry     = mat->vertexEntry();
    key.fragmentEntry   = mat->fragmentEntry();
    key.cullMode        = mat->cullMode();
    key.blendEnabled    = mat->blendEnabled();
    if (key.blendEnabled) {
        key.blendState.enabled  = true;
        key.blendState.srcColor = rhi::BlendFactor::SrcAlpha;
        key.blendState.dstColor = rhi::BlendFactor::OneMinusSrcAlpha;
        key.blendState.colorOp  = rhi::BlendOp::Add;
        key.blendState.srcAlpha = rhi::BlendFactor::One;
        key.blendState.dstAlpha = rhi::BlendFactor::OneMinusSrcAlpha;
        key.blendState.alphaOp  = rhi::BlendOp::Add;
    }
    key.depthTestEnabled  = mat->depthTestEnabled();
    key.depthWriteEnabled = mat->depthWriteEnabled();
    key.pipelineLayout    = m_pipelineLayout.get();
    key.topology          = mesh->topology();
    key.colorFormat       = m_colorFormat;
    key.depthStencilFormat = m_depthFormat;
    key.sampleCount       = 1;

    auto pipelineResult = pipelineCache->getOrCreate(key);
    if (!pipelineResult) return tl::make_unexpected(pipelineResult.error());

    m_currentPass->setPipeline(*pipelineResult);
    flushPassBindGroups();
    m_currentPass->setBindGroup(1, mat->materialBindGroup());
    m_currentPass->setBindGroup(2, alloc.bindGroup, &alloc.dynamicOffset, 1);
    m_currentPass->drawIndexed(sub.indexCount, 1, sub.indexOffset);

    return {};
}

Result<void> ForwardRenderer::drawScreenRect(Material* material) {
    auto meshResult = getScreenRectMesh();
    if (!meshResult) return tl::make_unexpected(meshResult.error());

    // Screen rect uses identity transform (NDC coordinates).
    Transform identity;
    return drawMesh(*meshResult, identity, material);
}

Result<Mesh*> ForwardRenderer::getScreenRectMesh() {
    if (m_screenRectMesh) return m_screenRectMesh.get();

    // Create a fullscreen quad in NDC space: covers [-1, 1] x [-1, 1].
    std::vector<Vertex> vertices = {
        // position,             normal,              uv,       color,             tangent
        {{-1.0f, -1.0f, 0.0f}, {0, 0, 1}, {0, 1}, Color::white(), {1, 0, 0, 1}},
        {{ 1.0f, -1.0f, 0.0f}, {0, 0, 1}, {1, 1}, Color::white(), {1, 0, 0, 1}},
        {{ 1.0f,  1.0f, 0.0f}, {0, 0, 1}, {1, 0}, Color::white(), {1, 0, 0, 1}},
        {{-1.0f,  1.0f, 0.0f}, {0, 0, 1}, {0, 0}, Color::white(), {1, 0, 0, 1}},
    };
    std::vector<u32> indices = {0, 1, 2, 0, 2, 3};
    SubMesh sub;
    sub.indexOffset = 0;
    sub.indexCount = 6;
    sub.materialIndex = 0;

    auto result = Mesh::create(m_ctx->device(), vertices, indices, {sub});
    if (!result) return tl::make_unexpected(result.error());
    m_screenRectMesh = std::move(*result);
    return m_screenRectMesh.get();
}

// ------ High-level convenience (renderFrame) -----------------------------------------------------------------------------------------

Result<void> ForwardRenderer::renderFrame(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Camera& camera,
    const std::vector<RenderObject>& objects,
    const Color& clearColor) {

    // ---- Update View UBO ----
    {
        ViewParamsUBO viewParams{};
        Matrix4x4 vp = camera.viewProjectionMatrix();
        std::memcpy(viewParams.viewProj, vp.m, sizeof(f32) * 16);

        Vector3 camPos = camera.position();
        viewParams.cameraPos[0] = camPos.x;
        viewParams.cameraPos[1] = camPos.y;
        viewParams.cameraPos[2] = camPos.z;
        viewParams.cameraPos[3] = 0.0f;

        Vector3 ld = m_light.direction.normalized();
        viewParams.lightDir[0] = ld.x;
        viewParams.lightDir[1] = ld.y;
        viewParams.lightDir[2] = ld.z;
        viewParams.lightDir[3] = 0.0f;

        viewParams.lightColor[0] = m_light.color.r;
        viewParams.lightColor[1] = m_light.color.g;
        viewParams.lightColor[2] = m_light.color.b;
        viewParams.lightColor[3] = m_light.color.a;

        viewParams.ambientColor[0] = m_light.ambient.r;
        viewParams.ambientColor[1] = m_light.ambient.g;
        viewParams.ambientColor[2] = m_light.ambient.b;
        viewParams.ambientColor[3] = m_light.ambient.a;

        void* mapped = m_viewUBO->map();
        if (mapped) {
            std::memcpy(mapped, &viewParams, sizeof(viewParams));
            m_viewUBO->unmap();
        }
    }

    // ---- Use multi-pass API ----
    beginFrame();

    beginRenderPass(colorTarget, depthTarget, clearColor);
    setPassBindGroup(0, m_viewBindGroup.get());

    for (auto& obj : objects) {
        auto result = drawMesh(obj.mesh.get(), obj.transform);
        if (!result) return result;
    }

    endRenderPass();
    endFrame();

    return {};
}

} // namespace ln
