﻿
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
	static PlatformWindow* mainWindow();
};

} // namespace ln
