
#include "Internal.hpp"
#include "ARIAudioBus.hpp"
#include "ARINode.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ARIOutputPin

ARIOutputPin::ARIOutputPin(int channels)
	: m_ownerNode(nullptr)
{
	m_resultBus = makeRef<ARIAudioBus>();
	m_resultBus->initialize2(channels, ARINode::ProcessingSizeInFrames);
}

ARIAudioBus* ARIOutputPin::bus() const
{
	return m_resultBus;
}

ARIAudioBus* ARIOutputPin::pull()
{
	m_ownerNode->processIfNeeded();
	return m_resultBus;
}

void ARIOutputPin::addLinkInput(ARIInputPin * input)
{
	m_connectedInputPins.add(input);
}

void ARIOutputPin::removeLinkInput(ARIInputPin * input)
{
	m_connectedInputPins.remove(input);
}

void ARIOutputPin::disconnect(int index)
{
	auto& otherPin = m_connectedInputPins[index];
	otherPin->removeLinkOutput(this);
	m_connectedInputPins.removeAt(index);
}

void ARIOutputPin::disconnectAll()
{
	for (auto& otherPin : m_connectedInputPins) {
		otherPin->removeLinkOutput(this);
	}
	m_connectedInputPins.clear();
}

} // namespace detail
} // namespace ln

