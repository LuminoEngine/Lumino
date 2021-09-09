
#pragma once

namespace ln {

class PlatformModule
{
public:
	/**
	 * Initialize PlatformModule.
	 */
	static bool initialize();

	/**
	 * Terminate PlatformModule.
	 */
	static void terminate();
};

} // namespace ln
