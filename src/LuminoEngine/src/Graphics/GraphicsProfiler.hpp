#pragma once
#include <LuminoEngine/Graphics/Common.hpp>

namespace ln {
namespace detail {

class GraphicsProfiler
{
public:
	GraphicsProfiler();

	void addConstantBuffer(ConstantBuffer* obj);
	void removeConstantBuffer(ConstantBuffer* obj);
	void addRenderTarget(RenderTargetTexture* obj);
	void removeRenderTarget(RenderTargetTexture* obj);
	void addDepthBuffer(DepthBuffer* obj);
	void removeDepthBuffer(DepthBuffer* obj);
	void addRenderPass(RenderPass* obj);
	void removeRenderPass(RenderPass* obj);

	int32_t constantBufferCount() const { return m_constantBufferCount; }
	int32_t renderTargetCount() const { return m_renderTargetCount; }
	int32_t depthBufferCount() const { return m_depthBufferCount; }
	int32_t renderPassCount() const { return m_renderPassCount; }

private:
	int32_t m_constantBufferCount;
	int32_t m_renderTargetCount;
	int32_t m_depthBufferCount;
	int32_t m_renderPassCount;
};

} // namespace detail
} // namespace ln

