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

#ifdef _MSC_VER
#define LN_FUNC_MACRO __FUNCTION__
#else
#define LN_FUNC_MACRO __PRETTY_FUNCTION__
#endif

//#define LN_LOG(level, tag) !(::ln::detail::LoggerInterface::getInstance() && ::ln::detail::LoggerInterface::getInstance()->checkLevel(level)) ? (void)0 : (*::ln::detail::LoggerInterface::getInstance()) += ::ln::detail::LogRecord(level, tag, __FILE__, LN_FUNC_MACRO, __LINE__)
//#define LN_LOG_FATAL LN_LOG(::ln::LogLevel::Fatal, nullptr)
//#define LN_LOG_ERROR LN_LOG(::ln::LogLevel::Error, nullptr)
//#define LN_LOG_WARNING LN_LOG(::ln::LogLevel::Warning, nullptr)
//#define LN_LOG_INFO LN_LOG(::ln::LogLevel::Info, nullptr)
//#define LN_LOG_DEBUG LN_LOG(::ln::LogLevel::Debug, nullptr)
//#define LN_LOG_VERBOSE LN_LOG(::ln::LogLevel::Verbose, nullptr)


#define LN_LOG_LOGGER_CALL(level, ...) ::ln::Logger::log(::ln::LogLocation{__FILE__, __LINE__, LN_FUNC_MACRO}, level, __VA_ARGS__)
#define LN_LOG_VERBOSE(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Verbose, __VA_ARGS__)
#define LN_LOG_DEBUG(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Debug, __VA_ARGS__)
#define LN_LOG_INFO(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Info, __VA_ARGS__)
#define LN_LOG_WARNING(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Warning, __VA_ARGS__)
#define LN_LOG_ERROR(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Error, __VA_ARGS__)
#define LN_LOG_FATAL(...) LN_LOG_LOGGER_CALL(::ln::LogLevel::Fatal __VA_ARGS__)

namespace ln {
class StringView;

/** ログの通知レベル */
LN_ENUM()
enum class LogLevel
{
    Unknown,

    // Lumino の開発として集めたい情報
    Verbose,
    Debug,

	// Lumino のユーザーに対して通知したい情報
    Info,
    Warning,
    Error,
    Fatal,

    Disble,
};

struct LogLocation
{
    const char* filename;
    int line;
    const char* funcname;

    constexpr LogLocation()
        : filename(nullptr)
        , line(0)
        , funcname(nullptr)
    {}

    constexpr LogLocation(const char* filename_, int line_, const char* funcname_)
        : filename(filename_)
        , line(line_)
        , funcname(funcname_)
    {}

    constexpr bool isEmpty() const noexcept
    {
        return filename == nullptr;
    }
};

/** グローバルなロギング機能をコントロールするためのクラスです。 */
class Logger
{
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

	static bool hasAnyAdapter();


    static bool shouldLog(LogLevel level);


    /** ログ出力 */

    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const char* format, TArgs&&... args)
    {
        log(location, level, std::string_view(format), std::forward<TArgs>(args)...);
    }

    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const Char* format, TArgs&&... args)
    {
        log(location, level, std::basic_string_view<Char>(format), std::forward<TArgs>(args)...);
    }

    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const std::string& format, TArgs&&... args)
    {
        log(location, level, std::string_view(format), std::forward<TArgs>(args)...);
    }

    template<typename TFormatString, typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const TFormatString& format, TArgs&&... args)
    {
        log(location, level, ::ln::toStdStringView(format), std::forward<TArgs>(args)...);
    }

    template<typename... TArgs>
    static inline void log(LogLocation location, LogLevel level, const std::basic_string_view<Char>& format, TArgs&&... args)
    {
        if (!shouldLog(level)) return;

        try {
            std::basic_string_view<Char> view(format.data(), format.length());
            const auto str = ::fmt::format(view, std::forward<TArgs>(args)...);
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
    static inline void log(LogLocation location, LogLevel level, const std::string_view& format, TArgs&&... args)
    {
        if (!shouldLog(level)) return;

        try {
            const auto str = ::fmt::format(format, std::forward<TArgs>(args)...);
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
};

namespace detail {

#ifdef _WIN32
typedef timeb LogTime;
#else
struct LogTime
{
    time_t time;
    unsigned short millitm;
};
#endif

//class LogRecord
//{
//public:
//    LogRecord(LogLevel level, const char* tag, const char* file, const char* func, int line);
//    const LogTime& getTime() const { return m_time; }
//    LogLevel GetLevel() const { return m_level; }
//    const char* tag() const { return m_tag; }
//    const char* getMessage() const;
//    const char* GetFile() const { return m_location.filename; }
//    const char* GetFunc() const { return m_location.funcname; }
//    const LogLocation& location() const { return m_location; }
//    int GetLine() const { return m_location.line; }
//    const std::thread::id& getThreadId() const { return m_threadId; }
//
//    //LogRecord& operator<<(const wchar_t* str);
//
//    template<typename T>
//    LogRecord& operator<<(const T& data)
//    {
//        m_message << data;
//        return *this;
//    }
//
//private:
//    LogTime m_time;
//    LogLevel m_level;
//    const char* m_tag;
//    LogLocation m_location;
//	std::thread::id m_threadId;
//    std::stringstream m_message;
//    mutable std::string m_messageStr;
//};

class ILoggerAdapter
{
public:
    virtual ~ILoggerAdapter();
    virtual void write(LogLocation source, LogLevel level, const char* str, size_t len) = 0;
};

class LoggerInterface
{
public:
    static LoggerInterface* getInstance();

    LoggerInterface();
    ~LoggerInterface();
    bool checkLevel(LogLevel level);
    //void operator+=(const LogRecord& record);
    void write(LogLocation source, LogLevel level, const char* str, size_t len);

    //class Impl;
    //Impl* m_impl;
    std::vector<std::shared_ptr<ILoggerAdapter>> m_adapters;
};

} // namespace detail

#ifdef SPDLOG_H
namespace integration {
inline void useSpdlog()
{
    class Adapter : public detail::ILoggerAdapter
    {
        void write(LogLocation source, LogLevel level, const char* str, size_t len) override
        {
            ::spdlog::default_logger_raw()->log({ source.filename, source.line, source.funcname }, toSpdlogLevel(level), std::string_view(str, len));
        }

        spdlog::level::level_enum toSpdlogLevel(LogLevel level) const
        {
            switch (level)
            {
            default:
            case LogLevel::Unknown:
                return spdlog::level::level_enum::trace;
            case LogLevel::Verbose:
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

    detail::LoggerInterface::getInstance()->m_adapters.push_back(std::make_shared<Adapter>());
}
}
#endif

} // namespace ln
