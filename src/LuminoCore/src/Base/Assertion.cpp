
#include <stdlib.h>
#include <stdarg.h>
#include <algorithm>
#include <Lumino/Base/Assertion.hpp>
#include <Lumino/Base/String.hpp>
#include <Lumino/Base/StringHelper.hpp>
#include <Lumino/Base/Logger.hpp>
#include "../Text/UnicodeUtils.hpp"

namespace ln {

//==============================================================================
// Assertion

static Assertion::NotifyVerificationHandler	g_notifyVerificationHandler = nullptr;

void Assertion::setNotifyVerificationHandler(NotifyVerificationHandler handler)
{
	g_notifyVerificationHandler = handler;
}

Assertion::NotifyVerificationHandler Assertion::getNotifyVerificationHandler()
{
	return g_notifyVerificationHandler;
}

namespace detail
{

void convertCharToWChar(const char* inStr, size_t inStrLen, wchar_t* outStr, size_t outStrLen)
{
#ifdef LN_MSVC
	size_t ret;
	mbstowcs_s(&ret, outStr, outStrLen, inStr, inStrLen);
#else
	mbstowcs(outStr, inStr, outStrLen - 1);
#endif
}

void convertWCharToChar16(const wchar_t* inStr, size_t inStrLen, char16_t* outStr, size_t outStrLen)
{
#if WCHAR_MAX <= 0xffff	// wchar_t == char16_t
	// assumed windows
	memcmp(outStr, inStr, sizeof(char16_t) * std::max(inStrLen, outStrLen));
#elif WCHAR_MAX <= 0xffffffff
	// assumed unix
	ln::UTFConversionOptions opt;
	opt.ReplacementChar = '?';
	ln::UnicodeUtils::convertUTF32toUTF16((const UTF32*)inStr, inStrLen, (UTF16*)outStr, outStrLen, &opt);
#else
#error "Invalid wchar_t size."
#endif
}

void convertChar16ToWChar(const char16_t* inStr, size_t inStrLen, wchar_t* outStr, size_t outStrLen)
{
#if WCHAR_MAX <= 0xffff	// wchar_t == char16_t
	// assumed windows
	memcmp(outStr, inStr, sizeof(wchar_t) * std::max(inStrLen, outStrLen));
#elif WCHAR_MAX <= 0xffffffff
	// assumed unix
	ln::UTFConversionOptions opt;
	opt.ReplacementChar = '?';
	ln::UnicodeUtils::convertUTF16toUTF32((const UTF16*)inStr, inStrLen, (UTF32*)outStr, outStrLen, &opt);
#else
#error "Invalid wchar_t size."
#endif
}

void convertChar16ToLocalChar(const char16_t* inStr, size_t inStrLen, char* outStr, size_t outStrLen)
{
	size_t len = (inStrLen < outStrLen) ? inStrLen : outStrLen;
	size_t i = 0;
	for (; i < len && inStr[i]; i++) {
		outStr[i] = static_cast<char>(inStr[i]);
	}
	outStr[i] = '\0';
}

void errorPrintf(Char* buf, size_t bufSize, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	
	const size_t BUFFER_SIZE = 511;
	char str[BUFFER_SIZE + 1] = {};
	StringHelper::vsprintf(str, BUFFER_SIZE, format, args);
	
#ifdef LN_USTRING16
	// char -> wchar_t
	wchar_t wstr[BUFFER_SIZE + 1] = {};
	convertCharToWChar(str, BUFFER_SIZE, wstr, BUFFER_SIZE);

	// wchar_t -> char16_t
	convertWCharToChar16(wstr, BUFFER_SIZE, buf, bufSize);
#else
	// char -> wchar_t
	convertCharToWChar(str, BUFFER_SIZE, buf, bufSize - 1);
#endif

	va_end(args);
}

void errorPrintf(Char* buf, size_t bufSize, const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);

#ifdef LN_USTRING16
	const size_t BUFFER_SIZE = 511;
	wchar_t str[BUFFER_SIZE + 1] = {};
	StringHelper::vsprintf(str, BUFFER_SIZE, format, args);

	// wchar_t -> char16_t
	convertWCharToChar16(str, BUFFER_SIZE, buf, bufSize - 1);
#else
	StringHelper::vsprintf(buf, bufSize, format, args);
#endif

	va_end(args);
}

void errorPrintf(Char* buf, size_t bufSize, const char16_t* format)
{
#ifdef LN_USTRING16
	memcpy(buf, format, sizeof(Char) * std::max(bufSize, StringHelper::strlen(format)));
#else
	convertChar16ToWChar(format, StringHelper::strlen(format), buf, bufSize - 1);
#endif
}

void errorPrintf(Char* buf, size_t bufSize, const String& format)
{
	StringHelper::strcpy(buf, bufSize, format.c_str());
}

void errorPrintf(Char* buf, size_t bufSize)
{
	// dummy
	buf[0] = '\0';
}

// Lumino default error notification
void printError(const Exception& e)
{
	const size_t BUFFER_SIZE = 512;
	char buf[BUFFER_SIZE] = {};
	int len = sprintf_s(buf, BUFFER_SIZE, "%s(%d):\"%s\" ", e.m_sourceFilePath, e.m_sourceFileLine, e.m_assertionMessage);
	convertChar16ToLocalChar(e.getMessage(), BUFFER_SIZE, buf + len, BUFFER_SIZE - len);
	LN_LOG_ERROR << buf;
}

void Exception_setSourceLocationInfo(Exception& e, const char* filePath, int fileLine, const char* assertionMessage)
{
	e.setSourceLocationInfo(filePath, fileLine, assertionMessage);
}

} // namespace detail


  //==============================================================================
  // Exception
  //==============================================================================
