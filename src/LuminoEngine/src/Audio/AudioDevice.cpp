
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

} // namespace detail
} // namespace ln

