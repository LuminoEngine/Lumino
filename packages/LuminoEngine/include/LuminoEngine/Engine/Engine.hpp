#pragma once

namespace ln {

class Engine {
public:
    static MaybeResult initialize();
	static void terminate();
};

} // namespace ln
