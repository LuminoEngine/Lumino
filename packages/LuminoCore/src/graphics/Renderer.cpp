#include <LuminoCore/graphics/Renderer.hpp>
#include <LuminoCore/graphics/GraphicsContext.hpp>
#include <LuminoCore/graphics/PipelineCache.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include <cstring>

// Precompiled BasicLit shader data for extracting BindGroupLayout reflection.
static const unsigned char s_basicLitShaderData[] = {
#include "shaders/BasicLit.lcsh.inl"
};

namespace ln {

// ------ Reflection helpers (local linkage) -----------------------------------------------------------------------

static rhi::BindingType mapElementKind(shader::ParameterBlockElementKind kind) {
    switch (kind) {
    case shader::ParameterBlockElementKind_ConstantBuffer: return rhi::BindingType::UniformBuffer;
    case shader::ParameterBlockElementKind_Texture:        return rhi::BindingType::SampledTexture;
    case shader::ParameterBlockElementKind_SamplerState:   return rhi::BindingType::Sampler;
    case shader::ParameterBlockElementKind_StorageBuffer:  return rhi::BindingType::StorageBuffer;
    default:                                               return rhi::BindingType::UniformBuffer;
    }
}

static rhi::BindGroupLayoutDesc buildBindGroupLayoutFromReflection(
    const shader::ParameterBlockLayout2& block,
    const shader::TargetBindingLayout2& mergedBindings) {

    rhi::BindGroupLayoutDesc desc;
    for (const auto& binding : mergedBindings.bindings) {
        if (binding.setIndex != block.setIndex) continue;
        rhi::BindGroupLayoutEntry entry;
        entry.binding    = binding.bindingIndex;
        entry.type       = mapElementKind(binding.kind);
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

// ------ Renderer::create ------------------------------------------------------------------------------------------

Result<Ref<Renderer>> Renderer::create(GraphicsContext* ctx) {
    auto result = create(ctx->device(), ctx->colorFormat(), ctx->depthFormat());
    if (!result) return tl::make_unexpected(result.error());
    (*result)->m_ctx = ctx;
    return result;
}

Result<Ref<Renderer>> Renderer::create(
    rhi::Device* device,
    rhi::TextureFormat colorFormat,
    rhi::TextureFormat depthFormat) {

    // Load BasicLit shader to extract BindGroupLayout info via reflection.
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(
        s_basicLitShaderData, sizeof(s_basicLitShaderData));
    if (!loadResult) return tl::make_unexpected(loadResult.error());
    auto unifiedShader = std::move(*loadResult);

    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No shader passes found"});
    }
    auto targetPassId = globalPasses[0]->getTargetShaderPassId(shader::ShaderTarget_SPIRV);
    auto* targetPass  = unifiedShader->targetShaderPass(targetPassId);
    if (!targetPass) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No SPIRV target pass"});
    }

    auto* viewBlock     = findParameterBlock(unifiedShader.get(), "viewData");
    auto* sceneBlock    = findParameterBlock(unifiedShader.get(), "sceneData");
    auto* materialBlock = findParameterBlock(unifiedShader.get(), "materialData");
    auto* objectBlock   = findParameterBlock(unifiedShader.get(), "objectData");
    if (!viewBlock || !sceneBlock || !materialBlock || !objectBlock) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError,
            "Missing required ParameterBlocks (viewData, sceneData, materialData, objectData)"});
    }

    int16_t objectCBSize = findConstantBufferSize(*objectBlock);
    if (objectCBSize <= 0) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError,
            "Invalid object constant buffer size in reflection"});
    }

    auto renderer = Ref<Renderer>::adopt(new Renderer());
    renderer->m_colorFormat   = colorFormat;
    renderer->m_depthFormat   = depthFormat;
    renderer->m_objectUBOSize = static_cast<u64>(objectCBSize);

    // ---- Set 0: View BindGroupLayout (camera) ----
    {
        auto desc = buildBindGroupLayoutFromReflection(*viewBlock, targetPass->bindingLayout);
        auto r    = device->createBindGroupLayout(desc);
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_viewBindGroupLayout = std::move(*r);
    }

    // ---- Set 1: Scene BindGroupLayout (lighting, etc.) ----
    {
        auto desc = buildBindGroupLayoutFromReflection(*sceneBlock, targetPass->bindingLayout);
        auto r    = device->createBindGroupLayout(desc);
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_sceneBindGroupLayout = std::move(*r);
    }

    // ---- Set 2: Material BindGroupLayout (used only for PipelineLayout construction) ----
    Ref<rhi::BindGroupLayout> matBGL;
    {
        auto desc = buildBindGroupLayoutFromReflection(*materialBlock, targetPass->bindingLayout);
        auto r    = device->createBindGroupLayout(desc);
        if (!r) return tl::make_unexpected(r.error());
        matBGL = std::move(*r);
    }

    // ---- Set 3: Object BindGroupLayout (dynamic UBO) ----
    {
        auto desc = buildBindGroupLayoutFromReflection(*objectBlock, targetPass->bindingLayout);
        for (auto& entry : desc.entries) {
            if (entry.type == rhi::BindingType::UniformBuffer) {
                entry.hasDynamicOffset = true;
            }
        }
        auto r = device->createBindGroupLayout(desc);
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_objectBindGroupLayout = std::move(*r);
    }

    // ---- PipelineLayout (Set 0, 1, 2, 3) ----
    {
        rhi::PipelineLayoutDesc plDesc;
        plDesc.bindGroupLayouts = {
            renderer->m_viewBindGroupLayout.get(),
            renderer->m_sceneBindGroupLayout.get(),
            matBGL.get(),
            renderer->m_objectBindGroupLayout.get(),
        };
        auto r = device->createPipelineLayout(plDesc);
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_pipelineLayout = std::move(*r);
    }

    // ---- View UBO (camera data, set=0) ----
    {
        renderer->m_viewUBOSize = sizeof(ViewParamsUBO);
        rhi::BufferDesc bufDesc;
        bufDesc.size  = renderer->m_viewUBOSize;
        bufDesc.usage = rhi::BufferUsage::Uniform;
        auto r = device->createBuffer(bufDesc);
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_viewUBO = std::move(*r);

        rhi::BindGroupDesc bgDesc;
        bgDesc.layout  = renderer->m_viewBindGroupLayout.get();
        bgDesc.entries = {
            {0, renderer->m_viewUBO.get(), 0, renderer->m_viewUBOSize, nullptr, nullptr},
        };
        auto bgr = device->createBindGroup(bgDesc);
        if (!bgr) return tl::make_unexpected(bgr.error());
        renderer->m_viewBindGroup = std::move(*bgr);
    }

    // ---- Dynamic UBO allocator for per-object data ----
    {
        auto r = DynamicUniformAllocator::create(
            device, renderer->m_objectBindGroupLayout.get(), 0,
            static_cast<u32>(renderer->m_objectUBOSize));
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_objectAllocator = std::move(*r);
    }

    return renderer;
}

