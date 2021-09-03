
#pragma once
#include "InputDriver.hpp"

namespace ln {

class GLFWInputDriver
    : public InputDriver
{
public:
    GLFWInputDriver();
    virtual ~GLFWInputDriver();

    void init();
    virtual void dispose() override;

    virtual int getJoystickCount() override;
    virtual void getJoystickState(int joysticNumber, JoystickDeviceState* state) override;

private:
};

} // namespace ln
