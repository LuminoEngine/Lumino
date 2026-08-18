#pragma once
#include <LuminoC/lumino_types.h>

#ifdef _MSC_VER
#define LN_FUNC_SIG __FUNCTION__
#else
#define LN_FUNC_SIG __PRETTY_FUNCTION__
#endif

//------------------------------------------------------------------------------
// ログ出力マクロ
//------------------------------------------------------------------------------

#define LN_LOG_TRACE(...)   ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, LN_LOG_LEVEL_TRACE,   __VA_ARGS__)
#define LN_LOG_DEBUG(...)   ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, LN_LOG_LEVEL_DEBUG,   __VA_ARGS__)
#define LN_LOG_VERBOSE(...) ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, LN_LOG_LEVEL_VERBOSE, __VA_ARGS__)
#define LN_LOG_INFO(...)    ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, LN_LOG_LEVEL_INFO,    __VA_ARGS__)
#define LN_LOG_WARNING(...) ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, LN_LOG_LEVEL_WARNING, __VA_ARGS__)
#define LN_LOG_ERROR(...)   ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, LN_LOG_LEVEL_ERROR,   __VA_ARGS__)
#define LN_LOG_FATAL(...)   ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_SIG}, LN_LOG_LEVEL_FATAL,   __VA_ARGS__)

namespace ln {

// ログレベルは C API と共通の LNLogLevel (lumino_types.h) を使用します。
// C++ 側と C API 側で enum を二重定義すると値の同期を取り続ける必要があるため、
// 定義を一本化しています。

/** LogLocation ソースコード上の位置情報 */
struct LogLocation {
    const char* file;
    int         line;
    const char* func;

    constexpr LogLocation(const char* f, int l, const char* fn)
        : file(f), line(l), func(fn) {}
};

/**
 * LogCallback カスタム出力コールバック
 * 
 * 登録されている場合、Logger はデフォルトの stderr 出力を行わず、このコールバックへログ情報を渡す。
 *
 * level   : ログレベル
 * file    : ソースファイル名（basename のみ）
 * line    : 行番号
 * func    : 関数名
 * message : printf 展開済みのメッセージ文字列（UTF-8
 */
using LogCallback = void (*)(LNLogLevel level, const char* file, int line,
                              const char* func, const char* message);

/**
 * ログ出力を制御するクラスです。
 * ログを出力するときは、通常は LN_LOG_INFO などのマクロを使用してください。
 *
 * @example
 * ```
 * LN_LOG_INFO("maxImageDimension1D: %u", maxImageDimension1D);
 * ```
 */
class Logger {
public:
    /** ログ出力レベルフィルタを設定する。設定レベル未満のログは破棄される。 */
    static void setLevel(LNLogLevel level);

    /** 指定レベルのログを出力すべきか判定する。 */
    static bool shouldLog(LNLogLevel level);

    /**
     * カスタムコールバックを登録する。
     * nullptr を渡すと登録を解除し、stderr へのデフォルト出力に戻る。
     * 登録後は stderr へのデフォルト出力は行われない。
     */
    static void setCallback(LogCallback cb);

    /** ログを出力する。printf 互換の書式指定子が使用可能。 */
    static void log(LogLocation location, LNLogLevel level, const char* format, ...);

    /** @internal */
    static const char* getBaseName(const char* path);
};

} // namespace ln
