#pragma once
#include "AudioNode.hpp"

namespace ln {

class AudioDestinationNode
	: public AudioNode
{
LN_CONSTRUCT_ACCESS:
	AudioDestinationNode();
	virtual ~AudioDestinationNode() = default;
	void init(detail::ARIDestinationNode* core);
	virtual detail::ARINode* coreNode() override;

private:
	Ref<detail::ARIDestinationNode> m_coreObject;
};

} // namespace ln

