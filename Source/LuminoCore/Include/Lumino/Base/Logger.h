
#pragma once

#ifdef _MSC_VER
#define LN_FUNC_MACRO	__FUNCTION__
#else
#define LN_FUNC_MACRO	__PRETTY_FUNCTION__
#endif

/*
	メッセージ文字列の型は char でも wchar_t でも良い。
	LN_LOG_INFO("message");
	LN_LOG_INFO(L"message");
	LN_LOG_INFO(_T("message"));

	ただし、書式指定で引数に文字列を使う場合は明示する必要がある。
	String str = "aaa";
	LN_LOG_INFO(_T("str:{0}"), str);
*/

#define LN_LOG(severity, ...)	::ln::Logger2::WriteLine(severity, __FILE__, LN_FUNC_MACRO, __LINE__, __VA_ARGS__);
#define LN_LOG_FATAL(...)		LN_LOG(::ln::LogLevel::Fatal, __VA_ARGS__);
#define LN_LOG_ERROR(...)		LN_LOG(::ln::LogLevel::Error, __VA_ARGS__);
#define LN_LOG_WARNING(...)		LN_LOG(::ln::LogLevel::Warning, __VA_ARGS__);
#define LN_LOG_INFO(...)		LN_LOG(::ln::LogLevel::Info, __VA_ARGS__);
#define LN_LOG_DEBUG(...)		LN_LOG(::ln::LogLevel::Debug, __VA_ARGS__);
#define LN_LOG_VERBOSE(...)		LN_LOG(::ln::LogLevel::Verbose, __VA_ARGS__);

LN_NAMESPACE_BEGIN

/** ログの通知レベル */
enum class LogLevel
{
	Fatal,
	Error,
	Warning,
	Info,
	Debug,
	Verbose,
};

/** ログのユーティリティです。*/
class Logger
{
public:

	/** 通知レベル */
	enum class Level
	{
		Info = 0,		/**< 情報 */
		Warning ,		/**< 警告 */
		Error,			/**< エラー */
	};

public:

	/**
		@brief		ログ機能を初期化し、ログファイルを新規作成します。
		@param[in]	filePath	: ログファイルのパス
		@return		true=成功 / false=失敗
	*/
	static bool Initialize(const TCHAR* filePath) throw();

	/**
		@brief		通知レベルを指定して書式指定メッセージを書き込みます。
		@param[in]	level	: 通知レベル (指定しない場合は Level_Info)
		@details	ログ機能が初期化されていない場合は何もしません。
	*/
	static void WriteLine(Level severity, const char* format, ...) throw();
	static void WriteLine(Level severity, const wchar_t* format, ...) throw();	/**< @overload WriteLine */
	static void WriteLine(const char* format, ...) throw();								/**< @overload WriteLine */
	static void WriteLine(const wchar_t* format, ...) throw();							/**< @overload WriteLine */
};


/** ログのユーティリティです。*/
class Logger2
{
public:

	/**
		@brief		ログ機能を初期化し、ログファイルを新規作成します。
		@param[in]	filePath	: ログファイルのパス
		@return		true=成功 / false=失敗
	*/
	static void Initialize(const StringRef& filePath);

	/**
		@brief		通知レベルを指定して書式指定メッセージを書き込みます。
		@param[in]	level	: 通知レベル (指定しない場合は Level_Info)
		@details	ログ機能が初期化されていない場合は何もしません。
	*/


	template<typename TChar, typename... TArgs>
	static void WriteLine(LogLevel level, const char* file, const char* func, int line, const TChar* format, const TArgs&... args) { WriteLineInternal(level, file, func, line, GenericString<TChar>::Format(format, args...)); }

private:
	static void WriteLineInternal(LogLevel level, const char* file, const char* func, int line, const StringA& message);
	static void WriteLineInternal(LogLevel level, const char* file, const char* func, int line, const StringW& message);
	static const char* GetLogLevelString(LogLevel level);
};

LN_NAMESPACE_END
