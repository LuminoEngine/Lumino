
#pragma once
#include "../Base/StringRef.h"

LN_NAMESPACE_BEGIN

/**
	@brief		コンソールウィンドウの操作を行います。
*/
class Console
{
public:
	
	/**
		@brief		コンソールウィンドウを割り当てます。
	*/
	static void alloc();
	
	/**
		@brief		コンソールウィンドウを解放します。
	*/
	static void Free();

	static void WriteLine();


	static void WriteLine(const StringRefA& str);
	static void WriteLine(const StringRefW& str);

	template<typename... TArgs>
	static void WriteLine(const StringRefA& str, const TArgs&... args);
	template<typename... TArgs>
	static void WriteLine(const StringRefW& str, const TArgs&... args);



	static void WriteLineError();

	template<typename... TArgs>
	static void WriteLineError(const StringRefA& str, const TArgs&... args);
	template<typename... TArgs>
	static void WriteLineError(const StringRefW& str, const TArgs&... args);

private:
	static void WriteInternal(const char* str);
	static void WriteInternal(const wchar_t* str);
	static void WriteInternalError(const char* str);
	static void WriteInternalError(const wchar_t* str);
};

//------------------------------------------------------------------------------
template<typename... TArgs>
void Console::WriteLine(const StringRefA& str, const TArgs&... args)
{
	StringA s = StringA::Format(str, args...);
	WriteInternal(s.c_str());
	WriteLine();
}

//------------------------------------------------------------------------------
template<typename... TArgs>
void Console::WriteLine(const StringRefW& str, const TArgs&... args)
{
	StringW s = StringW::Format(str, args...);
	WriteInternal(s.c_str());
	WriteLine();
}

//------------------------------------------------------------------------------
template<typename... TArgs>
void Console::WriteLineError(const StringRefA& str, const TArgs&... args)
{
	StringA s = StringA::Format(str, args...);
	WriteInternalError(s.c_str());
	WriteLineError();
}

//------------------------------------------------------------------------------
template<typename... TArgs>
void Console::WriteLineError(const StringRefW& str, const TArgs&... args)
{
	StringW s = StringW::Format(str, args...);
	WriteInternalError(s.c_str());
	WriteLineError();
}

LN_NAMESPACE_END
