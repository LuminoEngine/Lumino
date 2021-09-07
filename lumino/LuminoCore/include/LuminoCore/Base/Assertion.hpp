// Copyright (c) 2018+ lriki. Distributed under the MIT license.

/** @file */
#pragma once
#include <string>
#include <iostream>
#include "Common.hpp"

namespace ln {
class Exception;
class String;

#define _LN_CHECK(expr, level, exception, ...) (!(expr)) && ln::detail::notifyException<exception>(level, __FILE__, __LINE__, #expr, ##__VA_ARGS__)

/**
 * アプリケーション実行中に発生した軽微な問題を通知するためのマクロです。
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Warning` です。
 * 潜在的な問題の可能性がありますが、プログラムは実行を続けることができます。
 *
 * デフォルトの例外ハンドラの動作は、メッセージをロギングしてプログラムを続行します。
 */
#define LN_WARNING(...) ln::detail::notifyException<::ln::Exception>(::ln::ExceptionLevel::Warning, __FILE__, __LINE__, nullptr, ##__VA_ARGS__)

/**
 * アプリケーションが不正な状態になる可能性がある問題を通知するためのマクロです。
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Error` です。
 * オブジェクトが不正な状態となっている可能性があり、プログラムを実行し続けることは危険である状態を示します。
 * エラーの発生元となったオブジェクトにアクセスしないことでプログラムは実行を続けることができますが、できるだけ早く終了するべきです。
 *
 * デフォルトの例外ハンドラの動作は、メッセージをロギングしてプログラムを停止します。
 */
#define LN_ERROR(...) ln::detail::notifyException<::ln::Exception>(::ln::ExceptionLevel::Error, __FILE__, __LINE__, nullptr, ##__VA_ARGS__)

 /**
  * アプリケーションの継続が難しい致命的なエラーを通知するためのマクロです。
  *
  * このマクロが例外ハンドラを呼び出すときのレベルは `Fatal` です。
  * アプリケーションは直ちに終了しなければなりません。
  *
  * デフォルトの例外ハンドラの動作は、メッセージをロギングしてプログラムを停止します。
  */
#define LN_FATAL(...) ln::detail::notifyException<::ln::Exception>(::ln::ExceptionLevel::Fatal, __FILE__, __LINE__, nullptr, ##__VA_ARGS__)

/**
 * コードを実行する前の前提条件を検証するためのマクロです。
 * 
 * 式が false である場合、例外ハンドラが呼び出されます。デフォルトではエラーをロギングし、このマクロは true を返します。
 * ~~~
 * void func(int* ptr) {
 *     if (LN_REQUIRE(ptr != nullptr, "invalid args.")) return;
 * 	   ...
 * }
 * ~~~
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Warning` です。
 */
#define LN_REQUIRE(expr, ...) _LN_CHECK(expr, ::ln::ExceptionLevel::Warning, ::ln::LogicException, ##__VA_ARGS__)

/**
 * 処理の実行結果を検証するためのマクロです。
 * 
 * 式が false である場合、例外ハンドラが呼び出されます。デフォルトではエラーをロギングし、このマクロは true を返します。
 * ~~~
 * void func() {
 *     int result = test();
 *     if (LN_ENSURE(result != -1, "invalid call.")) return;
 * }
 * ~~~
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Error` です。
 */
#define LN_ENSURE(expr, ...) _LN_CHECK(expr, ::ln::ExceptionLevel::Error, ::ln::RuntimeException, ##__VA_ARGS__)

/**
 * 到達不能コードをマークするためのマクロです。
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Fatal` です。
 */
#define LN_UNREACHABLE() _LN_CHECK(0, ::ln::ExceptionLevel::Fatal, ::ln::LogicException, "Unreachable code.")

/**
 * 未実装の機能をマークするためのマクロです。
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Fatal` です。
 */
#define LN_NOTIMPLEMENTED()						_LN_CHECK(0, ::ln::ExceptionLevel::Fatal, ln::NotImplementedException)

#define LN_REQUIRE_RANGE(value, begin, end)		_LN_CHECK(begin <= value && value < end, ::ln::ExceptionLevel::Warning, ::ln::LogicException)
#define LN_REQUIRE_KEY(expr, ...)				_LN_CHECK(expr, ::ln::ExceptionLevel::Warning, ln::LogicException, ##__VA_ARGS__)
#define LN_ENSURE_IO(expr, ...)					_LN_CHECK(expr, ::ln::ExceptionLevel::Error, ln::IOException, ##__VA_ARGS__)

// internal
#ifdef LN_USTRING16
#define LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(className) \
	className(const char* message); \
	className(const wchar_t* message); \
	className(const Char* message);

#define LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(className) \
	className::className(const char* message) \
	{ \
		setMessage(message); \
	} \
	className::className(const wchar_t* message) \
	{ \
		setMessage(message); \
	} \
	className::className(const Char* message) \
	{ \
		setMessageU(message); \
	}
#else
#define LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(className) \
	className(const char* message); \
	className(const wchar_t* message);

#define LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(className) \
	className::className(const char* message) \
	{ \
		setMessage(message); \
	} \
	className::className(const wchar_t* message) \
	{ \
		setMessage(message); \
	}
#endif


/** 問題の通知レベル */
enum class ExceptionLevel
{
	Fatal,
	Error,
	Warning,
	Info,
};


namespace detail {
class ExceptionHelper;

template<class TException, typename... TArgs>
bool notifyException(ExceptionLevel level, const char* file, int line, const char* exprString, TArgs... args);

void Exception_setSourceLocationInfo(Exception& e, ExceptionLevel level, const char* filePath, int fileLine, const char* assertionMessage);

} // namespace detail

//------------------------------------------------------------------------------
// core errors

/** アプリケーションの実行中に発生したエラーを表します。 */
class Exception
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(Exception);

