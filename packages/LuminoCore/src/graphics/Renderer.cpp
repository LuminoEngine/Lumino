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
    auto* device     = ctx->device();
    auto  colorFormat = ctx->colorFormat();
    auto  depthFormat = ctx->depthFormat();

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

    // ---- Set 0: View BindGroupLayout (camera, dynamic UBO) ----
    {
        auto desc = buildBindGroupLayoutFromReflection(*viewBlock, targetPass->bindingLayout);
        for (auto& entry : desc.entries) {
            if (entry.type == rhi::BindingType::UniformBuffer) {
                entry.hasDynamicOffset = true;
            }
        }
        auto r    = device->createBindGroupLayout(desc);
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_viewBindGroupLayout = std::move(*r);
    }

    // ---- Set 1: Scene BindGroupLayout (lighting, dynamic UBO) ----
    {
        auto desc = buildBindGroupLayoutFromReflection(*sceneBlock, targetPass->bindingLayout);
        for (auto& entry : desc.entries) {
            if (entry.type == rhi::BindingType::UniformBuffer) {
                entry.hasDynamicOffset = true;
            }
        }
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

    // ---- Dynamic UBO allocator for per-frame view data (camera, set=0) ----
    {
        auto r = DynamicUniformAllocator::create(
            device, renderer->m_viewBindGroupLayout.get(), 0,
            static_cast<u32>(sizeof(ViewParamsUBO)));
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_viewAllocator = std::move(*r);
    }

    // ---- Dynamic UBO allocator for per-object data ----
    {
        auto r = DynamicUniformAllocator::create(
            device, renderer->m_objectBindGroupLayout.get(), 0,
            static_cast<u32>(renderer->m_objectUBOSize));
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_objectAllocator = std::move(*r);
    }

    renderer->m_ctx = ctx;
    return renderer;
}

// ------ Frame lifecycle -------------------------------------------------------------------------------------------

void Renderer::beginFrame() {
    u32 frame = m_frameCounter++;
    m_currentFrameSlot = frame % 2;
    m_viewAllocator->beginFrame(frame);
    m_objectAllocator->beginFrame(frame);
    m_currentCmd = m_ctx->currentCommandBuffer();
    m_drawCallCount = 0;
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

    // Allocate view UBO from the per-frame allocator and upload camera data.
    auto viewAlloc = m_viewAllocator->allocate();
    {
        ViewParamsUBO viewParams{};
        Matrix4x4 vp = camera.viewProjectionMatrix();
        std::memcpy(viewParams.viewProj, vp.m, sizeof(f32) * 16);
        Vector3 camPos = camera.position();
        viewParams.cameraPos[0] = camPos.x;
        viewParams.cameraPos[1] = camPos.y;
        viewParams.cameraPos[2] = camPos.z;
        viewParams.cameraPos[3] = 0.0f;
        std::memcpy(viewAlloc.cpuPtr, &viewParams, sizeof(viewParams));
    }

    beginRenderPass(colorTarget, depthTarget, clearColor);
    setPassBindGroup(0, viewAlloc.bindGroup, viewAlloc.dynamicOffset, 1);
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
        depthAttachment.stencilLoadOp  = rhi::LoadOp::Clear;
        depthAttachment.stencilStoreOp = rhi::StoreOp::Store;
        depthAttachment.clearStencil   = 0;
        rpDesc.depthStencilAttachment = &depthAttachment;
    }

    m_currentPass = m_currentCmd->beginRenderPass(rpDesc);

    // Clear any pass-scoped bind group state from the previous pass.
    for (u32 i = 0; i < kMaxBindGroupSets; ++i) {
        m_passBindGroups[i]                  = nullptr;
        m_passBindGroupDynamicOffsets[i]     = 0;
        m_passBindGroupDynamicOffsetCounts[i] = 0;
        m_passBindGroupDirty[i]              = false;
    }
}

void Renderer::endRenderPass() {
    m_currentPass->end();
    m_currentPass = nullptr;
}

