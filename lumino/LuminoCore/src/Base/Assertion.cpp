
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <algorithm>
#include <LuminoCore/Base/Assertion.hpp>
#include <LuminoCore/Base/UnicodeUtils.hpp>

namespace ln {

namespace detail {


template<class TChar>
inline static void strcpy(TChar* dst, size_t dstLen, const TChar* src) noexcept
{
	assert(dst);
	assert(src);
	if (dstLen == 0) return;
	dstLen--;
	while (dstLen && *src)
	{
		dstLen--;
		*dst++ = *src++;
	}
	*dst = '\0';
}


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
	memcpy(outStr, inStr, sizeof(char16_t) * LN_MAX(inStrLen, outStrLen));
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
	memcpy(outStr, inStr, sizeof(wchar_t) * LN_MAX(inStrLen, outStrLen));
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
#ifdef LN_MSVC
	size_t ret;
	mbstate_t state;
	memset(&state, 0, sizeof state);
    size_t len = std::min(outStrLen - 1, inStrLen);
	wcsrtombs_s(&ret, outStr, outStrLen, (const wchar_t**)&inStr, len, &state);
#else
	size_t len = (inStrLen < outStrLen) ? inStrLen : outStrLen;
	size_t i = 0;
	for (; i < len && inStr[i]; i++) {
		outStr[i] = static_cast<char>(inStr[i]);
	}
	outStr[i] = '\0';
#endif
}

void convertChar32ToLocalChar(const char32_t* inStr, size_t inStrLen, char* outStr, size_t outStrLen)
{
	// TODO: 今のところ ASCII のみのエラーメッセージしか想定していないためこれでもOK.
	// 後々 SJIS 対応するか、wchar_t で出力してしまってもいいだろう。
	UTFConversionOptions options;
	options.ReplacementChar = '?';
	UnicodeUtils::convertUTF32toUTF8(reinterpret_cast<const UTF32*>(inStr), inStrLen, reinterpret_cast<UTF8*>(outStr), outStrLen, &options);
}

//void errorPrintf(Char* buf, size_t bufSize, const char* format, ...)
//{
//	va_list args;
//	va_start(args, format);
//	
//	const size_t BUFFER_SIZE = 511;
//	char str[BUFFER_SIZE + 1] = {};
//	StringHelper::vsprintf(str, BUFFER_SIZE, format, args);
//	
//#ifdef LN_USTRING16
//	// char -> wchar_t
//	wchar_t wstr[BUFFER_SIZE + 1] = {};
//	convertCharToWChar(str, BUFFER_SIZE, wstr, BUFFER_SIZE);
//
//	// wchar_t -> char16_t
//	convertWCharToChar16(wstr, BUFFER_SIZE, buf, bufSize);
//#else
//	// char -> wchar_t
//	convertCharToWChar(str, BUFFER_SIZE, buf, bufSize - 1);
//#endif
//
//	va_end(args);
//}
//
//void errorPrintf(Char* buf, size_t bufSize, const wchar_t* format, ...)
//{
//	va_list args;
//	va_start(args, format);
//
//#ifdef LN_USTRING16
//	const size_t BUFFER_SIZE = 511;
//	wchar_t str[BUFFER_SIZE + 1] = {};
//	StringHelper::vsprintf(str, BUFFER_SIZE, format, args);
//
//	// wchar_t -> char16_t
//	convertWCharToChar16(str, BUFFER_SIZE, buf, bufSize - 1);
//#else
//	StringHelper::vsprintf(buf, bufSize, format, args);
//#endif
//
//	va_end(args);
//}
//
//void errorPrintf(Char* buf, size_t bufSize, const char16_t* format)
//{
//#ifdef LN_USTRING16
//	memcpy(buf, format, sizeof(Char) * LN_MAX(bufSize, StringHelper::strlen(format)));
//#else
//	convertChar16ToWChar(format, StringHelper::strlen(format), buf, bufSize - 1);
//#endif
//}
//
//void errorPrintf(Char* buf, size_t bufSize, const String& format)
//{
//	StringHelper::strcpy(buf, bufSize, format.c_str());
//}
//
//void errorPrintf(Char* buf, size_t bufSize)
//{
//	// dummy
//	buf[0] = '\0';
//}

// Lumino default error notification
void printError(const Exception& e)
{
	const size_t BUFFER_SIZE = 512;
	char buf[BUFFER_SIZE] = {};
	int len = snprintf(
		buf, BUFFER_SIZE, "%s(%d):\"%s\" ",
		ExceptionHelper::getSourceFilePath(e),
		ExceptionHelper::getSourceFileLine(e),
		(ExceptionHelper::getAssertionMessage(e)) ? ExceptionHelper::getAssertionMessage(e) : "");

	//if (!StringHelper::isNullOrEmpty(e.message())
	{
		buf[len] = '\n';
		len++;
        size_t messageLen = UnicodeStringUtils::strlen(e.message());
        size_t bufferLen = BUFFER_SIZE - len;
#if LN_USTRING32
		convertChar32ToLocalChar(e.message(), messageLen, buf + len, bufferLen);
#else
        convertChar16ToLocalChar(e.message(), messageLen, buf + len, bufferLen);
#endif
        if (messageLen >= bufferLen) {
            buf[BUFFER_SIZE - 4] = '.';
            buf[BUFFER_SIZE - 3] = '.';
            buf[BUFFER_SIZE - 2] = '.';
            buf[BUFFER_SIZE - 1] = '\0';
        }
	}

	//if (Logger::hasAnyAdapter()) {
	//	LN_LOG_ERROR << buf;
	//}
	//else {
		printf("%s\n", buf);
	//}
}

void notifyFatalError(const char* file, int line, const char* message)
{
	printf("%s(%d): Fatal: %s", file, line, message);
	*reinterpret_cast<int*>(0) = 0;	// crash
}

void ExceptionHelper::setSourceLocationInfo(Exception& e, ExceptionLevel level, const char* filePath, int fileLine, const char* assertionMessage)
{
	e.m_level = level;
	detail::strcpy(e.m_sourceFilePath, Exception::MaxPathSize - 1, filePath);
	e.m_sourceFileLine = fileLine;
	if (assertionMessage) {
		detail::strcpy(e.m_assertionMessage, Exception::MaxAssertionMessageSize - 1, assertionMessage);
	}
}

void ExceptionHelper::setMessage(Exception& e, const std::string& str)
{
#if LN_USTRING32
	e.m_message = UnicodeStringUtils::NarrowToU32(str.c_str(), str.length());
#else
	e.m_message = UnicodeStringUtils::NarrowToU16(str.c_str(), str.length());
#endif
}

void ExceptionHelper::setMessage(Exception& e, const std::wstring& str)
{
#if LN_USTRING32
	e.m_message = UnicodeStringUtils::WideToU32(str.c_str(), str.length());
#else
	e.m_message = UnicodeStringUtils::WideToU16(str.c_str(), str.length());
#endif
}

void ExceptionHelper::setMessage(Exception& e, const UStdString& str)
{
//#if LN_USTRING32
//	e.m_message = UnicodeStringUtils::U16ToU32(str.c_str(), str.length());
//#else
//	e.m_message = str;
//#endif
}

} // namespace detail