// ------ Frame lifecycle -------------------------------------------------------------------------------------------

void Renderer::beginFrame() {
    m_objectAllocator->beginFrame(m_frameCounter++);
    m_currentCmd = m_ctx->device()->getCommandBuffer();
}

void Renderer::endFrame() {
    m_currentCmd->submit();
    m_currentCmd = nullptr;
}

// ------ Pass lifecycle --------------------------------------------------------------------------------------------

void Renderer::beginRenderPass(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Camera& camera,
    const Color& clearColor) {

    // Upload camera data to view UBO.
    ViewParamsUBO viewParams{};
    Matrix4x4 vp = camera.viewProjectionMatrix();
    std::memcpy(viewParams.viewProj, vp.m, sizeof(f32) * 16);
    Vector3 camPos = camera.position();
    viewParams.cameraPos[0] = camPos.x;
    viewParams.cameraPos[1] = camPos.y;
    viewParams.cameraPos[2] = camPos.z;
    viewParams.cameraPos[3] = 0.0f;
    void* mapped = m_viewUBO->map();
    if (mapped) {
        std::memcpy(mapped, &viewParams, sizeof(viewParams));
        m_viewUBO->unmap();
    }

    beginRenderPass(colorTarget, depthTarget, clearColor);
    setPassBindGroup(0, m_viewBindGroup.get());
}

