#pragma once
#include "ARINode.hpp"

namespace ln {
class AudioNode;

namespace detail {

class PropagationParameters
{
public:
	PropagationParameters();

	int finalSamplingRate() const { return m_finalSamplingRate; }

	void setFinalSamplingRate(int rate) { m_finalSamplingRate = rate; }

private:
	int m_finalSamplingRate;
};

class CoreAudioDestinationNode
	: public AudioNodeCore
	, public IAudioDeviceRenderCallback
{
public:
	CoreAudioDestinationNode(AudioDevice* context, AudioNode* frontNode);
	virtual ~CoreAudioDestinationNode() = default;
	void init();

protected:
	void onCommit() override;
	void process() override;
	void render(float* outputBuffer, int length) override;

private:
	PropagationParameters m_propagationParameters;
};

} // namespace detail
} // namespace ln

