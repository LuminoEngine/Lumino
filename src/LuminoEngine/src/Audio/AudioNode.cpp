
#include "Internal.hpp"
#include <Lumino/Audio/AudioContext.hpp>
#include <Lumino/Audio/AudioNode.hpp>
#include "AudioManager.hpp"
#include "CoreAudioNode.hpp"
#include "../Engine/RenderingCommandList.hpp"

namespace ln {

//==============================================================================
// AudioNode

AudioNode::AudioNode()
	: m_inputConnectionsDirty(false)
	, m_outputConnectionsDirty(false)
{
}

void AudioNode::initialize()
{
	Object::initialize();
	m_context =	detail::EngineDomain::audioManager()->primaryContext();
	m_context->addAudioNode(this);
}

void AudioNode::dispose()
{
	Object::dispose();
	if (m_context) {
		disconnect();
		m_context->removeAudioNode(this);
		m_context = nullptr;
	}
}

void AudioNode::commit()
{
	if (m_inputConnectionsDirty)
	{
		coreNode()->disconnectAllInputSide();
		for (auto& node : m_inputConnections) {
			detail::CoreAudioNode::connect(node->coreNode(), coreNode());
		}
		m_inputConnectionsDirty = false;
	}

	if (m_outputConnectionsDirty)
	{
		coreNode()->disconnectAllOutputSide();
		for (auto& node : m_outputConnections) {
			detail::CoreAudioNode::connect(coreNode(), node->coreNode());
		}
		m_outputConnectionsDirty = false;
	}
}

#if LN_AUDIO_THREAD_ENABLED
std::shared_mutex& AudioNode::commitMutex()
{
	return context()->commitMutex;
}
#endif

void AudioNode::addConnectionInput(AudioNode * inputSide)
{
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(context() == inputSide->context())) return;
	if (!m_inputConnections.contains(inputSide))
	{
		LN_AUDIO_WRITE_LOCK_COMMIT;
		m_inputConnections.add(inputSide);
		m_inputConnectionsDirty = true;
	}
}

void AudioNode::addConnectionOutput(AudioNode * outputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(context() == outputSide->context())) return;
	if (!m_outputConnections.contains(outputSide))
	{
		LN_AUDIO_WRITE_LOCK_COMMIT;
		m_outputConnections.add(outputSide);
		m_outputConnectionsDirty = true;
	}
}

void AudioNode::removeConnectionInput(AudioNode* inputSide)
{
	if (LN_REQUIRE(inputSide)) return;
	if (m_inputConnections.remove(inputSide))
	{
		m_inputConnectionsDirty = true;
	}
}

void AudioNode::removeConnectionOutput(AudioNode* outputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (m_outputConnections.remove(outputSide))
	{
		m_outputConnectionsDirty = true;
	}
}

void AudioNode::connect(AudioNode * outputSide, AudioNode * inputSide)
{
	outputSide->addConnectionOutput(inputSide);
	inputSide->addConnectionInput(outputSide);
}

void AudioNode::disconnect(AudioNode* outputSide, AudioNode* inputSide)
{
	outputSide->removeConnectionOutput(inputSide);
	inputSide->removeConnectionInput(outputSide);
}

void AudioNode::disconnect()
{
	for (auto& node : m_inputConnections) {
		node->removeConnectionOutput(this);
	}
	for (auto& node : m_outputConnections) {
		node->removeConnectionInput(this);
	}
	m_inputConnectionsDirty = false;
	m_outputConnectionsDirty = false;
}

//==============================================================================
// AudioSourceNode

AudioSourceNode::AudioSourceNode()
{
	m_commitState.playbackRate = 1.0f;
	m_commitState.currentState = PlayingState::NoChanged;
	m_commitState.resetRequire = false;
}

void AudioSourceNode::setPlaybackRate(float rate)
{
	m_commitState.playbackRate = rate;
	//LN_ENQUEUE_RENDER_COMMAND_2(
	//	start, context()->manager(),
	//	Ref<detail::CoreAudioSourceNode>, m_coreObject,
	//	float, rate,
	//	{
	//		m_coreObject->setPlaybackRate(rate);
	//	});
}

void AudioSourceNode::start()
{
	m_commitState.resetRequire = true;
	m_commitState.currentState = PlayingState::Play;
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
	m_commitState.currentState = PlayingState::Stop;
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

void AudioSourceNode::initialize(const StringRef & filePath)
{
	AudioNode::initialize();
	auto decoder = context()->manager()->createAudioDecoder(filePath);

	m_coreObject = makeRef<detail::CoreAudioSourceNode>(context()->coreObject());
	m_coreObject->initialize(decoder);
}

detail::CoreAudioNode * AudioSourceNode::coreNode()
{
	return m_coreObject;
}

void AudioSourceNode::commit()
{
	AudioNode::commit();

	if (m_commitState.resetRequire) {
		m_coreObject->reset();
		m_commitState.resetRequire = false;
	}

	switch (m_commitState.currentState)
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
	m_commitState.currentState = PlayingState::NoChanged;

	m_coreObject->setPlaybackRate(m_commitState.playbackRate);
}

//==============================================================================
// AudioPannerNode

AudioPannerNode::AudioPannerNode()
{
}

void AudioPannerNode::initialize()
{
	AudioNode::initialize();
	m_coreObject = makeRef<detail::CoreAudioPannerNode>(context()->coreObject());
	m_coreObject->initialize();
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

void AudioDestinationNode::initialize(detail::CoreAudioDestinationNode* core)
{
	AudioNode::initialize();
	m_coreObject = core;
}

detail::CoreAudioNode * AudioDestinationNode::coreNode()
{
	return m_coreObject;
}

} // namespace ln

