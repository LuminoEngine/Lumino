#pragma once
#include "AudioNode.hpp"

namespace ln {

class AudioDestinationNode
	: public AudioNode
{
LN_CONSTRUCT_ACCESS:
	AudioDestinationNode();
	virtual ~AudioDestinationNode() = default;
	void init(detail::CoreAudioDestinationNode* core);
	virtual detail::AudioNodeCore* coreNode() override;

private:
	Ref<detail::CoreAudioDestinationNode> m_coreObject;
};

} // namespace ln

