
#include "Internal.hpp"
#include "AudioNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AudioChannel

AudioChannel::AudioChannel()
{
}

void AudioChannel::initialize(size_t length)
{
	Object::initialize();
}

//==============================================================================
// AudioBus

AudioBus::AudioBus()
{
}

void AudioBus::initialize(int channelCount, size_t length)
{
	Object::initialize();
	for (int i = 0; i < channelCount; ++i)
	{
		m_channels.add(newObject<AudioChannel>(length));
	}
}

//==============================================================================
// AudioOutputPin

AudioInputPin::AudioInputPin()
{
}

void AudioInputPin::initialize()
{
	Object::initialize();
}

//==============================================================================
// AudioOutputPin

AudioOutputPin::AudioOutputPin()
{
}

void AudioOutputPin::initialize()
{
	Object::initialize();
}

//==============================================================================
// AudioNode

AudioNode::AudioNode()
{
}

AudioInputPin * AudioNode::inputPin(int index) const
{
	return m_inputPins[index];
}

AudioOutputPin * AudioNode::outputPin(int index) const
{
	return m_outputPins[index];
}

void AudioNode::addInputPin()
{
	m_inputPins.add(newObject<AudioInputPin>());
}

void AudioNode::addOutputPin()
{
	m_outputPins.add(newObject<AudioOutputPin>());
}

//==============================================================================
// AudioDestinationNode

AudioDestinationNode::AudioDestinationNode()
{
}

void AudioDestinationNode::initialize()
{
	AudioNode::initialize();
	addInputPin();
}

} // namespace detail
} // namespace ln

