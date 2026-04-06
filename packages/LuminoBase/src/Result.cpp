#pragma once
#include <LuminoBase/Logger.hpp>
#include <LuminoBase/Result.hpp>

namespace ln {

namespace detail {

std::string formatString(const char* format, ...) {
    const int BUFFER_SIZE = 512;
    char buf[BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, BUFFER_SIZE, format, args);
    va_end(args);
    buf[BUFFER_SIZE - 1] = '\0'; // 念のため終端保証
    return buf;
}

// 0.11.0 移行の開発コンセプトで、よりモダングラフィックス API に似た振る舞いを目指すことにした。
// なので主に Dawn を参考にして、Error 発生時に合わせてエラーログを出力するようにしている。
tl::unexpected<Error> makeInternalError(
    const std::string& message, const char* file, const char* function, int line) {
    const char* filename = Logger::getBaseName(file);
    std::string logMessage = formatString(
        "%s(%d): %s: %s",
        filename,
        line,
        function,
        message.c_str());
    LogLocation loc{file, line, function};
    Logger::log(loc, LogLevel::Error, logMessage.c_str());
    
#ifdef _MSC_VER
    __debugbreak();
#endif
    return tl::make_unexpected(Error{ErrorCode::RuntimeError, logMessage});
}
} // namespace detail
} // namespace ln
