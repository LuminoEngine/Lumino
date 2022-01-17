#pragma once

#ifdef _WIN32

namespace ln {
class Application;

class Win32PlatformInterface
{
public:
    //static void init(Application* app);
	static int WinMain(Application* app);
	//static ln::Path findParentDirectoryContainingSpecifiedFile(StringView file);
};

} // namespace ln

#endif
