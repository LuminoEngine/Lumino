#include <LuminoBase/Logger.hpp>
#include "WebGL2CommandBuffer.hpp"
#include "WebGL2Device.hpp"
#include "WebGL2Pipeline.hpp"
#include <cstring>

namespace ln::rhi::webgl2 {

namespace {

/** ロードオペレーションが Load 以外なら、そのアタッチメントの内容は捨ててよい。 */
bool needsInvalidate(LoadOp op) {
    return op == LoadOp::DontCare;
}

} // namespace

// ------ WebGL2RenderPass ------------------------------------------------------------------------------------------------------

void WebGL2RenderPass::init(WebGL2CommandBuffer* owner, const RenderPassLayoutDesc& layoutDesc) {
    m_owner = owner;
    m_layoutDesc = layoutDesc;
}

void WebGL2RenderPass::setPipeline(RenderPipeline* pipeline) {
    GLCommand c{};
    c.type = GLCmdType::SetPipeline;
    c.setPipeline.pipeline = pipeline;
    m_owner->record(c);
}

void WebGL2RenderPass::setVertexBuffer(uint32_t slot, Buffer* buffer, uint64_t offset) {
    GLCommand c{};
    c.type = GLCmdType::SetVertexBuffer;
    c.setVertexBuffer.slot = slot;
    c.setVertexBuffer.buffer = buffer;
    c.setVertexBuffer.offset = offset;
    m_owner->record(c);
}

void WebGL2RenderPass::setIndexBuffer(Buffer* buffer, IndexFormat format, uint64_t offset) {
    GLCommand c{};
    c.type = GLCmdType::SetIndexBuffer;
    c.setIndexBuffer.buffer = buffer;
    c.setIndexBuffer.format = format;
    c.setIndexBuffer.offset = offset;
    m_owner->record(c);
}

void WebGL2RenderPass::setBindGroup(uint32_t index, BindGroup* group) {
    setBindGroup(index, group, nullptr, 0);
}

void WebGL2RenderPass::setBindGroup(uint32_t index, BindGroup* group,
                                    const uint32_t* dynamicOffsets, uint32_t dynamicOffsetCount) {
    GLCommand c{};
    c.type = GLCmdType::SetBindGroup;
    c.setBindGroup.setIndex = index;
    c.setBindGroup.group = group;
    c.setBindGroup.offsetPos = m_owner->recordDynamicOffsets(dynamicOffsets, dynamicOffsetCount);
    c.setBindGroup.offsetCount = dynamicOffsetCount;
    m_owner->record(c);
}

void WebGL2RenderPass::setViewport(float x, float y, float w, float h, float minDepth, float maxDepth) {
    GLCommand c{};
    c.type = GLCmdType::SetViewport;
    c.viewport = {x, y, w, h, minDepth, maxDepth};
    m_owner->record(c);
}

void WebGL2RenderPass::setScissorRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    GLCommand c{};
    c.type = GLCmdType::SetScissorRect;
    c.scissor = {x, y, w, h};
    m_owner->record(c);
}

void WebGL2RenderPass::setStencilReference(uint32_t reference) {
    GLCommand c{};
    c.type = GLCmdType::SetStencilReference;
    c.stencilRef.reference = reference;
    m_owner->record(c);
}

void WebGL2RenderPass::draw(uint32_t vertexCount, uint32_t instanceCount,
                            uint32_t firstVertex, uint32_t firstInstance) {
    GLCommand c{};
    c.type = GLCmdType::Draw;
    c.draw = {vertexCount, instanceCount, firstVertex, firstInstance};
    m_owner->record(c);
}

void WebGL2RenderPass::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                                   int32_t baseVertex, uint32_t firstInstance) {
    GLCommand c{};
    c.type = GLCmdType::DrawIndexed;
    c.drawIndexed = {indexCount, instanceCount, firstIndex, firstInstance, baseVertex};
    m_owner->record(c);
}

void WebGL2RenderPass::end() {
    GLCommand c{};
    c.type = GLCmdType::EndRenderPass;
    c.pass.passIndex = 0; // 直前に開始したパスを指すため、再生側で追跡する
    m_owner->record(c);
}

// ------ WebGL2CommandBuffer ---------------------------------------------------------------------------------------------------

