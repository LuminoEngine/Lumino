
#include <time.h>
#include <algorithm>
#include "../Internal.h"
#include <Lumino/Base/CRT.h>
#include <Lumino/Base/Exception.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Logger.h>
#include <Lumino/Base/Resource.h>

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

//==============================================================================
// Assertion
//==============================================================================

static Assertion::NotifyVerificationHandler g_notifyVerificationHandler = nullptr;

void Assertion::SetNotifyVerificationHandler(NotifyVerificationHandler handler)
{
	g_notifyVerificationHandler = handler;
}

Assertion::NotifyVerificationHandler Assertion::GetNotifyVerificationHandler()
{
	return g_notifyVerificationHandler;
}

//==============================================================================
// Exception
//==============================================================================

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
	m_stackBufferSize = BackTrace::GetInstance()->Backtrace(m_stackBuffer, LN_ARRAY_SIZE_OF(m_stackBuffer));

	// バックトレース文字列取得
	BackTrace::GetInstance()->AddressToFullSymbolString(
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
#pragma push_macro("GetMessage")
#undef GetMessage
const TCHAR* Exception::GetMessage() const { return LN_AFX_FUNCNAME(GetMessage)(); }
const TCHAR* Exception::LN_AFX_FUNCNAME(GetMessage)() const { return GetMessageOverride(); }
#pragma pop_macro("GetMessage")

//------------------------------------------------------------------------------
Exception& Exception::SetSourceLocationInfo(const char* filePath, int fileLine)
{
	// もしバックトレースが取れていなかったらそれ用の文字列バッファに入れてしまう
	if (m_symbolBuffer[0] == 0x00)
	{
		sprintf_s(m_symbolBuffer, LN_ARRAY_SIZE_OF(m_symbolBuffer), "File:%s Line:%d", filePath, fileLine);
	}

#ifdef LN_UNICODE
	// ワイド文字列へ変換 (文字コードは考慮しない)
	memset(m_sourceFilePath, 0, sizeof(m_sourceFilePath));
	size_t size;
	errno_t err = mbstowcs_s(&size, m_sourceFilePath, LN_MAX_PATH, filePath, LN_MAX_PATH - 1);
	if (err != 0) {
		return *this;
	}
#else
	StringTraits::tstrcpy(m_sourceFilePath, sizeof(m_sourceFilePath) / sizeof(TCHAR), filePath);
#endif

	m_sourceFileLine = fileLine;
	return *this;
}

//------------------------------------------------------------------------------
bool Exception::InitDumpFile(const char* filePath)
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
void Exception::SetMessage(const TCHAR* caption)
{
	AppendMessage(caption, _tcslen(caption));
	//int captionLen = _tcslen(caption);
	//_tcscpy_s(m_message, MaxMessageBufferSize, caption);

	//int pos = std::min(captionLen, MaxMessageBufferSize);
	//m_message[pos] = '\n';
	//m_message[pos + 1] = '\0';
	//AppendMessage(m_symbolBuffer, strlen(m_symbolBuffer));

}

//------------------------------------------------------------------------------
void Exception::SetMessage(const TCHAR* caption, const char* format, va_list args)
{
	AppendMessage(caption, _tcslen(caption));

	static const int BUFFER_SIZE = MaxMessageBufferSize;
	char buf[BUFFER_SIZE];
	//int captionLen = _tcslen(caption) + 1;	// +1 は'\n' の分
	//int detailsLen = BUFFER_SIZE - captionLen;

	int len = StringTraits::VSPrintf(buf, BUFFER_SIZE, format, args);
	AppendMessage(buf, len);
	//if (len >= detailsLen)
	//{
	//	// バッファに収まりきらない場合は終端を ... にして切る
	//	buf[detailsLen - 4] = '.';
	//	buf[detailsLen - 3] = '.';
	//	buf[detailsLen - 2] = '.';
	//	buf[detailsLen - 1] = '\0';
	//}

//	// キャプション
//	_tcscpy_s(m_message, BUFFER_SIZE, caption);
//	m_message[captionLen - 1] = '\n';
//
//	// TCHAR に合わせてメンバに格納
//#ifdef LN_UNICODE
//	size_t wlen;
//	mbstowcs_s(&wlen, m_message + captionLen, detailsLen, buf, _TRUNCATE);
//#else
//	strcpy_s(m_message + captionLen, detailsLen, buf);
//#endif
	//AppendMessage(m_symbolBuffer, strlen(m_symbolBuffer));
}

//------------------------------------------------------------------------------
void Exception::SetMessage(const TCHAR* caption, const wchar_t* format, va_list args)
{
	AppendMessage(caption, _tcslen(caption));

	static const int BUFFER_SIZE = MaxMessageBufferSize;
	wchar_t buf[BUFFER_SIZE];
	//int captionLen = _tcslen(caption) + 1;	// +1 は'\n' の分
	//int detailsLen = BUFFER_SIZE - captionLen;

	int len = StringTraits::VSPrintf(buf, BUFFER_SIZE, format, args);
	AppendMessage(buf, len);
//	if (len >= detailsLen)
//	{
//		// バッファに収まりきらない場合は終端を ... にして切る
//		buf[detailsLen - 4] = L'.';
//		buf[detailsLen - 3] = L'.';
//		buf[detailsLen - 2] = L'.';
//		buf[detailsLen - 1] = L'\0';
//	}
//
//	// キャプション
//	_tcscpy_s(m_message, BUFFER_SIZE, caption);
//	m_message[captionLen - 1] = '\n';
//
//	// TCHAR に合わせてメンバに格納
//#ifdef LN_UNICODE
//	wcscpy_s(m_message + captionLen, detailsLen, buf);
//#else
//	size_t mbcslen;
//	wcstombs_s(&mbcslen, m_message + captionLen, detailsLen, buf, _TRUNCATE);
//#endif
	//AppendMessage(m_symbolBuffer, strlen(m_symbolBuffer));
}

//------------------------------------------------------------------------------
void Exception::SetMessage(const TCHAR* caption, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	SetMessage(caption, format, args);
	va_end(args);
}

//------------------------------------------------------------------------------
void Exception::SetMessage(const TCHAR* caption, const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);
	SetMessage(caption, format, args);
	va_end(args);
}

