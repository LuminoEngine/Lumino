#pragma once
#include "ARINode.hpp"

namespace ln {
namespace detail {

class CAGainNode
	: public AudioNodeCore
{
public:
    struct StagingData// : public AudioNodeCore::StagingData
    {
        std::atomic<float> gain;
    //    void setGain(float value);
    //    float gain() const { return m_gain; }
    //private:
    //    float m_gain;
    } staging;

    CAGainNode(AudioDevice* context, AudioNode* frontNode);
    void init();
    void setGain(float value) { m_gain = value; }
    float gain() const { return m_gain; }

protected:
    void onCommit() override;
	void process() override;

private:
    float m_gain;
};

} // namespace detail
} // namespace ln

