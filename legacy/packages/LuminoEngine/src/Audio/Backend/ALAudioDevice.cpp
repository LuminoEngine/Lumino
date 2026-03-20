
#ifdef LN_USE_OPENAL
#include "Internal.hpp"
#include "ALAudioDevice.hpp"
#include "../ARIs/ARINode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ALAudioDevice

ALAudioDevice::ALAudioDevice()
	: m_alDevice(nullptr)
	, m_alContext(nullptr)
	, m_masterSource(0)
	, m_freeBuffers()
	, m_finalRenderdBuffer()
{
}

void ALAudioDevice::init(int frameLength)
{
	AudioDevice::init(frameLength, 2);
	m_finalRenderdBuffer.resize(frameLength * 2);

	m_alDevice = alcOpenDevice(nullptr);
	m_alContext = alcCreateContext(m_alDevice, nullptr);
	alcMakeContextCurrent(m_alContext);

	alGenSources(1, &m_masterSource);

	m_freeBuffers.resize(2);
	alGenBuffers(2, m_freeBuffers.data());
}

void ALAudioDevice::dispose()
{
	if (m_alContext)
	{
		alDeleteBuffers(m_freeBuffers.size(), m_freeBuffers.data());
	}
	if (m_masterSource)
	{
		alDeleteSources(1, &m_masterSource);
		m_masterSource = 0;
	}
	if (m_alContext)
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_alContext);
		m_alContext = nullptr;
		// TODO: OpenAL-soft で Context 解放がちゃんと動いていないようだ。確認しておく。55 byte メモリリークしている。
	}
	if (m_alDevice)
	{
		alcCloseDevice(m_alDevice);
		m_alDevice = nullptr;
	}
}

int ALAudioDevice::deviceSamplingRate()
{
	return 44100;
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
		render(m_finalRenderdBuffer.data(), m_finalRenderdBuffer.size());
		
		alBufferData(m_freeBuffers.back(), AL_FORMAT_STEREO16, m_finalRenderdBuffer.data(), sizeof(int16_t) * m_finalRenderdBuffer.size(), deviceSamplingRate());

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

#endif
