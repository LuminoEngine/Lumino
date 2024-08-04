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
};

} // namespace detail
} // namespace ln
