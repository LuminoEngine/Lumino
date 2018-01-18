/**
	@file	Exception.h
*/
#pragma once
#include <string>
#include "Common.h"


#define LN_EXCEPTION2

#ifdef LN_EXCEPTION2

LN_NAMESPACE_BEGIN
class Exception;
class String;

#define _LN_CHECK(expression, exception, ...)		(!(expression)) && ln::detail::notifyException<exception>(LN__FILE__, __LINE__, ##__VA_ARGS__)

// core
#define LN_REQUIRE(expression, ...)					_LN_CHECK(expression, ::ln::LogicException, ##__VA_ARGS__)
#define LN_ENSURE(expression, ...)					_LN_CHECK(expression, ::ln::RuntimeException, ##__VA_ARGS__)
#define LN_FATAL(expression, ...)					_LN_CHECK(expression, ::ln::FatalException, ##__VA_ARGS__)

// utils
#define LN_UNREACHABLE()							_LN_CHECK(0, ::ln::LogicException)
#define LN_NOTIMPLEMENTED()							_LN_CHECK(0, ln::NotImplementedException)
#define LN_REQUIRE_RANGE(value, begin, end)			_LN_CHECK(begin <= value && value < end, ::ln::LogicException)
#define LN_REQUIRE_KEY(expression, ...)				_LN_CHECK(expression, ln::LogicException, ##__VA_ARGS__)
#define LN_ENSURE_IO(expression)					_LN_CHECK(expression, ln::IOException)
#define LN_ENSURE_FILE_NOT_FOUND(expression, path)	_LN_CHECK(expression, ln::FileNotFoundException, path)
#define LN_ENSURE_ENCODING(expression, ...)			_LN_CHECK(expression, ln::EncodingException)
#define LN_ENSURE_INVALID_FORMAT(expression, ...)	_LN_CHECK(expression, ln::InvalidFormatException, ##__VA_ARGS__)
#define LN_ENSURE_WIN32(expression, err)			_LN_CHECK(expression, ln::Win32Exception, err)

// obsolete
#define LN_THROW(exp, type, ...)	{ _LN_CHECK(exp, type, ##__VA_ARGS__); }
#define LN_COMCALL(exp)				{ HRESULT hr = (exp); if (FAILED(hr)) { LN_ENSURE_WIN32(0, hr); } }

// internal
#define LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(className) \
	className(const char* message, ...); \
	className(const wchar_t* message, ...);
#define LN_EXCEPTION_FORMATTING_CONSTRUCTOR_IMPLEMENT(className) \
	className::className(const char* message, ...) \
	{ \
		va_list args; \
		va_start(args, message); \
		setMessage(message, args); \
		va_end(args); \
	} \
	className::className(const wchar_t* message, ...) \
	{ \
		va_list args; \
		va_start(args, message); \
		setMessage(message, args); \
		va_end(args); \
	}


class Assertion
{
public:
	using NotifyVerificationHandler = bool(*)(Exception& e);

	static void setNotifyVerificationHandler(NotifyVerificationHandler handler);
	static NotifyVerificationHandler getNotifyVerificationHandler();
};


namespace detail {

template<class TException, typename... TArgs>
bool notifyException(const Char* file, int line, TArgs... args);

void Exception_setSourceLocationInfo(Exception& e, const Char* filePath, int fileLine);

} // namespace detail

//------------------------------------------------------------------------------
// core errors

/**
	@brief		アプリケーションの実行中に発生したエラーを表します。
*/
class LUMINO_EXPORT Exception
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(Exception);

	Exception();
	virtual ~Exception();
	
	/** 例外を説明するメッセージを取得します。 */
	virtual const Char* getMessage() const;
	
	/** 例外のコピーを作成します。 */
	virtual Exception* copy() const;

protected:
	void setCaption(const Char* caption);
	virtual std::basic_string<Char> getCaption();
	void setMessage();
	void setMessage(const char* format, va_list args);
	void setMessage(const wchar_t* format, va_list args);
	void setMessage(const char* format, ...);
	void setMessage(const wchar_t* format, ...);