VoidResult WebGL2CommandBuffer::init(WebGL2Device* device) {
    m_device = device;
    glGenFramebuffers(1, &m_framebuffer);
    if (!m_framebuffer) {
        return LN_MAKE_ERROR("glGenFramebuffers failed.");
    }
    return LN_MAKE_SUCCESS();
}

uint32_t WebGL2CommandBuffer::recordDynamicOffsets(const uint32_t* offsets, uint32_t count) {
    const uint32_t pos = static_cast<uint32_t>(m_dynamicOffsets.size());
    for (uint32_t i = 0; i < count; ++i) {
        m_dynamicOffsets.push_back(offsets[i]);
    }
    return pos;
}

RenderPass* WebGL2CommandBuffer::beginRenderPass(const RenderPassDesc& desc) {
    RecordedRenderPass pass;
    pass.hasDepthStencil = false;

    for (const auto& src : desc.colorAttachments) {
        pass.colorAttachments.push_back(src);
        auto* view = static_cast<WebGL2TextureView*>(src.view);
        pass.layoutDesc.colorFormats.push_back(view ? view->format() : TextureFormat::Undefined);
    }
    if (desc.depthStencilAttachment && desc.depthStencilAttachment->view) {
        pass.hasDepthStencil = true;
        pass.depthStencil = *desc.depthStencilAttachment;
        auto* dsView = static_cast<WebGL2TextureView*>(desc.depthStencilAttachment->view);
        pass.layoutDesc.depthStencilFormat = dsView->format();
    }

    GLCommand c{};
    c.type = GLCmdType::BeginRenderPass;
    c.pass.passIndex = static_cast<uint32_t>(m_passes.size());
    m_commands.push_back(c);
    m_passes.push_back(pass);

    if (!m_currentRenderPass) {
        m_currentRenderPass = Ref<WebGL2RenderPass>::adopt(new WebGL2RenderPass());
    }
    m_currentRenderPass->init(this, m_passes.back().layoutDesc);
    return m_currentRenderPass.get();
}

void WebGL2CommandBuffer::submit() {
    replay();
    m_commands.clear();
    m_passes.clear();
    m_dynamicOffsets.clear();
}

