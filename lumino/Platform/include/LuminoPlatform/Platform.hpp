
#pragma once
#include "Common.hpp"

namespace ln {

class Platform
{
public:
	/**
	 * Processes event queue.
	 */
	static void processEvents();

	/**
	 * Get MainWindow.
	 */
	static detail::PlatformWindow* mainWindow();
};

} // namespace ln
