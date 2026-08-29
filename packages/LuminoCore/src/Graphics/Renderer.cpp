#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/Batch.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/PipelineCache.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <cstring>

namespace ln {

namespace {

rhi::BlendState resolveBlendState(BlendMode mode) {
    rhi::BlendState s;
    switch (mode) {
        case BlendMode::Normal:
            s.enabled = false;
            break;
        case BlendMode::Alpha:
            s.enabled  = true;
            s.srcColor = rhi::BlendFactor::SrcAlpha;
            s.dstColor = rhi::BlendFactor::OneMinusSrcAlpha;
            s.colorOp  = rhi::BlendOp::Add;
            s.srcAlpha = rhi::BlendFactor::One;
            s.dstAlpha = rhi::BlendFactor::OneMinusSrcAlpha;
            s.alphaOp  = rhi::BlendOp::Add;
            break;
        case BlendMode::Add:
            s.enabled  = true;
            s.srcColor = rhi::BlendFactor::SrcAlpha;
            s.dstColor = rhi::BlendFactor::One;
            s.colorOp  = rhi::BlendOp::Add;
            s.srcAlpha = rhi::BlendFactor::Zero;
            s.dstAlpha = rhi::BlendFactor::One;
            s.alphaOp  = rhi::BlendOp::Add;
            break;
        case BlendMode::Subtract:
            s.enabled  = true;
            s.srcColor = rhi::BlendFactor::SrcAlpha;
            s.dstColor = rhi::BlendFactor::One;
            s.colorOp  = rhi::BlendOp::ReverseSubtract;
            s.srcAlpha = rhi::BlendFactor::Zero;
            s.dstAlpha = rhi::BlendFactor::One;
            s.alphaOp  = rhi::BlendOp::Add;
            break;
        case BlendMode::Multiply:
            s.enabled  = true;
            s.srcColor = rhi::BlendFactor::Zero;
            s.dstColor = rhi::BlendFactor::SrcColor;
            s.colorOp  = rhi::BlendOp::Add;
            s.srcAlpha = rhi::BlendFactor::Zero;
            s.dstAlpha = rhi::BlendFactor::One;
            s.alphaOp  = rhi::BlendOp::Add;
            break;
    }
    return s;
}

} // namespace

// ------ Renderer::create ------------------------------------------------------------------------------------------

Result<Ref<Renderer>> Renderer::create(GraphicsContext* ctx) {
    auto* device      = ctx->device();
    auto* module      = ctx->module();
    auto  colorFormat = ctx->colorFormat();
    auto  depthFormat = ctx->depthFormat();

    auto renderer = Ref<Renderer>::adopt(new Renderer());
    renderer->m_colorFormat   = colorFormat;
    renderer->m_depthFormat   = depthFormat;

    uint32_t framesInFlight = ctx->maxFramesInFlight();
    renderer->m_framesInFlight = framesInFlight;

    // 動的 UBO アロケータの参照用として BasicLit ShaderPass の PipelineLayout を使う。
    // Web ビルドでは GraphicsModule / 組み込みシェーダがまだ利用できない場合がある。
    // その場合は UBO アロケータの作成をスキップする - 基本的なパスの begin/end だけが動作する。
    if (module) {
        const auto& refShaderPass = module->builtinShader(BuiltinShader::BasicLit);
        renderer->m_referencePipelineLayout = refShaderPass->pipelineLayout();
        renderer->m_viewSetIndex = refShaderPass->viewSetIndex();
        renderer->m_sceneSetIndex = refShaderPass->sceneSetIndex();
        renderer->m_objectSetIndex = refShaderPass->objectSetIndex();
        renderer->m_objectUBOSize = refShaderPass->objectUBOSize();

        // ---- フレームごとのビューデータ (カメラ) 用の動的 UBO アロケータ ----
        {
            auto r = DynamicUniformAllocator::create(
                device, renderer->m_referencePipelineLayout,
                static_cast<uint32_t>(renderer->m_viewSetIndex), 0,
                static_cast<uint32_t>(sizeof(ViewParamsUBO)),
                framesInFlight);
            if (!r) return LN_FORWARD_ERROR(r);
            renderer->m_viewAllocator = std::move(*r);
        }

        // ---- シーン単位のデータ (ライティング等) 用の動的 UBO アロケータ ----
        {
            auto r = DynamicUniformAllocator::create(
                device, renderer->m_referencePipelineLayout,
                static_cast<uint32_t>(renderer->m_sceneSetIndex), 0,
                static_cast<uint32_t>(sizeof(SceneParamsUBO)),
                framesInFlight);
            if (!r) return LN_FORWARD_ERROR(r);
            renderer->m_sceneAllocator = std::move(*r);
        }

        // ---- オブジェクト単位のデータ用の動的 UBO アロケータ ----
        {
            auto r = DynamicUniformAllocator::create(
                device, renderer->m_referencePipelineLayout,
                static_cast<uint32_t>(renderer->m_objectSetIndex), 0,
                static_cast<uint32_t>(renderer->m_objectUBOSize),
                framesInFlight);
            if (!r) return LN_FORWARD_ERROR(r);
            renderer->m_objectAllocator = std::move(*r);
        }
    }

    renderer->m_ctx = ctx;
    return renderer;
}

// ------ フレームのライフサイクル -------------------------------------------------------------------------------------------

void Renderer::beginFrame() {
    uint32_t frame = m_frameCounter++;
    m_currentFrameSlot = frame % m_framesInFlight;
    if (m_viewAllocator)   m_viewAllocator->beginFrame(frame);
    if (m_sceneAllocator)  m_sceneAllocator->beginFrame(frame);
    if (m_objectAllocator) m_objectAllocator->beginFrame(frame);
    m_currentCmd = m_ctx->currentCommandBuffer();
    m_drawCallCount = 0;

    // バッチプロセッサは初回使用時に遅延初期化する。
    if (!m_batchProcessor) {
        auto result = BatchProcessor::create(m_ctx);
        if (result) {
            m_batchProcessor = std::move(*result);
        }
    }

    // このフレームのフラッシュごとのバッファを再利用できるよう、スプライトメッシュプールを巻き戻す。
    if (m_batchProcessor) {
        m_batchProcessor->resetFrame();
    }
}

void Renderer::endFrame() {
    // 送信前に動的ユニフォームバッファを GPU へフラッシュする。
    if (m_viewAllocator)   (void)m_viewAllocator->flushFrame();
    if (m_sceneAllocator)  (void)m_sceneAllocator->flushFrame();
    if (m_objectAllocator) (void)m_objectAllocator->flushFrame();

    m_currentCmd->submit();
    m_currentCmd = nullptr;
}

// ------ パスのライフサイクル --------------------------------------------------------------------------------------------

void Renderer::beginRenderPass(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Camera& camera,
    const Color& clearColor,
    SortMode sortMode) {

    // フレームごとのアロケータからビュー UBO を確保し、カメラデータをアップロードする。
    auto viewAlloc = m_viewAllocator->allocate();
    {
        ViewParamsUBO viewParams{};
        Matrix4x4 vp = camera.viewProjectionMatrix();
        Matrix4x4 v  = camera.viewMatrix();
        Matrix4x4 p  = camera.projectionMatrix();
        Matrix4x4 ivp = vp.inversed();
        Matrix4x4 ip  = p.inversed();
        std::memcpy(viewParams.viewProj, vp.m, sizeof(float) * 16);
        Matrix4x4 iv = v.inversed();
        viewParams.cameraPos[0] = iv.m[12];
        viewParams.cameraPos[1] = iv.m[13];
        viewParams.cameraPos[2] = iv.m[14];
        viewParams.cameraPos[3] = 0.0f;
        std::memcpy(viewParams.view, v.m, sizeof(float) * 16);
        std::memcpy(viewParams.proj, p.m, sizeof(float) * 16);
        std::memcpy(viewParams.invViewProj, ivp.m, sizeof(float) * 16);
        std::memcpy(viewParams.invProj, ip.m, sizeof(float) * 16);
        if (colorTarget) {
            float w = static_cast<float>(colorTarget->width());
            float h = static_cast<float>(colorTarget->height());
            viewParams.screenSize[0] = w;
            viewParams.screenSize[1] = h;
            viewParams.screenSize[2] = 1.0f / w;
            viewParams.screenSize[3] = 1.0f / h;
        }
        std::memcpy(viewAlloc.cpuPtr, &viewParams, sizeof(viewParams));
    }

    beginRenderPass(colorTarget, depthTarget, clearColor);
    m_currentCamera2D = camera.is2D();
    m_currentViewMatrix = camera.viewMatrix();
    m_currentSortMode = sortMode;
    setPassBindGroup(static_cast<uint32_t>(m_viewSetIndex), viewAlloc.bindGroup, viewAlloc.dynamicOffset, 1);
}

void Renderer::beginRenderPass(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Color& clearColor) {

    rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments.push_back({colorTarget, rhi::LoadOp::Clear, rhi::StoreOp::Store, clearColor});

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

    // endRenderPassWithTransition のために現在のカラーターゲットを記録する。
    m_currentColorTarget = colorTarget;

    // 前のパスのパススコープの BindGroup 状態をクリアする。
    for (uint32_t i = 0; i < kMaxBindGroupSets; ++i) {
        m_passBindGroups[i]                  = nullptr;
        m_passBindGroupDynamicOffsets[i]     = 0;
        m_passBindGroupDynamicOffsetCounts[i] = 0;
        m_passBindGroupDirty[i]              = false;
    }

    // このレンダーパス用に内部コマンドバッファをクリアする。
    m_commandBuffer.clear();
}

void Renderer::beginRenderPass(const rhi::RenderPassDesc& rpDesc, const Camera& camera,
                                const std::string& shaderPassName, SortMode sortMode) {
    m_currentShaderPassName = shaderPassName.empty() ? std::string("Forward") : shaderPassName;
    // フレームごとのアロケータからビュー UBO を確保し、カメラデータをアップロードする。
    auto viewAlloc = m_viewAllocator->allocate();
    {
        ViewParamsUBO viewParams{};
        Matrix4x4 vp = camera.viewProjectionMatrix();
        Matrix4x4 v  = camera.viewMatrix();
        Matrix4x4 p  = camera.projectionMatrix();
        Matrix4x4 ivp = vp.inversed();
        Matrix4x4 ip  = p.inversed();
        std::memcpy(viewParams.viewProj, vp.m, sizeof(float) * 16);
        Matrix4x4 iv = v.inversed();
        viewParams.cameraPos[0] = iv.m[12];
        viewParams.cameraPos[1] = iv.m[13];
        viewParams.cameraPos[2] = iv.m[14];
        viewParams.cameraPos[3] = 0.0f;
        std::memcpy(viewParams.view, v.m, sizeof(float) * 16);
        std::memcpy(viewParams.proj, p.m, sizeof(float) * 16);
        std::memcpy(viewParams.invViewProj, ivp.m, sizeof(float) * 16);
        std::memcpy(viewParams.invProj, ip.m, sizeof(float) * 16);
        if (!rpDesc.colorAttachments.empty() && rpDesc.colorAttachments[0].view) {
            float w = static_cast<float>(rpDesc.colorAttachments[0].view->width());
            float h = static_cast<float>(rpDesc.colorAttachments[0].view->height());
            viewParams.screenSize[0] = w;
            viewParams.screenSize[1] = h;
            viewParams.screenSize[2] = 1.0f / w;
            viewParams.screenSize[3] = 1.0f / h;
        }
        std::memcpy(viewAlloc.cpuPtr, &viewParams, sizeof(viewParams));
    }

    beginRenderPass(rpDesc, m_currentShaderPassName);
    m_currentCamera2D = camera.is2D();
    m_currentViewMatrix = camera.viewMatrix();
    m_currentSortMode = sortMode;
    setPassBindGroup(static_cast<uint32_t>(m_viewSetIndex), viewAlloc.bindGroup, viewAlloc.dynamicOffset, 1);
}

void Renderer::beginRenderPass(const rhi::RenderPassDesc& rpDesc,
                                const std::string& shaderPassName) {
    m_currentShaderPassName = shaderPassName.empty() ? std::string("Forward") : shaderPassName;
    m_currentCamera2D = false;
    m_currentPass = m_currentCmd->beginRenderPass(rpDesc);

    // endRenderPassWithTransition のために現在のカラーターゲットを記録する。
    m_currentColorTarget = rpDesc.colorAttachments.empty() ? nullptr : rpDesc.colorAttachments[0].view;

    // 前のパスのパススコープの BindGroup 状態をクリアする。
    for (uint32_t i = 0; i < kMaxBindGroupSets; ++i) {
        m_passBindGroups[i]                  = nullptr;
        m_passBindGroupDynamicOffsets[i]     = 0;
        m_passBindGroupDynamicOffsetCounts[i] = 0;
        m_passBindGroupDirty[i]              = false;
    }

    // このレンダーパス用に内部コマンドバッファをクリアする。
    m_commandBuffer.clear();
}

void Renderer::endRenderPass() {
    // レンダーパスを終了する前に、蓄積した描画コマンドをフラッシュする。
    (void)flushBatch();

    m_currentPass->end();
    m_currentPass = nullptr;
    m_currentColorTarget = nullptr;
    m_currentCamera2D = false;
    // 次のパスへソート状態が漏れないようリセットする。カメラ付き beginRenderPass が
    // begin 時に再設定する。カメラ無しパスは Stable のまま。
    m_currentSortMode = SortMode::Stable;
    m_currentViewMatrix = Matrix4x4::identity();
}

void Renderer::beginOverlayRenderPass(rhi::TextureView* colorTarget) {
    rhi::ColorAttachment colorAttach;
    colorAttach.view       = colorTarget;
    colorAttach.loadOp     = rhi::LoadOp::Load;
    colorAttach.storeOp    = rhi::StoreOp::Store;
    colorAttach.clearColor = Color{0.0f, 0.0f, 0.0f, 0.0f};

    rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments.push_back(colorAttach);
    rpDesc.depthStencilAttachment = nullptr;

    m_currentPass = m_currentCmd->beginRenderPass(rpDesc);

    for (uint32_t i = 0; i < kMaxBindGroupSets; ++i) {
        m_passBindGroups[i]                   = nullptr;
        m_passBindGroupDynamicOffsets[i]      = 0;
        m_passBindGroupDynamicOffsetCounts[i] = 0;
        m_passBindGroupDirty[i]               = false;
    }

    // 頂点はすでに NDC なので、単位行列のビュー射影行列をアップロードする。
    auto viewAlloc = m_viewAllocator->allocate();
    ViewParamsUBO viewParams{};
    // viewProj, view, proj, invViewProj に列優先の単位行列を設定する。
    viewParams.viewProj[0]  = 1.0f;
    viewParams.viewProj[5]  = 1.0f;
    viewParams.viewProj[10] = 1.0f;
    viewParams.viewProj[15] = 1.0f;
    viewParams.view[0]  = 1.0f;
    viewParams.view[5]  = 1.0f;
    viewParams.view[10] = 1.0f;
    viewParams.view[15] = 1.0f;
    viewParams.proj[0]  = 1.0f;
    viewParams.proj[5]  = 1.0f;
    viewParams.proj[10] = 1.0f;
    viewParams.proj[15] = 1.0f;
    viewParams.invViewProj[0]  = 1.0f;
    viewParams.invViewProj[5]  = 1.0f;
    viewParams.invViewProj[10] = 1.0f;
    viewParams.invViewProj[15] = 1.0f;
    viewParams.invProj[0]  = 1.0f;
    viewParams.invProj[5]  = 1.0f;
    viewParams.invProj[10] = 1.0f;
    viewParams.invProj[15] = 1.0f;
    std::memcpy(viewAlloc.cpuPtr, &viewParams, sizeof(viewParams));
    setPassBindGroup(static_cast<uint32_t>(m_viewSetIndex), viewAlloc.bindGroup, viewAlloc.dynamicOffset, 1);
}

void Renderer::setPassBindGroup(uint32_t setIndex, rhi::BindGroup* bindGroup,
                                uint32_t dynamicOffset, uint32_t dynamicOffsetCount) {
    if (setIndex >= kMaxBindGroupSets) return;
    m_passBindGroups[setIndex]                = bindGroup;
    m_passBindGroupDynamicOffsets[setIndex]     = dynamicOffset;
    m_passBindGroupDynamicOffsetCounts[setIndex] = dynamicOffsetCount;
    m_passBindGroupDirty[setIndex]             = true;
}

void Renderer::flushPassBindGroups() {
    for (uint32_t i = 0; i < kMaxBindGroupSets; ++i) {
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

// ------ 描画 (バッチ) -----------------------------------------------------------------------------------------

void Renderer::drawMesh(Mesh* mesh, const Transform& transform, int32_t zIndex) {
    m_commandBuffer.drawMesh(mesh, transform, zIndex);
}

void Renderer::drawSprite(Material* material, int32_t zIndex,
                          const Matrix4x4& transform, const Vector2& offset,
                          const Vector2& size, const Vector2& pivot,
                          const Vector2& uvOffset, const Vector2& uvSize,
                          const Color& color) {
    m_commandBuffer.drawSprite(material, zIndex, transform, offset, size, pivot, uvOffset, uvSize, color);
}

Result<void> Renderer::flushBatch() {
    if (m_batchProcessor && !m_commandBuffer.commands().empty()) {
        auto result = m_batchProcessor->flush(this, &m_commandBuffer,
                                              m_currentViewMatrix, m_currentSortMode);
        m_commandBuffer.clear();
        return result;
    }
    return {};
}

// ------ 描画 (即時) ---------------------------------------------------------------------------------------

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

    // 現在アクティブなレンダーパス名に基づいて、使用する ShaderPass を解決する。
    // マテリアルに一致するパスが無ければこの描画をスキップする (Unity URP の ShaderTagId と同じ挙動)。
    ShaderPass* activePass = mat->findPass(m_currentShaderPassName);
    if (!activePass) {
        return {};
    }

    auto* pipelineCache = m_ctx->pipelineCache();

    // オブジェクト単位の UBO スロットを確保し、トランスフォームを書き込む。
    auto alloc = m_objectAllocator->allocate();
    {
        Matrix4x4 worldMatrix  = transform.matrix();
        Matrix4x4 normalMatrix = transform.normalMatrix();
        ObjectParamsUBO objParams{};
        std::memcpy(objParams.world,        worldMatrix.m,  sizeof(float) * 16);
        std::memcpy(objParams.normalMatrix, normalMatrix.m, sizeof(float) * 16);
        std::memcpy(alloc.cpuPtr, &objParams, sizeof(objParams));
    }

    // パイプラインを解決する。
    PipelineCacheKey key;
    key.shaderPass          = activePass;
    key.cullMode            = mat->cullMode();
    key.blendState          = resolveBlendState(mat->blendMode());
    key.depthTestEnabled    = mat->depthTestEnabled();
    key.depthWriteEnabled   = mat->depthWriteEnabled();
    // 半透明ブレンドのマテリアルは深度を書き込まない。
    // テクスチャの透明部分を含む矩形全体が深度を書き込むと、同 zIndex の後続スプライトが
    // 深度テスト (Less) で弾かれ、円が矩形の輪郭で「見切れ」る不具合が起きる。
    // 半透明の前後関係は描画順 (ペインターズアルゴリズム, sortKey の sequence) で解決するため、
    // 透明矩形が深度バッファを上書きしないようにする。
    if (mat->blendMode() != BlendMode::Normal) {
        key.depthWriteEnabled = false;
    }
    // ステンシルマスクが有効なときは、描画をマスク領域内に制限するためステンシルテストを有効にする。
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
    if (!pipelineResult) return LN_FORWARD_ERROR(pipelineResult);

    m_currentPass->setPipeline(*pipelineResult);

    // ステンシルマスクが有効なときは、動的なステンシル参照値を設定する。
    if (m_stencilRef > 0) {
        m_currentPass->setStencilReference(m_stencilRef);
    }

    // setPipeline の後、保留中のパススコープの BindGroup をフラッシュする。
    flushPassBindGroups();

    // WebGPU では、drawIndexed の前にパイプラインレイアウトで宣言された全スロットに
    // BindGroup がバインドされている必要がある。シェーダが sceneData を ParameterBlock として
    // 宣言しているのにユーザーが setPassBindGroup でシーンの BindGroup を設定していない場合、
    // ゼロ初期化した SceneParamsUBO を確保してバインドし、バリデーションを通す。
    if (m_sceneSetIndex >= 0 && !m_passBindGroups[m_sceneSetIndex]) {
        auto sceneAlloc = m_sceneAllocator->allocate();
        SceneParamsUBO defaultScene{};
        std::memcpy(sceneAlloc.cpuPtr, &defaultScene, sizeof(defaultScene));
        setPassBindGroup(static_cast<uint32_t>(m_sceneSetIndex),
                         sceneAlloc.bindGroup, sceneAlloc.dynamicOffset, 1);
        flushPassBindGroups();
    }

    // Renderer 側のキャッシュからマテリアルの BindGroup を取得 (無ければ作成) する。
    auto matBGResult = getOrCreateMaterialBindGroup(mat, activePass);
    if (!matBGResult) return LN_FORWARD_ERROR(matBGResult);

    int16_t matSet = activePass->materialSetIndex();
    m_currentPass->setBindGroup(static_cast<uint32_t>(matSet), *matBGResult);
    m_currentPass->setBindGroup(static_cast<uint32_t>(m_objectSetIndex), alloc.bindGroup, &alloc.dynamicOffset, 1);
    m_currentPass->drawIndexed(sub.indexCount, 1, sub.indexOffset);
    ++m_drawCallCount;

    return {};
}

Result<void> Renderer::drawSingleSubMesh(
    Mesh* mesh, uint32_t submeshIndex, Material* material, const Transform& transform) {

    const auto& submeshes = mesh->submeshes();
    if (submeshIndex >= submeshes.size()) return {};

    m_currentPass->setVertexBuffer(0, mesh->vertexBuffer());
    m_currentPass->setIndexBuffer(mesh->indexBuffer(), rhi::IndexFormat::Uint32);
    return drawSubmesh(mesh, material, transform, submeshes[submeshIndex]);
}

Result<void> Renderer::drawScreenRect(Material* material) {
    auto meshResult = getScreenRectMesh();
    if (!meshResult) return LN_FORWARD_ERROR(meshResult);

    Transform identity;
    return drawMeshImmediate(*meshResult, identity, material);
}

// ------ ステンシルマスク --------------------------------------------------------------------------------------------

Result<void> Renderer::drawStencilMaskMesh(
    Mesh* mesh, const Transform& transform, Material* material,
    rhi::CompareFunction compare, uint32_t stencilRef, rhi::StencilOp passOp) {

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

        // ステンシルマスクの描画は専用のステンシルシェーダパスを使う - まずアクティブな
        // レンダーパス名を試し、無ければマテリアルの既定パスにフォールバックする。
        ShaderPass* activePass = mat->findPass(m_currentShaderPassName);
        if (!activePass) activePass = mat->shaderPass();
        if (!activePass) continue;

        // オブジェクト単位の UBO を確保する
        auto alloc = m_objectAllocator->allocate();
        {
            Matrix4x4 worldMatrix  = transform.matrix();
            Matrix4x4 normalMatrix = transform.normalMatrix();
            ObjectParamsUBO objParams{};
            std::memcpy(objParams.world,        worldMatrix.m,  sizeof(float) * 16);
            std::memcpy(objParams.normalMatrix, normalMatrix.m, sizeof(float) * 16);
            std::memcpy(alloc.cpuPtr, &objParams, sizeof(objParams));
        }

        // ステンシル書き込み用のパイプラインキーを構築する
        PipelineCacheKey key;
        key.shaderPass         = activePass;
        key.cullMode           = rhi::CullMode::None;
        key.blendState.enabled = false;
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
        if (!pipelineResult) return LN_FORWARD_ERROR(pipelineResult);

        m_currentPass->setPipeline(*pipelineResult);
        m_currentPass->setStencilReference(stencilRef);
        flushPassBindGroups();

        // drawSubmesh と同様: シーンの BindGroup が未設定なら既定のものを自動でバインドする。
        if (m_sceneSetIndex >= 0 && !m_passBindGroups[m_sceneSetIndex]) {
            auto sceneAlloc = m_sceneAllocator->allocate();
            SceneParamsUBO defaultScene{};
            std::memcpy(sceneAlloc.cpuPtr, &defaultScene, sizeof(defaultScene));
            setPassBindGroup(static_cast<uint32_t>(m_sceneSetIndex),
                             sceneAlloc.bindGroup, sceneAlloc.dynamicOffset, 1);
            flushPassBindGroups();
        }

        auto matBGResult = getOrCreateMaterialBindGroup(mat, activePass);
        if (!matBGResult) return LN_FORWARD_ERROR(matBGResult);

        int16_t matSet = activePass->materialSetIndex();
        m_currentPass->setBindGroup(static_cast<uint32_t>(matSet), *matBGResult);
        m_currentPass->setBindGroup(static_cast<uint32_t>(m_objectSetIndex), alloc.bindGroup, &alloc.dynamicOffset, 1);
        m_currentPass->drawIndexed(sub.indexCount, 1, sub.indexOffset);
        ++m_drawCallCount;
    }
    return {};
}

Result<void> Renderer::pushStencilMask(Mesh* mesh, const Transform& transform, Material* material) {
    // ステンシル状態を変更する前に、保留中のバッチコマンドをフラッシュする。
    auto flushResult = flushBatch();
    if (!flushResult) return flushResult;

    // 後の pop のためにマスク情報を保存する。
    m_stencilMaskStack.push_back({mesh, transform, material});

    // マスクメッシュをステンシルバッファへ描画する: stencil == m_stencilRef の場所をインクリメントする。
    // この後、マスク領域は stencil == m_stencilRef + 1 になる。
    auto result = drawStencilMaskMesh(
        mesh, transform, material,
        rhi::CompareFunction::Equal,  // compare: stencil == 現在の参照値 の場所で合格
        m_stencilRef,                 // 動的な参照値
        rhi::StencilOp::IncrementClamp);
    if (!result) return result;

    m_stencilRef++;
    return {};
}

Result<void> Renderer::popStencilMask() {
    // ステンシル状態を変更する前に、保留中のバッチコマンドをフラッシュする。
    auto flushResult = flushBatch();
    if (!flushResult) return flushResult;

    if (m_stencilMaskStack.empty()) {
        return LN_MAKE_ERROR("Stencil mask stack underflow");
    }

    auto entry = m_stencilMaskStack.back();
    m_stencilMaskStack.pop_back();

    // マスクメッシュを再描画してステンシル値をデクリメントして戻す。
    auto result = drawStencilMaskMesh(
        entry.mesh, entry.transform, entry.material,
        rhi::CompareFunction::Equal,
        m_stencilRef,                 // インクリメント済みの値に一致させる
        rhi::StencilOp::DecrementClamp);
    if (!result) return result;

    m_stencilRef--;
    return {};
}

Result<rhi::Sampler*> Renderer::getOrCreateSampler(const SamplerState& state) {
    // SamplerState は取りうる値が少ないので、そのままビットパックしてキーにする。
    uint32_t key = static_cast<uint32_t>(state.filter) |
                   (static_cast<uint32_t>(state.address) << 8);

    auto it = m_samplerPool.find(key);
    if (it != m_samplerPool.end()) return it->second.get();

    auto sampResult = m_ctx->device()->createSampler(state.toSamplerDesc());
    if (!sampResult) return LN_FORWARD_ERROR(sampResult);
    auto* sampler = sampResult->get();
    m_samplerPool.emplace(key, std::move(*sampResult));
    return sampler;
}

Renderer::~Renderer() {
    *m_alive = false;
}

void Renderer::evictMaterialCache(Material* mat) {
    // フレームの途中で捨てても GPU 使用中の破棄にはならない (後述の bg.reset() と同じ理由)。
    for (auto it = m_materialCache.begin(); it != m_materialCache.end(); ) {
        if (it->first.mat == mat) {
            it = m_materialCache.erase(it);
        } else {
            ++it;
        }
    }
    m_trackedMaterials.erase(mat);
}

Result<rhi::BindGroup*> Renderer::getOrCreateMaterialBindGroup(Material* mat, ShaderPass* pass) {
    auto* device = m_ctx->device();
    uint32_t frameSlot = m_currentFrameSlot;

    auto& cache = m_materialCache[MaterialBindKey{mat, pass}];

    // 初回アクセス時に vector を初期化する。
    if (cache.writtenParamVersion.empty()) {
        cache.paramBuffers.resize(m_framesInFlight);
        cache.bindGroups.resize(m_framesInFlight);
        cache.writtenParamVersion.assign(m_framesInFlight, 0);

        // Material が破棄されたらこのエントリを捨てられるようにしておく。
        if (m_trackedMaterials.insert(mat).second) {
            auto alive = m_alive;
            mat->addDestroyCallback([this, alive](Material* m) {
                if (*alive) evictMaterialCache(m);
            });
        }
    }

    const auto& layoutDesc = pass->materialLayoutDesc();
    const auto& bindingNames = pass->materialBindingNames();

    if (mat->bindingVersion() != cache.bindingVersion) {
        cache.bindingVersion = mat->bindingVersion();

        // 使用中のフレームスロットの BindGroup も一緒に捨てるが、Vulkan は
        // VulkanBindGroup::finalize() が FrameResourceManager 経由で遅延解放し、
        // WebGPU は参照カウントで保持されるため、GPU 使用中の破棄にはならない。
        for (auto&& bg : cache.bindGroups) bg.reset();

        // SampledTexture バインディングごとにテクスチャを解決し、ビューを作成/更新する
        for (size_t i = 0; i < layoutDesc.entries.size(); ++i) {
            const auto& layoutEntry = layoutDesc.entries[i];
            if (layoutEntry.type != rhi::BindingType::SampledTexture) continue;

            // 名前付きテクスチャを検索し、無ければ baseTexture にフォールバックする
            rhi::Texture* tex = mat->baseTexture(); // 既定値
            if (i < bindingNames.size()) {
                auto it = mat->namedTextures().find(bindingNames[i]);
                if (it != mat->namedTextures().end()) {
                    tex = it->second.get();
                }
            }

            uint32_t binding = layoutEntry.binding;
            if (tex && (!cache.textureViews.count(binding) || cache.lastTextures[binding] != tex)) {
                auto tvResult = device->createTextureView(tex);
                if (!tvResult) return LN_FORWARD_ERROR(tvResult);
                cache.textureViews[binding] = std::move(*tvResult);
                cache.lastTextures[binding] = tex;
            }
        }

        // Sampler バインディングごとにサンプラーを解決する。
        // 名前付きの上書きが無ければマテリアル単位の設定が使われる。
        static const std::string kNoTextureName;
        const auto& samplerTextureNames = pass->materialSamplerTextureNames();
        for (size_t i = 0; i < layoutDesc.entries.size(); ++i) {
            const auto& layoutEntry = layoutDesc.entries[i];
            if (layoutEntry.type != rhi::BindingType::Sampler) continue;

            const std::string& texName =
                (i < samplerTextureNames.size()) ? samplerTextureNames[i] : kNoTextureName;
            auto sampResult = getOrCreateSampler(mat->resolveSamplerState(texName));
            if (!sampResult) return LN_FORWARD_ERROR(sampResult);

            // Sampler は m_samplerPool で共有されるため、毎回代入しても再生成は起きない。
            cache.samplers[layoutEntry.binding] = Ref<rhi::Sampler>::retain(*sampResult);
        }
    }

    // このスロットの UBO も BindGroup も最新なら、そのまま使い回す。
    if (cache.writtenParamVersion[frameSlot] == mat->paramVersion() && cache.bindGroups[frameSlot]) {
        return cache.bindGroups[frameSlot].get();
    }

    // フレームごとのバッファが無ければ作成する。
    // マテリアル定数バッファを持たないシェーダ (例: フルスクリーン blit) では
    // materialParamBufferSize() == 0 になる。0 バイトの UBO 作成はバックエンドの
    // バリデーションエラーになり得るため、CB がある場合のみ作成して書き込む。
    // この場合 materialLayoutDesc に UniformBuffer エントリは存在しないため、
    // 後段の BindGroup 構築で paramBuffer が参照されることもない。
    if (mat->materialParamBufferSize() > 0 && !cache.paramBuffers[frameSlot]) {
        rhi::BufferDesc bufDesc;
        bufDesc.size = mat->materialParamBufferSize();
        bufDesc.usage = rhi::BufferUsage::Uniform;
        auto bufResult = device->createBuffer(bufDesc);
        if (!bufResult) return LN_FORWARD_ERROR(bufResult);
        cache.paramBuffers[frameSlot] = std::move(*bufResult);
    }

    // writeBuffer で UBO データを書き込む (全バックエンドで使える)。
    if (mat->materialParamBufferSize() > 0 && cache.writtenParamVersion[frameSlot] != mat->paramVersion()) {
        auto uboSize = mat->materialParamBufferSize();
        uint8_t uboStaging[512];
        std::vector<uint8_t> uboStagingHeap;
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

    // パスの PipelineLayout を使い、マテリアルセットのインデックスで BindGroup を作成する。
    // エントリはリフレクションを使って動的に構築する。
    if (!cache.bindGroups[frameSlot]) {
        int16_t matSet = pass->materialSetIndex();
        if (layoutDesc.entries.size() > rhi::kMaxBindGroupEntries) {
            return LN_MAKE_ERROR("Material bind group has too many entries.");
        }
        rhi::BindGroupEntry entries[rhi::kMaxBindGroupEntries] = {};
        size_t entryCount = 0;

        for (size_t i = 0; i < layoutDesc.entries.size(); ++i) {
            const auto& layoutEntry = layoutDesc.entries[i];
            rhi::BindGroupEntry entry{};
            entry.binding = layoutEntry.binding;

            if (layoutEntry.type == rhi::BindingType::UniformBuffer) {
                entry.buffer = cache.paramBuffers[frameSlot].get();
                entry.size = mat->materialParamBufferSize();
            } else if (layoutEntry.type == rhi::BindingType::SampledTexture) {
                auto viewIt = cache.textureViews.find(layoutEntry.binding);
                if (viewIt != cache.textureViews.end()) {
                    entry.textureView = viewIt->second.get();
                }
            } else if (layoutEntry.type == rhi::BindingType::Sampler) {
                auto sampIt = cache.samplers.find(layoutEntry.binding);
                if (sampIt != cache.samplers.end()) {
                    entry.sampler = sampIt->second.get();
                }
            }
            entries[entryCount++] = entry;
        }

        auto bgResult = pass->pipelineLayout()->createBindGroup(
            static_cast<uint32_t>(matSet), entries, entryCount);
        if (!bgResult) return LN_FORWARD_ERROR(bgResult);
        cache.bindGroups[frameSlot] = std::move(*bgResult);
    }

    cache.writtenParamVersion[frameSlot] = mat->paramVersion();
    return cache.bindGroups[frameSlot].get();
}

Result<Mesh*> Renderer::getScreenRectMesh() {
    if (m_screenRectMesh) return m_screenRectMesh.get();

    // NDC での全画面クアッド: [-1,1]x[-1,1] を覆う。
    // UV: (0,0) = 左上, (1,1) = 右下。
    std::vector<Vertex> vertices = {
        {{-1.0f, -1.0f, 0.0f}, {0, 0, 1}, {0, 1}, Color::white(), {1, 0, 0, 1}},
        {{ 1.0f, -1.0f, 0.0f}, {0, 0, 1}, {1, 1}, Color::white(), {1, 0, 0, 1}},
        {{ 1.0f,  1.0f, 0.0f}, {0, 0, 1}, {1, 0}, Color::white(), {1, 0, 0, 1}},
        {{-1.0f,  1.0f, 0.0f}, {0, 0, 1}, {0, 0}, Color::white(), {1, 0, 0, 1}},
    };
    std::vector<uint32_t> indices = {0, 1, 2, 0, 2, 3};
    SubMesh sub;
    sub.indexOffset   = 0;
    sub.indexCount    = 6;
    sub.materialIndex = 0;

    auto result = Mesh::create(m_ctx->device(), vertices, indices, {sub});
    if (!result) return LN_FORWARD_ERROR(result);
    m_screenRectMesh = std::move(*result);
    return m_screenRectMesh.get();
}

} // namespace ln
