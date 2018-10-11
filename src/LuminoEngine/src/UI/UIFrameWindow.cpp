
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include "UIManager.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Platform/PlatformManager.hpp"

namespace ln {

UIFrameWindow::UIFrameWindow()
{
}

UIFrameWindow::~UIFrameWindow()
{
}

void UIFrameWindow::initialize(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize)
{
	m_manager = detail::EngineDomain::uiManager();
	m_swapChain = newObject<SwapChain>(platformMainWindow, backbufferSize);
}

void UIFrameWindow::dispose()
{
	if (m_swapChain) {
		m_swapChain->dispose();
	}

	if (m_platformWindow) {
		m_platformWindow->detachEventListener(this);
		detail::EngineDomain::platformManager()->windowManager()->destroyWindow(m_platformWindow);	// TODO: dispose で破棄で。
	}
}

void UIFrameWindow::renderContents()
{
	GraphicsContext* ctx = m_manager->graphicsManager()->graphicsContext();

	ctx->setColorBuffer(0, m_swapChain->colorBuffer());
	ctx->setDepthBuffer(m_swapChain->depthBuffer());
	ctx->clear(ClearFlags::All, Color(0.4, 0.4, 0.4), 1.0f, 0x00);
}

void UIFrameWindow::present()
{
	GraphicsContext* ctx = m_manager->graphicsManager()->graphicsContext();

	ctx->present(m_swapChain);
}

SwapChain* UIFrameWindow::swapChain() const
{
	return m_swapChain;
}

bool UIFrameWindow::onPlatformEvent(const detail::PlatformEventArgs& e)
{
	switch (e.type)
	{
	case PlatformEventType::close:
		//exit();
		break;
	default:
		break;
	}
	return false;
}

} // namespace ln

