#pragma once

#ifdef LN_USE_SDL
#ifdef __EMSCRIPTEN__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "AudioDevice.hpp"

namespace ln {
namespace detail {

class SDLAudioDevice
	: public AudioDevice
{
public:
	SDLAudioDevice();
	virtual ~SDLAudioDevice() = default;
	void init();
	virtual void dispose() override;
	virtual int deviceSamplingRate() override;
	virtual void updateProcess() override;
	virtual void run() override;

	void SDLAudioCallback(Uint8 * stream, int len);
	static void SDLCALL SDLAudioCallbackStatic(void *userdata, Uint8 * stream, int len);

private:
	int m_masterSampleRate;
	int m_masterChannels;
	std::vector<float> m_renderdBuffer;
	std::vector<int16_t> m_finalRenderdBuffer;
};

} // namespace detail
} // namespace ln
#endif