private:
	void appendMessage(const Char* message, size_t len);
	void setSourceLocationInfo(const Char* filePath, int fileLine);

	static const int MaxPathSize = 260;
	Char					m_sourceFilePath[MaxPathSize];
	int						m_sourceFileLine;
	void*					m_stackBuffer[32];
	int						m_stackBufferSize;
	std::basic_string<Char>	m_caption;
	std::basic_string<Char>	m_message;

	friend void detail::Exception_setSourceLocationInfo(Exception& e, const Char* filePath, int fileLine);
};

/**
	@brief		前提条件の間違いなどプログラム内の論理的な誤りが原因で発生したエラーを表します。
*/
class LUMINO_EXPORT LogicException
	: public Exception
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(LogicException);
	LogicException();
	virtual Exception* copy() const;
};

/**
	@brief		主にアプリケーションの実行環境が原因で発生したエラーを表します。
*/
class LUMINO_EXPORT RuntimeException
	: public Exception
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(RuntimeException);
	RuntimeException();
	virtual Exception* copy() const;
};

/**
	@brief		アプリケーションの継続が難しい致命的なエラーを表します。
*/
class LUMINO_EXPORT FatalException
	: public Exception
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(FatalException);
	FatalException();
	virtual Exception* copy() const;
};


//------------------------------------------------------------------------------
// extension errors

/**
	@brief		未実装の機能を呼び出した場合のエラーを表します。
*/
class LUMINO_EXPORT NotImplementedException
	: public LogicException
{
public:
	NotImplementedException();
	NotImplementedException(const Char* message);
	virtual Exception* copy() const;
};

/**
	@brief		I/O エラーが発生した場合のエラーを表します。
*/
class LUMINO_EXPORT IOException
	: public RuntimeException
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(IOException);
	IOException();
	virtual Exception* copy() const;
};

/**
	@brief		ファイルアクセスに失敗した場合のエラーを表します。
*/
class LUMINO_EXPORT FileNotFoundException
	: public IOException
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(FileNotFoundException);
	FileNotFoundException();
	virtual Exception* copy() const;
};

/**
	@brief		エンコーディングの変換に失敗した場合のエラーを表します。
*/
class LUMINO_EXPORT EncodingException
	: public RuntimeException
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(EncodingException);
	EncodingException();
	virtual Exception* copy() const;
};

/**
	@brief		無効な形式のデータが入力された場合のエラーを表します。
*/
class LUMINO_EXPORT InvalidFormatException
	: public RuntimeException
{
public:
	LN_EXCEPTION_FORMATTING_CONSTRUCTOR_DECLARE(InvalidFormatException);
	InvalidFormatException();
	virtual Exception* copy() const;
};

/**
	@brief	WindowsAPI のエラーを表します。 (GetLastError)
*/
//class Win32Exception 
//	: public Exception
//{
//public:
//	Win32Exception();
//	Win32Exception(uint32_t dwLastError);
//	virtual Exception* copy() const;
//
//	uint32_t getLastErrorCode() const { return m_dwLastErrorCode; }
//	const std::basic_string<Char>& getFormatMessage() const { return m_formatMessage; }
//
//private:
//	void setMessage(uint32_t dwLastError);
//	uint32_t				m_dwLastErrorCode;
//	std::basic_string<Char>	m_formatMessage;
//};
//
//



namespace detail {

void errorPrintf(Char* buf, size_t bufSize, const char* format, ...);
void errorPrintf(Char* buf, size_t bufSize, const wchar_t* format, ...);
void errorPrintf(Char* buf, size_t bufSize, const char16_t* format);
void errorPrintf(Char* buf, size_t bufSize, const String& format);
void errorPrintf(Char* buf, size_t bufSize);

template<class TException, typename... TArgs>
inline bool notifyException(const Char* file, int line, TArgs... args)
{
	const size_t BUFFER_SIZE = 512;
	Char str[BUFFER_SIZE] = {};
	errorPrintf(str, BUFFER_SIZE, args...);
	TException e(str);
	detail::Exception_setSourceLocationInfo(e, file, line);
	auto h = Assertion::getNotifyVerificationHandler();
	if (h != nullptr && h(e)) return true;
	throw e;
	return true;
}

} // namespace detail
LN_NAMESPACE_END






