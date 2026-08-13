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

// 0.11.0 以降の開発コンセプトとして、モダングラフィックス API に近い振る舞いを目指している。
// そのため主に Dawn を参考にして、Error の生成時に合わせてエラーログも出力するようにしている。
tl::unexpected<Error> makeInternalError(
    const std::string& message, const char* file, const char* function, int line) {
    return makeInternalErrorWithCode(ErrorCode::RuntimeError, message, file, function, line);
}

tl::unexpected<Error> makeInternalErrorWithCode(
    ErrorCode code, const std::string& message, const char* file, const char* function, int line) {
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
    // DeviceLost はタブ復帰やドライバ更新で起きる想定内の実行時状態のため、
    // デバッガ接続時でもブレークしない。
    if (code != ErrorCode::DeviceLost) {
        __debugbreak();
    }
#endif
    return tl::make_unexpected(Error{code, logMessage});
}
} // namespace detail
} // namespace ln
