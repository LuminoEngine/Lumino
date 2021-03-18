#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class ARIInputPin
	: public RefObject
{
public:
	ARIInputPin(int channels);
	virtual ~ARIInputPin() = default;

	ARIAudioBus* bus() const;

	ARIAudioBus* pull();

	// TODO: internal
	void setOwnerNode(ARINode* node) { m_ownerNode = node; }
	void addLinkOutput(ARIOutputPin* output);
	void removeLinkOutput(ARIOutputPin* output);
	bool isConnected() const { return !m_connectedOutputPins.isEmpty(); }

	const List<Ref<ARIOutputPin>>& connectedOutputPins() const { return m_connectedOutputPins; }
	void disconnect(int index);
	void disconnectAll();

private:
	ARINode* m_ownerNode;
	Ref<ARIAudioBus> m_summingBus;	// Total output
	List<Ref<ARIOutputPin>> m_connectedOutputPins;
};

} // namespace detail
} // namespace ln

