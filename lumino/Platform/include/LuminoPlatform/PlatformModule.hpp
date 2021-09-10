#pragma once
#include <LuminoEngine/Engine/Module.hpp>

namespace ln {

class PlatformModule : public Module
{
public:
	/**
	 * Initialize PlatformModule.
	 */
	static PlatformModule* initialize();

	/**
	 * Terminate PlatformModule.
	 */
	static void terminate();
};

} // namespace ln
