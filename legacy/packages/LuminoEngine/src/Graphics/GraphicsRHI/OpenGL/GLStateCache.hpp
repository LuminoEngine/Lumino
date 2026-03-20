#pragma once
#include "GLHelper.hpp"

namespace ln {
namespace detail {

// https://emscripten.org/docs/optimizing/Optimizing-WebGL.html
// OpenGL API の呼び出し 20 個で 1ms 弱かかる。
// NOTE: wasm で c++ 側で時間測るとき、1ms「以下」は信用しないほうがいいかも。
//       ElapsedTimer で即経過時間取っても、 1ms が出ることがある。

    // submitMeshDrawCommands全体で 70ms くらい。
    // onDrawPrimitiveIndexed を消すと submitMeshDrawCommands が: 17ms
    // onSubmitStatusも消すと: 1ms以下。
    // なぜか glUseProgram が滅茶苦茶時間かかってた。0.1msくらい。
    // 
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
    void useProgram(GLuint program);

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
    std::optional<GLuint> m_program;
};

} // namespace detail
} // namespace ln
