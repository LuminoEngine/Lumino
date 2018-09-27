
#pragma once
#include "InputDriver.hpp"
#include "Win32JoystickDriver.hpp"

namespace ln {
class Win32JoystickDriver;

class GLFWInputDriver
	: public InputDriver
{
public:
	GLFWInputDriver();
	virtual ~GLFWInputDriver();

	void initialize();
	virtual void dispose() override;

	virtual int getJoystickCount() override;
	virtual void getJoystickState(int joysticNumber, JoystickDeviceState* state) override;

private:
};

} // namespace ln
