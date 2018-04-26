
#include "Internal.hpp"
#include <Lumino/Graphics/SwapChain.hpp>
#include <Lumino/UI/UIFrameWindow.hpp>
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

