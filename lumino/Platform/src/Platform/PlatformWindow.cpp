
#include "Internal.hpp"
#include <LuminoPlatform/PlatformWindow.hpp>

namespace ln {

//==============================================================================
// PlatformWindow

PlatformWindow::PlatformWindow()
	: m_eventListeners()
	, m_dpiFactor(1.0f)
{
}

void PlatformWindow::attachEventListener(IPlatforEventListener* listener)
{
	m_eventListeners.add(listener);
}

void PlatformWindow::detachEventListener(IPlatforEventListener* listener)
{
	m_eventListeners.remove(listener);
}

bool PlatformWindow::sendEventToAllListener(const PlatformEventArgs& e)
{
	for (IPlatforEventListener* listener : m_eventListeners) {
		if (listener->onPlatformEvent(e)) {
			return true;
		}
	}
	return false;
}

} // namespace ln

