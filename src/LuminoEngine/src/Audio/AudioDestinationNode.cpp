
#include "Internal.hpp"
#include "Core/CoreAudioNode.hpp"
#include <LuminoEngine/Audio/AudioDestinationNode.hpp>

namespace ln {

//==============================================================================
// AudioDestinationNode

AudioDestinationNode::AudioDestinationNode()
{
}

void AudioDestinationNode::init(detail::CoreAudioDestinationNode* core)
{
	AudioNode::init();
	m_coreObject = core;
}

detail::AudioNodeCore* AudioDestinationNode::coreNode()
{
	return m_coreObject;
}

} // namespace ln