	using NotificationHandler = bool(*)(Exception& e);

	Exception();
	virtual ~Exception();
	
	/** 例外を説明するメッセージを取得します。 */
	virtual const Char* getMessage() const;
	
	/** 例外のコピーを作成します。 */
	virtual Exception* copy() const;

	/** エラーハンドラを設定します。 */
	static void setNotificationHandler(NotificationHandler handler);

	/** エラーハンドラを取得します。 */
	static NotificationHandler notificationHandler();

protected:
	void setCaption(const Char* caption);
	virtual std::basic_string<Char> getCaption();
	void setMessage();
	void setMessage(const char* format, va_list args);
	void setMessage(const wchar_t* format, va_list args);
	void setMessage(const char* format, ...);
	void setMessage(const wchar_t* format, ...);
	void setMessageU(const Char* message);

private:
	void appendMessage(const Char* message, size_t len);
	void setSourceLocationInfo(ExceptionLevel level, const char* filePath, int fileLine, const char* assertionMessage);

	static const int MaxPathSize = 260;
	static const int MaxAssertionMessageSize = 100;
	ExceptionLevel m_level;
	char					m_sourceFilePath[MaxPathSize];
	int						m_sourceFileLine;
	char					m_assertionMessage[MaxAssertionMessageSize];
	void*					m_stackBuffer[32];
	int						m_stackBufferSize;
	std::basic_string<Char>	m_caption;
	std::basic_string<Char>	m_message;

	friend void detail::Exception_setSourceLocationInfo(Exception& e, ExceptionLevel level, const char* filePath, int fileLine, const char* assertionMessage);
	friend class detail::ExceptionHelper;
};

/** 前提条件の間違いなどプログラム内の論理的な誤りが原因で発生したエラーを表します。 */
class LogicException
	: public Exception
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(LogicException);
	LogicException();
	virtual Exception* copy() const;
};

/** 主にアプリケーションの実行環境が原因で発生したエラーを表します。 */
class RuntimeException
	: public Exception
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(RuntimeException);
	RuntimeException();
	virtual Exception* copy() const;
};

/** アプリケーションの継続が難しい致命的なエラーを表します。 */
class FatalException
	: public Exception
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(FatalException);
	FatalException();
	virtual Exception* copy() const;
};

//------------------------------------------------------------------------------
// extension errors

/** 未実装の機能を呼び出した場合のエラーを表します。 */
class NotImplementedException
	: public LogicException
{
public:
	NotImplementedException();
	NotImplementedException(const Char* message);
	virtual Exception* copy() const;
};

/** I/O エラーが発生した場合のエラーを表します。 */
class IOException
	: public RuntimeException
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(IOException);
	IOException();
	virtual Exception* copy() const;
};

namespace detail {

void errorPrintf(Char* buf, size_t bufSize, const char* format, ...);
void errorPrintf(Char* buf, size_t bufSize, const wchar_t* format, ...);
void errorPrintf(Char* buf, size_t bufSize, const char16_t* format);
void errorPrintf(Char* buf, size_t bufSize, const String& format);
void errorPrintf(Char* buf, size_t bufSize);
void printError(const Exception& e);

template<class TException, typename... TArgs>
inline bool notifyException(ExceptionLevel level, const char* file, int line, const char* exprString, TArgs... args)
{
	const size_t BUFFER_SIZE = 512;
	Char str[BUFFER_SIZE] = {};
	errorPrintf(str, BUFFER_SIZE, args...);

	TException e(str);
	detail::Exception_setSourceLocationInfo(e, level, file, line, exprString);

	auto h = Exception::notificationHandler();
	if (h != nullptr && h(e)) {
		return true;
	}

	printError(e);

	if (level == ExceptionLevel::Fatal || level == ExceptionLevel::Error) {
		throw e;
	}

	return true;
}

class ExceptionHelper
{
public:
	static const char* getSourceFilePath(const Exception& e) { return e.m_sourceFilePath; }
	static const int getSourceFileLine(const Exception& e) { return e.m_sourceFileLine; }
	static const char* getAssertionMessage(const Exception& e) { return e.m_assertionMessage; }
};

} // namespace detail
} // namespace ln

