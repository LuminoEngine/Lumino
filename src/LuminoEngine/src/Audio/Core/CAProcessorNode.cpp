
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioProcessorNode.hpp>
#include "CAProcessorNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// CAProcessorNode

CAProcessorNode::CAProcessorNode(AudioDevice* context, AudioProcessorNode* frontNode)
	: AudioNodeCore(context, frontNode)
    , m_ownerNode(frontNode)
{
}

void CAProcessorNode::init(int numberOfInputChannels, int numberOfOutputChannels)
{
    AudioNodeCore::init();
    addInputPin(numberOfInputChannels);
    addOutputPin(numberOfOutputChannels);
}

void CAProcessorNode::process()
{
    AudioBus* source = inputPin(0)->bus();
    AudioBus* destination = outputPin(0)->bus();
    m_ownerNode->onAudioProcess(source, destination);
}

} // namespace detail
} // namespace ln

