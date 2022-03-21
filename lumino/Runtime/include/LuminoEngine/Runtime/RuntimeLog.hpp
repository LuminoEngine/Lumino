#pragma once

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
	static void write(LogLevel level, const StringView& tag, const StringView& text);
	
private:
};

} // namespace ln
