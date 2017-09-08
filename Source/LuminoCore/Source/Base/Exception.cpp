
#include <time.h>
#include <algorithm>
#include "../Internal.h"
#include <Lumino/Base/CRT.h>
#include <Lumino/Base/Exception.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/StringHelper.h>
#include <Lumino/Base/Logger.h>
#include <Lumino/Base/Resource.h>

#ifdef LN_EXCEPTION2

LN_NAMESPACE_BEGIN


//==============================================================================
// Assertion
//==============================================================================

static Assertion::NotifyVerificationHandler	g_notifyVerificationHandler = nullptr;

void Assertion::setNotifyVerificationHandler(NotifyVerificationHandler handler)
{
	g_notifyVerificationHandler = handler;
}

Assertion::NotifyVerificationHandler Assertion::getNotifyVerificationHandler()
{
	return g_notifyVerificationHandler;
}

//
//namespace detail {
//
//void makeExceptionMessage(Exception* e)
//{
//	e->setme
//}
//
//void makeExceptionMessage(Exception* e, const char* format, ...)
//{
//	va_list args;
//	va_start(args, format);
//	va_end(args);
//}
//
//void makeExceptionMessage(Exception* e, const wchar_t* format, ...)
//{
//	va_list args;
//	va_start(args, format);
//	va_end(args);
//}
//
//void makeExceptionMessage(Exception* e, const char16_t* message)
//{
//}
//
//} // namespace detail

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

Exception::Exception()
	: m_sourceFilePath(_LT(""))
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
	return _LT("ln::Exception");
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
	int len = StringTraits::vsprintf(buf, BUFFER_SIZE, format, args);

	// char to UChar
	Char ucharBuf[BUFFER_SIZE];
	safeCharToUChar(buf, ucharBuf, LN_ARRAY_SIZE_OF(ucharBuf));
	appendMessage(ucharBuf, len);
}

void Exception::setMessage(const wchar_t* format, va_list args)
{
	// caption
	m_message = getCaption();

	// format char
	static const int BUFFER_SIZE = 512;
	wchar_t buf[BUFFER_SIZE];
	int len = StringTraits::vsprintf(buf, BUFFER_SIZE, format, args);

	// char to UChar
	Char ucharBuf[BUFFER_SIZE];
	safeWCharToUChar(buf, ucharBuf, LN_ARRAY_SIZE_OF(ucharBuf));
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

void Exception::appendMessage(const Char* message, size_t len)
{
	m_message.append(message, len);
}

void Exception::setSourceLocationInfo(const Char* filePath, int fileLine)
{
	StringTraits::tstrcpy(m_sourceFilePath, MaxPathSize - 1, filePath);
	m_sourceFileLine = fileLine;
}

//==============================================================================
// LogicException
//==============================================================================
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
	setCaption(InternalResource::getString(InternalResource::NotImplementedError).c_str());
}

Exception* NotImplementedException::copy() const
{
	return LN_NEW NotImplementedException(*this);
}

//==============================================================================
// IOException
//==============================================================================
IOException::IOException()
{
	setCaption(InternalResource::getString(InternalResource::IOError).c_str());
}

Exception* IOException::copy() const
{
	return LN_NEW IOException(*this);
}

//==============================================================================
// FileNotFoundException
//==============================================================================
FileNotFoundException::FileNotFoundException()
{
	setCaption(InternalResource::getString(InternalResource::FileNotFoundError).c_str());
}

Exception* FileNotFoundException::copy() const
{
	return LN_NEW FileNotFoundException(*this);
}

//==============================================================================
// EncodingException
//==============================================================================
EncodingException::EncodingException()
{
	setCaption(InternalResource::getString(InternalResource::EncodingError).c_str());
}

Exception* EncodingException::copy() const
{
	return LN_NEW EncodingException(*this);
}

//==============================================================================
// EncodingException
//==============================================================================
InvalidFormatException::InvalidFormatException()
{
	setCaption(InternalResource::getString(InternalResource::InvalidFormatError).c_str());
}

Exception* InvalidFormatException::copy() const
{
	return LN_NEW InvalidFormatException(*this);
}

LN_NAMESPACE_END





#else

#ifdef LN_EXCEPTION_BACKTRACE
	#if defined(LN_OS_WIN32)	// Cygwin もこっち
		#include "Win32/BackTrace.h"
	#elif defined(LN_OS_MAC)
		#include "Unix/SimpleBackTrace.h"
	#else
		#include "Unix/BackTrace.h"
	#endif
#endif

LN_NAMESPACE_BEGIN

namespace detail {

bool notifyFatalError(const char* file, int line, const char* message)
{
	auto h = Assertion::getNotifyFataiErrorHandler();
	if (h != nullptr && h(file, line, message)) return true;
	printf("%s : %s(%d)", message, file, line);
	*reinterpret_cast<int*>(0) = 0;
	return true;
}

} // namespace detail

