
#include "Internal.hpp"
#include "AudioDevice.hpp"
#include "../Decoder/AudioDecoder.hpp"

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

void AudioDevice::init(int frames, int channels)
{
	//ARINode::ProcessingSizeInFrames
	m_masterChannels = channels;
	m_renderdBuffer.resize(frames * m_masterChannels);

    m_failoutMilliseconds = (static_cast<double>(frames) / deviceSamplingRate()) * 1000;
}

void AudioDevice::dispose()
{
	m_renderCallback = nullptr;
}

void AudioDevice::render(int16_t* outputBuffer, int length)
{
//    static float step = 0.0;
//    static float frequency = 440;
//
//    for (int i = 0; i < length / 2; i++) {
//        float v = std::sin(2.0 * Math::PI * step);
//        outputBuffer[(i * 2) + 0] = outputBuffer[(i * 2) + 1] = static_cast<int16_t>(v * 32767.f);
//        step += frequency / deviceSamplingRate();//4096;
//        //printf("%d\n", (i * 2) + 1);
//    }
////memset(outputBuffer, 0, length * sizeof(uint16_t));
//    return;




    //
    //m_failoutTimer.start();
	m_renderCallback->render(m_renderdBuffer.data(), m_renderdBuffer.size());
	AudioDecoder::convertFromFloat32(outputBuffer, m_renderdBuffer.data(), length, PCMFormat::S16L);

    //std::cout << m_failoutTimer.elapsedMilliseconds() << " / " << m_failoutMilliseconds << std::endl;
    
    //int sum = 0;
    //for (int i = 0; i < length; i++) {
    //    sum += outputBuffer[i];
    //}
    //std::cout << length << " " << sum << std::endl;
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

