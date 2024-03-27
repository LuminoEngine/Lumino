
#include "Internal.hpp"
#include <LuminoFramework/Audio/AudioContext.hpp>
#include <LuminoFramework/Audio/AudioGainNode.hpp>
#include "ARIs/ARIGainNode.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// AudioGainNode

AudioGainNode::AudioGainNode()
    //: m_gain(1.0f)
{
}

AudioGainNode::~AudioGainNode()
{
}

void AudioGainNode::init()
{
    // TODO: AudioNode::init() の後にしたい。
    // detail::EngineDomain::audioManager()->primaryContext() じゃなくて context() にしたい
    m_coreObject = makeRef<detail::ARIGainNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject(), this);
    m_coreObject->init();

	AudioNode::init();
}

void AudioGainNode::setGain(float value)
{
    m_coreObject->staging.gain = value;

    //detail::ScopedWriteLock lock(propertyMutex());
    //m_gain = value;
}

float AudioGainNode::gain() const
{
    return m_coreObject->staging.gain;
}

detail::ARINode* AudioGainNode::coreNode()
{
	return m_coreObject;
}

//void AudioGainNode::commit()
//{
//    AudioNode::commit();
//
//    detail::ScopedReadLock lock(propertyMutex());
//    m_coreObject->setGain(m_gain);
//}

} // namespace ln

