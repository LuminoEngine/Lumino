#pragma once

namespace ln {
namespace detail {

class TextInputMethodSystem
{
public:
    static void SetInputScreenPos(intptr_t hwnd, int clientX, int clientY);
};

} // namespace detail
} // namespace ln

