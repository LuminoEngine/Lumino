#pragma once

namespace ln {

class AndroidPlatformInterface
{
public:
	static void initialize(int viewWidth, int viewHeight);
    static void finalize();
    static void updateFrame();
    static void addAssetArchive(const ln::String& fileFullPath);
};

} // namespace ln

