
#pragma once
#include <xaudio2.h>
#include <x3daudio.h>
#include <VersionHelpers.h>

namespace ln {

// XAudio 関係のモジュールローダー
class XAudio2Module
{
public:
	typedef void (STDAPIVCALLTYPE *Proc_X3DAudioInitialize) (UINT32, FLOAT32, __out X3DAUDIO_HANDLE);
	typedef void(*Proc_X3DAudioCalculate) (__in const X3DAUDIO_HANDLE, __in const X3DAUDIO_LISTENER*, __in const X3DAUDIO_EMITTER*, UINT32, __inout X3DAUDIO_DSP_SETTINGS*);

	// XAudio2 Functions
	HRESULT XAudio2Create(__deref_out IXAudio2** ppXAudio2, UINT32 Flags X2DEFAULT(0), XAUDIO2_PROCESSOR XAudio2Processor X2DEFAULT(XAUDIO2_DEFAULT_PROCESSOR));
	
	// XAudio2 Functions
	Proc_X3DAudioInitialize	X3DAudioInitialize;
	Proc_X3DAudioCalculate	X3DAudioCalculate;
	
	XAudio2Module();
	~XAudio2Module();
	bool Initialize();

private:
	HMODULE	m_XAudio2Module;
	HMODULE	m_X3DAudioModule;
	
	// XAudio2 Versions
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ee415802%28v=vs.85%29.aspx
	// - XAudio2.8 以降、X3DAudio は XAudio2 に統合された。lib ファイルは XAUDIO2_8.LIB を使う。
	// - XAudio2.8 以降、再頒布ファイルは必要ない。
};

//------------------------------------------------------------------------------
inline HRESULT XAudio2Module::XAudio2Create(__deref_out IXAudio2** ppXAudio2, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor)
{
	// Windows7
	return ::XAudio2Create(ppXAudio2, Flags, XAudio2Processor);
}

//------------------------------------------------------------------------------
inline XAudio2Module::XAudio2Module()
	: m_XAudio2Module(NULL)
	, m_X3DAudioModule(NULL)
{}

//------------------------------------------------------------------------------
inline XAudio2Module::~XAudio2Module()
{
	if (m_XAudio2Module != NULL) ::FreeLibrary(m_XAudio2Module);
	if (m_X3DAudioModule != NULL) ::FreeLibrary(m_X3DAudioModule);
}

//------------------------------------------------------------------------------
inline bool XAudio2Module::Initialize()
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

} // namespace ln

