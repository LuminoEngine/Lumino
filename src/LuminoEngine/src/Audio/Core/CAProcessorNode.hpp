#pragma once
#include "CoreAudioNode.hpp"

namespace ln {
class AudioProcessorNode;
namespace detail {

class CAProcessorNode
	: public AudioNodeCore
{
public:
    CAProcessorNode(AudioDevice* context, AudioProcessorNode* frontNode);
    void init(int numberOfInputChannels, int numberOfOutputChannels);

protected:
	virtual void process() override;

private:
    AudioProcessorNode* m_ownerNode;
};

} // namespace detail
} // namespace ln

