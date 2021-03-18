#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class ARIOutputPin
	: public RefObject
{
public:
	ARIOutputPin(int channels);
	virtual ~ARIOutputPin() = default;

	ARIAudioBus* bus() const;

	// process() から呼び出してはならない
	ARIAudioBus* pull();


	// TODO: internal
	void setOwnerNode(ARINode* node) { m_ownerNode = node; }
	void addLinkInput(ARIInputPin* input);
	void removeLinkInput(ARIInputPin* input);
	bool isConnected() const { return !m_connectedInputPins.isEmpty(); }

	const List<Ref<ARIInputPin>>& connectedInputPins() const { return m_connectedInputPins; }
	void disconnect(int index);
	void disconnectAll();

private:
	ARINode* m_ownerNode;
	Ref<ARIAudioBus> m_resultBus;	// result of m_ownerNode->process()
	List<Ref<ARIInputPin>> m_connectedInputPins;
};

} // namespace detail
} // namespace ln