#else

#include <exception>
#include <assert.h>

// exp の条件が満たされなかった場合、type に指定した例外を throw する
#define LN_THROW( exp, type, ... )	{ if (!(exp)) { type e = type(__VA_ARGS__); e.setSourceLocationInfo(__FILE__, __LINE__); throw e; } }

// HRESULT を返す関数の呼び出しユーティリティ (Win32 用)
#define LN_COMCALL(exp)				{ HRESULT hr = (exp); if (FAILED(hr)) { LN_THROW(0, COMException, hr); } }

/// 例外クラスの基本的なコンストラクタを宣言する
#define LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(className) \
	className(); \
	className(const char* message, ...); \
	className(const wchar_t* message, ...); \
	virtual ~className() throw()

/// 例外クラスの基本的なコンストラクタを定義する
#define LN_EXCEPTION_BASIC_CONSTRUCTOR_IMPLEMENT(className, captionStringId) \
	className::className() \
	{ \
		setMessage(InternalResource::getString(captionStringId).c_str()); \
	} \
	className::className(const char* message, ...) \
	{ \
		va_list args; \
		va_start(args, message); \
		setMessage(InternalResource::getString(captionStringId).c_str(), message, args); \
		va_end(args); \
	} \
	className::className(const wchar_t* message, ...) \
	{ \
		va_list args; \
		va_start(args, message); \
		setMessage(InternalResource::getString(captionStringId).c_str(), message, args); \
		va_end(args); \
	} \
	className::~className() throw() \
	{ \
	}



// TODO: コンパイルオプションで指定したい
//#define LN_DO_CHECK_ASSERT
#define LN_DO_CHECK_THROW




// 式を評価し、結果が false であれば、後続のブロックを実行する


/*
	
	Use case:

	int Func(int* ptr)
	{
		// 引数をチェックしてすぐ return する場合
		if (LN_CHECK_ARG(ptr != nullptr)) return -1;

		// メンバ変数をチェックしてすぐ return する場合
		if (LN_CHECK_STATE(m_state != 0)) return -1;

		// そのまま続行しても問題ないが、一応問題としてマークしておきたい場合
		int len = printf();
		LN_CHECK_STATE(len != 0);

		// 他の関数の戻り値のエラーチェック
		FILE* fp = fopen();
		if (LN_CHECK_STATE(fp != NULL)) return -1;

		// 通過禁止
		LN_UNREACHABLE();
	}

	const Object& At(int index)
	{
		LN_VERIFY(index >= 0);
	}
*/
#if defined(LN_DO_CHECK_ASSERT)
#define LN_CHECK(expression, exception, ...)		((!(expression)) && ln::detail::notifyAssert([](){ assert(!#expression); }))
#elif defined(LN_DO_CHECK_THROW)
#define LN_CHECK(expression, exception, ...)		(!(expression)) && ln::detail::notifyException<exception>(__FILE__, __LINE__, ##__VA_ARGS__)
#else
#define LN_FAIL_CHECK(expression, exception)		(!(expression))
#endif

#define LN_CHECK_ARG(expression, ...)				LN_CHECK(expression, ::ln::ArgumentException, ##__VA_ARGS__)
#define LN_CHECK_STATE(expression, ...)				LN_CHECK(expression, ::ln::InvalidOperationException, ##__VA_ARGS__)
#define LN_CHECK_FORMAT(expression, ...)			LN_CHECK(expression, ::ln::InvalidFormatException, ##__VA_ARGS__)
#define LN_CHECK_RANGE(value, begin, end)			LN_CHECK(begin <= value && value < end, ::ln::OutOfRangeException)

#define LN_FATAL(expression, message)				{ if (!(expression)) ln::detail::notifyFatalError(__FILE__, __LINE__, message); }
#define LN_VERIFY(expression, exception, ...)		{ if (!(expression)) ln::detail::notifyException<exception>(__FILE__, __LINE__, ##__VA_ARGS__); }
#define LN_VERIFY_ARG(expression, ...)				{ if (!(expression)) ln::detail::notifyException<::ln::ArgumentException>(__FILE__, __LINE__, ##__VA_ARGS__); }
#define LN_VERIFY_STATE(expression, ...)			{ if (!(expression)) ln::detail::notifyException<::ln::InvalidOperationException>(__FILE__, __LINE__, ##__VA_ARGS__); }
#define LN_VERIFY_FORMAT(expression, ...)			{ if (!(expression)) ln::detail::notifyException<::ln::InvalidFormatException>(__FILE__, __LINE__, ##__VA_ARGS__); }

