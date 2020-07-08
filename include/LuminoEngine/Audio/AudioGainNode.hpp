#pragma once
#include "AudioNode.hpp"

namespace ln {
namespace detail {
class ARIGainNode;
}

class AudioGainNode
	: public AudioNode
{
public:
    void setGain(float value);
    float gain() const;

LN_CONSTRUCT_ACCESS:
    AudioGainNode();
    virtual ~AudioGainNode();
	void init();
	virtual detail::ARINode* coreNode() override;
    //virtual void commit() override;

private:
	Ref<detail::ARIGainNode> m_coreObject;
    //float m_gain;
};

} // namespace ln

