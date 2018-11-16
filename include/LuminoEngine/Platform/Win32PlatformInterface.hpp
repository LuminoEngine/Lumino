#pragma once

#ifdef _WIN32

namespace ln {

class Win32PlatformInterface
{
public:
    static void initialize();
	static int WinMain();
    static void addAssetArchive(const ln::StringRef& fileFullPath, const ln::StringRef& password);
};

} // namespace ln

#endif
