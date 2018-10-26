
#include "Internal.hpp"

#ifdef LN_OS_WIN32
#include "DSoundAudioDevice.hpp"

#pragma comment(lib, "dsound.lib")

namespace ln {
namespace detail {

//==============================================================================
// DSoundAudioDevice

DSoundAudioDevice::DSoundAudioDevice()
	: m_nextSectorNumber(0)
	, m_sctorSize(0)
{
}

void DSoundAudioDevice::initialize(int frameLength)
{
	AudioDevice::initialize(frameLength, 2);

	DWORD bufferSize = frameLength;
	bufferSize *= 2;	// channel.
	bufferSize *= 2;	// double buffering
	bufferSize *= sizeof(int16_t);

	HWND hWnd = GetDesktopWindow();

	HRESULT hr;

	hr = DirectSoundCreate8(NULL, &m_dsound, NULL);
	if (FAILED(hr)) {
		LN_LOG_ERROR << "DirectSoundCreate8 error. (" << hr << ")";
		return;
	}

	DSCAPS outCaps;
	outCaps.dwSize = sizeof(outCaps);
	hr = m_dsound->GetCaps(&outCaps);
	if (FAILED(hr)) {
		LN_LOG_ERROR << "DirectSoundCreate8 error. (" << hr << ")";
		return;
	}
	if (!(outCaps.dwFlags & DSCAPS_PRIMARYSTEREO)) {
		LN_LOG_ERROR << "The output device does not support stereo playback.";
		return;
	}

	// Dump caps.
	{
		LN_LOG_DEBUG << "DirectSound CAPS:";
		if (!(outCaps.dwFlags & DSCAPS_PRIMARYMONO)) {
			LN_LOG_DEBUG << "DSCAPS_PRIMARYMONO";
		}
		if (!(outCaps.dwFlags & DSCAPS_PRIMARYSTEREO)) {
			LN_LOG_DEBUG << "DSCAPS_PRIMARYSTEREO";
		}
		if (!(outCaps.dwFlags & DSCAPS_PRIMARY8BIT)) {
			LN_LOG_DEBUG << "DSCAPS_PRIMARY8BIT";
		}
		if (!(outCaps.dwFlags & DSCAPS_PRIMARY16BIT)) {
			LN_LOG_DEBUG << "DSCAPS_PRIMARY16BIT";
		}
		if (!(outCaps.dwFlags & DSCAPS_CONTINUOUSRATE)) {
			LN_LOG_DEBUG << "DSCAPS_CONTINUOUSRATE";
		}
		if (!(outCaps.dwFlags & DSCAPS_EMULDRIVER)) {
			LN_LOG_DEBUG << "DSCAPS_EMULDRIVER";
		}
		if (!(outCaps.dwFlags & DSCAPS_CERTIFIED)) {
			LN_LOG_DEBUG << "DSCAPS_CERTIFIED";
		}
		if (!(outCaps.dwFlags & DSCAPS_SECONDARYMONO)) {
			LN_LOG_DEBUG << "DSCAPS_SECONDARYMONO";
		}
		if (!(outCaps.dwFlags & DSCAPS_SECONDARYSTEREO)) {
			LN_LOG_DEBUG << "DSCAPS_SECONDARYSTEREO";
		}
		if (!(outCaps.dwFlags & DSCAPS_SECONDARY8BIT)) {
			LN_LOG_DEBUG << "DSCAPS_SECONDARY8BIT";
		}
		if (!(outCaps.dwFlags & DSCAPS_SECONDARY16BIT)) {
			LN_LOG_DEBUG << "DSCAPS_SECONDARY16BIT";
		}
	}

	hr = m_dsound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(hr)) {
		LN_LOG_ERROR << "SetCooperativeLevel error. (" << hr << ")";
		return;
	}

	// Create primary buffer
	DSBUFFERDESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(DSBUFFERDESC));
	bufferDescription.dwSize = sizeof(DSBUFFERDESC);
	bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
	hr = m_dsound->CreateSoundBuffer(&bufferDescription, &m_primaryBuffer, NULL);
	if (FAILED(hr)) {
		LN_LOG_ERROR << "CreateSoundBuffer error. (" << hr << ")";
		return;
	}

	// Set the primary buffer format.
	WAVEFORMATEX wf;
	wf.cbSize = sizeof(WAVEFORMATEX);
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 2;
	wf.nSamplesPerSec = deviceSamplingRate();
	wf.wBitsPerSample = 16;
	wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	hr = m_primaryBuffer->SetFormat(&wf);
	if (FAILED(hr)) {
		LN_LOG_ERROR << "SetFormat error. (" << hr << ")";
		return;
	}