static void safeCharToUChar(const char* src, Char* dst, int dstSize) LN_NOEXCEPT
{
	// TODO: 日本語対応
	int i = 0;
	for (; i < dstSize - 1 && *src; i++, src++)
	{
		dst[i] = *src;
	}
	dst[i] = '\0';
}

static void safeWCharToUChar(const wchar_t* src, Char* dst, int dstSize) LN_NOEXCEPT
{
	// TODO: 日本語対応
	int i = 0;
	for (; i < dstSize - 1 && *src; i++, src++)
	{
		dst[i] = *src;
	}
	dst[i] = '\0';
}

LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(Exception);

Exception::Exception()
	: m_sourceFilePath{}
	, m_sourceFileLine(0)
	, m_stackBuffer()
	, m_stackBufferSize(0)
	, m_caption()
	, m_message()
{
}

Exception::~Exception()
{
}

const Char* Exception::getMessage() const
{
	return m_message.c_str();
}
Exception* Exception::copy() const
{
	return LN_NEW Exception(*this);
}

std::basic_string<Char> Exception::getCaption()
{
	return m_caption;
}

void Exception::setMessage()
{
	m_message.clear();
}

void Exception::setCaption(const Char* caption)
{
	m_caption = caption;
}

void Exception::setMessage(const char* format, va_list args)
{
	// caption
	m_message = getCaption();

	// format char
	static const int BUFFER_SIZE = 512;
	char buf[BUFFER_SIZE];
	int len = StringHelper::vsprintf(buf, BUFFER_SIZE, format, args);

	// char to Char
	Char ucharBuf[BUFFER_SIZE];
	safeCharToUChar(buf, ucharBuf, BUFFER_SIZE);
	appendMessage(ucharBuf, len);
}

void Exception::setMessage(const wchar_t* format, va_list args)
{
	// caption
	m_message = getCaption();

	// format char
	static const int BUFFER_SIZE = 512;
	wchar_t buf[BUFFER_SIZE];
	int len = StringHelper::vsprintf(buf, BUFFER_SIZE, format, args);

	// char to Char
	Char ucharBuf[BUFFER_SIZE];
	safeWCharToUChar(buf, ucharBuf, BUFFER_SIZE);
	appendMessage(ucharBuf, len);
}

void Exception::setMessage(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	setMessage(format, args);
	va_end(args);
}

void Exception::setMessage(const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);
	setMessage(format, args);
	va_end(args);
}

void Exception::setMessageU(const Char* message)
{
	appendMessage(message, StringHelper::strlen(message));
}

void Exception::appendMessage(const Char* message, size_t len)
{
	m_message.append(message, len);
}

void Exception::setSourceLocationInfo(const char* filePath, int fileLine, const char* assertionMessage)
{
	StringHelper::strcpy(m_sourceFilePath, MaxPathSize - 1, filePath);
	m_sourceFileLine = fileLine;
	StringHelper::strcpy(m_assertionMessage, MaxAssertionMessageSize - 1, assertionMessage);
}

//==============================================================================
// LogicException
//==============================================================================
LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(LogicException);

LogicException::LogicException()
{
	setCaption(_LT("ln::LogicException"));	// TODO
}

Exception* LogicException::copy() const
{
	return LN_NEW LogicException(*this);
}

//==============================================================================
// RuntimeException
//==============================================================================
LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(RuntimeException);

RuntimeException::RuntimeException()
{
	setCaption(_LT("ln::RuntimeException"));	// TODO
}

Exception* RuntimeException::copy() const
{
	return LN_NEW RuntimeException(*this);
}

//==============================================================================
// FatalException
//==============================================================================
LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(FatalException);

FatalException::FatalException()
{
	setCaption(_LT("ln::FatalException"));
}

Exception* FatalException::copy() const
{
	return LN_NEW FatalException(*this);
}

//==============================================================================
// NotImplementedException
//==============================================================================
NotImplementedException::NotImplementedException()
{
	setCaption(_LT("ln::NotImplementedException"));
}

NotImplementedException::NotImplementedException(const Char* message)
	: NotImplementedException()
{
}

Exception* NotImplementedException::copy() const
{
	return LN_NEW NotImplementedException(*this);
}

//==============================================================================
// IOException
//==============================================================================
LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(IOException);

IOException::IOException()
{
	setCaption(_LT("ln::IOException"));
}

Exception* IOException::copy() const
{
	return LN_NEW IOException(*this);
}

} // namespace ln


