
#include "Internal.hpp"
#include "ARIAudioBus.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"
#include "ARIGainNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ARIGainNode

ARIGainNode::ARIGainNode(AudioDevice* context, AudioNode* frontNode)
	: ARINode(context, frontNode)
    , m_gain(1.0f)
{
}

void ARIGainNode::init()
{
    ARINode::init();

    staging.gain = 1.0f;

    int numChannels = 2;
    addOutputPin(numChannels);
    addInputPin(numChannels);
}

void ARIGainNode::onCommit()
{
    m_gain = staging.gain;
}

void ARIGainNode::process()
{
    ARIAudioBus* destination = outputPin(0)->bus();
    ARIAudioBus* source = inputPin(0)->bus();
    destination->copyWithGainFrom(*source, Math::clamp01(m_gain));
}

} // namespace detail
} // namespace ln

