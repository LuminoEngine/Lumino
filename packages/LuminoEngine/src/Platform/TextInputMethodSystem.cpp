#include "Internal.hpp"
#include <LuminoEngine/Platform/detail/TextInputMethodSystem.hpp>

namespace ln {
namespace detail {

//==============================================================================
// TextInputMethodSystem

void TextInputMethodSystem::SetInputScreenPos(intptr_t hwnd_, int clientX, int clientY)
{
#if defined(LN_WIN32)
    if (HWND hwnd = (HWND)hwnd_)
    {
        if (HIMC himc = ::ImmGetContext(hwnd))
        {
            COMPOSITIONFORM cf;
            cf.ptCurrentPos.x = clientX;
            cf.ptCurrentPos.y = clientY;
            cf.dwStyle = CFS_FORCE_POSITION;
            ::ImmSetCompositionWindow(himc, &cf);
            ::ImmReleaseContext(hwnd, himc);
        }
    }
#else
    LN_NOTIMPLEMENTED();
#endif
}

} // namespace detail
} // namespace ln

