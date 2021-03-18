
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioPannerNode.hpp>
#include "ARIs/ARIPannerNode.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// AudioPannerNode

AudioPannerNode::AudioPannerNode()
{
}

void AudioPannerNode::init()
{
	AudioNode::init();
	m_coreObject = makeRef<detail::ARIPannerNode>(context()->coreObject(), this);
	m_coreObject->init();
}

detail::ARINode* AudioPannerNode::coreNode()
{
	return m_coreObject;
}

} // namespace ln