void WebGL2CommandBuffer::beginPass(const RecordedRenderPass& pass) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    // アタッチメントを張り替える。FBO をアタッチメントの組み合わせでキャッシュすると、
    // テクスチャ破棄後に GL 名が再利用されたときに古いエントリが残るため、毎回張り直す。
    std::vector<GLuint> attachments;
    GLenum drawBuffers[kMaxMultiRenderTargets];
    for (size_t i = 0; i < pass.colorAttachments.size(); ++i) {
        auto* view = static_cast<WebGL2TextureView*>(pass.colorAttachments[i].view);
        const GLuint tex = view ? view->textureHandle() : 0;
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i), GL_TEXTURE_2D, tex, 0);
        drawBuffers[i] = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i);
        attachments.push_back(tex);
    }
    // 前のパスで使っていた余分なカラーアタッチメントを外す。
    for (size_t i = pass.colorAttachments.size(); i < m_lastColorCount; ++i) {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i), GL_TEXTURE_2D, 0, 0);
    }
    m_lastColorCount = pass.colorAttachments.size();
    glDrawBuffers(static_cast<GLsizei>(pass.colorAttachments.size()), drawBuffers);

    GLuint depthTexture = 0;
    if (pass.hasDepthStencil) {
        depthTexture = static_cast<WebGL2TextureView*>(pass.depthStencil.view)->textureHandle();
    }
    // Depth24Stencil8 も Depth32Float も同じ口で扱う。前者はステンシルを含むため
    // GL_DEPTH_STENCIL_ATTACHMENT を使う。
    const bool depthOnly = pass.hasDepthStencil &&
        static_cast<WebGL2TextureView*>(pass.depthStencil.view)->format() == TextureFormat::Depth32Float;
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           depthOnly ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, depthTexture, 0);
    attachments.push_back(depthTexture);

    if (attachments != m_lastAttachments) {
        m_lastAttachments = attachments;
        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            LN_LOG_ERROR("[WebGL2] Framebuffer is incomplete. (0x%04x)", static_cast<unsigned>(status));
        }
    }

    // クリアはシザーテストと書き込みマスクの影響を受けるため、先に全開放しておく。
    glDisable(GL_SCISSOR_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0xFF);

    for (size_t i = 0; i < pass.colorAttachments.size(); ++i) {
        const auto& att = pass.colorAttachments[i];
        if (att.loadOp == LoadOp::Clear) {
            const GLfloat rgba[4] = {att.clearColor.r, att.clearColor.g, att.clearColor.b, att.clearColor.a};
            glClearBufferfv(GL_COLOR, static_cast<GLint>(i), rgba);
        } else if (needsInvalidate(att.loadOp)) {
            const GLenum target = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i);
            glInvalidateFramebuffer(GL_FRAMEBUFFER, 1, &target);
        }
    }
    if (pass.hasDepthStencil) {
        const auto& ds = pass.depthStencil;
        if (ds.depthLoadOp == LoadOp::Clear && ds.stencilLoadOp == LoadOp::Clear && !depthOnly) {
            glClearBufferfi(GL_DEPTH_STENCIL, 0, ds.clearDepth, static_cast<GLint>(ds.clearStencil));
        } else {
            if (ds.depthLoadOp == LoadOp::Clear) {
                const GLfloat depth = ds.clearDepth;
                glClearBufferfv(GL_DEPTH, 0, &depth);
            }
            if (ds.stencilLoadOp == LoadOp::Clear && !depthOnly) {
                const GLint stencil = static_cast<GLint>(ds.clearStencil);
                glClearBufferiv(GL_STENCIL, 0, &stencil);
            }
        }
    }

    // ビューポートの既定はアタッチメント全域。
    // 頂点シェーダで gl_Position.y を反転しているため、レンダーターゲットの行 0 は
    // 画像の最上段になる。GL のビューポート y も行 0 起点なので、RHI (左上原点) と
    // 一致し、y 座標の変換は不要になる。
    uint32_t width = 0;
    uint32_t height = 0;
    if (!pass.colorAttachments.empty() && pass.colorAttachments[0].view) {
        width = pass.colorAttachments[0].view->width();
        height = pass.colorAttachments[0].view->height();
    } else if (pass.hasDepthStencil) {
        width = pass.depthStencil.view->width();
        height = pass.depthStencil.view->height();
    }
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void WebGL2CommandBuffer::endPass(const RecordedRenderPass& pass) {
    // StoreOp::DontCare のアタッチメントは破棄してよいことをドライバに伝える。
    GLenum discard[kMaxMultiRenderTargets + 1];
    GLsizei count = 0;
    for (size_t i = 0; i < pass.colorAttachments.size(); ++i) {
        if (pass.colorAttachments[i].storeOp == StoreOp::DontCare) {
            discard[count++] = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i);
        }
    }
    if (pass.hasDepthStencil && pass.depthStencil.depthStoreOp == StoreOp::DontCare) {
        discard[count++] = GL_DEPTH_ATTACHMENT;
    }
    if (count > 0) {
        glInvalidateFramebuffer(GL_FRAMEBUFFER, count, discard);
    }
}

