
#include "../Internal.hpp"
#include "GLFWInputDriver.hpp"

namespace ln {

//==============================================================================
// GLFWInputDriver

GLFWInputDriver::GLFWInputDriver()
{
}

GLFWInputDriver::~GLFWInputDriver()
{
}

void GLFWInputDriver::init()
{
	LN_LOG_DEBUG << "GLFWInputDriver";
}

void GLFWInputDriver::dispose()
{
    InputDriver::dispose();
}

int GLFWInputDriver::getJoystickCount()
{
    return 0;
}

void GLFWInputDriver::getJoystickState(int joysticNumber, JoystickDeviceState* state)
{
    state->reset();
}

} // namespace ln
