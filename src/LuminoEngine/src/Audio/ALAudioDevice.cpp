
#include "Internal.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include "ALAudioDevice.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ALAudioDevice

ALAudioDevice::ALAudioDevice()
{
}

void ALAudioDevice::initialize()
{
	m_alDevice = alcOpenDevice(nullptr);
	m_alContext = alcCreateContext(m_alDevice, nullptr);
	alcMakeContextCurrent(m_alContext);
	alGenSources(1, &m_masterSource);

	m_freeBuffers.resize(2);
	alGenBuffers(2, m_freeBuffers.data());

	m_sampleRate = 44100;	// TODO
}

void ALAudioDevice::dispose()
{
	if (m_alContext)
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_alContext);
		m_alContext = nullptr;
	}
	if (m_alDevice)
	{
		alcCloseDevice(m_alDevice);
		m_alDevice = nullptr;
	}
}

void ALAudioDevice::updateProcess()
{
	// remove the buffer that finished processing
	ALint processedCount;
	alGetSourcei(m_masterSource, AL_BUFFERS_PROCESSED, &processedCount);
	while (processedCount > 0)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(m_masterSource, 1, &buffer);
		m_freeBuffers.push_back(buffer);
		--processedCount;
	}

	// render data and set buffer to source
	if (m_freeBuffers.size() > 0)
	{
		alBufferData(m_freeBuffers.back(), AL_FORMAT_STEREO16, buf, samples_read*num_channels * 2, m_sampleRate);
		alSourceQueueBuffers(m_masterSource, 1, &m_freeBuffers.back());
		m_freeBuffers.pop_back();

		// play if needed
		ALint state;
		alGetSourcei(m_masterSource, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alSourcePlay(m_masterSource);
		}
	}
}

} // namespace detail
} // namespace ln

