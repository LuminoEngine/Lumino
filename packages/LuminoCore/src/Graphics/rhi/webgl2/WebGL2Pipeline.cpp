#include <LuminoBase/Logger.hpp>
#include <LuminoShader/Common.hpp>
#include "WebGL2Pipeline.hpp"
#include "WebGL2Device.hpp"
#include <algorithm>

namespace ln::rhi::webgl2 {

// ------ WebGL2BindGroup -------------------------------------------------------------------------------------------------------

void WebGL2BindGroup::init(const BindGroupEntry* entries, size_t entryCount) {
    for (size_t i = 0; i < entryCount; ++i) {
        m_entries.push_back(entries[i]);
    }
}

const BindGroupEntry* WebGL2BindGroup::find(uint32_t binding) const {
    for (const auto& e : m_entries) {
        if (e.binding == binding) return &e;
    }
    return nullptr;
}

// ------ WebGL2PipelineLayout --------------------------------------------------------------------------------------------------

void WebGL2PipelineLayout::init(const PipelineLayoutDesc& desc) {
    m_setLayouts = desc.setLayouts;
    m_combinedSamplers = desc.combinedSamplers;
}

Result<Ref<BindGroup>> WebGL2PipelineLayout::createBindGroup(
    uint32_t setIndex, const BindGroupEntry* entries, size_t entryCount) {
    if (setIndex >= m_setLayouts.size()) {
        return LN_MAKE_ERROR("WebGL2PipelineLayout::createBindGroup: setIndex out of range.");
    }
    if (entryCount > kMaxBindGroupEntries) {
        return LN_MAKE_ERROR("BindGroup entry count exceeds kMaxBindGroupEntries.");
    }
    auto bg = Ref<WebGL2BindGroup>::adopt(new WebGL2BindGroup());
    bg->init(entries, entryCount);
    return Ref<BindGroup>(bg);
}

// ------ WebGL2RenderPipeline --------------------------------------------------------------------------------------------------

VoidResult WebGL2RenderPipeline::init(const RenderPipelineDesc& desc) {
    if (!desc.vertexShader || !desc.fragmentShader) {
        return LN_MAKE_ERROR("RenderPipelineDesc requires vertex and fragment shaders.");
    }
    if (!desc.layout) {
        return LN_MAKE_ERROR("RenderPipelineDesc requires a PipelineLayout.");
    }
    m_debugName = desc.debugName.empty() ? std::string("LuminoPipeline") : desc.debugName;
    // プログラムの破棄はシェーダモジュールと同じコンテキスト上で行う必要がある。
    m_device = static_cast<WebGL2ShaderModule*>(desc.vertexShader)->device();
    auto* layout = static_cast<WebGL2PipelineLayout*>(desc.layout);
    m_combinedSamplers = layout->combinedSamplers();
    m_topology = toGLPrimitive(desc.topology);
    m_vertexBuffers = desc.vertexBuffers;
    m_cullMode = desc.cullMode;
    m_frontFace = desc.frontFace;
    m_blend = desc.blendStates.empty() ? BlendState{} : desc.blendStates[0];
    m_depthStencil = desc.depthStencil;

    // ---- プログラムのリンク ----
    // ESSL 300 のエントリポイントは main 固定。SPIRV-Cross がエントリポイントごとに
    // 別ソースを出力するため、desc.vertexEntry / fragmentEntry は参照しない。
    auto vsResult = static_cast<WebGL2ShaderModule*>(desc.vertexShader)->getOrCompile(GL_VERTEX_SHADER);
    if (!vsResult) return LN_FORWARD_ERROR(vsResult);
    auto fsResult = static_cast<WebGL2ShaderModule*>(desc.fragmentShader)->getOrCompile(GL_FRAGMENT_SHADER);
    if (!fsResult) return LN_FORWARD_ERROR(fsResult);

    m_program = glCreateProgram();
    if (!m_program) {
        return LN_MAKE_ERROR("glCreateProgram failed. (%s)", m_debugName.c_str());
    }
    glAttachShader(m_program, *vsResult);
    glAttachShader(m_program, *fsResult);
    glLinkProgram(m_program);
    // シェーダは各 ShaderModule が所有し続けるため、ここではデタッチのみ行う。
    glDetachShader(m_program, *vsResult);
    glDetachShader(m_program, *fsResult);

    GLint linked = GL_FALSE;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        GLint logLength = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<size_t>(logLength > 0 ? logLength : 1), '\0');
        glGetProgramInfoLog(m_program, logLength, nullptr, log.data());
        return LN_MAKE_ERROR("Failed to link GLSL ES program. (%s) %s", m_debugName.c_str(), log.c_str());
    }

    // ---- combined sampler にテクスチャユニット番号を割り当てる ----
    // ESSL 300 には layout(binding = N) が無いため、名前でロケーションを引いて設定する。
    // 対応表の添字がそのままユニット番号になる (P1 で確定したスキーマ)。
    glUseProgram(m_program);
    for (size_t i = 0; i < m_combinedSamplers.size(); ++i) {
        const GLint loc = glGetUniformLocation(m_program, m_combinedSamplers[i].name.c_str());
        if (loc < 0) {
            // 両ステージとも参照していない組は最適化で消える。エラーにはしない。
            continue;
        }
        glUniform1i(loc, static_cast<GLint>(i));
    }

    // ---- ユニフォームブロックにバインディングポイントを割り当てる ----
    uint32_t nextBindingPoint = 0;
    for (size_t set = 0; set < layout->setLayouts().size(); ++set) {
        const auto& entries = layout->setLayouts()[set].entries;

        // 動的オフセットの並び順は WebGPU と同じく binding 番号の昇順とする。
        std::vector<uint32_t> dynamicBindings;
        for (const auto& e : entries) {
            if (e.hasDynamicOffset && e.type == BindingType::UniformBuffer) {
                dynamicBindings.push_back(e.binding);
            }
        }
        std::sort(dynamicBindings.begin(), dynamicBindings.end());

        for (const auto& e : entries) {
            if (e.type != BindingType::UniformBuffer) continue;

            const std::string blockName = shader::glslUniformBlockName(
                static_cast<int32_t>(set), static_cast<int32_t>(e.binding));
            const GLuint blockIndex = glGetUniformBlockIndex(m_program, blockName.c_str());
            if (blockIndex == GL_INVALID_INDEX) {
                // どちらのステージからも参照されていないブロックはリンク時に消える。
                continue;
            }
            glUniformBlockBinding(m_program, blockIndex, nextBindingPoint);

            GLUniformBlockBinding ub;
            ub.set = static_cast<uint32_t>(set);
            ub.binding = e.binding;
            ub.bindingPoint = nextBindingPoint;
            ub.dynamicIndex = -1;
            auto itr = std::find(dynamicBindings.begin(), dynamicBindings.end(), e.binding);
            if (itr != dynamicBindings.end()) {
                ub.dynamicIndex = static_cast<int32_t>(std::distance(dynamicBindings.begin(), itr));
            }
            m_uniformBlocks.push_back(ub);
            ++nextBindingPoint;
        }
    }

    return LN_MAKE_SUCCESS();
}

