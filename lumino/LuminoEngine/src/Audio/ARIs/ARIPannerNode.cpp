
#include "Internal.hpp"
#include "ChromiumWebCore.hpp"
#include "../Backend/AudioDevice.hpp"
#include "ARIAudioBus.hpp"
#include "ARIInputPin.hpp"
#include "ARIOutputPin.hpp"
#include "ARIPannerNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ARIPannerNode

ARIPannerNode::ARIPannerNode(AudioDevice* context, AudioNode* frontNode)
	: ARINode(context, frontNode)
{
}

void ARIPannerNode::init()
{
	ARINode::init();

	unsigned numChannels = 2;
	addOutputPin(numChannels);
	addInputPin(numChannels);

	m_panner = blink::Panner::Create(blink::Panner::kPanningModelEqualPower, 0, nullptr);
	m_distanceEffect = std::make_shared<blink::DistanceEffect>();
	m_coneEffect = std::make_shared<blink::ConeEffect>();
}

void ARIPannerNode::onCommit()
{
}

void ARIPannerNode::process()
{
	//test
	static float count = 0;
	count += 0.01;
	m_emitter.m_position = Vector3(cos(count), 0, sin(count));



	ARIAudioBus* destination = outputPin(0)->bus();

	if (!m_panner.get()) {
		destination->setSilentAndZero();
		return;
	}

	ARIAudioBus* source = inputPin(0)->bus();
	if (!source) {
		destination->setSilentAndZero();
		return;
	}

	double azimuth;
	double elevation;
	azimuthElevation(&azimuth, &elevation);
	//printf("azimuth:%f\n", azimuth);

	m_panner->Pan(azimuth, elevation, source, destination, destination->length(), ARIAudioBus::kSpeakers);

	float total_gain = distanceConeGain();
	//printf("tt:%f\n", total_gain);

	destination->copyWithGainFrom(*destination, total_gain);
}

void ARIPannerNode::azimuthElevation(double* outAzimuth, double* outElevation)
{
	double cached_azimuth_;
	double cached_elevation_;

	// TODO: dirty and cache

	auto& listener = context()->listener();
	blink::CalculateAzimuthElevation(
		&cached_azimuth_, &cached_elevation_, 
		m_emitter.m_position, listener.m_position, listener.m_forward, listener.m_up);

	*outAzimuth = cached_azimuth_;
	*outElevation = cached_elevation_;
}

float ARIPannerNode::distanceConeGain()
{
	// TODO: dirty and cache

	float cached_distance_cone_gain_;

	cached_distance_cone_gain_ = blink::CalculateDistanceConeGain(
		m_emitter.m_position, m_emitter.m_direction, context()->listener().m_position, m_distanceEffect.get(), m_coneEffect.get());

	return cached_distance_cone_gain_;
}

} // namespace detail
} // namespace ln

