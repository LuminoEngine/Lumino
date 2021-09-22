#include "Internal.hpp"
#include <LuminoFFI/RuntimeLog.hpp>

namespace ln {
    
void Log::setLevel(LogLevel level)
{
    ln::Logger::setLevel(static_cast<ln::LogLevel>(level));
}

void Log::allocConsole()
{
    ln::Console::allocate();
}

void Log::write(LogLevel level, const StringRef& tag, const StringRef& text)
{
    LN_LOG(static_cast<ln::LogLevel>(level), ln::String(tag).toStdString().c_str()) << ln::String(text);
}

} // namespace ln
