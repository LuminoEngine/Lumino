
#include "Internal.hpp"
#include "VectorMath.h"
#include "ARIAudioBus.hpp"
#include "ARIInputPin.hpp"
#include "ARIDestinationNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ARIPropagationParameters

ARIPropagationParameters::ARIPropagationParameters()
	: m_finalSamplingRate(0)
{}

//==============================================================================
// ARIDestinationNode

ARIDestinationNode::ARIDestinationNode(AudioDevice* context, AudioNode* frontNode)
	: ARINode(context, frontNode)
{
}

void ARIDestinationNode::init()
{
	ARINode::init();
	addInputPin(2);
}

void ARIDestinationNode::onCommit()
{
}

void ARIDestinationNode::process()
{
	LN_UNREACHABLE();
}

void ARIDestinationNode::render(float * outputBuffer, int length)
{
	m_marked = true;

    ////ARIChannel* ch1 = output->channel(0);
    ////ARIChannel* ch2 = output->channel(1);
    ////float* data1 = ch1->mutableData();
    ////float* data2 = ch2->mutableData();
    //static float step = 0.0;
    //static float frequency = 440;

    //for (int i = 0; i < length / 2; i++) {
    //    float v = std::sin(2.0 * Math::PI * step);
    //    outputBuffer[(i*2) + 0] = outputBuffer[(i * 2) + 1] = v;
    //    step += frequency / context()->deviceSamplingRate();//4096;
    //}

    //return;


	ARIAudioBus* bus = inputPin(0)->pull();

	// Clamp values at 0db (i.e., [-1.0, 1.0])
	const float kLowThreshold = -1.0f;
	const float kHighThreshold = 1.0f;
	for (unsigned i = 0; i < bus->numberOfChannels(); ++i)
	{
		ARIChannel * channel = bus->channel(i);
		::blink::VectorMath::vclip(channel->constData(), 1, &kLowThreshold, &kHighThreshold, channel->mutableData(), 1, channel->length());
	}

	bus->mergeToChannelBuffers(outputBuffer, length);
}

} // namespace detail
} // namespace ln

