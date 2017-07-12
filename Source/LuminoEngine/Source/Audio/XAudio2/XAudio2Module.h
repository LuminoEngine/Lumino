
#pragma once
#include <xaudio2.h>
#include <x3daudio.h>

// XAudio 関係のモジュールローダー
class XAudio2Module
{
public:
	typedef void (STDAPIVCALLTYPE *Proc_X3DAudioInitialize) (UINT32, FLOAT32, __out X3DAUDIO_HANDLE);
	typedef void(*Proc_X3DAudioCalculate) (__in const X3DAUDIO_HANDLE, __in const X3DAUDIO_LISTENER*, __in const X3DAUDIO_EMITTER*, UINT32, __inout X3DAUDIO_DSP_SETTINGS*);

	XAudio2Module();
	~XAudio2Module();

	bool initialize();

	HRESULT XAudio2Create(__deref_out IXAudio2** ppXAudio2, UINT32 Flags X2DEFAULT(0), XAUDIO2_PROCESSOR XAudio2Processor X2DEFAULT(XAUDIO2_DEFAULT_PROCESSOR));

	Proc_X3DAudioInitialize	X3DAudioInitialize;

	Proc_X3DAudioCalculate	X3DAudioCalculate;

private:
	HMODULE	m_XAudio2Module;
	HMODULE	m_X3DAudioModule;
};