//------------------------------------------------------------------------------
// GetMessage() を直接オーバーライドすると、windows.h と使うときに A/W を考慮せねばならず煩雑になる。それを回避するために用意した。
const TCHAR* Exception::GetMessageOverride() const
{
	return m_message;
}

//------------------------------------------------------------------------------
void Exception::AppendMessage(const char* message, size_t len)
{
	size_t curLen = _tcslen(m_message);
	size_t remainLen = (MaxMessageBufferSize - curLen) - 2;	// -2 は "\r\0"
	len = std::min(len, remainLen);

	TCHAR* head = m_message + curLen;
#ifdef LN_UNICODE
	size_t wlen;
	mbstowcs_s(&wlen, head, remainLen + 2, message, _TRUNCATE);
#else
	strncpy_s(head, remainLen + 2, message, len);
#endif

	head[len] = '\n';
	head[len + 1] = '\0';
}

//------------------------------------------------------------------------------
void Exception::AppendMessage(const wchar_t* message, size_t len)
{
	size_t curLen = _tcslen(m_message);
	size_t remainLen = (MaxMessageBufferSize - curLen) - 2;	// -2 は "\r\0"
	len = std::min(len, remainLen);

	TCHAR* head = m_message + curLen;
#ifdef LN_UNICODE
	wcsncpy_s(head, remainLen + 2, message, len);
#else
	size_t mbcslen;
	wcstombs_s(&mbcslen, head, remainLen + 2, message, _TRUNCATE);
#endif

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
Exception* COMException::Copy() const
{
	return LN_NEW COMException(*this);
}

LN_NAMESPACE_END
