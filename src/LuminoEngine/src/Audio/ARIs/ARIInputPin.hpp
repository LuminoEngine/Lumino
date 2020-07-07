#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class AudioNodeCore;
class CoreAudioOutputPin;

class CoreAudioInputPin
	: public RefObject
{
public:
	CoreAudioInputPin(int channels);
	virtual ~CoreAudioInputPin() = default;

	ARIAudioBus* bus() const;

	ARIAudioBus* pull();

	// TODO: internal
	void setOwnerNode(AudioNodeCore* node) { m_ownerNode = node; }
	void addLinkOutput(CoreAudioOutputPin* output);
	void removeLinkOutput(CoreAudioOutputPin* output);
	bool isConnected() const { return !m_connectedOutputPins.isEmpty(); }

	const List<Ref<CoreAudioOutputPin>>& connectedOutputPins() const { return m_connectedOutputPins; }
	void disconnect(int index);
	void disconnectAll();

private:

	AudioNodeCore* m_ownerNode;
	Ref<ARIAudioBus> m_summingBus;	// Total output
	List<Ref<CoreAudioOutputPin>> m_connectedOutputPins;
};

} // namespace detail
} // namespace ln

