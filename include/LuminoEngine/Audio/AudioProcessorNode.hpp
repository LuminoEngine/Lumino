#pragma once
#include "AudioNode.hpp"

namespace ln {
class AudioBus;
namespace detail {
class CAProcessorNode;
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
	virtual detail::AudioNodeCore* coreNode() override;
    virtual void commit() override;

private:
	Ref<detail::CAProcessorNode> m_coreObject;

    //Event<PhysicsObjectComponent*>	m_onAudioProcess;

    friend class detail::CAProcessorNode;
};

} // namespace ln

