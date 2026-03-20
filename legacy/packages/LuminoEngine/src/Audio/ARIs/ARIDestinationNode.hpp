#pragma once
#include "../Backend/AudioDevice.hpp"	// for IAudioDeviceRenderCallback
#include "ARINode.hpp"

namespace ln {
namespace detail {

class ARIPropagationParameters
{
public:
	ARIPropagationParameters();

	int finalSamplingRate() const { return m_finalSamplingRate; }

	void setFinalSamplingRate(int rate) { m_finalSamplingRate = rate; }

private:
	int m_finalSamplingRate;
};

class ARIDestinationNode
	: public ARINode
	, public IAudioDeviceRenderCallback
{
public:
	ARIDestinationNode(AudioDevice* context, AudioNode* frontNode);
	virtual ~ARIDestinationNode() = default;
	void init();

protected:
	void onCommit() override;
	void process() override;
	void render(float* outputBuffer, int length) override;

private:
	ARIPropagationParameters m_propagationParameters;
};

} // namespace detail
} // namespace ln

