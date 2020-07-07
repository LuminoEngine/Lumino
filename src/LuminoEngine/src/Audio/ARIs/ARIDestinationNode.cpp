
#include "Internal.hpp"
#include "VectorMath.h"
#include "ARIAudioBus.hpp"
#include "ARIInputPin.hpp"
#include "ARIDestinationNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// PropagationParameters

PropagationParameters::PropagationParameters()
	: m_finalSamplingRate(0)
{}

//==============================================================================
// CoreAudioDestinationNode

CoreAudioDestinationNode::CoreAudioDestinationNode(AudioDevice* context, AudioNode* frontNode)
	: AudioNodeCore(context, frontNode)
{
}

void CoreAudioDestinationNode::init()
{
	AudioNodeCore::init();
	addInputPin(2);
}

void CoreAudioDestinationNode::onCommit()
{
}

void CoreAudioDestinationNode::process()
{
	LN_UNREACHABLE();
}

void CoreAudioDestinationNode::render(float * outputBuffer, int length)
{
	m_marked = true;

    ////AudioChannel* ch1 = output->channel(0);
    ////AudioChannel* ch2 = output->channel(1);
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
		AudioChannel * channel = bus->channel(i);
		::blink::VectorMath::vclip(channel->constData(), 1, &kLowThreshold, &kHighThreshold, channel->mutableData(), 1, channel->length());
	}

	bus->mergeToChannelBuffers(outputBuffer, length);
}

} // namespace detail
} // namespace ln

