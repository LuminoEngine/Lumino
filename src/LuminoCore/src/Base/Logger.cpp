
#include "Internal.hpp"
#include <thread>
#include <cstdio>
#include <vector>
#include <memory>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#include <sys/timeb.h>
#endif
#include <time.h>
#include <sstream>
#include <iomanip>
#include <Lumino/Base/Logger.hpp>

namespace ln {

//==============================================================================
// LogHelper

class LogHelper
{
public:
#ifdef _WIN32
    typedef timeb Time;

    static void getTime(Time* t)
    {
        ::ftime(t);
    }
#else
    struct Time
    {
        time_t time;
        unsigned short millitm;
    };

    static void GetTime(Time* t)
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
    void Open(const char* filePath)
    {
        Close();
        m_file = ::open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }

    int Write(const void* buf, size_t count)
    {
        return (m_file != -1) ? static_cast<int>(::write(m_file, buf, count)) : -1;
    }

    off_t Seek(off_t offset, int whence)
    {
        return (m_file != -1) ? ::lseek(m_file, offset, whence) : -1;
    }

    void Close()
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

LogRecord::LogRecord(LogLevel level, const char* file, const char* func, int line)
    : m_level(level)
    , m_file(file)
    , m_func(func)
    , m_line(line)
    , m_threadId(0) //Thread::getCurrentThreadId())
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
// StdErrLoggerAdapter

class StdErrLoggerAdapter : public ILoggerAdapter
{
public:
    virtual void write(const char* str, size_t len) override
    {
        std::cerr << str;
    }
};

//==============================================================================
// LoggerInterface::Impl

class LoggerInterface::Impl
{
public:
    std::vector<std::shared_ptr<ILoggerAdapter>> m_adapters;
};

//==============================================================================
// LoggerInterface

static LoggerInterface g_logger;
static bool g_logEnabled = true;
static std::string g_logFilePath = "LuminoLog.txt";
static LogLevel g_maxLevel = LogLevel::Info;

static const char* GetLogLevelString(LogLevel level)
{
    switch (level) {
        case LogLevel::Fatal:
            return "Fatal";
        case LogLevel::Error:
            return "Error";
        case LogLevel::Warning:
            return "Warning";
        case LogLevel::Info:
            return "Info";
        case LogLevel::Debug:
            return "Debug";
        case LogLevel::Verbose:
            return "Verbose";
        default:
            return "";
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

bool LoggerInterface::CheckLevel(LogLevel level)
{
    return level <= g_maxLevel;
}

void LoggerInterface::operator+=(const LogRecord& record)
{
    if (!g_logFile.IsOpend()) {
        g_logFile.open(g_logFilePath.c_str());
    }

    tm t;
    char date[64];
    LogHelper::GetLocalTime(&t, &record.getTime().time);
    strftime(date, sizeof(date), "%Y/%m/%d %H:%M:%S", &t);

    g_logSS.str("");                           // バッファをクリアする。
    g_logSS.clear(std::stringstream::goodbit); // ストリームの状態をクリアする。この行がないと意図通りに動作しない
    g_logSS << date << " ";
    g_logSS << std::setw(5) << std::left << GetLogLevelString(record.GetLevel()) << " ";
    g_logSS << "[" << record.getThreadId() << "]";
    g_logSS << "[" << record.GetFunc() << "(" << record.GetLine() << ")] ";
    g_logSS << record.getMessage() << std::endl;

    auto str = g_logSS.str();
    g_logFile.write(str.c_str(), str.length());

    for (auto& adapter : m_impl->m_adapters) {
        adapter->write(str.c_str(), str.length());
    }
}

} // namespace detail

//==============================================================================
// GlobalLogger

bool GlobalLogger::addFileAdapter(const StringRef& filePath)
{
    return false;
}

void GlobalLogger::addStdErrAdapter()
{
    detail::LoggerInterface::getInstance()->m_impl->m_adapters.push_back(
        std::make_shared<detail::StdErrLoggerAdapter>());
}

} // namespace ln