void Renderer::beginOverlayRenderPass(rhi::TextureView* colorTarget) {
    rhi::ColorAttachment colorAttach;
    colorAttach.view       = colorTarget;
    colorAttach.loadOp     = rhi::LoadOp::Load;
    colorAttach.storeOp    = rhi::StoreOp::Store;
    colorAttach.clearColor = Color{0.0f, 0.0f, 0.0f, 0.0f};

    rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments      = {colorAttach};
    rpDesc.depthStencilAttachment = nullptr;

    m_currentPass = m_currentCmd->beginRenderPass(rpDesc);

    for (u32 i = 0; i < kMaxBindGroupSets; ++i) {
        m_passBindGroups[i]                   = nullptr;
        m_passBindGroupDynamicOffsets[i]      = 0;
        m_passBindGroupDynamicOffsetCounts[i] = 0;
        m_passBindGroupDirty[i]               = false;
    }

    // Vertices are already in NDC, so upload an identity view-projection matrix.
    auto viewAlloc = m_viewAllocator->allocate();
    ViewParamsUBO viewParams{};
    // Column-major identity matrix.
    viewParams.viewProj[0]  = 1.0f;
    viewParams.viewProj[5]  = 1.0f;
    viewParams.viewProj[10] = 1.0f;
    viewParams.viewProj[15] = 1.0f;
    std::memcpy(viewAlloc.cpuPtr, &viewParams, sizeof(viewParams));
    setPassBindGroup(0, viewAlloc.bindGroup, viewAlloc.dynamicOffset, 1);
}

void Renderer::setPassBindGroup(u32 setIndex, rhi::BindGroup* bindGroup,
                                u32 dynamicOffset, u32 dynamicOffsetCount) {
    if (setIndex >= kMaxBindGroupSets) return;
    m_passBindGroups[setIndex]                = bindGroup;
    m_passBindGroupDynamicOffsets[setIndex]     = dynamicOffset;
    m_passBindGroupDynamicOffsetCounts[setIndex] = dynamicOffsetCount;
    m_passBindGroupDirty[setIndex]             = true;
}

