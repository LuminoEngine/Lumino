#pragma once

#include <cstdarg>

#ifdef _MSC_VER
#define LN_FUNC_SIG __FUNCTION__
#else
#define LN_FUNC_SIG __PRETTY_FUNCTION__
#endif

//------------------------------------------------------------------------------
// Logging macros
//------------------------------------------------------------------------------

#define LN_LOG_TRACE(...)   ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, ::ln::LogLevel::Trace,   __VA_ARGS__)
#define LN_LOG_DEBUG(...)   ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, ::ln::LogLevel::Debug,   __VA_ARGS__)
#define LN_LOG_VERBOSE(...) ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, ::ln::LogLevel::Verbose, __VA_ARGS__)
#define LN_LOG_INFO(...)    ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, ::ln::LogLevel::Info,    __VA_ARGS__)
#define LN_LOG_WARNING(...) ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, ::ln::LogLevel::Warning, __VA_ARGS__)
#define LN_LOG_ERROR(...)   ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, ::ln::LogLevel::Error,   __VA_ARGS__)
#define LN_LOG_FATAL(...)   ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, ::ln::LogLevel::Fatal,   __VA_ARGS__)

namespace ln {

//------------------------------------------------------------------------------
// LogLevel
//------------------------------------------------------------------------------

enum class LogLevel {
    Trace   = 0, ///< 関数の入口・分岐のトレース
    Debug   = 1, ///< デバッグに有用な情報
    Verbose = 2, ///< Info に加えた詳細情報
    Info    = 3, ///< 通常運用で有用な情報（Release デフォルト）
    Warning = 4, ///< 問題になる可能性がある状況
    Error   = 5, ///< 処理を続行できない障害
    Fatal   = 6, ///< プログラム実行が不可能な状況
    Off     = 7, ///< 全出力を無効化するキーワード
};

//------------------------------------------------------------------------------
// LogLocation ソースコード上の位置情報
//------------------------------------------------------------------------------

struct LogLocation {
    const char* file;
    int         line;
    const char* func;

    constexpr LogLocation(const char* f, int l, const char* fn)
        : file(f), line(l), func(fn) {}
};

//------------------------------------------------------------------------------
// LogCallback カスタム出力コールバック
//
// 登録されている場合、Logger はデフォルトの stderr 出力を行わず
// このコールバックへログ情報を渡す。
//   level   : ログレベル
//   file    : ソースファイル名（basename のみ）
//   line    : 行番号
//   func    : 関数名
//   message : printf 展開済みのメッセージ文字列（UTF-8）
//------------------------------------------------------------------------------

using LogCallback = void (*)(LogLevel level, const char* file, int line,
                              const char* func, const char* message);

class Logger {
public:
    /// ログ出力レベルフィルタを設定する。設定レベル未満のログは破棄される。
    static void setLevel(LogLevel level);

    /// 現在のログレベルフィルタを返す。
    static LogLevel level();

    /// 指定レベルのログを出力すべきか判定する。
    static bool shouldLog(LogLevel level);

    /// カスタムコールバックを登録する。nullptr を渡すと解除と同義。
    /// 登録後は stderr へのデフォルト出力は行われない。
    static void setCallback(LogCallback cb);

    /// 登録済みコールバックを解除し、デフォルト stderr 出力に戻す。
    static void clearCallback();

    /// ログを出力する。printf 互換の書式指定子が使用可能。
    static void log(LogLocation location, LogLevel level, const char* format, ...);

private:
    static void writeCore(LogLocation location, LogLevel level, const char* message);
};

} // namespace ln
