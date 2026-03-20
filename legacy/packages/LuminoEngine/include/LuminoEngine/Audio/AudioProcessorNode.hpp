#pragma once
#include "AudioNode.hpp"

namespace ln {
class AudioBus;
namespace detail {
class ARIProcessorNode;
}

class AudioProcessorNode
	: public AudioNode
{
public:

protected:
    virtual void onAudioProcess(AudioBus* input, AudioBus* output);

LN_CONSTRUCT_ACCESS:
    AudioProcessorNode();
    virtual ~AudioProcessorNode();
	void init(int numberOfInputChannels, int numberOfOutputChannels);
	virtual detail::ARINode* coreNode() override;
    //virtual void commit() override;

private:
	Ref<detail::ARIProcessorNode> m_coreObject;

    //Event<PhysicsObjectComponent*>	m_onAudioProcess;

    friend class detail::ARIProcessorNode;
};

} // namespace ln

