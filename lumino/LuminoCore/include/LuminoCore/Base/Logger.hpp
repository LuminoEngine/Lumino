// Copyright (c) 2018+ lriki. Distributed under the MIT license.

#pragma once

#include <sstream>
#include <thread>
#ifdef _WIN32
#include <sys/timeb.h>
#else
#include <time.h>
#endif
#include "UnicodeUtils.hpp"
#include "String.hpp"

#ifdef _MSC_VER
#define LN_FUNC_MACRO __FUNCTION__
#else
#define LN_FUNC_MACRO __PRETTY_FUNCTION__
#endif

/**
 * Logger usage samples
 * ----------
 *
 * ```
 * LN_LOG_INFO("Hello!");
 * LN_LOG_ERROR("Error message.");
 * LN_LOG_ERROR("With arg: {}", 1); // by std::fmt format
 * LN_LOG_INFO(U"UTF-32 string.");
 * LN_LOG_INFO(U"UTF-32 with arg: {}", 1);
 * ```
 */

#define LN_LOG_LOGGER_CALL(level, ...) ::ln::Logger::log(::ln::LogLocation{ __FILE__, __LINE__, LN_FUNC_MACRO }, level, __VA_ARGS__)
#define LN_LOG_TRACE(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Trace, __VA_ARGS__)
#define LN_LOG_DEBUG(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Debug, __VA_ARGS__)
#define LN_LOG_VERBOSE(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Verbose, __VA_ARGS__)
#define LN_LOG_INFO(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Info, __VA_ARGS__)
#define LN_LOG_WARNING(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Warning, __VA_ARGS__)
#define LN_LOG_ERROR(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Error, __VA_ARGS__)
#define LN_LOG_FATAL(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Fatal __VA_ARGS__)

namespace ln {
class StringView;

/**
 * ログの通知レベル
 * 
 * レベルは大きく次のように分類されます。
 * - Lumino 自体または Lumino を使用したアプリケーションを開発するプログラマが不具合調査などに活用できる情報。 (Trace, Debug)
 * - Lumino を使用したアプリケーションのユーザーや、ライブラリユーザーにとって有用な情報。 (Verbose 以上)
 */
LN_ENUM()
enum class LogLevel {
    Unknown = 0,

    // 開発者として集めたい情報
	
	/** 関数の入口や分岐をマークするために使用します。 */
    Trace = 1,

    /** デバッグに有用な情報を出力するために使用します。 */
    Debug = 2,

    // ユーザーに対して通知したい情報
	
    /** Infoレベルの情報に加えて、詳細な情報を記録します。デフォルトでは出力されません。 */
    Verbose = 3,
	
	/** ユーザーが問題解決を行う際に有用な情報を記録します。 (Default) */
    Info = 4,

    /** 問題を引き起こす可能性のある異常な状況を記録します。または、問題をプログラムで解決して処理を続行しましたが、注意を要する状態を記録します。 */
    Warning = 5,

    /** 処理を正常に続行できない障害を記録します。 */
    Error = 6,

    /** プログラムの実行が不可能な状況を記録します。通常、プログラムは強制終了します。 */
    Fatal = 7,

    /** 全てのログ出力を無効化するためのキーワードです。 */
    Disble = 8,
};

struct LogLocation {
    const char* filename;
    int line;
    const char* funcname;

    constexpr LogLocation()
        : filename(nullptr)
        , line(0)
        , funcname(nullptr) {}

    constexpr LogLocation(const char* filename_, int line_, const char* funcname_)
        : filename(filename_)
        , line(line_)
        , funcname(funcname_) {}

    constexpr bool isEmpty() const noexcept {
        return filename == nullptr;
    }
};

class ILoggerAdapter {
public:
    virtual ~ILoggerAdapter();
    virtual void write(LogLocation source, LogLevel level, const char* str, size_t len) = 0;
    virtual void flush() = 0;
};

/**
 * This class is for controlling the log function.
 *
 * The default adapter depends on the runtime environment.
 * - Emscripten: stdout
 * - Other(Desktop, etc): stderr
 *
 * The default log level changes depending on the build configuration.
 * - DEBUG: LogLevel::Debug
 * - RELEASE: LogLevel::Info
 */
class Logger {
public:
    /**
     * ログをファイルへ出力するためのアダプタを追加します。
     * @param[in]   filePath : ログファイルのパス
     * @return      true=成功 / false=失敗
     */
    static bool addFileAdapter(const std::string& filePath);

    /**
     * ログを標準エラーへ出力するためのアダプタを追加します。
     */
    static void addStdErrAdapter();

    /**
     * Android システムの Logcat へ出力するためのアダプタを追加します。
     */
    static void addLogcatAdapter();

    /**
     * macOS/iOS システムの NLog へ出力するためのアダプタを追加します。
     */
    static void addNLogAdapter();

