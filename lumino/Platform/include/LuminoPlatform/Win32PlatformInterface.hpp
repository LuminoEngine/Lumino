#pragma once

#ifdef _WIN32

namespace ln {
class Application;
namespace detail {
class PlatformWindow;
}

class Win32PlatformInterface
{
public:
    //static void init(Application* app);
	static int WinMain(Application* app);
	//static ln::Path findParentDirectoryContainingSpecifiedFile(StringRef file);
};

} // namespace ln

#endif
