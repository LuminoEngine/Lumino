// Copyright (c) 2018+ lriki. Distributed under the MIT license.

/** @file */
#pragma once
#include <string>
#include <iostream>
#include <fmt/format.h>
#include "Common.hpp"
#include "UnicodeUtils.hpp"

namespace ln {
class Exception;
class String;
namespace detail { struct ExceptionHelper; }

#define _LN_CHECK(expr, level, ...) (!(expr)) && ln::detail::notifyException(level, __FILE__, __LINE__, #expr, ##__VA_ARGS__)

/**
 * アプリケーション実行中に発生した軽微な問題を通知するためのマクロです。
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Warning` です。
 * 潜在的な問題の可能性がありますが、プログラムは実行を続けることができます。
 *
 * デフォルトの例外ハンドラの動作は、メッセージをロギングしてプログラムを続行します。
 */
#define LN_WARNING(...) ln::detail::notifyException(::ln::ExceptionLevel::Warning, __FILE__, __LINE__, nullptr, ##__VA_ARGS__)

/**
 * アプリケーションが不正な状態になる可能性がある問題を通知するためのマクロです。
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Error` です。
 * オブジェクトが不正な状態となっている可能性があり、プログラムを実行し続けることは危険である状態を示します。
 * エラーの発生元となったオブジェクトにアクセスしないことでプログラムは実行を続けることができますが、できるだけ早く終了するべきです。
 *
 * デフォルトの例外ハンドラの動作は、メッセージをロギングしてプログラムを停止します。
 */
#define LN_ERROR(...) ln::detail::notifyException(::ln::ExceptionLevel::Error, __FILE__, __LINE__, nullptr, ##__VA_ARGS__)

 /**
  * アプリケーションの継続が難しい致命的なエラーを通知するためのマクロです。
  *
  * このマクロが例外ハンドラを呼び出すときのレベルは `Fatal` です。
  * アプリケーションは直ちに終了しなければなりません。
  *
  * デフォルトの例外ハンドラの動作は、メッセージをロギングしてプログラムを停止します。
  */
#define LN_FATAL(...) ln::detail::notifyException(::ln::ExceptionLevel::Fatal, __FILE__, __LINE__, nullptr, ##__VA_ARGS__)

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
#define LN_REQUIRE(expr, ...) _LN_CHECK(expr, ::ln::ExceptionLevel::Warning, ##__VA_ARGS__)

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
#define LN_ENSURE(expr, ...) _LN_CHECK(expr, ::ln::ExceptionLevel::Error, ##__VA_ARGS__)

/**
 * 到達不能コードをマークするためのマクロです。
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Fatal` です。
 */
#define LN_UNREACHABLE() ::ln::detail::notifyFatalError(__FILE__, __LINE__, "Unreachable code.")

/**
 * 未実装の機能をマークするためのマクロです。
 *
 * このマクロが例外ハンドラを呼び出すときのレベルは `Fatal` です。
 */
#define LN_NOTIMPLEMENTED()						::ln::detail::notifyFatalError(__FILE__, __LINE__, "Not implemented.")

#define LN_REQUIRE_RANGE(value, begin, end)		_LN_CHECK(begin <= value && value < end, ::ln::ExceptionLevel::Warning)
#define LN_REQUIRE_KEY(expr, ...)				_LN_CHECK(expr, ::ln::ExceptionLevel::Warning, ##__VA_ARGS__)
#define LN_ENSURE_IO(expr, ...)					_LN_CHECK(expr, ::ln::ExceptionLevel::Error, u"IO Error", ##__VA_ARGS__)

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


/** アプリケーションの実行中に発生したエラーを表します。 */
class Exception
{
public:
	using NotificationHandler = bool(*)(Exception& e);

	Exception();
	Exception(const Char* message);
	virtual ~Exception();

	const Char* message() const { return m_message.c_str(); }

	/** 例外のコピーを作成します。 */
	virtual Exception* copy() const;

	/** エラーハンドラを設定します。 */
	static void setNotificationHandler(NotificationHandler handler);

	/** エラーハンドラを取得します。 */
	static NotificationHandler notificationHandler();

private:
	static const int MaxPathSize = 260;
	static const int MaxAssertionMessageSize = 100;

	ExceptionLevel m_level;
	char m_sourceFilePath[MaxPathSize];					// __FILE__
	int m_sourceFileLine;								// __LINE__
	char m_assertionMessage[MaxAssertionMessageSize];	// #expr (LN_REQUIRE)
	std::basic_string<Char>	m_message;

	friend struct detail::ExceptionHelper;
};

//namespace detail {
//class ExceptionHelper;
//
//template<typename... TArgs>
//bool notifyException(ExceptionLevel level, const char* file, int line, const char* exprString, TArgs... args);
//
//
//} // namespace detail


namespace detail {

struct ExceptionHelper
{
	static const char* getSourceFilePath(const Exception& e) { return e.m_sourceFilePath; }
	static const int getSourceFileLine(const Exception& e) { return e.m_sourceFileLine; }
	static const char* getAssertionMessage(const Exception& e) { return e.m_assertionMessage; }
	static void setSourceLocationInfo(Exception& e, ExceptionLevel level, const char* filePath, int fileLine, const char* assertionMessage);
	static void setMessage(Exception& e, const std::string& str);
	static void setMessage(Exception& e, const std::wstring& str);
	static void setMessage(Exception& e, const std::u16string& str);
};

template<class... TArgs> inline std::u16string formatMessage(const char* format, TArgs&&... args) noexcept
{
	const auto str = ::fmt::format(format, std::forward<TArgs>(args)...);
	return UnicodeStringUtils::NarrowToU16(str.c_str(), str.length());
}
template<class... TArgs> inline std::u16string formatMessage(const wchar_t* format, TArgs&&... args) noexcept
{
	const auto str = ::fmt::format(format, std::forward<TArgs>(args)...);
	return UnicodeStringUtils::WideToU16(str.c_str(), str.length());
}
template<class... TArgs> inline std::u16string formatMessage(const char16_t* format, TArgs&&... args) noexcept
{
	return ::fmt::format(format, std::forward<TArgs>(args)...);
}
template<class T, class... TArgs> inline std::u16string formatMessage(const T& format, TArgs&&... args) noexcept
{
	return ::fmt::format(format.c_str(), std::forward<TArgs>(args)...);
}
inline std::u16string formatMessage() noexcept { return {}; }
void printError(const Exception& e);
void notifyFatalError(const char* file, int line, const char* message);

template<class... TArgs>
inline bool notifyException(ExceptionLevel level, const char* file, int line, const char* exprString, TArgs&&... args)
{
	Exception e;
	ExceptionHelper::setMessage(e, formatMessage(std::forward<TArgs>(args)...));
	ExceptionHelper::setSourceLocationInfo(e, level, file, line, exprString);

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

} // namespace detail
} // namespace ln

