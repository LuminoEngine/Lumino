#pragma once
#include "AudioNode.hpp"

namespace ln {

class AudioDestinationNode
	: public AudioNode
{
public:	// TODO: internal
	virtual detail::ARINode* coreNode() override;

LN_CONSTRUCT_ACCESS:
	AudioDestinationNode();
	virtual ~AudioDestinationNode() = default;
	void init(detail::ARIDestinationNode* core);

private:
	Ref<detail::ARIDestinationNode> m_coreObject;
};

} // namespace ln

