#include <lumino_core/graphics/ForwardRenderer.hpp>
#include <cstring>

namespace lumino {

Result<Ref<ForwardRenderer>> ForwardRenderer::create(
    rhi::Device* device,
    rhi::TextureFormat colorFormat,
    rhi::TextureFormat depthFormat) {

    auto renderer = Ref<ForwardRenderer>::adopt(new ForwardRenderer());
    renderer->colorFormat_ = colorFormat;
    renderer->depthFormat_ = depthFormat;

    // ── Set 0: View BindGroupLayout ──
    // Binding 0: ViewParamsUBO (Vertex + Fragment)
    rhi::BindGroupLayoutDesc viewBGLDesc;
    viewBGLDesc.entries = {
        {0, rhi::ShaderStage::Vertex | rhi::ShaderStage::Fragment, rhi::BindingType::UniformBuffer},
    };
    auto viewBGLResult = device->createBindGroupLayout(viewBGLDesc);
    if (!viewBGLResult) return tl::make_unexpected(viewBGLResult.error());
    renderer->viewBindGroupLayout_ = std::move(*viewBGLResult);

    // ── Set 1: Material BindGroupLayout (placeholder — actual layout from Material) ──
    // Binding 0: MaterialParams UBO, Binding 1: texture, Binding 2: sampler
    rhi::BindGroupLayoutDesc matBGLDesc;
    matBGLDesc.entries = {
        {0, rhi::ShaderStage::Vertex | rhi::ShaderStage::Fragment, rhi::BindingType::UniformBuffer},
        {1, rhi::ShaderStage::Fragment, rhi::BindingType::SampledTexture},
        {2, rhi::ShaderStage::Fragment, rhi::BindingType::Sampler},
    };
    Ref<rhi::BindGroupLayout> matBGL;
    {
        auto r = device->createBindGroupLayout(matBGLDesc);
        if (!r) return tl::make_unexpected(r.error());
        matBGL = std::move(*r);
    }

    // ── Set 2: Object BindGroupLayout ──
    // Binding 0: ObjectParamsUBO (Vertex)
    rhi::BindGroupLayoutDesc objBGLDesc;
    objBGLDesc.entries = {
        {0, rhi::ShaderStage::Vertex, rhi::BindingType::UniformBuffer},
    };
    auto objBGLResult = device->createBindGroupLayout(objBGLDesc);
    if (!objBGLResult) return tl::make_unexpected(objBGLResult.error());
    renderer->objectBindGroupLayout_ = std::move(*objBGLResult);

    // ── PipelineLayout (Set 0, Set 1, Set 2) ──
    rhi::PipelineLayoutDesc plDesc;
    plDesc.bindGroupLayouts = {
        renderer->viewBindGroupLayout_.get(),
        matBGL.get(),
        renderer->objectBindGroupLayout_.get(),
    };
    auto plResult = device->createPipelineLayout(plDesc);
    if (!plResult) return tl::make_unexpected(plResult.error());
    renderer->pipelineLayout_ = std::move(*plResult);

    // ── Per-view UBO ──
    rhi::BufferDesc viewBufDesc;
    viewBufDesc.size = sizeof(ViewParamsUBO);
    viewBufDesc.usage = rhi::BufferUsage::Uniform;
    auto viewBufResult = device->createBuffer(viewBufDesc);
    if (!viewBufResult) return tl::make_unexpected(viewBufResult.error());
    renderer->viewUBO_ = std::move(*viewBufResult);

    // ── Per-view BindGroup ──
    rhi::BindGroupDesc viewBGDesc;
    viewBGDesc.layout = renderer->viewBindGroupLayout_.get();
    viewBGDesc.entries = {
        {0, renderer->viewUBO_.get(), 0, sizeof(ViewParamsUBO), nullptr, nullptr},
    };
    auto viewBGResult = device->createBindGroup(viewBGDesc);
    if (!viewBGResult) return tl::make_unexpected(viewBGResult.error());
    renderer->viewBindGroup_ = std::move(*viewBGResult);

    return renderer;
}

Result<void> ForwardRenderer::ensureObjectResources(rhi::Device* device, size_t count) {
    while (objectUBOs_.size() < count) {
        // Create object UBO
        rhi::BufferDesc bufDesc;
        bufDesc.size = sizeof(ObjectParamsUBO);
        bufDesc.usage = rhi::BufferUsage::Uniform;
        auto bufResult = device->createBuffer(bufDesc);
        if (!bufResult) return tl::make_unexpected(bufResult.error());
        auto& buf = *bufResult;

        // Create object BindGroup
        rhi::BindGroupDesc bgDesc;
        bgDesc.layout = objectBindGroupLayout_.get();
        bgDesc.entries = {
            {0, buf.get(), 0, sizeof(ObjectParamsUBO), nullptr, nullptr},
        };
        auto bgResult = device->createBindGroup(bgDesc);
        if (!bgResult) return tl::make_unexpected(bgResult.error());

        objectUBOs_.push_back(std::move(buf));
        objectBindGroups_.push_back(std::move(*bgResult));
    }
    return {};
}

Result<void> ForwardRenderer::renderFrame(
    rhi::Device* device,
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Camera& camera,
    const std::vector<RenderObject>& objects,
    const Color& clearColor) {

    // Count total draw calls (submeshes across all objects).
    size_t totalDraws = 0;
    for (auto& obj : objects) {
        totalDraws += obj.mesh->submeshes().size();
    }

    // Ensure we have enough per-object resources.
    auto ensureResult = ensureObjectResources(device, totalDraws);
    if (!ensureResult) return tl::make_unexpected(ensureResult.error());

    // ── Update View UBO ──
    {
        ViewParamsUBO viewParams{};
        Matrix4x4 vp = camera.viewProjectionMatrix();
        std::memcpy(viewParams.viewProj, vp.m, sizeof(f32) * 16);

        Vector3 camPos = camera.position();
        viewParams.cameraPos[0] = camPos.x;
        viewParams.cameraPos[1] = camPos.y;
        viewParams.cameraPos[2] = camPos.z;
        viewParams.cameraPos[3] = 0.0f;

        Vector3 ld = light_.direction.normalized();
        viewParams.lightDir[0] = ld.x;
        viewParams.lightDir[1] = ld.y;
        viewParams.lightDir[2] = ld.z;
        viewParams.lightDir[3] = 0.0f;

        viewParams.lightColor[0] = light_.color.r;
        viewParams.lightColor[1] = light_.color.g;
        viewParams.lightColor[2] = light_.color.b;
        viewParams.lightColor[3] = light_.color.a;

        viewParams.ambientColor[0] = light_.ambient.r;
        viewParams.ambientColor[1] = light_.ambient.g;
        viewParams.ambientColor[2] = light_.ambient.b;
        viewParams.ambientColor[3] = light_.ambient.a;

        void* mapped = viewUBO_->map();
        if (mapped) {
            std::memcpy(mapped, &viewParams, sizeof(viewParams));
            viewUBO_->unmap();
        }
    }

    // ── Begin Render Pass ──
    rhi::DepthStencilAttachment depthAttachment;
    depthAttachment.view = depthTarget;
    depthAttachment.depthLoadOp = rhi::LoadOp::Clear;
    depthAttachment.depthStoreOp = rhi::StoreOp::Store;
    depthAttachment.clearDepth = 1.0f;

    rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments = {{colorTarget, rhi::LoadOp::Clear, rhi::StoreOp::Store, clearColor}};
    rpDesc.depthStencilAttachment = &depthAttachment;

    auto* cmd = device->createCommandBuffer();
    auto* pass = cmd->beginRenderPass(rpDesc);

    // ── Draw Objects ──
    size_t drawIndex = 0;
    for (auto& obj : objects) {
        auto* mesh = obj.mesh.get();
        Transform transform = obj.transform;
        Matrix4x4 worldMatrix = transform.matrix();
        Matrix4x4 normalMatrix = transform.normalMatrix();

        pass->setVertexBuffer(0, mesh->vertexBuffer());
        pass->setIndexBuffer(mesh->indexBuffer(), rhi::IndexFormat::Uint32);

        for (auto& sub : mesh->submeshes()) {
            // Update object UBO
            {
                ObjectParamsUBO objParams{};
                std::memcpy(objParams.world, worldMatrix.m, sizeof(f32) * 16);
                std::memcpy(objParams.normalMatrix, normalMatrix.m, sizeof(f32) * 16);

                void* mapped = objectUBOs_[drawIndex]->map();
                if (mapped) {
                    std::memcpy(mapped, &objParams, sizeof(objParams));
                    objectUBOs_[drawIndex]->unmap();
                }
            }

            // Determine material
            Material* mat = nullptr;
            if (sub.materialIndex < mesh->materials().size()) {
                mat = mesh->materials()[sub.materialIndex].get();
            }
            if (!mat && !mesh->materials().empty()) {
                mat = mesh->materials()[0].get();
            }

            if (mat && mat->pipeline()) {
                pass->setPipeline(mat->pipeline());
                pass->setBindGroup(0, viewBindGroup_.get());
                pass->setBindGroup(1, mat->materialBindGroup());
            }

            pass->setBindGroup(2, objectBindGroups_[drawIndex].get());
            pass->drawIndexed(sub.indexCount, 1, sub.indexOffset);

            ++drawIndex;
        }
    }

    pass->end();
    cmd->submit();

    return {};
}

} // namespace lumino