void WebGL2CommandBuffer::applyDrawState(ReplayState& state) {
    if (!state.pipeline) {
        LN_LOG_ERROR("[WebGL2] draw without a pipeline.");
        return;
    }

    // ---- 頂点属性 ----
    // ponytail: VAO は使わず、描画のたびに属性を張り直す。ES 3.0 / WebGL2 は既定 VAO を
    // 使えるため単純さを優先した。描画数が増えて JS 呼び出しが問題になったら、
    // (頂点レイアウト, バッファ, オフセット) をキーにした VAO キャッシュへ差し替える。
    uint32_t newMask = 0;
    const auto& layouts = state.pipeline->vertexBuffers();
    for (size_t slot = 0; slot < layouts.size() && slot < kMaxVertexBufferSlots; ++slot) {
        auto* vb = static_cast<WebGL2Buffer*>(state.vertexBuffers[slot]);
        if (!vb) continue;
        glBindBuffer(GL_ARRAY_BUFFER, vb->handle());
        const auto& layout = layouts[slot];
        for (const auto& attr : layout.attributes) {
            const GLVertexFormatInfo fmt = toGLVertexFormat(attr.format);
            const GLuint loc = attr.location;
            glVertexAttribPointer(
                loc, fmt.size, fmt.type, fmt.normalized, static_cast<GLsizei>(layout.stride),
                reinterpret_cast<const void*>(
                    static_cast<uintptr_t>(state.vertexOffsets[slot] + attr.offset)));
            glVertexAttribDivisor(loc, layout.perInstance ? 1 : 0);
            newMask |= (1u << loc);
        }
    }
    // 前のパイプラインで有効にしていた属性のうち、今回使わないものは無効にする。
    // 有効なままだと WebGL がバッファ長の検証を行い、描画が失敗することがある。
    const uint32_t toEnable = newMask & ~state.enabledAttribs;
    const uint32_t toDisable = state.enabledAttribs & ~newMask;
    for (uint32_t i = 0; i < 32; ++i) {
        if (toEnable & (1u << i)) glEnableVertexAttribArray(i);
        if (toDisable & (1u << i)) glDisableVertexAttribArray(i);
    }
    state.enabledAttribs = newMask;

    // ---- ユニフォームバッファ ----
    for (const auto& ub : state.pipeline->uniformBlocks()) {
        if (ub.set >= kMaxBindGroupSets) continue;
        auto* group = state.groups[ub.set];
        if (!group) continue;
        const BindGroupEntry* entry = group->find(ub.binding);
        if (!entry || !entry->buffer) continue;

        uint64_t offset = entry->offset;
        if (ub.dynamicIndex >= 0) {
            const uint32_t pos = state.dynamicOffsetPos[ub.set] + static_cast<uint32_t>(ub.dynamicIndex);
            if (static_cast<uint32_t>(ub.dynamicIndex) < state.dynamicOffsetCount[ub.set]) {
                offset += m_dynamicOffsets[pos];
            }
        }
        const uint64_t size = (entry->size > 0) ? entry->size : (entry->buffer->size() - offset);
        glBindBufferRange(GL_UNIFORM_BUFFER, ub.bindingPoint,
                          static_cast<WebGL2Buffer*>(entry->buffer)->handle(),
                          static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size));
    }

    // ---- テクスチャ (combined sampler) ----
    // 対応表の添字がそのままテクスチャユニット番号。リンク時に glUniform1i で設定済み。
    const auto& combined = state.pipeline->combinedSamplers();
    for (size_t unit = 0; unit < combined.size(); ++unit) {
        const auto& cs = combined[unit];
        GLuint texture = 0;
        GLuint sampler = 0;
        if (cs.textureSet < kMaxBindGroupSets && state.groups[cs.textureSet]) {
            if (const BindGroupEntry* e = state.groups[cs.textureSet]->find(cs.textureBinding)) {
                if (e->textureView) {
                    texture = static_cast<WebGL2TextureView*>(e->textureView)->textureHandle();
                }
            }
        }
        if (cs.samplerSet < kMaxBindGroupSets && state.groups[cs.samplerSet]) {
            if (const BindGroupEntry* e = state.groups[cs.samplerSet]->find(cs.samplerBinding)) {
                if (e->sampler) {
                    sampler = static_cast<WebGL2Sampler*>(e->sampler)->handle();
                }
            }
        }
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + unit));
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindSampler(static_cast<GLuint>(unit), sampler);
    }
}