//==============================================================================
// Exception

static Exception::NotificationHandler	g_notifyVerificationHandler = nullptr;

void Exception::setNotificationHandler(NotificationHandler handler)
{
	g_notifyVerificationHandler = handler;
}

Exception::NotificationHandler Exception::notificationHandler()
{
	return g_notifyVerificationHandler;
}

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

Exception::Exception(const Char* message)
	: Exception()
{
}

Exception::Exception()
	: m_level(ExceptionLevel::Fatal)
	, m_sourceFilePath{}
	, m_sourceFileLine(0)
	, m_assertionMessage{}
	, m_message()
{
}

Exception::~Exception()
{
}

Exception* Exception::copy() const
{
	return LN_NEW Exception(*this);
}


//void Exception::setMessage()
//{
//	m_message.clear();
//}
//
//void Exception::setCaption(const Char* caption)
//{
//	m_caption = caption;
//}
//
//void Exception::setMessage(const char* format, va_list args)
//{
//	// caption
//	m_message = getCaption();
//
//	// format char
//	static const int BUFFER_SIZE = 512;
//	char buf[BUFFER_SIZE];
//	int len = StringHelper::vsprintf(buf, BUFFER_SIZE, format, args);
//
//	// char to Char
//	Char ucharBuf[BUFFER_SIZE];
//	safeCharToUChar(buf, ucharBuf, BUFFER_SIZE);
//	appendMessage(ucharBuf, len);
//}
//
//void Exception::setMessage(const wchar_t* format, va_list args)
//{
//	// caption
//	m_message = getCaption();
//
//	// format char
//	static const int BUFFER_SIZE = 512;
//	wchar_t buf[BUFFER_SIZE];
//	int len = StringHelper::vsprintf(buf, BUFFER_SIZE, format, args);
//
//	// char to Char
//	Char ucharBuf[BUFFER_SIZE];
//	safeWCharToUChar(buf, ucharBuf, BUFFER_SIZE);
//	appendMessage(ucharBuf, len);
//}
//
//void Exception::setMessage(const char* format, ...)
//{
//	va_list args;
//	va_start(args, format);
//	setMessage(format, args);
//	va_end(args);
//}
//
//void Exception::setMessage(const wchar_t* format, ...)
//{
//	va_list args;
//	va_start(args, format);
//	setMessage(format, args);
//	va_end(args);
//}
//
//void Exception::setMessageU(const Char* message)
//{
//	appendMessage(message, StringHelper::strlen(message));
//}
//
//void Exception::appendMessage(const Char* message, size_t len)
//{
//	m_message.append(message, len);
//}

} // namespace ln


