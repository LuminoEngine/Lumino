
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioSourceNode.hpp>
#include "Decoder/AudioDecoder.hpp"
#include "Core/AudioSourceNodeCore.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// AudioSourceNode

AudioSourceNode::AudioSourceNode()
{
	m_commitState.playbackRate = 1.0f;
	m_commitState.requestedState = PlayingState::NoChanged;
	m_commitState.resetRequire = false;
}

void AudioSourceNode::setPlaybackRate(float rate)
{
    detail::ScopedWriteLock lock(propertyMutex());
	m_commitState.playbackRate = rate;
	//LN_ENQUEUE_RENDER_COMMAND_2(
	//	start, context()->manager(),
	//	Ref<detail::AudioSourceNodeCore>, m_coreObject,
	//	float, rate,
	//	{
	//		m_coreObject->setPlaybackRate(rate);
	//	});
}

void AudioSourceNode::setLoop(bool value)
{
    m_commitState.loop = value;
}

bool AudioSourceNode::loop() const
{
    return m_commitState.loop;
}

void AudioSourceNode::start()
{
	m_commitState.resetRequire = true;
	m_commitState.requestedState = PlayingState::Play;
	//LN_ENQUEUE_RENDER_COMMAND_1(
	//	start, context()->manager(),
	//	Ref<detail::AudioSourceNodeCore>, m_coreObject,
	//	{
	//		m_coreObject->start();
	//	});
}

void AudioSourceNode::stop()
{
	m_commitState.resetRequire = true;
	m_commitState.requestedState = PlayingState::Stop;
	//LN_ENQUEUE_RENDER_COMMAND_1(
	//	start, context()->manager(),
	//	Ref<detail::AudioSourceNodeCore>, m_coreObject,
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

void AudioSourceNode::init(detail::AudioDecoder* decoder)
{
	m_coreObject = makeRef<detail::AudioSourceNodeCore>(detail::EngineDomain::audioManager()->primaryContext()->coreObject());
	m_coreObject->init(decoder);

    AudioNode::init();
}

detail::AudioNodeCore * AudioSourceNode::coreNode()
{
	return m_coreObject;
}

AudioSourceNode::PlayingState AudioSourceNode::playingState() const
{
    switch (m_coreObject->playingState())
    {
    case detail::AudioSourceNodeCore::PlayingState::None:
    case detail::AudioSourceNodeCore::PlayingState::Stopped:
        return PlayingState::Stop;
    case detail::AudioSourceNodeCore::PlayingState::Playing:
        return PlayingState::Play;
    case detail::AudioSourceNodeCore::PlayingState::Pausing:
        return PlayingState::Pause;
    default:
        LN_UNREACHABLE();
        return PlayingState::Stop;
    }
}

void AudioSourceNode::commit()
{
	AudioNode::commit();

    detail::ScopedReadLock lock(propertyMutex());

	if (m_commitState.resetRequire) {
		m_coreObject->reset();
		m_commitState.resetRequire = false;
	}

	switch (m_commitState.requestedState)
	{
	case PlayingState::NoChanged:
		break;
	case  PlayingState::Stop:
		m_coreObject->stop();
		break;
	case  PlayingState::Play:
		m_coreObject->start();
		break;
	case  PlayingState::Pause:
		LN_NOTIMPLEMENTED();
		break;
	default:
		break;
	}
	m_commitState.requestedState = PlayingState::NoChanged;

	m_coreObject->setPlaybackRate(m_commitState.playbackRate);
    m_coreObject->setLoop(m_commitState.loop);
}

} // namespace ln

