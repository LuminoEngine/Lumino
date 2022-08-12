
#include "Internal.hpp"
#include <LuminoGraphics/GPU/ConstantBuffer.hpp>
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/GPU/DepthBuffer.hpp>
#include <LuminoGraphics/GPU/RenderPass.hpp>
#include "GraphicsProfiler.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GraphicsProfiler

GraphicsProfiler::GraphicsProfiler()
	: m_constantBufferCount(0)
	, m_renderTargetCount(0)
	, m_depthBufferCount(0)
	, m_renderPassCount(0)
{
}

void GraphicsProfiler::addConstantBuffer(ConstantBuffer* obj)
{
	if (LN_REQUIRE(!obj->m_profiling)) return;
	m_constantBufferCount++;
	obj->m_profiling = true;
}

void GraphicsProfiler::removeConstantBuffer(ConstantBuffer* obj)
{
	if (LN_REQUIRE(obj->m_profiling)) return;
	m_constantBufferCount--;
	obj->m_profiling = false;
}

void GraphicsProfiler::addRenderTarget(RenderTargetTexture* obj)
{
	if (LN_REQUIRE(!obj->m_profiling)) return;
	m_renderTargetCount++;
	obj->m_profiling = true;
}

void GraphicsProfiler::removeRenderTarget(RenderTargetTexture* obj)
{
	if (LN_REQUIRE(obj->m_profiling)) return;
	m_renderTargetCount--;
	obj->m_profiling = false;
}

void GraphicsProfiler::addDepthBuffer(DepthBuffer* obj)
{
	if (LN_REQUIRE(!obj->m_profiling)) return;
	m_depthBufferCount++;
	obj->m_profiling = true;
}

void GraphicsProfiler::removeDepthBuffer(DepthBuffer* obj)
{
	if (LN_REQUIRE(obj->m_profiling)) return;
	m_depthBufferCount--;
	obj->m_profiling = false;
}

void GraphicsProfiler::addRenderPass(RenderPass* obj)
{
	if (LN_REQUIRE(!obj->m_profiling)) return;
	m_renderPassCount++;
	obj->m_profiling = true;
}

void GraphicsProfiler::removeRenderPass(RenderPass* obj)
{
	if (LN_REQUIRE(obj->m_profiling)) return;
	m_renderPassCount--;
	obj->m_profiling = false;
}

} // namespace detail
} // namespace ln