void Renderer::beginRenderPass(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Color& clearColor) {

    rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments = {{colorTarget, rhi::LoadOp::Clear, rhi::StoreOp::Store, clearColor}};

    rhi::DepthStencilAttachment depthAttachment;
    if (depthTarget) {
        depthAttachment.view          = depthTarget;
        depthAttachment.depthLoadOp   = rhi::LoadOp::Clear;
        depthAttachment.depthStoreOp  = rhi::StoreOp::Store;
        depthAttachment.clearDepth    = 1.0f;
        rpDesc.depthStencilAttachment = &depthAttachment;
    }

    m_currentPass = m_currentCmd->beginRenderPass(rpDesc);

    // Clear any pass-scoped bind group state from the previous pass.
    for (u32 i = 0; i < kMaxBindGroupSets; ++i) {
        m_passBindGroups[i]     = nullptr;
        m_passBindGroupDirty[i] = false;
    }
}

void Renderer::endRenderPass() {
    m_currentPass->end();
    m_currentPass = nullptr;
}

void Renderer::setPassBindGroup(u32 setIndex, rhi::BindGroup* bindGroup) {
    if (setIndex >= kMaxBindGroupSets) return;
    m_passBindGroups[setIndex]     = bindGroup;
    m_passBindGroupDirty[setIndex] = true;
}

void Renderer::flushPassBindGroups() {
    for (u32 i = 0; i < kMaxBindGroupSets; ++i) {
        if (m_passBindGroupDirty[i] && m_passBindGroups[i]) {
            m_currentPass->setBindGroup(i, m_passBindGroups[i]);
            m_passBindGroupDirty[i] = false;
        }
    }
}

// ------ Drawing ---------------------------------------------------------------------------------------------------

Result<void> Renderer::drawMesh(Mesh* mesh, const Transform& transform) {
    m_currentPass->setVertexBuffer(0, mesh->vertexBuffer());
    m_currentPass->setIndexBuffer(mesh->indexBuffer(), rhi::IndexFormat::Uint32);

    for (const auto& sub : mesh->submeshes()) {
        Material* mat = nullptr;
        if (sub.materialIndex < mesh->materials().size()) {
            mat = mesh->materials()[sub.materialIndex].get();
        }
        if (!mat && !mesh->materials().empty()) {
            mat = mesh->materials()[0].get();
        }
        if (!mat) continue;

        auto result = drawSubmesh(mesh, mat, transform, sub);
        if (!result) return result;
    }
    return {};
}

Result<void> Renderer::drawMesh(Mesh* mesh, const Transform& transform, Material* material) {
    m_currentPass->setVertexBuffer(0, mesh->vertexBuffer());
    m_currentPass->setIndexBuffer(mesh->indexBuffer(), rhi::IndexFormat::Uint32);

    for (const auto& sub : mesh->submeshes()) {
        auto result = drawSubmesh(mesh, material, transform, sub);
        if (!result) return result;
    }
    return {};
}

