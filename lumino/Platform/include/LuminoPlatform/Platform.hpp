
#pragma once
#include "Common.hpp"

namespace ln {

class Platform {
public:
    /**
	 * Processes event queue.
	 */
    static bool processEvents();

    /**
	 * Get MainWindow.
	 */
    static PlatformWindow* mainWindow();
};

} // namespace ln
