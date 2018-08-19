
#include "Internal.hpp"
#include "ALAudioDevice.hpp"
#include "AudioNode.hpp"

#include "AudioDecoder.hpp"	// TODO: test
#include <Lumino/Engine/Diagnostics.hpp>	// TODO: test

namespace ln {
namespace detail {

	//WaveDecoder wd;// TODO: test
	//std::vector<float> tmpBuffer;

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

	m_masterSampleRate = 44100;	// TODO
	m_masterChannels = 2;

	m_renderdBuffer.resize(AudioNode::ProcessingSizeInFrames * m_masterChannels);
	m_finalRenderdBuffer.resize(AudioNode::ProcessingSizeInFrames * m_masterChannels);
	//m_finalRenderdBuffer.resize(m_masterSampleRate * m_masterChannels);

	// TODO: test
	//auto diag = newObject<DiagnosticsManager>();
	//wd.initialize(FileStream::create(u"D:\\tmp\\8_MapBGM2.wav"), diag);
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
		printf("processed\n");
	}

	// render data and set buffer to source
	if (m_freeBuffers.size() > 0)
	{
		render(m_renderdBuffer.data(), m_renderdBuffer.size());
		AudioDecoder::convertFromFloat32(m_finalRenderdBuffer.data(), m_renderdBuffer.data(), m_finalRenderdBuffer.size(), PCMFormat::S16L);

		alBufferData(m_freeBuffers.back(), AL_FORMAT_STEREO16, m_finalRenderdBuffer.data(), sizeof(int16_t) * m_finalRenderdBuffer.size(), m_masterSampleRate);
		alSourceQueueBuffers(m_masterSource, 1, &m_freeBuffers.back());
		m_freeBuffers.pop_back();

		// play if needed
		ALint state;
		alGetSourcei(m_masterSource, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alSourcePlay(m_masterSource);
		}
		printf("queue\n");
	}
}

} // namespace detail
} // namespace ln

