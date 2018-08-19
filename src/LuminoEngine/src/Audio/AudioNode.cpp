
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
{
}

void AudioNode::initialize()
{
	Object::initialize();
	m_context =	detail::EngineDomain::audioManager()->primaryContext();
}

void AudioNode::connect(AudioNode * outputSide, AudioNode * inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->m_context == inputSide->m_context)) return;

	detail::AudioManager* manager = outputSide->m_context->manager();
	Ref<detail::CoreAudioNode> output = outputSide->coreNode();
	Ref<detail::CoreAudioNode> input = inputSide->coreNode();
	LN_ENQUEUE_RENDER_COMMAND_2(
		connect, manager,
		Ref<detail::CoreAudioNode>, output,
		Ref<detail::CoreAudioNode>, input,
		{
			detail::CoreAudioNode::connect(output, input);
		});

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

	m_coreObject = makeRef<detail::CoreAudioSourceNode>();
	m_coreObject->initialize(decoder);
}

detail::CoreAudioNode * AudioSourceNode::coreNode()
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

