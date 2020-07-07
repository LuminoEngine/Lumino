
#include "Internal.hpp"
#include "ARINode.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"

namespace ln {
namespace detail {

//==============================================================================
// CoreAudioInputPin

CoreAudioInputPin::CoreAudioInputPin(int channels)
	: m_ownerNode(nullptr)
{
	m_summingBus = makeRef<AudioBus>();
	m_summingBus->initialize2(channels, AudioNodeCore::ProcessingSizeInFrames);
}

AudioBus* CoreAudioInputPin::bus() const
{
	return m_summingBus;
}

AudioBus* CoreAudioInputPin::pull()
{
	m_summingBus->setSilentAndZero();

	for (auto& output : m_connectedOutputPins)
	{
		AudioBus* bus = output->pull();
		m_summingBus->sumFrom(bus);
	}

	return m_summingBus;
}

void CoreAudioInputPin::addLinkOutput(CoreAudioOutputPin * output)
{
	m_connectedOutputPins.add(output);
}

void CoreAudioInputPin::removeLinkOutput(CoreAudioOutputPin * output)
{
	m_connectedOutputPins.remove(output);
}

void CoreAudioInputPin::disconnect(int index)
{
	auto& otherPin = m_connectedOutputPins[index];
	otherPin->removeLinkInput(this);
	m_connectedOutputPins.removeAt(index);
}

void CoreAudioInputPin::disconnectAll()
{
	for (auto& otherPin : m_connectedOutputPins) {
		otherPin->removeLinkInput(this);
	}
	m_connectedOutputPins.clear();
}

} // namespace detail
} // namespace ln