void WebGL2CommandBuffer::replay() {
    if (m_commands.empty()) return;

    ReplayState state{};
    const RecordedRenderPass* currentPass = nullptr;

    for (const auto& cmd : m_commands) {
        switch (cmd.type) {
            case GLCmdType::BeginRenderPass: {
                currentPass = &m_passes[cmd.pass.passIndex];
                beginPass(*currentPass);
                // パイプラインはパスをまたいで引き継がない (WebGPU のエンコーダと同じ扱い)。
                // beginPass が書き込みマスクを開放するため、引き継ぐと状態がずれる。
                state.pipeline = nullptr;
                break;
            }
            case GLCmdType::EndRenderPass: {
                if (currentPass) {
                    endPass(*currentPass);
                    currentPass = nullptr;
                }
                break;
            }
            case GLCmdType::SetPipeline: {
                state.pipeline = static_cast<const WebGL2RenderPipeline*>(cmd.setPipeline.pipeline);
                glUseProgram(state.pipeline->program());
                state.pipeline->applyFixedFunctionState();
                state.pipeline->applyStencilReference(state.stencilReference);
                break;
            }
            case GLCmdType::SetVertexBuffer: {
                if (cmd.setVertexBuffer.slot < kMaxVertexBufferSlots) {
                    state.vertexBuffers[cmd.setVertexBuffer.slot] = cmd.setVertexBuffer.buffer;
                    state.vertexOffsets[cmd.setVertexBuffer.slot] = cmd.setVertexBuffer.offset;
                }
                break;
            }
            case GLCmdType::SetIndexBuffer: {
                state.indexBuffer = cmd.setIndexBuffer.buffer;
                state.indexFormat = cmd.setIndexBuffer.format;
                state.indexOffset = cmd.setIndexBuffer.offset;
                break;
            }
            case GLCmdType::SetBindGroup: {
                if (cmd.setBindGroup.setIndex < kMaxBindGroupSets) {
                    const uint32_t i = cmd.setBindGroup.setIndex;
                    state.groups[i] = static_cast<WebGL2BindGroup*>(cmd.setBindGroup.group);
                    state.dynamicOffsetPos[i] = cmd.setBindGroup.offsetPos;
                    state.dynamicOffsetCount[i] = cmd.setBindGroup.offsetCount;
                }
                break;
            }
            case GLCmdType::SetViewport: {
                const auto& v = cmd.viewport;
                glViewport(static_cast<GLint>(v.x), static_cast<GLint>(v.y),
                           static_cast<GLsizei>(v.w), static_cast<GLsizei>(v.h));
                glDepthRangef(v.minDepth, v.maxDepth);
                break;
            }
            case GLCmdType::SetScissorRect: {
                const auto& s = cmd.scissor;
                glEnable(GL_SCISSOR_TEST);
                glScissor(static_cast<GLint>(s.x), static_cast<GLint>(s.y),
                          static_cast<GLsizei>(s.w), static_cast<GLsizei>(s.h));
                break;
            }
            case GLCmdType::SetStencilReference: {
                state.stencilReference = cmd.stencilRef.reference;
                if (state.pipeline) {
                    state.pipeline->applyStencilReference(state.stencilReference);
                }
                break;
            }
            case GLCmdType::Draw: {
                if (cmd.draw.firstInstance != 0) {
                    LN_LOG_ERROR("[WebGL2] firstInstance is not supported.");
                    break;
                }
                applyDrawState(state);
                glDrawArraysInstanced(state.pipeline->topology(),
                                      static_cast<GLint>(cmd.draw.firstVertex),
                                      static_cast<GLsizei>(cmd.draw.vertexCount),
                                      static_cast<GLsizei>(cmd.draw.instanceCount));
                break;
            }
            case GLCmdType::DrawIndexed: {
                if (cmd.drawIndexed.firstInstance != 0 || cmd.drawIndexed.baseVertex != 0) {
                    LN_LOG_ERROR("[WebGL2] firstInstance / baseVertex are not supported.");
                    break;
                }
                if (!state.indexBuffer) {
                    LN_LOG_ERROR("[WebGL2] drawIndexed without an index buffer.");
                    break;
                }
                applyDrawState(state);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                             static_cast<WebGL2Buffer*>(state.indexBuffer)->handle());
                const uint64_t byteOffset = state.indexOffset +
                    static_cast<uint64_t>(cmd.drawIndexed.firstIndex) * indexStride(state.indexFormat);
                glDrawElementsInstanced(state.pipeline->topology(),
                                        static_cast<GLsizei>(cmd.drawIndexed.indexCount),
                                        toGLIndexType(state.indexFormat),
                                        reinterpret_cast<const void*>(static_cast<uintptr_t>(byteOffset)),
                                        static_cast<GLsizei>(cmd.drawIndexed.instanceCount));
                break;
            }
        }
    }

    // 次のフレームへ状態を持ち越さないよう、有効にした頂点属性を戻しておく。
    for (uint32_t i = 0; i < 32; ++i) {
        if (state.enabledAttribs & (1u << i)) glDisableVertexAttribArray(i);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WebGL2CommandBuffer::finalize() {
    m_currentRenderPass = nullptr;
    if (m_framebuffer) {
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = 0;
    }
    CommandBuffer::finalize();
}

} // namespace ln::rhi::webgl2
