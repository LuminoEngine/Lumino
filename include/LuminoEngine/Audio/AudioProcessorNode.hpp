#pragma once
#include "AudioNode.hpp"

namespace ln {
namespace detail {
class CAProcessorNode;
}

class AudioProcessorNode
	: public AudioNode
{
public:

protected:
    virtual void onAudioProcess(/* AudioBus* input, AudioBus* output */);

LN_CONSTRUCT_ACCESS:
    AudioProcessorNode();
    virtual ~AudioProcessorNode();
	void initialize(int numberOfInputChannels, int numberOfOutputChannels);
	virtual detail::CoreAudioNode* coreNode() override;
    virtual void commit() override;

private:
	Ref<detail::CAProcessorNode> m_coreObject;

    friend class detail::CAProcessorNode;
};

} // namespace ln

