#include "pch.hpp"
#include <cstdarg>
#include <atomic>
#include <LuminoBase/Logger.hpp>

namespace ln {

namespace {

// ログメッセージの固定バッファサイズ（超過分は切り捨て）
static constexpr int kLogBufferSize = 1024;

// ログレベルの 1 文字省略形。LogLevel の値をそのまま添字にする。
static constexpr char kLevelChars[] = "TDVIWEF";

// ファイルスコープのグローバル変数
static std::atomic<LogLevel> g_level{LogLevel::Info};
static std::atomic<LogCallback> g_callback{nullptr};

} // anonymous namespace

void Logger::setLevel(LogLevel level) {
    g_level.store(level, std::memory_order_relaxed);
}

bool Logger::shouldLog(LogLevel level) {
    return level >= g_level.load(std::memory_order_relaxed);
}

void Logger::setCallback(LogCallback cb) {
    g_callback.store(cb, std::memory_order_relaxed);
}

void Logger::log(LogLocation location, LogLevel level, const char* format, ...) {
    if (!shouldLog(level)) return;

    char buf[kLogBufferSize];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, kLogBufferSize, format, args);
    va_end(args);
    buf[kLogBufferSize - 1] = '\0'; // 念のため終端保証

    const char* filename = getBaseName(location.file);
    if (LogCallback cb = g_callback.load(std::memory_order_relaxed)) {
        cb(level, filename, location.line, location.func, buf);
    }
    else {
        // デフォルト: "[L] file:line message\n" を stderr へ
        const int index = static_cast<int>(level);
        const char levelChar = (index >= 0 && index <= static_cast<int>(LogLevel::Fatal)) ? kLevelChars[index] : '-';
        std::fprintf(stderr, "[%c] %s:%d %s\n", levelChar, filename, location.line, buf);
    }
}

// __FILE__ からファイル名部分のみを取り出す
const char* Logger::getBaseName(const char* path) {
    const char* p = path;
    const char* last = path;
    while (*p) {
        if (*p == '/' || *p == '\\') {
            last = p + 1;
        }
        ++p;
    }
    return last;
}

} // namespace ln
