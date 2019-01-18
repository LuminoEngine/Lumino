#pragma once
#include "CoreAudioNode.hpp"

namespace ln {
namespace detail {

class CAGainNode
	: public CoreAudioNode
{
public:
    CAGainNode(AudioDevice* context);
    void initialize();
    void setGain(float value) { m_gain = value; }
    float gain() const { return m_gain; }

protected:
	virtual void process() override;

private:
    float m_gain;
};

} // namespace detail
} // namespace ln

