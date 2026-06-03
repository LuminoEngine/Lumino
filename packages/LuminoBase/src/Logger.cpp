#include "pch.hpp"
#include <cstdarg>
#include <mutex>
#include <LuminoBase/Logger.hpp>

namespace ln {

namespace {

// ログメッセージの固定バッファサイズ（超過分は切り捨て）
static constexpr int kLogBufferSize = 1024;

// ログレベルの 1 文字省略形
static const char* levelChar(LogLevel level) {
    switch (level) {
        case LogLevel::Trace:
            return "T";
        case LogLevel::Debug:
            return "D";
        case LogLevel::Verbose:
            return "V";
        case LogLevel::Info:
            return "I";
        case LogLevel::Warning:
            return "W";
        case LogLevel::Error:
            return "E";
        case LogLevel::Fatal:
            return "F";
        default:
            return "-";
    }
}

// ファイルスコープ globals（g_mutex で保護）
static LogLevel g_level = LogLevel::Info;
static LogCallback g_callback = nullptr;
static std::mutex g_mutex;

} // anonymous namespace

void Logger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_level = level;
}

LogLevel Logger::level() {
    std::lock_guard<std::mutex> lock(g_mutex);
    return g_level;
}

bool Logger::shouldLog(LogLevel level) {
    std::lock_guard<std::mutex> lock(g_mutex);
    return level >= g_level;
}

void Logger::setCallback(LogCallback cb) {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_callback = cb;
}

void Logger::clearCallback() {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_callback = nullptr;
}

void Logger::log(LogLocation location, LogLevel level, const char* format, ...) {
    // レベルフィルタ（ロックを短く取る）
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (level < g_level) return;
    }

    char buf[kLogBufferSize];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, kLogBufferSize, format, args);
    va_end(args);
    buf[kLogBufferSize - 1] = '\0'; // 念のため終端保証

    writeCore(location, level, buf);
}

void Logger::writeCore(LogLocation location, LogLevel level, const char* message) {
    std::lock_guard<std::mutex> lock(g_mutex);

    if (g_callback) {
        g_callback(level, getBaseName(location.file), location.line, location.func, message);
    }
    else {
        // デフォルト: "[L] file:line message\n" を stderr へ
        std::fprintf(stderr, "[%s] %s:%d %s\n", levelChar(level), getBaseName(location.file), location.line, message);
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
