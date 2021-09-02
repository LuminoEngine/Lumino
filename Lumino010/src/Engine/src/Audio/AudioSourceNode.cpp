
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioSourceNode.hpp>
#include "Decoder/AudioDecoder.hpp"
#include "ARIs/ARISourceNode.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// AudioSourceNode

AudioSourceNode::AudioSourceNode()
{
}

void AudioSourceNode::init(detail::AudioDecoder* decoder)
{
	m_coreObject = makeRef<detail::ARISourceNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject(), this);
	m_coreObject->init(decoder);


	AudioNode::init();
}

void AudioSourceNode::setPlaybackRate(float rate)
{
    detail::ScopedWriteLock lock(propertyMutex());
	m_coreObject->staging.playbackRate = rate;
	//LN_ENQUEUE_RENDER_COMMAND_2(
	//	start, context()->manager(),
	//	Ref<detail::ARISourceNode>, m_coreObject,
	//	float, rate,
	//	{
	//		m_coreObject->setPlaybackRate(rate);
	//	});
}

void AudioSourceNode::setLoop(bool value)
{
	m_coreObject->staging.loop = value;
}

bool AudioSourceNode::loop() const
{
    return m_coreObject->staging.loop;
}

void AudioSourceNode::start()
{
	m_coreObject->staging.resetRequire = true;
	m_coreObject->staging.requestedState = PlayingState::Play;
	//LN_ENQUEUE_RENDER_COMMAND_1(
	//	start, context()->manager(),
	//	Ref<detail::ARISourceNode>, m_coreObject,
	//	{
	//		m_coreObject->start();
	//	});
}

void AudioSourceNode::stop()
{
	m_coreObject->staging.resetRequire = true;
	m_coreObject->staging.requestedState = PlayingState::Stop;
	//LN_ENQUEUE_RENDER_COMMAND_1(
	//	start, context()->manager(),
	//	Ref<detail::ARISourceNode>, m_coreObject,
	//	{
	//		m_coreObject->stop();
	//	});
}

void AudioSourceNode::pause()
{
	LN_NOTIMPLEMENTED();
}

void AudioSourceNode::resume()
{
	LN_NOTIMPLEMENTED();
}

detail::ARINode * AudioSourceNode::coreNode()
{
	return m_coreObject;
}

PlayingState AudioSourceNode::playingState() const
{
    switch (m_coreObject->playingState())
    {
    //case detail::ARISourceNode::PlayingState::None:
    case detail::ARISourceNode::State::Stopped:
        return PlayingState::Stop;
    case detail::ARISourceNode::State::Playing:
        return PlayingState::Play;
    case detail::ARISourceNode::State::Pausing:
        return PlayingState::Pause;
    default:
        LN_UNREACHABLE();
        return PlayingState::Stop;
    }
}

//void AudioSourceNode::commit()
//{
//	AudioNode::commit();
//
//    detail::ScopedReadLock lock(propertyMutex());
//
//	if (m_commitState.resetRequire) {
//		m_coreObject->reset();
//		m_commitState.resetRequire = false;
//	}
//
//	switch (m_commitState.requestedState)
//	{
//	case PlayingState::NoChanged:
//		break;
//	case  PlayingState::Stop:
//		m_coreObject->stop();
//		break;
//	case  PlayingState::Play:
//		m_coreObject->reset();
//		m_coreObject->start();
//		break;
//	case  PlayingState::Pause:
//		LN_NOTIMPLEMENTED();
//		break;
//	default:
//		break;
//	}
//	m_commitState.requestedState = PlayingState::NoChanged;
//
//	m_coreObject->setPlaybackRate(m_commitState.playbackRate);
//    m_coreObject->setLoop(m_commitState.loop);
//}

} // namespace ln

