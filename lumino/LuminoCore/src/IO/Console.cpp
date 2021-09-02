
#include "Internal.hpp"
#include <LuminoCore/IO/Console.hpp>

namespace ln {

void Console::allocate()
{
#ifdef LN_OS_WIN32
    if (::AllocConsole())
    {
        freopen("CON", "r", stdin);
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    }
#endif
}

} // namespace ln
