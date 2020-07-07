
#include "Internal.hpp"
#include "ARINode.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"
#include "../Decoder/AudioDecoder.hpp"	// for AudioSourceNodeCore
#include "ChromiumWebCore.hpp"
#include "../ARIs/VectorMath.h"

namespace ln {
namespace detail {

////==============================================================================
//// AudioContextCore
//
//AudioContextCore::AudioContextCore()
//{
//}
//
//void AudioContextCore::init()
//{
//#ifdef LN_USE_SDL
//	auto device = makeRef<SDLAudioDevice>();
//	device->init();
//	m_device = device;
//#else
//	auto device = makeRef<ALAudioDevice>();
//	device->init();
//	m_device = device;
//#endif
//}
//
//void AudioContextCore::dispose()
//{
//	if (m_device)
//	{
//		m_device->dispose();
//		m_device.reset();
//	}
//}
//

//==============================================================================
// AudioNodeCore

AudioNodeCore::AudioNodeCore(AudioDevice* context, AudioNode* frontNode)
	: m_context(context)
	, m_frontNode(frontNode)
{
}

void AudioNodeCore::init()
{
	Object::init();
}

bool AudioNodeCore::isInputConnected() const
{
	return m_inputPins.findIf([](auto& x) { return x->isConnected(); }).hasValue();
}

bool AudioNodeCore::isOutputConnected() const
{
	return m_outputPins.findIf([](auto& x) { return x->isConnected(); }).hasValue();
}

CoreAudioInputPin* AudioNodeCore::inputPin(int index) const
{
	return m_inputPins[index];
}

CoreAudioOutputPin* AudioNodeCore::outputPin(int index) const
{
	return m_outputPins[index];
}

void AudioNodeCore::pullInputs()
{
	for (auto & in : m_inputPins)
	{
		in->pull();
	}
}

void AudioNodeCore::processIfNeeded()
{
	m_marked = true;

	//if (frontNode()) {
	//	if (m_inputPins.isEmpty() && m_outputPins.isEmpty()) {
	//		frontNode()->m_a
	//	}
	//}

	pullInputs();
	process();
}

void AudioNodeCore::connect(AudioNodeCore * outputSide, AudioNodeCore * inputSide)
{
	outputSide->outputPin(0)->addLinkInput(inputSide->inputPin(0));
	inputSide->inputPin(0)->addLinkOutput(outputSide->outputPin(0));
}

void AudioNodeCore::disconnectAllInputSide()
{
	for (auto& pin : m_inputPins) {
		pin->disconnectAll();
	}
}

void AudioNodeCore::disconnectAllOutputSide()
{
	for (auto& pin : m_outputPins) {
		pin->disconnectAll();
	}
}

CoreAudioInputPin* AudioNodeCore::addInputPin(int channels)
{
	auto pin = makeRef<CoreAudioInputPin>(channels);
	pin->setOwnerNode(this);
	m_inputPins.add(pin);
	return pin;
}

CoreAudioOutputPin* AudioNodeCore::addOutputPin(int channels)
{
	auto pin = makeRef<CoreAudioOutputPin>(channels);
	pin->setOwnerNode(this);
	m_outputPins.add(pin);
	return pin;
}

} // namespace detail
} // namespace ln

