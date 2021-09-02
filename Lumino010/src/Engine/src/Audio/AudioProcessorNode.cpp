
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioProcessorNode.hpp>
#include "ARIs/ARIProcessorNode.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// AudioProcessorNode

AudioProcessorNode::AudioProcessorNode()
{
}

AudioProcessorNode::~AudioProcessorNode()
{
}

void AudioProcessorNode::init(int numberOfInputChannels, int numberOfOutputChannels)
{
    // TODO: AudioNode::init() の後にしたい。
    // detail::EngineDomain::audioManager()->primaryContext() じゃなくて context() にしたい
    m_coreObject = makeRef<detail::ARIProcessorNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject(), this);
    m_coreObject->init(numberOfInputChannels, numberOfOutputChannels);

	AudioNode::init();
}

detail::ARINode* AudioProcessorNode::coreNode()
{
	return m_coreObject;
}

//void AudioProcessorNode::commit()
//{
//    //AudioNode::commit();
//
//    //detail::ScopedReadLock lock(propertyMutex());
//    ////m_coreObject->setGain(m_gain);
//}

void AudioProcessorNode::onAudioProcess(AudioBus* input, AudioBus* output)
{
}

} // namespace ln