//==============================================================================
// Assertion
//==============================================================================

static Assertion::NotifyVerificationHandler	g_notifyVerificationHandler = nullptr;
static Assertion::NotifyFataiErrorHandler	g_notifyFataiErrorHandler = nullptr;

void Assertion::setNotifyVerificationHandler(NotifyVerificationHandler handler)
{
	g_notifyVerificationHandler = handler;
}

Assertion::NotifyVerificationHandler Assertion::getNotifyVerificationHandler()
{
	return g_notifyVerificationHandler;
}

void Assertion::setNotifyFataiErrorHandler(NotifyFataiErrorHandler handler)
{
	g_notifyFataiErrorHandler = handler;
}

Assertion::NotifyFataiErrorHandler Assertion::getNotifyFataiErrorHandler()
{
	return g_notifyFataiErrorHandler;
}

//==============================================================================
// Exception
//==============================================================================

static void safeCharToUChar(const char* src, Char* dst, int dstSize) LN_NOEXCEPT
{
	// TODO: 日本語対応
	int i = 0;
	for (; i < dstSize-1 && *src; i++, src++)
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




static char gDumpFilePath[LN_MAX_PATH] = { 0 };

//------------------------------------------------------------------------------
Exception::Exception()
	: m_sourceFileLine(0)
	, m_stackBufferSize(0)
{
	memset(m_stackBuffer, 0, sizeof(m_stackBuffer));
	memset(m_symbolBuffer, 0, sizeof(m_symbolBuffer));
	memset(m_message, 0, sizeof(m_message));

#ifdef LN_EXCEPTION_BACKTRACE
#ifdef LN_OS_MAC
	m_stackBufferSize = SimleBackTrace::BackTrace(m_stackBuffer, LN_ARRAY_SIZE_OF(m_stackBuffer));
	SimleBackTrace::MakeSymbolString(m_stackBuffer, m_stackBufferSize, m_symbolBuffer, LN_ARRAY_SIZE_OF(m_symbolBuffer));
#else
	// バックトレース記録
	m_stackBufferSize = BackTrace::getInstance()->Backtrace(m_stackBuffer, LN_ARRAY_SIZE_OF(m_stackBuffer));

	// バックトレース文字列取得
	BackTrace::getInstance()->AddressToFullSymbolString(
		m_stackBuffer, 
		std::min(m_stackBufferSize, 32),
		m_symbolBuffer, 
		LN_ARRAY_SIZE_OF(m_symbolBuffer));
#endif
#endif

	// ファイルに保存
	if (strlen(gDumpFilePath) > 0) 
	{
		FILE* fp;
		if (fopen_s(&fp, gDumpFilePath, "w") == 0) 
		{
			time_t timer;
			timer = ::time(NULL);

			char str[256];
			ctime_s(str, 256, &timer);
			
			fprintf(fp, "%s\n%s\n\n", str, m_symbolBuffer);
			fclose(fp);
		}
	}
}

//------------------------------------------------------------------------------
Exception::~Exception() throw()
{
}

//------------------------------------------------------------------------------
const Char* Exception::getMessage() const { return LN_AFX_FUNCNAME(getMessage)(); }
const Char* Exception::LN_AFX_FUNCNAME(getMessage)() const { return getMessageOverride(); }

//------------------------------------------------------------------------------
Exception& Exception::setSourceLocationInfo(const char* filePath, int fileLine)
{
	// もしバックトレースが取れていなかったらそれ用の文字列バッファに入れてしまう
	if (m_symbolBuffer[0] == 0x00)
	{
		sprintf_s(m_symbolBuffer, LN_ARRAY_SIZE_OF(m_symbolBuffer), "File:%s Line:%d", filePath, fileLine);
	}
	safeCharToUChar(filePath, m_sourceFilePath, LN_ARRAY_SIZE_OF(m_sourceFilePath));

	m_sourceFileLine = fileLine;
	return *this;
}

//------------------------------------------------------------------------------
bool Exception::initDumpFile(const char* filePath)
{
	// ファイルパス保持
	memset(gDumpFilePath, 0, sizeof(gDumpFilePath));
	strcpy_s(gDumpFilePath, LN_MAX_PATH, filePath);

	// ファイル初期化
	FILE* fp;
	if (fopen_s(&fp, filePath, "w") == 0) {
		fclose(fp);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void Exception::setMessage(const Char* caption)
{
	appendMessage(caption, StringTraits::tcslen(caption));
}

//------------------------------------------------------------------------------
void Exception::setMessage(const Char* caption, const char* format, va_list args)
{
	// caption
	appendMessage(caption, StringTraits::tcslen(caption));

	// format char
	static const int BUFFER_SIZE = MaxMessageBufferSize;
	char buf[BUFFER_SIZE];
	int len = StringTraits::vsprintf(buf, BUFFER_SIZE, format, args);

	// char to UChar
	Char ucharBuf[BUFFER_SIZE];
	safeCharToUChar(buf, ucharBuf, LN_ARRAY_SIZE_OF(ucharBuf));
	appendMessage(ucharBuf, len);
}

//------------------------------------------------------------------------------
void Exception::setMessage(const Char* caption, const wchar_t* format, va_list args)
{
	// caption
	appendMessage(caption, StringTraits::tcslen(caption));

	// format char
	static const int BUFFER_SIZE = MaxMessageBufferSize;
	wchar_t buf[BUFFER_SIZE];
	int len = StringTraits::vsprintf(buf, BUFFER_SIZE, format, args);

	// char to UChar
	Char ucharBuf[BUFFER_SIZE];
	safeWCharToUChar(buf, ucharBuf, LN_ARRAY_SIZE_OF(ucharBuf));
	appendMessage(ucharBuf, len);
}

//------------------------------------------------------------------------------
void Exception::setMessage(const Char* caption, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	setMessage(caption, format, args);
	va_end(args);
}

//------------------------------------------------------------------------------
void Exception::setMessage(const Char* caption, const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);
	setMessage(caption, format, args);
	va_end(args);
}

//------------------------------------------------------------------------------
// GetMessage() を直接オーバーライドすると、windows.h と使うときに A/W を考慮せねばならず煩雑になる。それを回避するために用意した。
const Char* Exception::getMessageOverride() const
{
	return m_message;
}

void Exception::appendMessage(const Char* message, size_t len)
{
	// TODO: Char 用の strncpy あったほうがいい気がする・・
	size_t curLen = StringTraits::tcslen(m_message);
	size_t remainLen = (MaxMessageBufferSize - curLen) - 2;	// -2 は "\n\0"
	len = std::min(len, remainLen);
	Char* head = m_message + curLen;

	for (size_t i = 0; i < len; i++)
	{
		head[i] = message[i];
	}
	head[len] = '\n';
	head[len + 1] = '\0';
}

//==============================================================================
// VerifyException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(VerifyException, InternalResource::VerifyError);

//==============================================================================
// ArgumentException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(ArgumentException, InternalResource::ArgumentError);

//==============================================================================
// InvalidOperationException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(InvalidOperationException, InternalResource::InvalidOperationError);

//==============================================================================
// NotImplementedException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(NotImplementedException, InternalResource::NotImplementedError);

//==============================================================================
// OutOfMemoryException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(OutOfMemoryException, InternalResource::OutOfMemoryError);

//==============================================================================
// OutOfRangeException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(OutOfRangeException, InternalResource::OutOfRangeError);

//==============================================================================
// KeyNotFoundException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(KeyNotFoundException, InternalResource::KeyNotFoundError);

//==============================================================================
// OverflowException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(OverflowException, InternalResource::OverflowError);

//==============================================================================
// IOException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(IOException, InternalResource::IOError);

//==============================================================================
// FileNotFoundException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(FileNotFoundException, InternalResource::FileNotFoundError);

//==============================================================================
// DirectoryNotFoundException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(DirectoryNotFoundException, InternalResource::DirectoryNotFoundError);

//==============================================================================
// InvalidFormatException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(InvalidFormatException, InternalResource::InvalidFormatError);

//==============================================================================
// EndOfStreamException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(EndOfStreamException, InternalResource::EndOfStreamError);

//==============================================================================
// EncodingException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(EncodingException, InternalResource::EncodingError);

//==============================================================================
// RuntimeException
//==============================================================================
LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(RuntimeException, InternalResource::RuntimeError);


//==============================================================================
// Win32Exception
//==============================================================================

//------------------------------------------------------------------------------
Win32Exception::Win32Exception(uint32_t/*DWORD*/ dwLastError)
	: m_dwLastErrorCode( dwLastError )
{
	// エラーメッセージ取得
    memset(m_pFormatMessage, 0, sizeof(m_pFormatMessage));
#ifdef LN_OS_WIN32
    :: FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_dwLastErrorCode,
        0, m_pFormatMessage, sizeof(m_pFormatMessage)/sizeof(m_pFormatMessage[0]), NULL);
#endif
}

//==============================================================================
// COMException
//==============================================================================

//------------------------------------------------------------------------------
COMException::COMException(uint32_t hresult)
	: m_HRESULT(hresult)
{
}

//------------------------------------------------------------------------------
COMException::~COMException() throw()
{
}

//------------------------------------------------------------------------------
Exception* COMException::copy() const
{
	return LN_NEW COMException(*this);
}

LN_NAMESPACE_END

#endif

