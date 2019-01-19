#pragma once
#include "AudioNode.hpp"

namespace ln {
namespace detail {
class CAGainNode;
}

class AudioGainNode
	: public AudioNode
{
public:
    void setGain(float value);
    float gain() const { return m_gain; }

LN_CONSTRUCT_ACCESS:
    AudioGainNode();
    virtual ~AudioGainNode();
	void init();
	virtual detail::CoreAudioNode* coreNode() override;
    virtual void commit() override;

private:
	Ref<detail::CAGainNode> m_coreObject;
    float m_gain;
};

} // namespace ln

