
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioNode.hpp>
#include "AudioManager.hpp"
#include "CoreAudioNode.hpp"
#include "../Engine/RenderingCommandList.hpp"

namespace ln {

//==============================================================================
// AudioNode

AudioNode::AudioNode()
	//: m_inputConnectionsDirty(false)
	//, m_outputConnectionsDirty(false)
{
}

void AudioNode::init()
{
	Object::init();
	m_context =	detail::EngineDomain::audioManager()->primaryContext();
	m_context->addAudioNode(this);
}

void AudioNode::onDispose(bool explicitDisposing)
{
    // TODO: dispose は finalize からも呼ばれる。この時は this の参照カウントが 0 で、
    // dispose 終了後にデストラクタが呼ばれる。そのため、↓のようにして別の Ref に参照を持たせても
    // オブジェクトはデストラクトされてしまう。
    // 修正方針のひとつとして、dispose を参照カウント 0 の状態で呼び出さないようにするのもありかもしれない。・・・でも atomic になるように注意して調査すること。
	if (m_context) {
        LN_CHECK(RefObjectHelper::getReferenceCount(this) > 0);
		m_context->disposeNodeOnGenericThread(this);
	}

	Object::onDispose(explicitDisposing);
}

void AudioNode::commit()
{

	//if (m_inputConnectionsDirty)
	//{
	//	coreNode()->disconnectAllInputSide();
	//	for (auto& node : m_inputConnections) {
	//		detail::CoreAudioNode::connect(node->coreNode(), coreNode());
	//	}
	//	m_inputConnectionsDirty = false;
	//}

	//if (m_outputConnectionsDirty)
	//{
	//	coreNode()->disconnectAllOutputSide();
	//	for (auto& node : m_outputConnections) {
	//		detail::CoreAudioNode::connect(coreNode(), node->coreNode());
	//	}
	//	m_outputConnectionsDirty = false;
	//}
}

detail::AudioRWMutex& AudioNode::commitMutex()
{
	return context()->commitMutex();
}

//void AudioNode::addConnectionInput(AudioNode * inputSide)
//{
//	if (LN_REQUIRE(inputSide)) return;
//	if (LN_REQUIRE(context() == inputSide->context())) return;
//
//	LN_AUDIO_WRITE_LOCK_COMMIT;
//	m_connectionCommands.push_back({ OperationCode::ConnectionInput, inputSide});
//}
//
//void AudioNode::addConnectionOutput(AudioNode * outputSide)
//{
//	if (LN_REQUIRE(outputSide)) return;
//	if (LN_REQUIRE(context() == outputSide->context())) return;
//
//	LN_AUDIO_WRITE_LOCK_COMMIT;
//	m_connectionCommands.push_back({ OperationCode::ConnectionOutput, outputSide });
//}
//
//void AudioNode::removeConnectionInput(AudioNode* inputSide)
//{
//	if (LN_REQUIRE(inputSide)) return;
//	if (LN_REQUIRE(context() == inputSide->context())) return;
//
//	LN_AUDIO_WRITE_LOCK_COMMIT;
//	m_connectionCommands.push_back({ OperationCode::DisconnectionInput, inputSide });
//}
//
//void AudioNode::removeConnectionOutput(AudioNode* outputSide)
//{
//	if (LN_REQUIRE(outputSide)) return;
//	if (LN_REQUIRE(context() == outputSide->context())) return;
//
//	LN_AUDIO_WRITE_LOCK_COMMIT;
//	m_connectionCommands.push_back({ OperationCode::DisconnectionOutput, outputSide });
//}

void AudioNode::connect(AudioNode * outputSide, AudioNode * inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	outputSide->context()->sendConnect(outputSide, inputSide);
	//outputSide->addConnectionOutput(inputSide);
	//inputSide->addConnectionInput(outputSide);
}

void AudioNode::disconnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	outputSide->context()->sendDisconnect(outputSide, inputSide);
	//outputSide->removeConnectionOutput(inputSide);
	//inputSide->removeConnectionInput(outputSide);
}

void AudioNode::disconnect()
{
	LN_NOTIMPLEMENTED();
	//for (auto& node : m_inputConnections) {
	//	node->removeConnectionOutput(this);
	//}
	//for (auto& node : m_outputConnections) {
	//	node->removeConnectionInput(this);
	//}
	//m_inputConnectionsDirty = false;
	//m_outputConnectionsDirty = false;
}

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
	//	Ref<detail::CoreAudioSourceNode>, m_coreObject,
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
	//	Ref<detail::CoreAudioSourceNode>, m_coreObject,
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
	//	Ref<detail::CoreAudioSourceNode>, m_coreObject,
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

void AudioSourceNode::init(const StringRef & filePath)
{
    Ref<detail::AudioDecoder> decoder = detail::EngineDomain::audioManager()->createAudioDecoder(filePath);
	m_coreObject = makeRef<detail::CoreAudioSourceNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject());
	m_coreObject->init(decoder);

    AudioNode::init();
}

detail::CoreAudioNode * AudioSourceNode::coreNode()
{
	return m_coreObject;
}

AudioSourceNode::PlayingState AudioSourceNode::playingState() const
{
    switch (m_coreObject->playingState())
    {
    case detail::CoreAudioSourceNode::PlayingState::None:
    case detail::CoreAudioSourceNode::PlayingState::Stopped:
        return PlayingState::Stop;
    case detail::CoreAudioSourceNode::PlayingState::Playing:
        return PlayingState::Play;
    case detail::CoreAudioSourceNode::PlayingState::Pausing:
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

//==============================================================================
// AudioPannerNode

AudioPannerNode::AudioPannerNode()
{
}

void AudioPannerNode::init()
{
	AudioNode::init();
	m_coreObject = makeRef<detail::CoreAudioPannerNode>(context()->coreObject());
	m_coreObject->init();
}

detail::CoreAudioNode* AudioPannerNode::coreNode()
{
	return m_coreObject;
}

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

detail::CoreAudioNode * AudioDestinationNode::coreNode()
{
	return m_coreObject;
}

} // namespace ln

