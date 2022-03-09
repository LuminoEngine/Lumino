#pragma once

namespace ln {

class Runtime {
public:
    /**
	 * Initialize Runtime.
	 */
    static Result initialize();

    /**
	 * terminate Runtime.
	 */
    static void terminate();
};

} // namespace ln
