#include "../Internal.h"
#include <Lumino/Base/Environment.h>
#include <Lumino/IO/Console.h>

#ifdef LN_USTRING
#else

LN_NAMESPACE_BEGIN

//==============================================================================
// Console
//==============================================================================

#if defined(LN_OS_WIN32)

class ConsoleInternal
{
public:
	ConsoleInternal()
		: m_in(NULL)
		, m_out(NULL)
		, m_err(NULL)
		, m_consoleAlloced(false)
	{}

	~ConsoleInternal()
	{
		Free();
	}

	void alloc()
	{
		// コンソールアプリとして実行している場合はこの if に入ることはない
		if (!m_consoleAlloced && ::AllocConsole())
		{
			freopen_s(&m_in, "CON", "r", stdin);
			freopen_s(&m_out, "CON", "w", stdout);
			freopen_s(&m_err, "CON", "w", stderr);
			m_consoleAlloced = true;
		}
	}

	void Free()
	{
		if (m_in != NULL) { 
			fclose(m_in);
			m_in = NULL;
		}
		if (m_out != NULL) {
			fclose(m_out);
			m_out = NULL;
		}
		if (m_err != NULL) {
			fclose(m_err);
			m_err = NULL;
		}
		if (m_consoleAlloced) {
			::FreeConsole();
			m_consoleAlloced = false;
		}
	}

private:
	FILE*	m_in;
	FILE*	m_out;
	FILE*	m_err;
	bool	m_consoleAlloced;
};
static ConsoleInternal g_console;

//------------------------------------------------------------------------------
void Console::alloc()
{
	g_console.alloc();
}

//------------------------------------------------------------------------------
void Console::Free()
{
	g_console.Free();
}

#endif

//------------------------------------------------------------------------------
void Console::writeLine()
{
	writeInternal(Environment::getNewLine<char>());
}

//------------------------------------------------------------------------------
void Console::writeLine(const StringRefA& str)
{
	StringA s(str);
	writeInternal(s.c_str());
	writeLine();
}

//------------------------------------------------------------------------------
void Console::writeLine(const StringRefW& str)
{
	StringW s(str);
	writeInternal(s.c_str());
	writeLine();
}

//------------------------------------------------------------------------------
void Console::writeLineError()
{
	writeInternalError(Environment::getNewLine<char>());
}

//------------------------------------------------------------------------------
void Console::writeInternal(const char* str)
{
	if (LN_CHECK_ARG(str != nullptr)) return;
	fprintf(stdout, "%s", str);
}

//------------------------------------------------------------------------------
void Console::writeInternal(const wchar_t* str)
{
	if (LN_CHECK_ARG(str != nullptr)) return;
	fwprintf(stdout, L"%s", str);
}

//------------------------------------------------------------------------------
void Console::writeInternalError(const char* str)
{
	if (LN_CHECK_ARG(str != nullptr)) return;
	fprintf(stderr, "%s", str);
}

//------------------------------------------------------------------------------
void Console::writeInternalError(const wchar_t* str)
{
	if (LN_CHECK_ARG(str != nullptr)) return;
	fwprintf(stderr, L"%s", str);
}

LN_NAMESPACE_END

#endif
