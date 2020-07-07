#pragma once
#include "Common.hpp"

namespace ln {
class AudioNode;

namespace detail {
class AudioNodeCore;
class CoreAudioInputPin;

class CoreAudioOutputPin
	: public RefObject
{
public:
	CoreAudioOutputPin(int channels);
	virtual ~CoreAudioOutputPin() = default;

	ARIAudioBus* bus() const;

	// process() から呼び出してはならない
	ARIAudioBus* pull();


	// TODO: internal
	void setOwnerNode(AudioNodeCore* node) { m_ownerNode = node; }
	void addLinkInput(CoreAudioInputPin* input);
	void removeLinkInput(CoreAudioInputPin* input);
	bool isConnected() const { return !m_connectedInputPins.isEmpty(); }

	const List<Ref<CoreAudioInputPin>>& connectedInputPins() const { return m_connectedInputPins; }
	void disconnect(int index);
	void disconnectAll();

private:
	AudioNodeCore* m_ownerNode;
	Ref<ARIAudioBus> m_resultBus;	// result of m_ownerNode->process()
	List<Ref<CoreAudioInputPin>> m_connectedInputPins;
};

} // namespace detail
} // namespace ln

