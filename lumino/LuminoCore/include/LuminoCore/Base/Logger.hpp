// Copyright (c) 2018+ lriki. Distributed under the MIT license.

#pragma once

#include <sstream>
#include <thread>
#ifdef _WIN32
#include <sys/timeb.h>
#else
#include <time.h>
#endif

#ifdef _MSC_VER
#define LN_FUNC_MACRO __FUNCTION__
#else
#define LN_FUNC_MACRO __PRETTY_FUNCTION__
#endif

#define LN_LOG(level, tag) !(::ln::detail::LoggerInterface::getInstance() && ::ln::detail::LoggerInterface::getInstance()->checkLevel(level)) ? (void)0 : (*::ln::detail::LoggerInterface::getInstance()) += ::ln::detail::LogRecord(level, tag, __FILE__, LN_FUNC_MACRO, __LINE__)
#define LN_LOG_FATAL LN_LOG(::ln::LogLevel::Fatal, nullptr)
#define LN_LOG_ERROR LN_LOG(::ln::LogLevel::Error, nullptr)
#define LN_LOG_WARNING LN_LOG(::ln::LogLevel::Warning, nullptr)
#define LN_LOG_INFO LN_LOG(::ln::LogLevel::Info, nullptr)
#define LN_LOG_DEBUG LN_LOG(::ln::LogLevel::Debug, nullptr)
#define LN_LOG_VERBOSE LN_LOG(::ln::LogLevel::Verbose, nullptr)

namespace ln {
class StringRef;

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

class LogRecord
{
public:
    LogRecord(LogLevel level, const char* tag, const char* file, const char* func, int line);
    const LogTime& getTime() const { return m_time; }
    LogLevel GetLevel() const { return m_level; }
    const char* tag() const { return m_tag; }
    const char* getMessage() const;
    const char* GetFile() const { return m_file; }
    const char* GetFunc() const { return m_func; }
    int GetLine() const { return m_line; }
    const std::thread::id& getThreadId() const { return m_threadId; }

    //LogRecord& operator<<(const wchar_t* str);

    template<typename T>
    LogRecord& operator<<(const T& data)
    {
        m_message << data;
        return *this;
    }

private:
    LogTime m_time;
    LogLevel m_level;
    const char* m_tag;
    const char* m_file;
    const char* m_func;
    int m_line;
	std::thread::id m_threadId;
    std::stringstream m_message;
    mutable std::string m_messageStr;
};

class ILoggerAdapter
{
public:
    virtual ~ILoggerAdapter();
    virtual void write(LogLevel level, const char* str, size_t len) = 0;
};

class LoggerInterface
{
public:
    static LoggerInterface* getInstance();

    LoggerInterface();
    ~LoggerInterface();
    bool checkLevel(LogLevel level);
    void operator+=(const LogRecord& record);

    class Impl;
    Impl* m_impl;
};

} // namespace detail
} // namespace ln
