#pragma once

namespace ln {

class AndroidPlatformInterface
{
public:
	static void initialize(int viewWidth, int viewHeight);
    static void finalize();
    static void updateFrame();
    static void addAssetArchive(const ln::StringRef& fileFullPath, const ln::StringRef& password = ln::StringRef());
};

} // namespace ln

