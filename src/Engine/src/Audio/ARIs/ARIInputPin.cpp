
#include "Internal.hpp"
#include "ARIAudioBus.hpp"
#include "ARINode.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ARIInputPin

ARIInputPin::ARIInputPin(int channels)
	: m_ownerNode(nullptr)
{
	m_summingBus = makeRef<ARIAudioBus>();
	m_summingBus->initialize2(channels, ARINode::ProcessingSizeInFrames);
}

ARIAudioBus* ARIInputPin::bus() const
{
	return m_summingBus;
}

ARIAudioBus* ARIInputPin::pull()
{
	m_summingBus->setSilentAndZero();

	for (auto& output : m_connectedOutputPins)
	{
		ARIAudioBus* bus = output->pull();
		m_summingBus->sumFrom(bus);
	}

	return m_summingBus;
}

void ARIInputPin::addLinkOutput(ARIOutputPin * output)
{
	m_connectedOutputPins.add(output);
}

void ARIInputPin::removeLinkOutput(ARIOutputPin * output)
{
	m_connectedOutputPins.remove(output);
}

void ARIInputPin::disconnect(int index)
{
	auto& otherPin = m_connectedOutputPins[index];
	otherPin->removeLinkInput(this);
	m_connectedOutputPins.removeAt(index);
}

void ARIInputPin::disconnectAll()
{
	for (auto& otherPin : m_connectedOutputPins) {
		otherPin->removeLinkInput(this);
	}
	m_connectedOutputPins.clear();
}

} // namespace detail
} // namespace ln

