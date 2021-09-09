#pragma once

namespace ln {

class AndroidPlatformInterface
{
public:
	static void init(int viewWidth, int viewHeight);
    static void finalize();
    static void updateFrame();
};

} // namespace ln

