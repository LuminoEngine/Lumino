
#include "../Internal.h"
#include <time.h>
#include <Lumino/Base/Version.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Environment.h>
#include <Lumino/Base/Logger.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Logger
//==============================================================================

static const int	TEMP_BUFFER_SIZE = 2048;
static uint64_t		g_logStartTime;

static TCHAR		g_logFilePath[LN_MAX_PATH] = { 0 };

class FileClose
{
public:
	FileClose() {}
	~FileClose()
	{
		FILE* stream;
		if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
		{
			_ftprintf(
				stream,
				_T("============== Application Exit ============== \n"));
			fclose(stream);
		}
	}
};
static FileClose g_fileClose;

//------------------------------------------------------------------------------
bool Logger::Initialize(const TCHAR* filePath) throw()
{
	//if (log_dir)
	//{
	//	_tcscpy(gLogFilePath, log_dir);
	//	size_t len = _tcslen(log_dir);
	//	gLogFilePath[len] = '/';//'\\';
	//	_tcscpy(&gLogFilePath[len + 1], log_name);
	//}
	//else
	//{
	//	System::Environment::getCurrentDirectory(gLogFilePath);
	//	size_t len = _tcslen(gLogFilePath);
	//	gLogFilePath[len] = '/';//'\\';
	//	_tcscpy(&gLogFilePath[len + 1], log_name);
	//}
	_tcscpy_s(g_logFilePath, LN_MAX_PATH, filePath);

	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("w+")) != 0) {
		g_logFilePath[0] = _T('\0');
		return false;
	}

	//time_t timer;
	//time(&timer);
	//char* ts = asctime(localtime(&timer));

	fprintf(stream, "===============================================================\n");
	fprintf(stream, " %s Log    Version:%s\n", LUMINO_NAME, LUMINO_CORE_VERSION_STRING);//    %s\n", LUMINO_CORE_VERSION_STRING, ts);
	fprintf(stream, "---------------------------------------------------------------\n");

#ifdef LN_DEBUG
	fprintf(stream, "BuildConfig : Debug\n");
#else
	fprintf(stream, "BuildConfig : Release\n");
#endif

#ifdef LN_DEBUG
	fprintf(stream, "CharSet     : Unicode\n");
#else
	fprintf(stream, "CharSet     : MultiByte\n");
#endif

	fprintf(stream, "===============================================================\n");

	fclose(stream);

	g_logStartTime = Environment::GetTickCount();
	return true;
}

//------------------------------------------------------------------------------
static const char* GetInfoString(Logger::Level level)
{
	switch (level)
	{
	default:
	case Logger::Level::Info:    return "Info    ";
	case Logger::Level::Warning: return "Warning ";
	case Logger::Level::Error:   return "Error   ";
	}
}

//------------------------------------------------------------------------------
void Logger::WriteLine(Level level, const char* format, ...) throw()
{
	if (g_logFilePath[0] == '\0') {
		return;
	}
	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
	{
		char buf[TEMP_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		StringTraits::VSPrintf(buf, TEMP_BUFFER_SIZE, format, args);
		va_end(args);

		fprintf(stream, "%llu %s: ", Environment::GetTickCount() - g_logStartTime, GetInfoString(level));
		fprintf(stream, "%s\n", buf);

		fclose(stream);
	}
}

//------------------------------------------------------------------------------
void Logger::WriteLine(Level level, const wchar_t* format, ...) throw()
{
	if (g_logFilePath[0] == '\0') {
		return;
	}
	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
	{
		wchar_t buf[TEMP_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		StringTraits::VSPrintf(buf, TEMP_BUFFER_SIZE, format, args);
		va_end(args);

		fprintf(stream, "%llu %s: ", Environment::GetTickCount() - g_logStartTime, GetInfoString(level));
		fwprintf(stream, L"%s\n", buf);

		fclose(stream);
	}

}
//------------------------------------------------------------------------------
void Logger::WriteLine(const char* format, ...) throw()
{
	if (g_logFilePath[0] == '\0') {
		return;
	}
	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
	{
		char buf[TEMP_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		StringTraits::VSPrintf(buf, TEMP_BUFFER_SIZE, format, args);
		va_end(args);

		fprintf(stream, "%llu %s: ", Environment::GetTickCount() - g_logStartTime, GetInfoString(Level::Info));
		fprintf(stream, "%s\n", buf);

		fclose(stream);
	}
}

//------------------------------------------------------------------------------
void Logger::WriteLine(const wchar_t* format, ...) throw()
{
	if (g_logFilePath[0] == '\0') {
		return;
	}
	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
	{
		wchar_t buf[TEMP_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		StringTraits::VSPrintf(buf, TEMP_BUFFER_SIZE, format, args);
		va_end(args);

		fprintf(stream, "%llu %s: ", Environment::GetTickCount() - g_logStartTime, GetInfoString(Level::Info));
		fwprintf(stream, L"%s\n", buf);

		fclose(stream);
	}

}

LN_NAMESPACE_END
