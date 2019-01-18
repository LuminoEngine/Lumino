#pragma once
#include "CoreAudioNode.hpp"

namespace ln {
class AudioProcessorNode;
namespace detail {

class CAProcessorNode
	: public CoreAudioNode
{
public:
    CAProcessorNode(AudioDevice* context, AudioProcessorNode* ownerNode);
    void initialize(int numberOfInputChannels, int numberOfOutputChannels);

protected:
	virtual void process() override;

private:
    AudioProcessorNode* m_ownerNode;
};

} // namespace detail
} // namespace ln

