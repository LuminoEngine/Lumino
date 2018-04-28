
#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsContext.hpp>
#include <Lumino/UI/UIFrameWindow.hpp>
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

void UIFrameWindow::initialize(const SizeI& size)
{
	m_manager = detail::EngineDomain::uiManager();

	detail::WindowCreationSettings settings;
	//settings.title = m_settings.mainWindowTitle;
	settings.clientSize = size;
	settings.fullscreen = false;
	settings.resizable = true;
	//settings.mainWindowSettings.userWindow = m_settings.userMainWindow;

	m_platformWindow = detail::EngineDomain::platformManager()->windowManager()->createWindow(settings);
	m_platformWindow->attachEventListener(this);

	m_swapChain = newObject<SwapChain>(m_platformWindow, size);
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
	
	ctx->clear(ClearFlags::All, Color::White, 1.0f, 0x00);
}

void UIFrameWindow::present()
{
	GraphicsContext* ctx = m_manager->graphicsManager()->graphicsContext();

	ctx->present(m_swapChain);
}

bool UIFrameWindow::onPlatformEvent(const detail::PlatformEventArgs& e)
{
	switch (e.type)
	{
	case detail::PlatformEventType::close:
		//exit();
		break;
	default:
		break;
	}
	return false;
}

} // namespace ln

