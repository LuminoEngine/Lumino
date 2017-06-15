
#include "XAudio2Module.h"
#include <VersionHelpers.h>

//==============================================================================
// XAudio2Module
//==============================================================================

// XAudio2 Versions
// https://msdn.microsoft.com/en-us/library/windows/desktop/ee415802%28v=vs.85%29.aspx
// - XAudio2.8 以降、X3DAudio は XAudio2 に統合された。lib ファイルは XAUDIO2_8.LIB を使う。
// - XAudio2.8 以降、再頒布ファイルは必要ない。

//------------------------------------------------------------------------------
XAudio2Module::XAudio2Module()
	: m_XAudio2Module(NULL)
	, m_X3DAudioModule(NULL)
{
}

//------------------------------------------------------------------------------
XAudio2Module::~XAudio2Module()
{
	if (m_XAudio2Module != NULL) ::FreeLibrary(m_XAudio2Module);
	if (m_X3DAudioModule != NULL) ::FreeLibrary(m_X3DAudioModule);
}

//------------------------------------------------------------------------------
HRESULT XAudio2Module::XAudio2Create(__deref_out IXAudio2** ppXAudio2, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor)
{
	// Windows7
	return ::XAudio2Create(ppXAudio2, Flags, XAudio2Processor);
}

//------------------------------------------------------------------------------
bool XAudio2Module::initialize()
{
	// Windows10
	if (IsWindowsVersionOrGreater(10, 0, 0))
	{
		m_XAudio2Module = ::LoadLibrary(TEXT("XAudio2_9.dll"));
		if (m_XAudio2Module == NULL) return false;

		DWORD r = 0;
		r |= ((X3DAudioInitialize = (Proc_X3DAudioInitialize)GetProcAddress(m_XAudio2Module, "X3DAudioInitialize")) == NULL);
		r |= ((X3DAudioCalculate = (Proc_X3DAudioCalculate)GetProcAddress(m_XAudio2Module, "X3DAudioCalculate")) == NULL);
		if (r != 0) return false;
	}
	// Windows8
	if (IsWindowsVersionOrGreater(6, 2, 0))
	{
		m_XAudio2Module = ::LoadLibrary(TEXT("XAudio2_8.dll"));
		if (m_XAudio2Module == NULL) return false;

		DWORD r = 0;
		r |= ((X3DAudioInitialize = (Proc_X3DAudioInitialize)GetProcAddress(m_XAudio2Module, "X3DAudioInitialize")) == NULL);
		r |= ((X3DAudioCalculate = (Proc_X3DAudioCalculate)GetProcAddress(m_XAudio2Module, "X3DAudioCalculate")) == NULL);
		if (r != 0) return false;
	}
	// Windows7
	else if (IsWindowsVersionOrGreater(6, 1, 0))
	{
		m_X3DAudioModule = ::LoadLibrary(TEXT("X3DAudio1_7.dll"));
		if (m_X3DAudioModule == NULL) return false;

		DWORD r = 0;
		r |= ((X3DAudioInitialize = (Proc_X3DAudioInitialize)GetProcAddress(m_X3DAudioModule, "X3DAudioInitialize")) == NULL);
		r |= ((X3DAudioCalculate = (Proc_X3DAudioCalculate)GetProcAddress(m_X3DAudioModule, "X3DAudioCalculate")) == NULL);
		if (r != 0) return false;
	}

	return true;
}