void WebGL2RenderPipeline::applyFixedFunctionState() const {
    // カリング。
    if (m_cullMode == CullMode::None) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
        glCullFace(m_cullMode == CullMode::Front ? GL_FRONT : GL_BACK);
    }

    // ワインディングは反転させる。頂点シェーダで gl_Position.y を反転している
    // (SPIRV-Cross の flip_vert_y) ため、画面上の巻き方向が入れ替わるため。
    glFrontFace(m_frontFace == FrontFace::CCW ? GL_CW : GL_CCW);

    // ブレンド。
    if (m_blend.enabled) {
        glEnable(GL_BLEND);
        glBlendFuncSeparate(toGLBlendFactor(m_blend.srcColor), toGLBlendFactor(m_blend.dstColor),
                            toGLBlendFactor(m_blend.srcAlpha), toGLBlendFactor(m_blend.dstAlpha));
        glBlendEquationSeparate(toGLBlendOp(m_blend.colorOp), toGLBlendOp(m_blend.alphaOp));
    } else {
        glDisable(GL_BLEND);
    }
    const GLboolean writeColor = m_blend.colorWriteEnabled ? GL_TRUE : GL_FALSE;
    glColorMask(writeColor, writeColor, writeColor, writeColor);

    // 深度。GL は GL_DEPTH_TEST を無効にすると書き込みも行われないため、
    // 「テストはしないが書き込む」場合は GL_ALWAYS でテストを有効にする
    // (WebGPU の depthCompare = Always と同じ扱い)。
    if (!m_depthStencil.depthTestEnable && !m_depthStencil.depthWriteEnable) {
        glDisable(GL_DEPTH_TEST);
    } else {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(m_depthStencil.depthTestEnable
            ? toGLCompareFunc(m_depthStencil.depthCompare)
            : GL_ALWAYS);
    }
    glDepthMask(m_depthStencil.depthWriteEnable ? GL_TRUE : GL_FALSE);

    // ステンシル。参照値は applyStencilReference で別途設定する。
    if (m_depthStencil.stencilTestEnable) {
        glEnable(GL_STENCIL_TEST);
        glStencilOpSeparate(GL_FRONT,
            toGLStencilOp(m_depthStencil.stencilFront.failOp),
            toGLStencilOp(m_depthStencil.stencilFront.depthFailOp),
            toGLStencilOp(m_depthStencil.stencilFront.passOp));
        glStencilOpSeparate(GL_BACK,
            toGLStencilOp(m_depthStencil.stencilBack.failOp),
            toGLStencilOp(m_depthStencil.stencilBack.depthFailOp),
            toGLStencilOp(m_depthStencil.stencilBack.passOp));
        glStencilMask(m_depthStencil.stencilWriteMask);
    } else {
        glDisable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
    }
}

void WebGL2RenderPipeline::applyStencilReference(uint32_t reference) const {
    if (!m_depthStencil.stencilTestEnable) return;
    glStencilFuncSeparate(GL_FRONT,
        toGLCompareFunc(m_depthStencil.stencilFront.compare),
        static_cast<GLint>(reference), m_depthStencil.stencilReadMask);
    glStencilFuncSeparate(GL_BACK,
        toGLCompareFunc(m_depthStencil.stencilBack.compare),
        static_cast<GLint>(reference), m_depthStencil.stencilReadMask);
}

void WebGL2RenderPipeline::finalize() {
    if (m_program) {
        if (m_device && m_device->isContextCurrent()) glDeleteProgram(m_program);
        m_program = 0;
    }
    RenderPipeline::finalize();
}

} // namespace ln::rhi::webgl2
