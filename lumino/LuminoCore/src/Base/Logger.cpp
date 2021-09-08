
#include "Internal.hpp"
#include <cstdio>
#include <vector>
#include <memory>
#include <mutex>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <time.h>
#include <sstream>
#include <iomanip>
#include <LuminoCore/Base/Logger.hpp>

#ifdef LN_OS_ANDROID
#include <android/log.h>
#endif

namespace ln {

void writeNSLog(const char* str);

//==============================================================================
// LogHelper

class LogHelper
{
public:
#ifdef _WIN32
    static void getTime(detail::LogTime* t)
    {
        ::ftime(t);
    }
#else
    static void getTime(detail::LogTime* t)
    {
        timeval tv;
        ::gettimeofday(&tv, NULL);

        t->time = tv.tv_sec;
        t->millitm = static_cast<unsigned short>(tv.tv_usec / 1000);
    }
#endif

    static void GetLocalTime(struct tm* t, const time_t* time)
    {
#ifdef _WIN32
        ::localtime_s(t, time);
#else
        ::localtime_r(time, t);
#endif
    }
};

//==============================================================================
// LogFile

class LogFile
{
public:
    LogFile()
        : m_file(-1)
    {
    }

    ~LogFile()
    {
        close();
    }

    bool IsOpend() const
    {
        return m_file != -1;
    }

#ifdef _WIN32
    void open(const char* filePath)
    {
        close();
        ::_sopen_s(&m_file, filePath, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE);
    }

    int write(const void* buf, size_t count)
    {
        return (m_file != -1) ? ::_write(m_file, buf, static_cast<unsigned int>(count)) : -1;
    }

    off_t seek(off_t offset, int whence)
    {
        return (m_file != -1) ? ::_lseek(m_file, offset, whence) : -1;
    }

    void close()
    {
        if (m_file != -1) {
            ::_close(m_file);
            m_file = -1;
        }
    }
#else
    void open(const char* filePath)
    {
        close();
        m_file = ::open(filePath, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }

    int write(const void* buf, size_t count)
    {
        return (m_file != -1) ? static_cast<int>(::write(m_file, buf, count)) : -1;
    }

    off_t seek(off_t offset, int whence)
    {
        return (m_file != -1) ? ::lseek(m_file, offset, whence) : -1;
    }

    void close()
    {
        if (m_file != -1) {
            ::close(m_file);
            m_file = -1;
        }
    }
#endif

private:
    int m_file;
};

//==============================================================================
// Logger

static LogFile g_logFile;
static std::stringstream g_logSS;

namespace detail {

//==============================================================================
// LogRecord

LogRecord::LogRecord(LogLevel level, const char* tag, const char* file, const char* func, int line)
    : m_level(level)
    , m_tag(tag)
    , m_file(file)
    , m_func(func)
    , m_line(line)
    , m_threadId(std::this_thread::get_id())
{
    LogHelper::getTime(&m_time);
}

const char* LogRecord::getMessage() const
{
    m_messageStr = m_message.str();
    return m_messageStr.c_str();
}

//LogRecord& LogRecord::operator<<(const wchar_t* str)
//{
//	StringA s = StringA::fromNativeCharString(str);
//	m_message << s.c_str();
//	return *this;
//}

ILoggerAdapter::~ILoggerAdapter()
{
}

//==============================================================================
// FileLoggerAdapter

class FileLoggerAdapter : public ILoggerAdapter
{
public:
	virtual void write(LogLevel level, const char* str, size_t len) override
	{
		g_logFile.write(str, len);
	}
};

//==============================================================================
// StdErrLoggerAdapter

class StdErrLoggerAdapter : public ILoggerAdapter
{
public:
    virtual void write(LogLevel level, const char* str, size_t len) override
    {
        std::cerr << str;
    }
};

#ifdef LN_OS_ANDROID
//==============================================================================
// AndroidLogcatLoggerAdapter

class AndroidLogcatLoggerAdapter : public ILoggerAdapter
{
public:
	virtual void write(LogLevel level, const char* str, size_t len) override
	{
		auto localLevel = ANDROID_LOG_ERROR;
		switch (level)
		{
		case LogLevel::Fatal:
			localLevel = ANDROID_LOG_FATAL;
		case LogLevel::Error:
			localLevel = ANDROID_LOG_ERROR;
		case LogLevel::Warning:
			localLevel = ANDROID_LOG_WARN;
		case LogLevel::Info:
			localLevel = ANDROID_LOG_INFO;
		case LogLevel::Debug:
			localLevel = ANDROID_LOG_DEBUG;
		case LogLevel::Verbose:
			localLevel = ANDROID_LOG_VERBOSE;
		}
		__android_log_print(localLevel, "Lumino", "%s", str);
	}
};
#endif

#if defined(LN_OS_MAC) || defined(LN_OS_IOS)
//==============================================================================
// NLogLoggerAdapter

class NLogLoggerAdapter : public ILoggerAdapter
{
public:
	virtual void write(LogLevel level, const char* str, size_t len) override
	{
        writeNSLog(str);
	}
};
#endif

//==============================================================================
// LoggerInterface::Impl

class LoggerInterface::Impl
{
public:
	bool hasAdapter() const { return !m_adapters.empty(); }

