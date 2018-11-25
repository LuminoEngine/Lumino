
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioGainNode.hpp>
#include "CAGainNode.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// AudioGainNode

AudioGainNode::AudioGainNode()
    : m_gain(1.0f)
{
}

AudioGainNode::~AudioGainNode()
{
}

void AudioGainNode::initialize()
{
    // TODO: AudioNode::initialize() の後にしたい。
    // detail::EngineDomain::audioManager()->primaryContext() じゃなくて context() にしたい
    m_coreObject = makeRef<detail::CAGainNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject());
    m_coreObject->initialize();

	AudioNode::initialize();
}

void AudioGainNode::setGain(float value)
{
    detail::ScopedWriteLock lock(propertyMutex());
    m_gain = value;
}

detail::CoreAudioNode* AudioGainNode::coreNode()
{
	return m_coreObject;
}

void AudioGainNode::commit()
{
    AudioNode::commit();

    detail::ScopedReadLock lock(propertyMutex());
    m_coreObject->setGain(m_gain);
}

} // namespace ln