Result<void> Renderer::drawSubmesh(
    Mesh* mesh, Material* mat, const Transform& transform, const SubMesh& sub) {

    auto* pipelineCache = m_ctx->pipelineCache();

    // Allocate per-object UBO slot and write transforms.
    auto alloc = m_objectAllocator->allocate();
    {
        Matrix4x4 worldMatrix  = transform.matrix();
        Matrix4x4 normalMatrix = transform.normalMatrix();
        ObjectParamsUBO objParams{};
        std::memcpy(objParams.world,        worldMatrix.m,  sizeof(f32) * 16);
        std::memcpy(objParams.normalMatrix, normalMatrix.m, sizeof(f32) * 16);
        std::memcpy(alloc.cpuPtr, &objParams, sizeof(objParams));
    }

    // Resolve pipeline.
    PipelineCacheKey key;
    key.vertexShader       = mat->vertexShader();
    key.fragmentShader     = mat->fragmentShader();
    key.vertexEntry        = mat->vertexEntry();
    key.fragmentEntry      = mat->fragmentEntry();
    key.cullMode           = mat->cullMode();
    key.blendEnabled       = mat->blendEnabled();
    if (key.blendEnabled) {
        key.blendState.enabled  = true;
        key.blendState.srcColor = rhi::BlendFactor::SrcAlpha;
        key.blendState.dstColor = rhi::BlendFactor::OneMinusSrcAlpha;
        key.blendState.colorOp  = rhi::BlendOp::Add;
        key.blendState.srcAlpha = rhi::BlendFactor::One;
        key.blendState.dstAlpha = rhi::BlendFactor::OneMinusSrcAlpha;
        key.blendState.alphaOp  = rhi::BlendOp::Add;
    }
    key.depthTestEnabled    = mat->depthTestEnabled();
    key.depthWriteEnabled   = mat->depthWriteEnabled();
    key.pipelineLayout      = m_pipelineLayout.get();
    key.topology            = mesh->topology();
    key.colorFormat         = m_colorFormat;
    key.depthStencilFormat  = m_depthFormat;
    key.sampleCount         = 1;

    auto pipelineResult = pipelineCache->getOrCreate(key);
    if (!pipelineResult) return tl::make_unexpected(pipelineResult.error());

    m_currentPass->setPipeline(*pipelineResult);

    // After setPipeline, flush any pending pass-scoped bind groups.
    flushPassBindGroups();

    m_currentPass->setBindGroup(2, mat->materialBindGroup());
    m_currentPass->setBindGroup(3, alloc.bindGroup, &alloc.dynamicOffset, 1);
    m_currentPass->drawIndexed(sub.indexCount, 1, sub.indexOffset);

    return {};
}

Result<void> Renderer::drawScreenRect(Material* material) {
    auto meshResult = getScreenRectMesh();
    if (!meshResult) return tl::make_unexpected(meshResult.error());

    Transform identity;
    return drawMesh(*meshResult, identity, material);
}

Result<Mesh*> Renderer::getScreenRectMesh() {
    if (m_screenRectMesh) return m_screenRectMesh.get();

    // Fullscreen quad in NDC: covers [-1,1]x[-1,1].
    // UV: (0,0) = top-left, (1,1) = bottom-right.
    std::vector<Vertex> vertices = {
        {{-1.0f, -1.0f, 0.0f}, {0, 0, 1}, {0, 1}, Color::white(), {1, 0, 0, 1}},
        {{ 1.0f, -1.0f, 0.0f}, {0, 0, 1}, {1, 1}, Color::white(), {1, 0, 0, 1}},
        {{ 1.0f,  1.0f, 0.0f}, {0, 0, 1}, {1, 0}, Color::white(), {1, 0, 0, 1}},
        {{-1.0f,  1.0f, 0.0f}, {0, 0, 1}, {0, 0}, Color::white(), {1, 0, 0, 1}},
    };
    std::vector<u32> indices = {0, 1, 2, 0, 2, 3};
    SubMesh sub;
    sub.indexOffset   = 0;
    sub.indexCount    = 6;
    sub.materialIndex = 0;

    auto result = Mesh::create(m_ctx->device(), vertices, indices, {sub});
    if (!result) return tl::make_unexpected(result.error());
    m_screenRectMesh = std::move(*result);
    return m_screenRectMesh.get();
}

} // namespace ln
