
#pragma once
#include <array>
#include <LuminoPlatform/PlatformEvent.hpp>
#include "Common.hpp"

namespace ln {
    
/**
 * マウスの入力を受け取るためのクラスです。
 */
LN_CLASS(Static)
class Mouse
{
public:
    /**
     * ボタンが現在押されているかを判定します。
     */
    LN_METHOD()
    static bool pressed(MouseButtons button);

    /**
     * ボタンが新しく押された瞬間を判定します。
     */
    LN_METHOD()
    static bool triggered(MouseButtons button);

    /**
     * ボタンが離された瞬間を判定します。
     */
    LN_METHOD()
    static bool triggeredOff(MouseButtons button);

    /**
     * ボタンが新しく押された瞬間とリピート状態を判定します。
     */
    LN_METHOD()
    static bool repeated(MouseButtons button);

    /**
     * マウスポインタの位置を取得します。
     */
    LN_METHOD(Property)
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
