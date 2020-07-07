#pragma once
#include "ARINode.hpp"

namespace ln {
class AudioProcessorNode;
namespace detail {

class CAProcessorNode
	: public ARINode
{
public:
    CAProcessorNode(AudioDevice* context, AudioProcessorNode* frontNode);
    void init(int numberOfInputChannels, int numberOfOutputChannels);

protected:
    void onCommit() override;
	void process() override;

private:
    AudioProcessorNode* m_ownerNode;
};

} // namespace detail
} // namespace ln