	// create secondary buffer.
	ZeroMemory(&bufferDescription, sizeof(DSBUFFERDESC));
	bufferDescription.dwSize = sizeof(DSBUFFERDESC);
	bufferDescription.dwFlags =
		DSBCAPS_STICKYFOCUS |
		DSBCAPS_GLOBALFOCUS |
		DSBCAPS_GETCURRENTPOSITION2 |
		DSBCAPS_LOCHARDWARE;  // hardware mixing
	bufferDescription.dwBufferBytes = bufferSize;
	bufferDescription.lpwfxFormat = &wf;
	hr = m_dsound->CreateSoundBuffer(&bufferDescription, &m_secondaryBuffer, NULL);
	if (FAILED(hr)) {
		bufferDescription.dwFlags = (DSBCAPS_STICKYFOCUS |
			DSBCAPS_GLOBALFOCUS |
			DSBCAPS_GETCURRENTPOSITION2 |
			DSBCAPS_LOCSOFTWARE);  // Force software mixing
		hr = m_dsound->CreateSoundBuffer(&bufferDescription, &m_secondaryBuffer, NULL);
		if (FAILED(hr)) {
			LN_LOG_ERROR << "CreateSoundBuffer error. (" << hr << ")";
			return;
		}
	}

	// Get the buffer size
	DSBCAPS dsbcaps;
	dsbcaps.dwSize = sizeof(DSBCAPS);
	hr = m_secondaryBuffer->GetCaps(&dsbcaps);
	if (FAILED(hr)) {
		LN_LOG_ERROR << "GetCaps error. (" << hr << ")";
		return;
	}
	DWORD dsBufferSize = dsbcaps.dwBufferBytes;
	m_sctorSize = dsBufferSize / 2;

	// initial clear
	{
		LPVOID audioPtr;
		DWORD dataLen;
		hr = m_secondaryBuffer->Lock(0, dsBufferSize, &audioPtr, &dataLen, NULL, NULL, 0);
		if (FAILED(hr)) {
			LN_LOG_ERROR << "Lock error. (" << hr << ")";
			return;
		}

		ZeroMemory(audioPtr, dataLen);

		hr = m_secondaryBuffer->Unlock(audioPtr, dataLen, NULL, 0);
		if (FAILED(hr)) {
			LN_LOG_ERROR << "Unlock error. (" << hr << ")";
			return;
		}
	}
}

void DSoundAudioDevice::dispose()
{
	if (m_secondaryBuffer) {
		m_secondaryBuffer->Release();
		m_secondaryBuffer = nullptr;
	}

	if (m_primaryBuffer) {
		m_primaryBuffer->Release();
		m_primaryBuffer = nullptr;
	}

	if (m_dsound) {
		m_dsound->Release();
		m_dsound = nullptr;
	}
}

int DSoundAudioDevice::deviceSamplingRate()
{
	return 44100;
}

void DSoundAudioDevice::updateProcess()
{
	void* AP1 = 0, *AP2 = 0;
	DWORD AB1 = 0, AB2 = 0;
	DWORD point = 0;
	m_secondaryBuffer->GetCurrentPosition(&point, 0);
	if (m_nextSectorNumber == 0 && m_sctorSize <= point) {
		if (SUCCEEDED(m_secondaryBuffer->Lock(0, m_sctorSize, &AP1, &AB1, &AP2, &AB2, 0))) {
			render((int16_t*)AP1, AB1 / sizeof(int16_t));
			m_secondaryBuffer->Unlock(AP1, AB1, AP2, AB2);
			m_nextSectorNumber = 1;
		}
	}
	else if (m_nextSectorNumber == 1 && point < m_sctorSize) {
		if (SUCCEEDED(m_secondaryBuffer->Lock(m_sctorSize, m_sctorSize, &AP1, &AB1, &AP2, &AB2, 0))) {
			render((int16_t*)AP1, AB1 / sizeof(int16_t));
			m_secondaryBuffer->Unlock(AP1, AB1, AP2, AB2);
			m_nextSectorNumber = 0;
		}
	}

	DWORD status = 0;
	m_secondaryBuffer->GetStatus(&status);
	if (!(status & DSBSTATUS_PLAYING)) {
		m_secondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}
}

} // namespace detail
} // namespace ln

#endif
