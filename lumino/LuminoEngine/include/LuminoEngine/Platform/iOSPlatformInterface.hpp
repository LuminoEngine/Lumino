#pragma once

namespace ln {

class iOSPlatformInterface
{
public:

	static void nativeInitialize(int viewWidth, int viewHeight);

	static void nativeUpdateFrame();

	static void nativeFinalize();

	static void applicationDidEnterBackground();

	static void applicationWillEnterForeground();

	static void applicationScreenSizeChanged(int viewWidth, int viewHeight);
};

} // namespace ln
