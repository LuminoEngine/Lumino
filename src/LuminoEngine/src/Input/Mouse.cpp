
#include "Internal.hpp"
#include <Lumino/Input/Mouse.hpp>
#include "InputManager.hpp"
#include "InputDriver.hpp"

namespace ln {

//==============================================================================
// Input

static detail::MouseInputDevice* Mouse_activeDevice()
{
	return detail::EngineDomain::inputManager()->mouseInputDevice();
}

bool Mouse::isPressed(MouseButtons button)
{
	return Mouse_activeDevice()->isPressed(button);
}

bool Mouse::isTriggered(MouseButtons button)
{
	return Mouse_activeDevice()->isTriggered(button);
}

bool Mouse::isOffTriggered(MouseButtons button)
{
	return Mouse_activeDevice()->isOffTriggered(button);
}

bool Mouse::isRepeated(MouseButtons button)
{
	return Mouse_activeDevice()->isRepeated(button);
}

Point Mouse::position()
{
	return Mouse_activeDevice()->position();
}

namespace detail {

//==============================================================================
// MouseInputDevice

MouseInputDevice::MouseInputDevice()
	: m_manager(detail::EngineDomain::inputManager())
{
}

bool MouseInputDevice::isPressed(MouseButtons button) const
{
	return m_elements[MouseButtonToIndex(button)].isPressed();
}

bool MouseInputDevice::isTriggered(MouseButtons button) const
{
	return m_elements[MouseButtonToIndex(button)].isTriggered();
}

bool MouseInputDevice::isOffTriggered(MouseButtons button) const
{
	return m_elements[MouseButtonToIndex(button)].isOffTriggered();
}

bool MouseInputDevice::isRepeated(MouseButtons button) const
{
	return m_elements[MouseButtonToIndex(button)].isRepeated(m_manager->repeatIntervalStartTime(), m_manager->repeatIntervalStepTime());
}

Point MouseInputDevice::position()
{
	return m_manager->inputDriver()->getMousePoint().toFloatPoint();
}

void MouseInputDevice::updateFrame(float elapsedTime)
{
	InputDriver* driver = m_manager->inputDriver();

	m_elements[0].current = driver->queryMouseState(MouseAction::LeftClick) ? 1.0f : 0.0f;
	m_elements[1].current = driver->queryMouseState(MouseAction::RightClick) ? 1.0f : 0.0f;
	m_elements[2].current = driver->queryMouseState(MouseAction::MiddleClick) ? 1.0f : 0.0f;
	m_elements[3].current = driver->queryMouseState(MouseAction::X1Click) ? 1.0f : 0.0f;
	m_elements[4].current = driver->queryMouseState(MouseAction::X2Click) ? 1.0f : 0.0f;
	
	for (auto& element : m_elements) {
		element.updateFrame(elapsedTime, m_manager->repeatIntervalStartTime(), m_manager->repeatIntervalStepTime());
	}
}

int MouseInputDevice::MouseButtonToIndex(MouseButtons button)
{
	return ((int)button) - 1;
}

} // namespace detail
} // namespace ln
