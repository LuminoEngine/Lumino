#include "DX12DeviceContext.hpp"
#include "DX12RenderPass.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12RenderPass

DX12RenderPass::DX12RenderPass()
	: m_device(nullptr)
	, m_clearFlags(ClearFlags::None)
	, m_clearColor()
	, m_clearDepth(1.0f)
	, m_clearStencil(0x00)
{
}

bool DX12RenderPass::init(DX12Device* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil)
{
	LN_CHECK(device);
	m_device = device;
	m_clearFlags = clearFlags;
	m_clearColor = clearColor;
	m_clearDepth = clearDepth;
	m_clearStencil = clearStencil;

	return true;
}

void DX12RenderPass::dispose()
{
	m_device = nullptr;
	IRenderPass::dispose();
}

} // namespace detail
} // namespace ln
