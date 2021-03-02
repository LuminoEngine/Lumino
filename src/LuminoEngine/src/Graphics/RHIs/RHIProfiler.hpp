#pragma once
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
	
class RHIProfiler
{
public:
	RHIProfiler();
	void addVertexLayout(IVertexDeclaration* obj);
	void removeVertexLayout(IVertexDeclaration* obj);
	void addVertexBuffer(IVertexBuffer* obj);
	void removeVertexBuffer(IVertexBuffer* obj);
	void addIndexBuffer(IIndexBuffer* obj);
	void removeIndexBuffer(IIndexBuffer* obj);
	void addTexture2D(ITexture* obj);
	void removeTexture2D(ITexture* obj);
	void addRenderTarget(ITexture* obj);
	void removeRenderTarget(ITexture* obj);
	void addDepthBuffer(IDepthBuffer* obj);
	void removeDepthBuffer(IDepthBuffer* obj);
	void addSamplerState(ISamplerState* obj);
	void removeSamplerState(ISamplerState* obj);
	void addShaderPass(IShaderPass* obj);
	void removeShaderPass(IShaderPass* obj);
	void addUniformBuffer(IUniformBuffer* obj);
	void removeUniformBuffer(IUniformBuffer* obj);
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
};

} // namespace detail
} // namespace ln

