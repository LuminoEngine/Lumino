#pragma once

#ifdef _WIN32

namespace ln {
namespace detail {
class PlatformWindow;
}

class Win32PlatformInterface
{
public:
    static void init();
	static int WinMain();
};

} // namespace ln

#endif
