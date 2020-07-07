#pragma once
#include "ARINode.hpp"

namespace ln {
namespace detail {

class ARIGainNode
	: public ARINode
{
public:
    struct StagingData// : public ARINode::StagingData
    {
        std::atomic<float> gain;
    //    void setGain(float value);
    //    float gain() const { return m_gain; }
    //private:
    //    float m_gain;
    } staging;

    ARIGainNode(AudioDevice* context, AudioNode* frontNode);
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

