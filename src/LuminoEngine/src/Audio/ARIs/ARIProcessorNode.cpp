
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioProcessorNode.hpp>
#include "ARIAudioBus.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"
#include "ARIProcessorNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// CAProcessorNode

CAProcessorNode::CAProcessorNode(AudioDevice* context, AudioProcessorNode* frontNode)
	: ARINode(context, frontNode)
    , m_ownerNode(frontNode)
{
}

void CAProcessorNode::init(int numberOfInputChannels, int numberOfOutputChannels)
{
    ARINode::init();
    addInputPin(numberOfInputChannels);
    addOutputPin(numberOfOutputChannels);
}

void CAProcessorNode::onCommit()
{
}

void CAProcessorNode::process()
{
    ARIAudioBus* source = inputPin(0)->bus();
    ARIAudioBus* destination = outputPin(0)->bus();
    LN_NOTIMPLEMENTED();
    //m_ownerNode->onAudioProcess(source, destination);
}

} // namespace detail
} // namespace ln

