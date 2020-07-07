
#include "Internal.hpp"
#include "ARIAudioBus.hpp"
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
	m_resultBus = makeRef<ARIAudioBus>();
	m_resultBus->initialize2(channels, AudioNodeCore::ProcessingSizeInFrames);
}

ARIAudioBus* CoreAudioOutputPin::bus() const
{
	return m_resultBus;
}

ARIAudioBus* CoreAudioOutputPin::pull()
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

