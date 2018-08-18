
#include "Internal.hpp"
#include "AudioDevice.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AudioDevice

AudioDevice::AudioDevice()
	: m_renderCallback(nullptr)
{
}

void AudioDevice::dispose()
{
}

void AudioDevice::render(float * outputBuffer, int length)
{
	m_renderCallback->render(outputBuffer, length);
}

} // namespace detail
} // namespace ln

