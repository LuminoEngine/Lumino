
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

Point MouseInputDevice::position()
{
	return m_manager->inputDriver()->getMousePoint().toFloatPoint();
}

} // namespace detail
} // namespace ln
