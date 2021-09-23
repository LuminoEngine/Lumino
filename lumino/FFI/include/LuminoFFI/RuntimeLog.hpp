#pragma once
#include "Common.hpp"

namespace ln {

/** log */
LN_CLASS(Static)
class Log
{
public:
	/** setLevel */
	LN_METHOD()
	static void setLevel(LogLevel level);

	/** allocConsole */
	LN_METHOD()
	static void allocConsole();

	/** write */
	LN_METHOD()
	static void write(LogLevel level, const StringRef& tag, const StringRef& text);
	
private:
};

} // namespace ln
