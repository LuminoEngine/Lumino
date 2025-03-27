#include "Internal.hpp"
#include <LuminoCore/Base/Logger.hpp>
#include <LuminoCore/Base/Result.hpp>
#include <LuminoCore/IO/detail/PathHelper.hpp>

namespace ln {
namespace detail {

std::string formatString(const char* format, ...) {
    const int BUFFER_SIZE = 512;
    char buf[BUFFER_SIZE + 1] = {};
    va_list list;
    va_start(list, format);
    int r = StringHelper::vsprintf(buf, BUFFER_SIZE, format, list);
    va_end(list);
    buf[std::min(r, BUFFER_SIZE)] = '\0';
    return buf;
}

// 0.11.0 移行の開発コンセプトで、よりモダングラフィックス API に似た振る舞いを目指すことにした。
// なので主に Dawn を参考にして、Error 発生時に合わせてエラーログを出力するようにしている。
ErrType<ErrorCode> makeInternalError(const std::string& message, const char* file, const char* function, int line) {
    const char* filename = PathTraits::getFileName(file, file + strlen(file));
    std::string logMessage = formatString("%s(%d): %s: %s", filename, line, function, message.c_str());
    LN_LOG_ERROR(logMessage);

#ifdef LN_MSVC
    __debugbreak();
#endif

    return ln::err(ErrorCode::Unknown);
}

} // namespace detail
} // namespace ln
