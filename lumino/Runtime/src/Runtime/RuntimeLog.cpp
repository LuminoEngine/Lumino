#include "Internal.hpp"
#include <LuminoEngine/Runtime/RuntimeLog.hpp>

namespace ln {
    
void Log::setLevel(LogLevel level)
{
    ln::Logger::setLevel(static_cast<ln::LogLevel>(level));
}

void Log::allocConsole()
{
    ln::Console::allocate();
}

void Log::write(LogLevel level, const StringView& tag, const StringView& text)
{
    LN_LOG_LOGGER_CALL(static_cast<ln::LogLevel>(level), text);
    //LN_LOG_LOGGER_CALL(static_cast<ln::LogLevel>(level), ln::String(tag).toStdString().c_str()) << ln::String(text);
}

} // namespace ln
