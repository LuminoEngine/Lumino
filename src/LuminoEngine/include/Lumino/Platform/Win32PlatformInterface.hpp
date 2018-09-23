#pragma once

#ifdef _WIN32

namespace ln {

class Win32PlatformInterface
{
public:
	static int WinMain();
};

} // namespace ln

#endif
