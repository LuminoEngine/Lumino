
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
}

void AudioNode::dispose()
{
	Object::dispose();
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

void AudioNode::connect(AudioNode * outputSide, AudioNode * inputSide)
{
	outputSide->addConnectionOutput(inputSide);
	inputSide->addConnectionInput(outputSide);
}

//==============================================================================
// AudioSourceNode

AudioSourceNode::AudioSourceNode()
{
}

void AudioSourceNode::setPlaybackRate(float rate)
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		start, context()->manager(),
		Ref<detail::CoreAudioSourceNode>, m_coreObject,
		float, rate,
		{
			m_coreObject->setPlaybackRate(rate);
		});
}

void AudioSourceNode::start()
{
	LN_ENQUEUE_RENDER_COMMAND_1(
		start, context()->manager(),
		Ref<detail::CoreAudioSourceNode>, m_coreObject,
		{
			m_coreObject->start();
		});
}

void AudioSourceNode::stop()
{
	LN_ENQUEUE_RENDER_COMMAND_1(
		start, context()->manager(),
		Ref<detail::CoreAudioSourceNode>, m_coreObject,
		{
			m_coreObject->stop();
		});
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

