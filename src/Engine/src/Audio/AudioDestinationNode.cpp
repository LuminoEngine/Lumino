
#include "Internal.hpp"
#include "ARIs/ARIDestinationNode.hpp"
#include <LuminoEngine/Audio/AudioDestinationNode.hpp>

namespace ln {

//==============================================================================
// AudioDestinationNode

AudioDestinationNode::AudioDestinationNode()
{
}

void AudioDestinationNode::init(detail::ARIDestinationNode* core)
{
	AudioNode::init();
	m_coreObject = core;
}

detail::ARINode* AudioDestinationNode::coreNode()
{
	return m_coreObject;
}

} // namespace ln

