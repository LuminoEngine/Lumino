#pragma once

#ifdef _WIN32

namespace ln {
class CoreApplication;

class Win32PlatformInterface {
public:
    static int WinMain(CoreApplication* app);
};

} // namespace ln

#endif
