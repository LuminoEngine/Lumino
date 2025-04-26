#include "GLVertexBuffer.hpp"
#include "GLIndexBuffer.hpp"
#include "GLShaderPass.hpp"
#include "OpenGLDeviceContext.hpp"
#include "GLStateCache.hpp"


namespace ln {
namespace detail {

GLStateCache::GLStateCache()
    : m_blendState()
    , m_rasterizerState()
    , m_depthStencilState()
    , m_vertexLayout(nullptr)
    , m_shaderPass(nullptr)
    , m_vertexBuffers{}
    , m_indexBufferOrNull()
    , m_program() {
}

void GLStateCache::clear() {
    m_blendState = std::nullopt;
    m_rasterizerState = std::nullopt;
    m_depthStencilState = std::nullopt;
    m_vertexLayout = nullptr;
    m_shaderPass = nullptr;
    m_vertexBuffers.fill(nullptr);
    m_indexBufferOrNull = std::nullopt;
    m_program = std::nullopt;
}

void GLStateCache::setBlendState(const BlendStateDesc& state) {
    if (m_blendState.has_value() && BlendStateDesc::equals(*m_blendState, state)) {
        return;
    }

    
    static const GLenum blendOpTable[] = {
        GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX,
    };

    static const GLenum blendFactorTable[] = { GL_ZERO,      GL_ONE,
                                    GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
                                    GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                                    GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
                                    GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA };

#ifdef GL_GLES_PROTOTYPES
    // OpenGL ES is unsupported
#else
    if (state.independentBlendEnable) {
        for (int i = 0; i < 8; i++) // TODO: num RT
        {
            const RenderTargetBlendDesc& desc = state.renderTargets[i];

            // blendEnable
            if (desc.blendEnable) {
                GL_CHECK_DEBUG(glEnablei(GL_BLEND, i));
            }
            else {
                GL_CHECK_DEBUG(glEnablei(GL_BLEND, i));
            }

            // sourceBlend
            // destinationBlend
            GL_CHECK_DEBUG(glBlendFuncSeparatei(
                i,
                blendFactorTable[(int)desc.sourceBlend],
                blendFactorTable[(int)desc.destinationBlend],
                blendFactorTable[(int)desc.sourceBlendAlpha],
                blendFactorTable[(int)desc.destinationBlendAlpha]));

            // blendOp
            GL_CHECK_DEBUG(
                glBlendEquationSeparatei(i, blendOpTable[(int)desc.blendOp], blendOpTable[(int)desc.blendOpAlpha]));
        }
    }
    else
#endif
    {
        const RenderTargetBlendDesc& desc = state.renderTargets[0];

        // blendEnable
        if (desc.blendEnable) {
            GL_CHECK_DEBUG(glEnable(GL_BLEND));
        }
        else {
            GL_CHECK_DEBUG(glDisable(GL_BLEND));
        }

        // blendOp
        {
            GL_CHECK_DEBUG(
                glBlendEquationSeparate(blendOpTable[(int)desc.blendOp], blendOpTable[(int)desc.blendOpAlpha]));
        }

        // sourceBlend
        // destinationBlend
        {
            GL_CHECK_DEBUG(glBlendFuncSeparate(
                blendFactorTable[(int)desc.sourceBlend],
                blendFactorTable[(int)desc.destinationBlend],
                blendFactorTable[(int)desc.sourceBlendAlpha],
                blendFactorTable[(int)desc.destinationBlendAlpha]));
        }
    }
    
    m_blendState = state;
}

void GLStateCache::setRasterizerState(const RasterizerStateDesc& state) {
    if (m_rasterizerState.has_value() && RasterizerStateDesc::equals(*m_rasterizerState, state)) {
        return;
    }

    // fillMode
#ifdef GL_GLES_PROTOTYPES
    // OpenGL ES is glPolygonMode unsupported
#else
    const GLenum tb[] = { GL_FILL, GL_LINE, GL_POINT };
    GL_CHECK_DEBUG(glPolygonMode(GL_FRONT_AND_BACK, tb[(int)state.fillMode]));
#endif
    // cullingMode
    GL_CHECK_DEBUG(glFrontFace(GL_CCW));
    switch (state.cullMode) {
        case CullMode::None:
            GL_CHECK_DEBUG(glDisable(GL_CULL_FACE));
            break;
        case CullMode::Front:
            GL_CHECK_DEBUG(glEnable(GL_CULL_FACE));
            GL_CHECK_DEBUG(glCullFace(GL_FRONT));
            break;
        case CullMode::Back:
            GL_CHECK_DEBUG(glEnable(GL_CULL_FACE));
            GL_CHECK_DEBUG(glCullFace(GL_BACK));
            break;
        default:
            LN_UNREACHABLE();
            break;
    }

    m_rasterizerState = state;
}

void GLStateCache::setDepthStencilState(const DepthStencilStateDesc& state) {
    if (m_depthStencilState.has_value() && DepthStencilStateDesc::equals(*m_depthStencilState, state)) {
        return;
    }
    GLenum cmpFuncTable[] = {
        GL_NEVER,    // Never
        GL_LESS,     // Less
        GL_LEQUAL,   // LessEqual
        GL_GREATER,  // Greater
        GL_GEQUAL,   // GreaterEqual
        GL_EQUAL,    // Equal
        GL_NOTEQUAL, // NotEqual
        GL_ALWAYS,   // Always
    };

    //// depthTestEnabled
    // if (depthStencilState.depthTestEnabled) {
    //	GL_CHECK_DEBUG(glEnable(GL_DEPTH_TEST));
    // }
    // else {
    //	GL_CHECK_DEBUG(glDisable(GL_DEPTH_TEST));
    // }

    if (state.depthTestFunc == ComparisonFunc::Always) {
        GL_CHECK_DEBUG(glDisable(GL_DEPTH_TEST));
    }
    else {
        GL_CHECK_DEBUG(glEnable(GL_DEPTH_TEST));
        GL_CHECK_DEBUG(glDepthFunc(cmpFuncTable[(int)state.depthTestFunc]));
    }

    // depthWriteEnabled
    GL_CHECK_DEBUG(glDepthMask(state.depthWriteEnabled ? GL_TRUE : GL_FALSE));

    // stencilEnabled
    if (state.stencilEnabled) {
        GL_CHECK_DEBUG(glEnable(GL_STENCIL_TEST));
    }
    else {
        GL_CHECK_DEBUG(glDisable(GL_STENCIL_TEST));
    }

    // stencilFunc
    // stencilReferenceValue
    // stencilFailOp
    // stencilDepthFailOp
    // stencilPassOp
    GLenum stencilOpTable[] = { GL_KEEP, GL_REPLACE };
#ifdef LN_FACE_FRONT_CW
    GL_CHECK_DEBUG(glStencilFuncSeparate(
        GL_BACK,
        cmpFuncTable[(int)state.frontFace.stencilFunc],
        state.stencilReferenceValue,
        0xFFFFFFFF));
    GL_CHECK_DEBUG(glStencilFuncSeparate(
        GL_FRONT,
        cmpFuncTable[(int)state.backFace.stencilFunc],
        state.stencilReferenceValue,
        0xFFFFFFFF));
    GL_CHECK_DEBUG(glStencilOpSeparate(
        GL_BACK,
        stencilOpTable[(int)state.frontFace.stencilFailOp],
        stencilOpTable[(int)state.frontFace.stencilDepthFailOp],
        stencilOpTable[(int)state.frontFace.stencilPassOp]));
    GL_CHECK_DEBUG(glStencilOpSeparate(
        GL_FRONT,
        stencilOpTable[(int)state.backFace.stencilFailOp],
        stencilOpTable[(int)state.backFace.stencilDepthFailOp],
        stencilOpTable[(int)state.backFace.stencilPassOp]));
#else
    GL_CHECK_DEBUG(glStencilFuncSeparate(
        GL_FRONT,
        cmpFuncTable[(int)state.frontFace.stencilFunc],
        state.stencilReferenceValue,
        0xFFFFFFFF));
    GL_CHECK_DEBUG(glStencilFuncSeparate(
        GL_BACK,
        cmpFuncTable[(int)state.backFace.stencilFunc],
        state.stencilReferenceValue,
        0xFFFFFFFF));
    GL_CHECK_DEBUG(glStencilOpSeparate(
        GL_FRONT,
        stencilOpTable[(int)state.frontFace.stencilFailOp],
        stencilOpTable[(int)state.frontFace.stencilDepthFailOp],
        stencilOpTable[(int)state.frontFace.stencilPassOp]));
    GL_CHECK_DEBUG(glStencilOpSeparate(
        GL_BACK,
        stencilOpTable[(int)state.backFace.stencilFailOp],
        stencilOpTable[(int)state.backFace.stencilDepthFailOp],
        stencilOpTable[(int)state.backFace.stencilPassOp]));
#endif

    //GL_CHECK_DEBUG(glStencilOp(stencilOpTable[(int)m_depthStencilState.frontFace.stencilFailOp], stencilOpTable[(int)m_depthStencilState.frontFace.stencilDepthFailOp], stencilOpTable[(int)m_depthStencilState.frontFace.stencilPassOp]));
    
    m_depthStencilState = state;
}


void GLStateCache::setPrimitiveData(
    const GLVertexDeclaration* vertexLayout,
    const GLShaderPass* shaderPass,
    const std::array<RHIResource*, MaxVertexStreams>& vertexBuffers)
{
    if (equalsPrimitiveData(vertexLayout, shaderPass, vertexBuffers)) {
        return;
    }

    const auto& attributes = shaderPass->attributes();
    size_t count = attributes.size();
    for (size_t iAttr = 0; iAttr < count; iAttr++) {
        auto& attr = attributes[iAttr];

        // glslang からは、 SV_InstanceID も取得できるが、これには layoutLocation が付いていない。
        if (attr.usage == kokage::AttributeUsage_InstanceID) continue;

        if (const auto* element = vertexLayout->findGLVertexElement(attr.usage, attr.index)) {
            GL_CHECK_DEBUG(glEnableVertexAttribArray(attr.layoutLocation));
            GL_CHECK_DEBUG(glBindBuffer(
                GL_ARRAY_BUFFER,
                static_cast<const GLVertexBuffer*>(vertexBuffers[element->streamIndex])->objectId()));
            GL_CHECK_DEBUG(glVertexAttribPointer(
                attr.layoutLocation,
                element->size,
                element->type,
                element->normalized,
                element->stride,
                (void*)(element->byteOffset)));

            if (element->instance) {
                GL_CHECK_DEBUG(glVertexAttribDivisor(attr.layoutLocation, 1));
            }
            else {
                GL_CHECK_DEBUG(glVertexAttribDivisor(attr.layoutLocation, 0));
            }
        }
        else {
            GL_CHECK_DEBUG(glDisableVertexAttribArray(attr.layoutLocation));
            GL_CHECK_DEBUG(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }
    }

    m_vertexLayout = vertexLayout;
    m_shaderPass = shaderPass;
    m_vertexBuffers = vertexBuffers;
}

void GLStateCache::setIndexBuffer(const GLIndexBuffer* indexBufferOrNull) {
    if (m_indexBufferOrNull.has_value() && *m_indexBufferOrNull == indexBufferOrNull) {
        return;
    }

    if (indexBufferOrNull) {
        GL_CHECK_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferOrNull->objectId()));
    }
    else {
        //GL_CHECK_DEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    m_indexBufferOrNull = indexBufferOrNull;
}

void GLStateCache::useProgram(GLuint program) {
    if (m_program.has_value() && *m_program == program) {
        return;
    }

    GL_CHECK_DEBUG(glUseProgram(program));

    m_program = program;
}

bool GLStateCache::equalsPrimitiveData(
    const GLVertexDeclaration* vertexLayout,
    const GLShaderPass* shaderPass,
    const std::array<RHIResource*, MaxVertexStreams>& vertexBuffers) const {
    if (m_vertexLayout != vertexLayout) {
        return false;
    }
    if (m_shaderPass != shaderPass) {
        return false;
    }
    for (size_t i = 0; i < vertexBuffers.size(); i++) {
        if (m_vertexBuffers[i] != vertexBuffers[i]) {
            return false;
        }
    }
    return true;
}

} // namespace detail
} // namespace ln
