#pragma once
#include "GLHelper.hpp"

namespace ln {
namespace detail {

class GLStateCache {
public:
    GLStateCache();

    void clear();
    void setBlendState(const BlendStateDesc& state);
    void setRasterizerState(const RasterizerStateDesc& state);
    void setDepthStencilState(const DepthStencilStateDesc& state);
    void setPrimitiveData(
        const GLVertexDeclaration* vertexLayout,
        const GLShaderPass* shaderPass,
        const std::array<RHIResource*, MaxVertexStreams>& vertexBuffers
    );
    void setIndexBuffer(const GLIndexBuffer* indexBufferOrNull);


private:
    bool equalsPrimitiveData(
        const GLVertexDeclaration* vertexLayout,
        const GLShaderPass* shaderPass,
        const std::array<RHIResource*, MaxVertexStreams>& vertexBuffers) const;

    std::optional<BlendStateDesc> m_blendState;
    std::optional<RasterizerStateDesc> m_rasterizerState;
    std::optional<DepthStencilStateDesc> m_depthStencilState;
    const GLVertexDeclaration* m_vertexLayout;
    const GLShaderPass* m_shaderPass;
    std::array<RHIResource*, MaxVertexStreams> m_vertexBuffers;
    std::optional<const GLIndexBuffer*> m_indexBufferOrNull;

    struct {
        GLboolean m_GL_BLEND;
        GLboolean m_GL_CULL_FACE;
        GLboolean m_GL_DEPTH_TEST;
        GLboolean m_GL_DEPTH_WRITEMASK;
        GLint m_GL_DEPTH_FUNC;
        GLint m_GL_CULL_FACE_MODE;
        GLint m_GL_BLEND_SRC_RGB;
        GLint m_GL_BLEND_DST_RGB;
        GLint m_GL_BLEND_SRC_ALPHA;
        GLint m_GL_BLEND_DST_ALPHA;
        GLint m_GL_BLEND_EQUATION;
        //GLint m_GL_ARRAY_BUFFER_BINDING;
        //GLint m_GL_ELEMENT_ARRAY_BUFFER_BINDING;
        GLint m_GL_CURRENT_PROGRAM;
        GLint m_vao;
        GLint m_drawFBO;
        GLfloat m_clearColor[4];
    } m_savedState;
};

} // namespace detail
} // namespace ln
