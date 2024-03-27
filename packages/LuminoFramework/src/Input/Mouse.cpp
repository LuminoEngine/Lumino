
#include "Internal.hpp"
#include <LuminoEngine/Input/Mouse.hpp>
#include "InputManager.hpp"
#include "InputDriver.hpp"

namespace ln {

//==============================================================================
// Input

static detail::MouseInputDevice* Mouse_activeDevice()
{
	return detail::EngineDomain::inputManager()->mouseInputDevice();
}

bool Mouse::pressed(MouseButtons button)
{
	return Mouse_activeDevice()->pressed(button);
}

bool Mouse::triggered(MouseButtons button)
{
	return Mouse_activeDevice()->triggered(button);
}

bool Mouse::triggeredOff(MouseButtons button)
{
	return Mouse_activeDevice()->triggeredOff(button);
}

bool Mouse::repeated(MouseButtons button)
{
	return Mouse_activeDevice()->repeated(button);
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

bool MouseInputDevice::pressed(MouseButtons button) const
{
	return m_elements[MouseButtonToIndex(button)].pressed();
}

bool MouseInputDevice::triggered(MouseButtons button) const
{
	return m_elements[MouseButtonToIndex(button)].triggered();
}

bool MouseInputDevice::triggeredOff(MouseButtons button) const
{
	return m_elements[MouseButtonToIndex(button)].triggeredOff();
}

bool MouseInputDevice::repeated(MouseButtons button) const
{
	return m_elements[MouseButtonToIndex(button)].repeated(m_manager->repeatIntervalStartTime(), m_manager->repeatIntervalStepTime());
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