    std::vector<std::shared_ptr<ILoggerAdapter>> m_adapters;
};

//==============================================================================
// LoggerInterface

static LoggerInterface g_logger;
static bool g_logEnabled = true;
static std::string g_logFilePath = "LuminoLog.txt";
static LogLevel g_maxLevel = LogLevel::Info;
static std::mutex g_logMutex;

static const char* GetLogLevelString(LogLevel level)
{
    switch (level) {
        case LogLevel::Fatal:
            return "F";
        case LogLevel::Error:
            return "E";
        case LogLevel::Warning:
            return "W";
        case LogLevel::Info:
            return "I";
        case LogLevel::Debug:
            return "D";
        case LogLevel::Verbose:
            return "V";
        default:
            return "-";
    }
}

LoggerInterface* LoggerInterface::getInstance()
{
    if (!g_logEnabled)
        return nullptr;
    return &g_logger;
}

LoggerInterface::LoggerInterface()
    : m_impl(new Impl())
{
}

LoggerInterface::~LoggerInterface()
{
    if (m_impl) {
        delete m_impl;
    }
}

bool LoggerInterface::checkLevel(LogLevel level)
{
    return level >= g_maxLevel;
}

void LoggerInterface::operator+=(const LogRecord& record)
{
	if (m_impl->hasAdapter())
	{
        std::lock_guard<std::mutex> lock(g_logMutex);

		tm t;
		char date[64];
		LogHelper::GetLocalTime(&t, &record.getTime().time);
		strftime(date, sizeof(date), "%Y/%m/%d %H:%M:%S", &t);

		g_logSS.str("");                           // バッファをクリアする。
		g_logSS.clear(std::stringstream::goodbit); // ストリームの状態をクリアする。この行がないと意図通りに動作しない
		g_logSS << date << " ";
		g_logSS << GetLogLevelString(record.GetLevel()) << " ";
		g_logSS << "[" << record.getThreadId() << "]";
        // TODO: ログの量を圧迫するので一時封印。
        // Assertion の機能はメッセージの一部として位置を記録するため、そちらは問題なし。
        // LogFormat みたいな設定を増やしておく
		//g_logSS << "[" << record.GetFunc() << "(" << record.GetLine() << ")]";
        if (record.tag())
            g_logSS << "[" << record.tag() << "] ";
        else
            g_logSS << " ";

		g_logSS << record.getMessage() << std::endl;

		auto str = g_logSS.str();

		for (auto& adapter : m_impl->m_adapters) {
			adapter->write(record.GetLevel(), str.c_str(), str.length());
		}
	}
}

} // namespace detail

//==============================================================================
// Logger

bool Logger::addFileAdapter(const std::string& filePath)
{
	if (g_logFile.IsOpend()) {
		return false;
	}

	g_logFile.open(filePath.c_str());

	detail::LoggerInterface::getInstance()->m_impl->m_adapters.push_back(
		std::make_shared<detail::FileLoggerAdapter>());

    return true;
}

void Logger::addStdErrAdapter()
{
    detail::LoggerInterface::getInstance()->m_impl->m_adapters.push_back(
        std::make_shared<detail::StdErrLoggerAdapter>());
}

void Logger::addLogcatAdapter()
{
#ifdef LN_OS_ANDROID
	detail::LoggerInterface::getInstance()->m_impl->m_adapters.push_back(
		std::make_shared<detail::AndroidLogcatLoggerAdapter>());
	LN_LOG_INFO << "Attached AndroidLogcatLoggerAdapter.";
#endif
}

void Logger::addNLogAdapter()
{
#if defined(LN_OS_MAC) || defined(LN_OS_IOS)
	detail::LoggerInterface::getInstance()->m_impl->m_adapters.push_back(
		std::make_shared<detail::NLogLoggerAdapter>());
	LN_LOG_INFO << "Attached NLogLoggerAdapter.";
#endif
}

bool Logger::hasAnyAdapter()
{
	return !detail::LoggerInterface::getInstance()->m_impl->m_adapters.empty();
}

void Logger::setLevel(LogLevel level)
{
	detail::g_maxLevel = level;
}

LogLevel Logger::level()
{
    return detail::g_maxLevel;
}

} // namespace ln