void Renderer::flushPassBindGroups() {
    for (u32 i = 0; i < kMaxBindGroupSets; ++i) {
        if (m_passBindGroupDirty[i] && m_passBindGroups[i]) {
            if (m_passBindGroupDynamicOffsetCounts[i] > 0) {
                m_currentPass->setBindGroup(i, m_passBindGroups[i],
                    &m_passBindGroupDynamicOffsets[i], m_passBindGroupDynamicOffsetCounts[i]);
            } else {
                m_currentPass->setBindGroup(i, m_passBindGroups[i]);
            }
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
    // When a stencil mask is active, enable stencil test to restrict drawing to masked area.
    if (m_stencilRef > 0) {
        key.stencilTestEnabled = true;
        key.stencilFront.compare   = rhi::CompareFunction::Equal;
        key.stencilFront.failOp    = rhi::StencilOp::Keep;
        key.stencilFront.depthFailOp = rhi::StencilOp::Keep;
        key.stencilFront.passOp    = rhi::StencilOp::Keep;
        key.stencilBack = key.stencilFront;
        key.stencilReadMask  = 0xFF;
        key.stencilWriteMask = 0x00;
    }
    key.pipelineLayout      = m_pipelineLayout.get();
    key.topology            = mesh->topology();
    key.colorFormat         = m_colorFormat;
    key.depthStencilFormat  = m_depthFormat;
    key.sampleCount         = 1;

    auto pipelineResult = pipelineCache->getOrCreate(key);
    if (!pipelineResult) return tl::make_unexpected(pipelineResult.error());

    m_currentPass->setPipeline(*pipelineResult);

    // Set dynamic stencil reference when stencil mask is active.
    if (m_stencilRef > 0) {
        m_currentPass->setStencilReference(m_stencilRef);
    }

    // After setPipeline, flush any pending pass-scoped bind groups.
    flushPassBindGroups();

    // Get or create the material's BindGroup from the Renderer-side cache.
    auto matBGResult = getOrCreateMaterialBindGroup(mat);
    if (!matBGResult) return tl::make_unexpected(matBGResult.error());

    m_currentPass->setBindGroup(2, *matBGResult);
    m_currentPass->setBindGroup(3, alloc.bindGroup, &alloc.dynamicOffset, 1);
    m_currentPass->drawIndexed(sub.indexCount, 1, sub.indexOffset);
    ++m_drawCallCount;

    return {};
}

Result<void> Renderer::drawScreenRect(Material* material) {
    auto meshResult = getScreenRectMesh();
    if (!meshResult) return tl::make_unexpected(meshResult.error());

    Transform identity;
    return drawMesh(*meshResult, identity, material);
}

// ------ Stencil Mask --------------------------------------------------------------------------------------------

Result<void> Renderer::drawStencilMaskMesh(
    Mesh* mesh, const Transform& transform, Material* material,
    rhi::CompareFunction compare, u32 stencilRef, rhi::StencilOp passOp) {

    auto* pipelineCache = m_ctx->pipelineCache();

    m_currentPass->setVertexBuffer(0, mesh->vertexBuffer());
    m_currentPass->setIndexBuffer(mesh->indexBuffer(), rhi::IndexFormat::Uint32);

    for (const auto& sub : mesh->submeshes()) {
        Material* mat = material;
        if (!mat && sub.materialIndex < mesh->materials().size()) {
            mat = mesh->materials()[sub.materialIndex].get();
        }
        if (!mat && !mesh->materials().empty()) {
            mat = mesh->materials()[0].get();
        }
        if (!mat) continue;

        // Allocate per-object UBO
        auto alloc = m_objectAllocator->allocate();
        {
            Matrix4x4 worldMatrix  = transform.matrix();
            Matrix4x4 normalMatrix = transform.normalMatrix();
            ObjectParamsUBO objParams{};
            std::memcpy(objParams.world,        worldMatrix.m,  sizeof(f32) * 16);
            std::memcpy(objParams.normalMatrix, normalMatrix.m, sizeof(f32) * 16);
            std::memcpy(alloc.cpuPtr, &objParams, sizeof(objParams));
        }

        // Build pipeline key for stencil write
        PipelineCacheKey key;
        key.vertexShader       = mat->vertexShader();
        key.fragmentShader     = mat->fragmentShader();
        key.vertexEntry        = mat->vertexEntry();
        key.fragmentEntry      = mat->fragmentEntry();
        key.cullMode           = rhi::CullMode::None;
        key.blendEnabled       = false;
        key.depthTestEnabled   = false;
        key.depthWriteEnabled  = false;
        key.colorWriteEnabled  = false;
        key.stencilTestEnabled = true;
        key.stencilFront.compare   = compare;
        key.stencilFront.failOp    = rhi::StencilOp::Keep;
        key.stencilFront.depthFailOp = rhi::StencilOp::Keep;
        key.stencilFront.passOp    = passOp;
        key.stencilBack            = key.stencilFront;
        key.stencilReadMask  = 0xFF;
        key.stencilWriteMask = 0xFF;
        key.pipelineLayout   = m_pipelineLayout.get();
        key.topology         = mesh->topology();
        key.colorFormat      = m_colorFormat;
        key.depthStencilFormat = m_depthFormat;
        key.sampleCount      = 1;

        auto pipelineResult = pipelineCache->getOrCreate(key);
        if (!pipelineResult) return tl::make_unexpected(pipelineResult.error());

        m_currentPass->setPipeline(*pipelineResult);
        m_currentPass->setStencilReference(stencilRef);
        flushPassBindGroups();

        auto matBGResult = getOrCreateMaterialBindGroup(mat);
        if (!matBGResult) return tl::make_unexpected(matBGResult.error());

        m_currentPass->setBindGroup(2, *matBGResult);
        m_currentPass->setBindGroup(3, alloc.bindGroup, &alloc.dynamicOffset, 1);
        m_currentPass->drawIndexed(sub.indexCount, 1, sub.indexOffset);
        ++m_drawCallCount;
    }
    return {};
}

Result<void> Renderer::pushStencilMask(Mesh* mesh, const Transform& transform, Material* material) {
    // Save mask info for later pop.
    m_stencilMaskStack.push_back({mesh, transform, material});

    // Draw mask mesh into stencil buffer: increment stencil where stencil == m_stencilRef.
    // After this, the mask area will have stencil == m_stencilRef + 1.
    auto result = drawStencilMaskMesh(
        mesh, transform, material,
        rhi::CompareFunction::Equal,  // compare: pass where stencil == current ref
        m_stencilRef,                 // dynamic reference value
        rhi::StencilOp::IncrementClamp);
    if (!result) return result;

    m_stencilRef++;
    return {};
}

Result<void> Renderer::popStencilMask() {
    if (m_stencilMaskStack.empty()) {
        return tl::make_unexpected(Error{ErrorCode::InvalidArgument, "Stencil mask stack underflow"});
    }

    auto entry = m_stencilMaskStack.back();
    m_stencilMaskStack.pop_back();

    // Redraw the mask mesh to decrement stencil back.
    auto result = drawStencilMaskMesh(
        entry.mesh, entry.transform, entry.material,
        rhi::CompareFunction::Equal,
        m_stencilRef,                 // match the incremented value
        rhi::StencilOp::DecrementClamp);
    if (!result) return result;

    m_stencilRef--;
    return {};
}

Result<rhi::BindGroup*> Renderer::getOrCreateMaterialBindGroup(Material* mat) {
    auto* device = m_ctx->device();
    u32 frameSlot = m_currentFrameSlot;

    auto& cache = m_materialCache[mat];

    // Check if the material's parameters have changed since last cache update.
    if (mat->paramVersion() != cache.paramVersion) {
        // Mark all frame slots dirty so each gets updated when used.
        for (auto& d : cache.dirty) d = true;
        cache.paramVersion = mat->paramVersion();
    }

    if (!cache.dirty[frameSlot]) {
        return cache.bindGroups[frameSlot].get();
    }

    // Create texture view if missing or texture changed
    if (mat->baseTexture() && !cache.textureView) {
        auto tvResult = device->createTextureView(mat->baseTexture());
        if (!tvResult) return tl::make_unexpected(tvResult.error());
        cache.textureView = std::move(*tvResult);
    }

    // Create sampler if missing
    if (!cache.sampler) {
        rhi::SamplerDesc samplerDesc;
        auto sampResult = device->createSampler(samplerDesc);
        if (!sampResult) return tl::make_unexpected(sampResult.error());
        cache.sampler = std::move(*sampResult);
    }

    // Create per-frame buffer if missing
    if (!cache.paramBuffers[frameSlot]) {
        rhi::BufferDesc bufDesc;
        bufDesc.size = mat->materialParamBufferSize();
        bufDesc.usage = rhi::BufferUsage::Uniform;
        auto bufResult = device->createBuffer(bufDesc);
        if (!bufResult) return tl::make_unexpected(bufResult.error());
        cache.paramBuffers[frameSlot] = std::move(*bufResult);
    }

    // Write UBO data
    {
        void* mapped = cache.paramBuffers[frameSlot]->map();
        if (mapped) {
            mat->writeMaterialUBO(mapped);
            cache.paramBuffers[frameSlot]->unmap();
        }
    }

    // Create BindGroup
    if (mat->materialBindGroupLayout() && cache.textureView && cache.sampler) {
        rhi::BindGroupDesc bgDesc;
        bgDesc.layout = mat->materialBindGroupLayout();
        bgDesc.entries = {
            {0, cache.paramBuffers[frameSlot].get(), 0, mat->materialParamBufferSize(), nullptr, nullptr},
            {1, nullptr, 0, 0, cache.textureView.get(), nullptr},
            {2, nullptr, 0, 0, nullptr, cache.sampler.get()},
        };
        auto bgResult = device->createBindGroup(bgDesc);
        if (!bgResult) return tl::make_unexpected(bgResult.error());
        cache.bindGroups[frameSlot] = std::move(*bgResult);
    }

    cache.dirty[frameSlot] = false;
    return cache.bindGroups[frameSlot].get();
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