// TODO: ln::detail::notifyXXXX へ流す
#define LN_REQUIRE(expression, ...)					LN_CHECK(expression, ::ln::InvalidOperationException, ##__VA_ARGS__)
#define LN_ENSURE(expression, ...)					LN_CHECK(expression, ::ln::InvalidOperationException, ##__VA_ARGS__)

#define LN_UNREACHABLE()							LN_VERIFY(0, ::ln::InvalidOperationException)
#define LN_NOTIMPLEMENTED()							LN_VERIFY(0, ln::NotImplementedException)

LN_NAMESPACE_BEGIN

class Exception;

class Assertion
{
public:
	using NotifyVerificationHandler = bool(*)(const Exception& e);
	using NotifyFataiErrorHandler = bool(*)(const char* file, int line, const char* message);

	static void setNotifyVerificationHandler(NotifyVerificationHandler handler);
	static NotifyVerificationHandler getNotifyVerificationHandler();

	static void setNotifyFataiErrorHandler(NotifyFataiErrorHandler handler);
	static NotifyFataiErrorHandler getNotifyFataiErrorHandler();
};

namespace detail
{
template<typename TAssert>
inline bool notifyAssert(TAssert callback)
{
	callback();
	return true;	// TODO: ユーザー通知
}

template<class TException, typename... TArgs>
inline bool notifyException(const char* file, int line, TArgs... args)
{
	TException e(args...);
	e.setSourceLocationInfo(file, line);
	auto h = Assertion::getNotifyVerificationHandler();
	if (h != nullptr && h(e)) return true;
	throw e;
	return true;
}

bool notifyFatalError(const char* file, int line, const char* message = nullptr);

} // namespace detail


/**
	@brief	例外ベースクラス

	@todo	VC++ で本ライブラリを DLL としてビルドするときは、DLLランタイムを使用しないと warning C4275 が発生する。
			これは、staticランタイムだと std::exception に dllexport がつかないため。
*/
class LUMINO_EXPORT Exception : public std::exception
{
public:
	Exception();
	virtual ~Exception() throw();
	Exception& setSourceLocationInfo(const char* filePath, int fileLine);

public:
	
	/**
		@brief	例外の詳細メッセージを取得します。
	*/
	const Char* getMessage() const;
	const Char* LN_AFX_FUNCNAME(getMessage)() const;

	/**
		@brief		例外発生時に詳細情報をダンプするファイルを初期化する
		@param[in]	filePath	: ダンプ先ファイル (文字数は LN_MAX_PATH 以内)
		@return		false の場合、ファイルアクセスに失敗した
		@details	指定されたパスで、ファイルを新規作成します。
					以降、この例外クラスのコンストラクタが呼ばれるたびに、詳細情報が追記されます。
	*/
	static bool initDumpFile(const char* filePath);

	/**
		@brief		例外のコピーを作成する
		@note		別スレッドで発生した例外を保持するために使用する。
	*/
	virtual Exception* copy() const { return LN_NEW Exception( *this ); }

public:
	// override std::exception
	virtual const char* what() const  throw() { return m_symbolBuffer; }

protected:
	void setMessage(const Char* caption);
	void setMessage(const Char* caption, const char* format, va_list args);
	void setMessage(const Char* caption, const wchar_t* format, va_list args);
	void setMessage(const Char* caption, const char* format, ...);
	void setMessage(const Char* caption, const wchar_t* format, ...);
	virtual const Char* getMessageOverride() const;

private:
	void appendMessage(const Char* message, size_t len);

private:
	static const int MaxMessageBufferSize = 1024;

