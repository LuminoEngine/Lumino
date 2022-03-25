#pragma once
#include <LuminoEngine/Engine/Module.hpp>
#include "Common.hpp"

namespace ln {

struct PlatformModuleSettings {
    WindowSystem windowSystem = WindowSystem::Native;
};

class PlatformModule : public Module
{
public:
	/**
	 * Initialize PlatformModule.
	 */
    static PlatformModule* initialize(const PlatformModuleSettings& settings = {});

	/**
	 * Terminate PlatformModule.
	 */
	static void terminate();
};

} // namespace ln
