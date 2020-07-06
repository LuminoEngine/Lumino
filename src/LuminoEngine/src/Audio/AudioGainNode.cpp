﻿
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioGainNode.hpp>
#include "Core/CAGainNode.hpp"
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
    m_coreObject = makeRef<detail::CAGainNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject(), this);
    m_coreObject->init();
    m_coreObject->staging.gain = 1.0f;

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

detail::AudioNodeCore* AudioGainNode::coreNode()
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