	Char		m_sourceFilePath[LN_MAX_PATH];
	int			m_sourceFileLine;
	void*		m_stackBuffer[32];
	int			m_stackBufferSize;
	char		m_symbolBuffer[2048];
	Char		m_message[MaxMessageBufferSize];
};

/**
	@brief	前提条件の検証エラーです。
*/
class VerifyException 
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(VerifyException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW VerifyException(*this); }
};

/**
	@brief	メソッドに渡された引数のいずれかが無効な場合にスローされる例外。
*/
class ArgumentException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(ArgumentException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW ArgumentException( *this ); }
};

/**
	@brief	オブジェクトの現在の状態に対して無効な呼び出しが行われた
*/
class InvalidOperationException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(InvalidOperationException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW InvalidOperationException(*this); }
};

/**
	@brief	未実装の機能を呼び出した
*/
class NotImplementedException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(NotImplementedException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW NotImplementedException( *this ); }
};

/**
	@brief	メモリ不足例外
*/
class OutOfMemoryException 
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(OutOfMemoryException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW OutOfMemoryException( *this ); }
};

/**
	@brief	許容範囲外の値が指定された場合にスローされる例外です。
*/
class OutOfRangeException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(OutOfRangeException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW OutOfRangeException(*this); }
};

/**
	@brief	検索に使用したキーが、コレクションのいずれのキーとも一致しない場合にスローされる例外です。
*/
class KeyNotFoundException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(KeyNotFoundException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW KeyNotFoundException(*this); }
};

/**
	@brief	数値演算によるオーバーフローが発生した。
*/
class OverflowException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(OverflowException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW OverflowException(*this); }
};

/**
	@brief		その他のIO例外
	@details	読み取り属性のファイルを書き込みモードでオープンしようとした時等。
*/
class IOException 
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(IOException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW IOException( *this ); }
};

/**
	@brief	ディスク上に存在しないファイルにアクセスしようとして失敗したときにスローされる例外
*/
class FileNotFoundException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(FileNotFoundException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW FileNotFoundException(*this); }
};

/**
	@brief	無効なディレクトリにアクセスしようとしたときにスローされる例外
*/
class DirectoryNotFoundException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(DirectoryNotFoundException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW DirectoryNotFoundException(*this); }
};

/**
	@brief	ファイルや文字列等の形式が不正
*/
class InvalidFormatException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(InvalidFormatException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW InvalidFormatException(*this); }
};

/**
	@brief		ストリームの末尾を越えて読み込もうとしたときにスローされる例外です。
*/
class EndOfStreamException
	: public IOException
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(EndOfStreamException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW EndOfStreamException(*this); }
};

/**
	@brief	文字コードの変換中、マッピングできない文字または不正シーケンスが見つかった
*/
class EncodingException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(EncodingException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW EncodingException(*this); }
};

/**
	@brief	ランタイム機能の呼び出しで予期しないエラーが発生した。
*/
class RuntimeException
	: public Exception
{
public:
	LN_EXCEPTION_BASIC_CONSTRUCTOR_DECLARE(RuntimeException);

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW RuntimeException(*this); }
};

/**
	@brief	WindowsAPI のエラーを示す例外クラス (GetLastError())
*/
class Win32Exception 
	: public Exception
{
public:
	Win32Exception(uint32_t/*DWORD*/ dwLastError);
	virtual ~Win32Exception() throw() {}

public:
	uint32_t/*DWORD*/			getLastErrorCode() const { return m_dwLastErrorCode; }
	const TCHAR*	getFormatMessage() const { return m_pFormatMessage; }

public:
	// override Exception
	virtual Exception* copy() const { return LN_NEW Win32Exception( *this ); }

private:
	uint32_t/*DWORD*/	m_dwLastErrorCode;
	TCHAR				m_pFormatMessage[512];	///< FormatMessage() で取得したメッセージ
};

/**
	@brief	COM のエラーを示す例外クラス (HRESULT)
*/
class COMException
	: public Exception
{
public:
	COMException(uint32_t hresult);
	virtual ~COMException() throw();

	uint32_t getHRESULT() const { return m_HRESULT; }

public:
	// override Exception
	virtual Exception* copy() const;

private:
	uint32_t	m_HRESULT;
};

LN_NAMESPACE_END

#endif
