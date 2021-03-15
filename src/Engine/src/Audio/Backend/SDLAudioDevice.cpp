
#ifdef LN_USE_SDL
#include "Internal.hpp"
#include "SDLAudioDevice.hpp"
#include "../ARIs/ARINode.hpp"

//#pragma comment(lib, "D:/Tech/Graphics/SDL2-2.0.8/lib/x86/SDL2.lib")

namespace ln {
namespace detail {

//==============================================================================
// SDLAudioDevice

SDLAudioDevice::SDLAudioDevice()
{
}

SDL_AudioDeviceID m_deviceId;

void SDLAudioDevice::init()
{
	m_masterSampleRate = deviceSamplingRate();//44100;	// TODO



	SDL_Init(SDL_INIT_AUDIO/* | SDL_INIT_EVENTS*/);


	SDL_AudioSpec Desired;
	SDL_AudioSpec Obtained;
	Desired.freq = deviceSamplingRate();
	Desired.format = AUDIO_S16LSB; /* 16-bit signed audio */
	Desired.channels = 2;
	Desired.samples = ARINode::ProcessingSizeInFrames/* / 2*/;//1024; /* Buffer size: 8K = 0.37 sec. */
	Desired.callback = SDLAudioCallbackStatic;//nullptr;// 
	Desired.userdata = this;
	//SDL_OpenAudio(&Desired, &Obtained);

	m_deviceId = SDL_OpenAudioDevice(NULL, SDL_FALSE, &Desired, &Obtained, 0);

	//SDL_PauseAudio(0);
	//SDL_PauseAudioDevice(m_deviceId, SDL_FALSE);

}

void SDLAudioDevice::dispose()
{
	SDL_CloseAudio();
}

int SDLAudioDevice::deviceSamplingRate()
{
	return 48000;
}

void SDLAudioDevice::updateProcess()
{
#if 0
	const Uint32 queued = SDL_GetQueuedAudioSize(1);
	if (queued <= m_finalRenderdBuffer.size()) {  /* time to requeue the whole thing? */

		render(m_renderdBuffer.data(), m_renderdBuffer.size());
		AudioDecoder::convertFromFloat32(m_finalRenderdBuffer.data(), m_renderdBuffer.data(), m_renderdBuffer.size(), PCMFormat::S16L);

		if (SDL_QueueAudio(m_deviceId, m_finalRenderdBuffer.data(), m_finalRenderdBuffer.size() * sizeof(uint16_t)) == 0) {
			printf("ff\n");
		}
		else {
			printf("dd\n");
		}
	}
#endif
}

void SDLAudioDevice::run()
{
	SDL_PauseAudioDevice(m_deviceId, SDL_FALSE);
	//SDL_PauseAudio(0);
}

void SDLAudioDevice::SDLAudioCallback(Uint8 * stream, int len)
{
	render((int16_t*)stream, len / sizeof(int16_t));
	//render(m_renderdBuffer.data(), m_renderdBuffer.size());
	//AudioDecoder::convertFromFloat32(stream, m_renderdBuffer.data(), len / sizeof(int16_t), PCMFormat::S16L);
	//AudioDecoder::convertFromFloat32(m_finalRenderdBuffer.data(), m_renderdBuffer.data(), m_finalRenderdBuffer.size(), PCMFormat::S16L);
}

void SDLCALL SDLAudioDevice::SDLAudioCallbackStatic(void *userdata, Uint8 * stream, int len)
{
	reinterpret_cast<SDLAudioDevice*>(userdata)->SDLAudioCallback(stream, len);
}

} // namespace detail
} // namespace ln

#endif
