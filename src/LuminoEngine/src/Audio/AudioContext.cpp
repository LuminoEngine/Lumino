
#include "Internal.hpp"
#include "AudioNode.hpp"
#include "AudioContext.hpp"
#include "ALAudioDevice.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AudioContext

AudioContext::AudioContext()
{
}

void AudioContext::initialize()
{
	auto device = makeRef<ALAudioDevice>();
	device->initialize();
	m_device = device;

	m_destinationNode = newObject<AudioDestinationNode>();
}

void AudioContext::dispose()
{
	if (m_device)
	{
		m_device->dispose();
		m_device.reset();
	}
}

void AudioContext::process()
{
	m_device->updateProcess();
}

} // namespace detail
} // namespace ln

