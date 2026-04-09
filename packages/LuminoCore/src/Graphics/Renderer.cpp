#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/Batch.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/PipelineCache.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <cstring>

namespace ln {

// ------ Renderer::create ------------------------------------------------------------------------------------------

Result<Ref<Renderer>> Renderer::create(GraphicsContext* ctx) {
    auto* device      = ctx->device();
    auto* module      = ctx->module();
    auto  colorFormat = ctx->colorFormat();
    auto  depthFormat = ctx->depthFormat();

    auto renderer = Ref<Renderer>::adopt(new Renderer());
    renderer->m_colorFormat   = colorFormat;
    renderer->m_depthFormat   = depthFormat;

    // Use the BasicLit ShaderPass's PipelineLayout as reference for dynamic UBO allocators.
    const auto& refShaderPass = module->builtinShader(BuiltinShader::BasicLit);
    renderer->m_referencePipelineLayout = refShaderPass->pipelineLayout();
    renderer->m_viewSetIndex = refShaderPass->viewSetIndex();
    renderer->m_objectSetIndex = refShaderPass->objectSetIndex();
    renderer->m_objectUBOSize = refShaderPass->objectUBOSize();

    // ---- Dynamic UBO allocator for per-frame view data (camera) ----
    {
        auto r = DynamicUniformAllocator::create(
            device, renderer->m_referencePipelineLayout,
            static_cast<u32>(renderer->m_viewSetIndex), 0,
            static_cast<u32>(sizeof(ViewParamsUBO)));
        if (!r) return tl::make_unexpected(r.error());
        renderer->m_viewAllocator = std::move(*r);
    }

    // ---- Dynamic UBO allocator for per-object data ----
    {
        auto r = DynamicUniformAllocator::create(
            device, renderer->m_referencePipelineLayout,
            static_cast<u32>(renderer->m_objectSetIndex), 0,
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

    // Lazy-initialize the batch processor on first use.
    if (!m_batchProcessor) {
        auto result = BatchProcessor::create(m_ctx);
        if (result) {
            m_batchProcessor = std::move(*result);
        }
    }
}

void Renderer::endFrame() {
    // Flush dynamic uniform buffers to GPU before submission.
    (void)m_viewAllocator->flushFrame();
    (void)m_objectAllocator->flushFrame();

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
    setPassBindGroup(static_cast<u32>(m_viewSetIndex), viewAlloc.bindGroup, viewAlloc.dynamicOffset, 1);
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

    // Track the current color target for endRenderPassWithTransition.
    m_currentColorTarget = colorTarget;

    // Clear any pass-scoped bind group state from the previous pass.
    for (u32 i = 0; i < kMaxBindGroupSets; ++i) {
        m_passBindGroups[i]                  = nullptr;
        m_passBindGroupDynamicOffsets[i]     = 0;
        m_passBindGroupDynamicOffsetCounts[i] = 0;
        m_passBindGroupDirty[i]              = false;
    }

    // Clear the internal command buffer for this render pass.
    m_commandBuffer.clear();
}

void Renderer::endRenderPass() {
    // Flush batched draw commands before ending the render pass.
    (void)flushBatch();

    m_currentPass->end();
    m_currentPass = nullptr;
    m_currentColorTarget = nullptr;
}

void Renderer::endRenderPassWithTransition() {
    // Flush batched draw commands before ending the render pass.
    (void)flushBatch();

    auto* colorTarget = m_currentColorTarget;
    m_currentPass->end();
    m_currentPass = nullptr;
    m_currentColorTarget = nullptr;
    if (colorTarget) {
        m_currentCmd->transitionToShaderRead(colorTarget);
    }
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
    setPassBindGroup(static_cast<u32>(m_viewSetIndex), viewAlloc.bindGroup, viewAlloc.dynamicOffset, 1);
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

// ------ Drawing (batched) -----------------------------------------------------------------------------------------

void Renderer::drawMesh(Mesh* mesh, const Transform& transform, i32 zIndex) {
    m_commandBuffer.drawMesh(mesh, transform, zIndex);
}

void Renderer::drawSprite(Material* material, i32 zIndex,
                          const Vector3& pos, const Vector2& size,
                          const Vector2& uvOffset, const Vector2& uvSize,
                          const Color& color, f32 rotation) {
    m_commandBuffer.drawSprite(material, zIndex, pos, size, uvOffset, uvSize, color, rotation);
}

Result<void> Renderer::flushBatch() {
    if (m_batchProcessor && !m_commandBuffer.commands().empty()) {
        auto result = m_batchProcessor->flush(this, &m_commandBuffer);
        m_commandBuffer.clear();
        return result;
    }
    return {};
}

// ------ Drawing (immediate) ---------------------------------------------------------------------------------------

Result<void> Renderer::drawMeshImmediate(Mesh* mesh, const Transform& transform) {
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

Result<void> Renderer::drawMeshImmediate(Mesh* mesh, const Transform& transform, Material* material) {
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
    key.shaderPass          = mat->shaderPass();
    key.cullMode            = mat->cullMode();
    key.blendEnabled        = mat->blendEnabled();
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
    key.topology            = mesh->topology();
    key.renderPass          = m_currentPass;

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

    int16_t matSet = mat->shaderPass()->materialSetIndex();
    m_currentPass->setBindGroup(static_cast<u32>(matSet), *matBGResult);
    m_currentPass->setBindGroup(static_cast<u32>(m_objectSetIndex), alloc.bindGroup, &alloc.dynamicOffset, 1);
    m_currentPass->drawIndexed(sub.indexCount, 1, sub.indexOffset);
    ++m_drawCallCount;

    return {};
}

Result<void> Renderer::drawSingleSubMesh(
    Mesh* mesh, u32 submeshIndex, Material* material, const Transform& transform) {

    const auto& submeshes = mesh->submeshes();
    if (submeshIndex >= submeshes.size()) return {};

    m_currentPass->setVertexBuffer(0, mesh->vertexBuffer());
    m_currentPass->setIndexBuffer(mesh->indexBuffer(), rhi::IndexFormat::Uint32);
    return drawSubmesh(mesh, material, transform, submeshes[submeshIndex]);
}

Result<void> Renderer::drawScreenRect(Material* material) {
    auto meshResult = getScreenRectMesh();
    if (!meshResult) return tl::make_unexpected(meshResult.error());

    Transform identity;
    return drawMeshImmediate(*meshResult, identity, material);
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
        key.shaderPass         = mat->shaderPass();
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
        key.topology         = mesh->topology();
        key.renderPass       = m_currentPass;

        auto pipelineResult = pipelineCache->getOrCreate(key);
        if (!pipelineResult) return tl::make_unexpected(pipelineResult.error());

        m_currentPass->setPipeline(*pipelineResult);
        m_currentPass->setStencilReference(stencilRef);
        flushPassBindGroups();

        auto matBGResult = getOrCreateMaterialBindGroup(mat);
        if (!matBGResult) return tl::make_unexpected(matBGResult.error());

        int16_t matSet = mat->shaderPass()->materialSetIndex();
        m_currentPass->setBindGroup(static_cast<u32>(matSet), *matBGResult);
        m_currentPass->setBindGroup(static_cast<u32>(m_objectSetIndex), alloc.bindGroup, &alloc.dynamicOffset, 1);
        m_currentPass->drawIndexed(sub.indexCount, 1, sub.indexOffset);
        ++m_drawCallCount;
    }
    return {};
}

Result<void> Renderer::pushStencilMask(Mesh* mesh, const Transform& transform, Material* material) {
    // Flush any pending batched commands before changing stencil state.
    auto flushResult = flushBatch();
    if (!flushResult) return flushResult;

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
    // Flush any pending batched commands before changing stencil state.
    auto flushResult = flushBatch();
    if (!flushResult) return flushResult;

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
    if (mat->baseTexture() && (!cache.textureView || cache.lastBaseTexture != mat->baseTexture())) {
        auto tvResult = device->createTextureView(mat->baseTexture());
        if (!tvResult) return tl::make_unexpected(tvResult.error());
        cache.textureView = std::move(*tvResult);
        cache.lastBaseTexture = mat->baseTexture();
        // Texture view changed, all bind groups must be recreated
        for (auto& d : cache.dirty) d = true;
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

    // Write UBO data via writeBuffer (compatible with all backends).
    {
        auto uboSize = mat->materialParamBufferSize();
        u8 uboStaging[512];
        std::vector<u8> uboStagingHeap;
        void* stagingPtr;
        if (uboSize <= sizeof(uboStaging)) {
            stagingPtr = uboStaging;
        } else {
            uboStagingHeap.resize(static_cast<size_t>(uboSize));
            stagingPtr = uboStagingHeap.data();
        }
        mat->writeMaterialUBO(stagingPtr);
        (void)device->writeBuffer(cache.paramBuffers[frameSlot].get(), 0, stagingPtr, uboSize);
    }

    // Create BindGroup via the material's PipelineLayout at the material set index.
    if (cache.textureView && cache.sampler) {
        int16_t matSet = mat->shaderPass()->materialSetIndex();
        std::vector<rhi::BindGroupEntry> entries = {
            {0, cache.paramBuffers[frameSlot].get(), 0, mat->materialParamBufferSize(), nullptr, nullptr},
            {1, nullptr, 0, 0, cache.textureView.get(), nullptr},
            {2, nullptr, 0, 0, nullptr, cache.sampler.get()},
        };
        auto bgResult = mat->shaderPass()->pipelineLayout()->createBindGroup(
            static_cast<u32>(matSet), entries);
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
