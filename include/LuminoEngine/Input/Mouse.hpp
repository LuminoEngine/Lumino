
#pragma once
#include <array>
#include <LuminoEngine/Platform/PlatformEvent.hpp>
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include "Common.hpp"

namespace ln {

class Mouse
{
public:
    static bool pressed(MouseButtons button);
    static bool triggered(MouseButtons button);
    static bool triggeredOff(MouseButtons button);
    static bool repeated(MouseButtons button);
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

    bool pressed(MouseButtons button) const;
    bool triggered(MouseButtons button) const;
    bool triggeredOff(MouseButtons button) const;
    bool repeated(MouseButtons button) const;
    Point position();

    void updateFrame(float elapsedTime);

private:
    static int MouseButtonToIndex(MouseButtons button);

    detail::InputManager* m_manager;
    std::array<InputDeviceElement, 5> m_elements; // MouseButtons
};

} // namespace detail
} // namespace ln
