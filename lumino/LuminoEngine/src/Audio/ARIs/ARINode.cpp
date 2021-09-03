
#include "Internal.hpp"
#include "ARINode.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"
#include "../Decoder/AudioDecoder.hpp"	// for ARISourceNode
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
// ARINode

ARINode::ARINode(AudioDevice* context, AudioNode* frontNode)
	: m_context(context)
	, m_frontNode(frontNode)
{
}

void ARINode::init()
{
	Object::init();
}

bool ARINode::isInputConnected() const
{
	return m_inputPins.findIf([](auto& x) { return x->isConnected(); }).hasValue();
}

bool ARINode::isOutputConnected() const
{
	return m_outputPins.findIf([](auto& x) { return x->isConnected(); }).hasValue();
}

ARIInputPin* ARINode::inputPin(int index) const
{
	return m_inputPins[index];
}

ARIOutputPin* ARINode::outputPin(int index) const
{
	return m_outputPins[index];
}

void ARINode::pullInputs()
{
	for (auto & in : m_inputPins)
	{
		in->pull();
	}
}

void ARINode::processIfNeeded()
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

void ARINode::connect(ARINode * outputSide, ARINode * inputSide)
{
	outputSide->outputPin(0)->addLinkInput(inputSide->inputPin(0));
	inputSide->inputPin(0)->addLinkOutput(outputSide->outputPin(0));
}

void ARINode::disconnectAllInputSide()
{
	for (auto& pin : m_inputPins) {
		pin->disconnectAll();
	}
}

void ARINode::disconnectAllOutputSide()
{
	for (auto& pin : m_outputPins) {
		pin->disconnectAll();
	}
}

void ARINode::disconnectAll()
{
	disconnectAllInputSide();
	disconnectAllOutputSide();
}

ARIInputPin* ARINode::addInputPin(int channels)
{
	auto pin = makeRef<ARIInputPin>(channels);
	pin->setOwnerNode(this);
	m_inputPins.add(pin);
	return pin;
}

ARIOutputPin* ARINode::addOutputPin(int channels)
{
	auto pin = makeRef<ARIOutputPin>(channels);
	pin->setOwnerNode(this);
	m_outputPins.add(pin);
	return pin;
}

} // namespace detail
} // namespace ln

