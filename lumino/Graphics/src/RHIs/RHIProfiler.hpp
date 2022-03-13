#pragma once
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {

class RHIProfiler {
public:
    RHIProfiler();

    //void resetFrame() { m_drawCall = 0; }
    //void increaseDrawCall() { m_drawCall++; }

    //int32_t drawCall() const { return m_drawCall; }

    void addVertexLayout(IVertexDeclaration* obj);
    void removeVertexLayout(IVertexDeclaration* obj);
    void addVertexBuffer(RHIResource* obj);
    void removeVertexBuffer(RHIResource* obj);
    void addIndexBuffer(RHIResource* obj);
    void removeIndexBuffer(RHIResource* obj);
    void addTexture2D(RHIResource* obj);
    void removeTexture2D(RHIResource* obj);
    void addRenderTarget(RHIResource* obj);
    void removeRenderTarget(RHIResource* obj);
    void addDepthBuffer(RHIResource* obj);
    void removeDepthBuffer(RHIResource* obj);
    void addSamplerState(ISamplerState* obj);
    void removeSamplerState(ISamplerState* obj);
    void addShaderPass(IShaderPass* obj);
    void removeShaderPass(IShaderPass* obj);
    void addUniformBuffer(RHIResource* obj);
    void removeUniformBuffer(RHIResource* obj);
    void addDescriptorPool(IDescriptorPool* obj);
    void removeDescriptorPool(IDescriptorPool* obj);
    void addRenderPass(IRenderPass* obj);
    void removeRenderPass(IRenderPass* obj);
    void addPipelineState(IPipeline* obj);
    void removePipelineState(IPipeline* obj);
    void addCommandList(ICommandList* obj);
    void removeCommandList(ICommandList* obj);
    void addSwapChain(ISwapChain* obj);
    void removeSwapChain(ISwapChain* obj);

    int32_t vertexLayoutCount() const { return m_vertexLayoutCount; }
    int32_t vertexBufferCount() const { return m_vertexBufferCount; }
    int32_t indexBufferCount() const { return m_indexBufferCount; }
    int32_t texture2DCount() const { return m_texture2DCount; }
    int32_t renderTargetCount() const { return m_renderTargetCount; }
    int32_t depthBufferCount() const { return m_depthBufferCount; }
    int32_t samplerStateCount() const { return m_samplerStateCount; }
    int32_t shaderPassCount() const { return m_shaderPassCount; }
    int32_t uniformBufferCount() const { return m_uniformBufferCount; }
    int32_t descriptorPoolCount() const { return m_descriptorPoolCount; }
    int32_t renderPassCount() const { return m_renderPassCount; }
    int32_t pipelineStateCount() const { return m_pipelineStateCount; }
    int32_t commandListCount() const { return m_commandListCount; }
    int32_t swapChainCount() const { return m_swapChainCount; }

private:
    int32_t m_vertexLayoutCount;
    int32_t m_vertexBufferCount;
    int32_t m_indexBufferCount;
    int32_t m_texture2DCount;
    int32_t m_renderTargetCount;
    int32_t m_depthBufferCount;
    int32_t m_samplerStateCount;
    int32_t m_shaderPassCount;
    int32_t m_uniformBufferCount;
    int32_t m_descriptorPoolCount;
    int32_t m_renderPassCount;
    int32_t m_pipelineStateCount;
    int32_t m_commandListCount;
    int32_t m_swapChainCount;

    //int32_t m_drawCall;
};

} // namespace detail
} // namespace ln
