
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioPannerNode.hpp>
#include "ARIS/CoreAudioNode.hpp"
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
	m_coreObject = makeRef<detail::CoreAudioPannerNode>(context()->coreObject(), this);
	m_coreObject->init();
}

detail::AudioNodeCore* AudioPannerNode::coreNode()
{
	return m_coreObject;
}

} // namespace ln

