#pragma once
#include "ARINode.hpp"

namespace ln {
namespace detail {

class ARIPannerNode
	: public ARINode
{
public:

protected:
	void onCommit() override;
	void process() override;

public:
	ARIPannerNode(AudioDevice* context, AudioNode* frontNode);
	virtual ~ARIPannerNode() = default;
	void init();

private:
	void azimuthElevation(double* outAzimuth, double* outElevation);
	float distanceConeGain();

	AudioEmitterParams m_emitter;

	std::shared_ptr<blink::Panner> m_panner;
	std::shared_ptr<blink::DistanceEffect> m_distanceEffect;
	std::shared_ptr<blink::ConeEffect> m_coneEffect;
};

} // namespace detail
} // namespace ln

