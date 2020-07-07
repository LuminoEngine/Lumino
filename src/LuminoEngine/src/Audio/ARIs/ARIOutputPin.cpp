
#include "Internal.hpp"
#include "ARINode.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"

namespace ln {
namespace detail {

//==============================================================================
// CoreAudioOutputPin

CoreAudioOutputPin::CoreAudioOutputPin(int channels)
	: m_ownerNode(nullptr)
{
	m_resultBus = makeRef<AudioBus>();
	m_resultBus->initialize2(channels, AudioNodeCore::ProcessingSizeInFrames);
}

AudioBus* CoreAudioOutputPin::bus() const
{
	return m_resultBus;
}

AudioBus* CoreAudioOutputPin::pull()
{
	m_ownerNode->processIfNeeded();
	return m_resultBus;
}

void CoreAudioOutputPin::addLinkInput(CoreAudioInputPin * input)
{
	m_connectedInputPins.add(input);
}

void CoreAudioOutputPin::removeLinkInput(CoreAudioInputPin * input)
{
	m_connectedInputPins.remove(input);
}

void CoreAudioOutputPin::disconnect(int index)
{
	auto& otherPin = m_connectedInputPins[index];
	otherPin->removeLinkOutput(this);
	m_connectedInputPins.removeAt(index);
}

void CoreAudioOutputPin::disconnectAll()
{
	for (auto& otherPin : m_connectedInputPins) {
		otherPin->removeLinkOutput(this);
	}
	m_connectedInputPins.clear();
}

} // namespace detail
} // namespace ln

