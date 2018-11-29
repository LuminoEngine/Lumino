
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioProcessorNode.hpp>
#include "CAProcessorNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// CAProcessorNode

CAProcessorNode::CAProcessorNode(AudioDevice* context, AudioProcessorNode* ownerNode)
	: CoreAudioNode(context)
    , m_ownerNode(ownerNode)
{
}

void CAProcessorNode::initialize(int numberOfInputChannels, int numberOfOutputChannels)
{
    CoreAudioNode::initialize();
    addInputPin(numberOfInputChannels);
    addOutputPin(numberOfOutputChannels);
}

void CAProcessorNode::process()
{
    CIAudioBus* destination = outputPin(0)->bus();
    CIAudioBus* source = inputPin(0)->bus();
}

} // namespace detail
} // namespace ln