    /**
     * どの程度詳細にログを生成するかを設定します。(defailt: Debug)
     *
     * 例えば LogLevel::Warning を指定した場合、Fatal, Error, Warning のレベルについてログを生成します。
     */
    static void setLevel(LogLevel level);

    static LogLevel level();

    static void addAdapter(std::shared_ptr<ILoggerAdapter> adapter);
    static bool hasAnyAdapter();

    static bool shouldLog(LogLevel level);

    static const char* getLevelStringNarrow(LogLevel level);

    /** ログ出力 */

    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const char* format, TArgs&&... args) {
        log(location, level, std::string_view(format), std::forward<TArgs>(args)...);
    }

    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const Char* format, TArgs&&... args) {
        log(location, level, std::basic_string_view<Char>(format), std::forward<TArgs>(args)...);
    }

    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const std::string& format, TArgs&&... args) {
        log(location, level, std::string_view(format), std::forward<TArgs>(args)...);
    }

    template<typename T>
    static inline void log(LogLocation location, LogLevel level, const T& value) {
        const auto str = ::ln::toString(value);
        log(location, level, std::u32string_view(str.c_str(), str.length()));
    }

    template<typename TFormatString, typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const TFormatString& format, TArgs&&... args) {
        log(location, level, ::ln::toStdStringView(format), std::forward<TArgs>(args)...);
    }

    //template<typename TFormatString, typename... TArgs>
    //static inline void log(LogLocation location, LogLevel level, const TFormatString& format, TArgs&&... args) {
    //    const auto str = ::ln::toString(format);
    //    log(location, level, std::u32string_view(str.c_str(), str.length()), std::forward<TArgs>(args)...);
    //}
	
    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const std::basic_string_view<Char>& format, TArgs&&... args) {
        if (!shouldLog(level)) return;

        try {
            std::basic_string_view<Char> view(format.data(), format.length());
            const auto str = ::LN_FMT_NAMESPACE::format(view, std::forward<TArgs>(args)...);
            log(location, level, UnicodeStringUtils::U32ToU8(str.c_str(), str.length()));
        }
        catch (const std::exception& ex) {
            // TODO: fatal error handler
            std::cerr << ex.what() << std::endl;
        }
        catch (...) {
            // TODO: fatal error handler
            std::cerr << "Unknown exception in logger" << std::endl;
        }
    }

    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const std::string_view& format, TArgs&&... args) {
        if (!shouldLog(level)) return;

        try {
            const auto str = ::LN_FMT_NAMESPACE::format(format, std::forward<TArgs>(args)...);
            log(location, level, str);
        }
        catch (const std::exception& ex) {
            // TODO: fatal error handler
            std::cerr << ex.what() << std::endl;
        }
        catch (...) {
            // TODO: fatal error handler
            std::cerr << "Unknown exception in logger" << std::endl;
        }
    }

    static void log(LogLocation location, LogLevel level, std::string_view message);
    static void flush();
};

namespace detail {

#ifdef _WIN32
typedef timeb LogTime;
#else
struct LogTime {
    time_t time;
    unsigned short millitm;
};
#endif

class LoggerInterface {
public:
    static LoggerInterface* getInstance();

    LoggerInterface();
    ~LoggerInterface();
    bool checkLevel(LogLevel level);
    // void operator+=(const LogRecord& record);
    void write(LogLocation source, LogLevel level, const char* str, size_t len);

    // class Impl;
    // Impl* m_impl;
    std::vector<std::shared_ptr<ILoggerAdapter>> m_adapters;
};

} // namespace detail

#ifdef SPDLOG_H
namespace integration {
inline void useSpdlog() {
    class Adapter : public ILoggerAdapter {
        void write(LogLocation source, LogLevel level, const char* str, size_t len) override {
            ::spdlog::default_logger_raw()->log({ source.filename, source.line, source.funcname }, toSpdlogLevel(level), std::string_view(str, len));
        }

        void flush() override {
            ::spdlog::default_logger_raw()->flush();
        }

        spdlog::level::level_enum toSpdlogLevel(LogLevel level) const {
            switch (level) {
                default:
                case LogLevel::Unknown:
                    return spdlog::level::level_enum::trace;
                case LogLevel::Verbose:
                    return spdlog::level::level_enum::trace;
                case LogLevel::Trace:
                    return spdlog::level::level_enum::trace;
                case LogLevel::Debug:
                    return spdlog::level::level_enum::debug;
                case LogLevel::Info:
                    return spdlog::level::level_enum::info;
                case LogLevel::Warning:
                    return spdlog::level::level_enum::warn;
                case LogLevel::Error:
                    return spdlog::level::level_enum::err;
                case LogLevel::Fatal:
                    return spdlog::level::level_enum::critical;
            }
        }
    };

    Logger::addAdapter(std::make_shared<Adapter>());
}
}
#endif

} // namespace ln
