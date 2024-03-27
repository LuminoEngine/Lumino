#include "Internal.hpp"
#include <LuminoFramework/Audio/AudioProcessorNode.hpp>
#include "ARIAudioBus.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"
#include "ARIProcessorNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ARIProcessorNode

ARIProcessorNode::ARIProcessorNode(AudioDevice* context, AudioProcessorNode* frontNode)
	: ARINode(context, frontNode)
    , m_ownerNode(frontNode)
{
}

void ARIProcessorNode::init(int numberOfInputChannels, int numberOfOutputChannels)
{
    ARINode::init();
    addInputPin(numberOfInputChannels);
    addOutputPin(numberOfOutputChannels);
}

void ARIProcessorNode::onCommit()
{
}

void ARIProcessorNode::process()
{
    ARIAudioBus* source = inputPin(0)->bus();
    ARIAudioBus* destination = outputPin(0)->bus();
    LN_NOTIMPLEMENTED();
    //m_ownerNode->onAudioProcess(source, destination);
}

} // namespace detail
} // namespace ln

