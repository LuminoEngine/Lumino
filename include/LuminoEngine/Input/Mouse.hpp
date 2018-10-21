
#pragma once
#include <array>
#include <LuminoEngine/Platform/PlatformEvent.hpp>
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include "Common.hpp"

namespace ln {

class Mouse
{
public:
    static bool isPressed(MouseButtons button);
    static bool isTriggered(MouseButtons button);
    static bool isOffTriggered(MouseButtons button);
    static bool isRepeated(MouseButtons button);
    static Point position();

private:
};

namespace detail {
class InputManager;

class MouseInputDevice
    : public RefObject
{
public:
    MouseInputDevice();

    bool isPressed(MouseButtons button) const;
    bool isTriggered(MouseButtons button) const;
    bool isOffTriggered(MouseButtons button) const;
    bool isRepeated(MouseButtons button) const;
    Point position();

    void updateFrame(float elapsedTime);

private:
    static int MouseButtonToIndex(MouseButtons button);

    detail::InputManager* m_manager;
    std::array<InputDeviceElement, 5> m_elements; // MouseButtons
};

} // namespace detail
} // namespace ln
