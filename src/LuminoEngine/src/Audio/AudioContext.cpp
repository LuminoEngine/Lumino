
#include "Internal.hpp"
#include "AudioNode.hpp"
#include "AudioContext.hpp"
#include "ALAudioDevice.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AudioContext

	Ref<AudioSourceNode> ssss;

AudioContext::AudioContext()
{
}

void AudioContext::initialize()
{
	auto device = makeRef<ALAudioDevice>();
	device->initialize();
	m_device = device;

	m_destinationNode = newObject<AudioDestinationNode>();

	m_device->setRenderCallback(m_destinationNode);


	// TODO: test
	ssss = newObject<AudioSourceNode>(u"D:\\tmp\\8_MapBGM2.wav");
	AudioNode::connect(ssss, m_destinationNode);
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

