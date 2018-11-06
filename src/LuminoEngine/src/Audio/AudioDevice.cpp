﻿
#include "Internal.hpp"
#include "AudioDevice.hpp"
#include "AudioDecoder.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AudioDevice

AudioDevice::AudioDevice()
	: m_renderCallback(nullptr)
	, m_listener()
	, m_masterChannels(0)
	, m_renderdBuffer()
{
}

void AudioDevice::initialize(int frames, int channels)
{
	//CoreAudioNode::ProcessingSizeInFrames
	m_masterChannels = channels;
	m_renderdBuffer.resize(frames * m_masterChannels);
}

void AudioDevice::dispose()
{
	m_renderCallback = nullptr;
}

void AudioDevice::render(int16_t* outputBuffer, int length)
{
	m_renderCallback->render(m_renderdBuffer.data(), m_renderdBuffer.size());
	AudioDecoder::convertFromFloat32(outputBuffer, m_renderdBuffer.data(), length, PCMFormat::S16L);

    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += outputBuffer[i];
    }
    std::cout << length << " " << sum << std::endl;
}

//==============================================================================
// NullAudioDevice

NullAudioDevice::NullAudioDevice()
{
}

int NullAudioDevice::deviceSamplingRate()
{
	return 48000;
}

void NullAudioDevice::updateProcess()
{
}

} // namespace detail
} // namespace ln

