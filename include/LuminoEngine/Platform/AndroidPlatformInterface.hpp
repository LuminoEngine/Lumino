#pragma once

namespace ln {

class AndroidPlatformInterface
{
public:
	static void initialize(int viewWidth, int viewHeight);
    static void finalize();
    static void updateFrame();
};

} // namespace ln

