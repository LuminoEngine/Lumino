// License: BSD 3 Clause
// Copyright (C) 2010, Google Inc. All rights reserved.
// Copyright (C) 2018+, The Lumino Authors. All rights reserved.

// chromium/third_party/blink/renderer/platform
// chromium/third_party/blink/renderer/modules/webaudio

#pragma once

#include "CoreAudioNode.hpp"	// TODO: AudioBus

#define PLATFORM_EXPORT
#define USING_FAST_MALLOC(x)
#define WTF_MAKE_NONCOPYABLE(x)

namespace ln {
namespace detail {
class CoreAudioBus;
namespace blink {
using AudioBus = CoreAudioBus;

class HRTFDatabaseLoader;

// Distance models are defined according to the OpenAL specification :
// http://connect.creativelabs.com/openal/Documentation/OpenAL%201.1%20Specification.htm.

class PLATFORM_EXPORT DistanceEffect {

public:
	enum ModelType { kModelLinear = 0, kModelInverse = 1, kModelExponential = 2 };

	DistanceEffect();

	// Returns scalar gain for the given distance the current distance model is
	// used
	double Gain(double distance);

	ModelType Model() { return model_; }

	void SetModel(ModelType model) { model_ = model; }

	// Distance params
	void SetRefDistance(double ref_distance) { ref_distance_ = ref_distance; }
	void SetMaxDistance(double max_distance) { max_distance_ = max_distance; }
	void SetRolloffFactor(double rolloff_factor) {
		rolloff_factor_ = rolloff_factor;
	}

	double RefDistance() const { return ref_distance_; }
	double MaxDistance() const { return max_distance_; }
	double RolloffFactor() const { return rolloff_factor_; }

protected:
	double LinearGain(double distance);
	double InverseGain(double distance);
	double ExponentialGain(double distance);

	ModelType model_;
	double ref_distance_;
	double max_distance_;
	double rolloff_factor_;
};

// Cone gain is defined according to the OpenAL specification
class PLATFORM_EXPORT ConeEffect {

public:
	ConeEffect();

	// Returns scalar gain for the given source/listener positions/orientations
	double Gain(Vector3 source_position,
		Vector3 source_orientation,
		Vector3 listener_position);

	// Angles in degrees
	void SetInnerAngle(double inner_angle) { inner_angle_ = inner_angle; }
	double InnerAngle() const { return inner_angle_; }

	void SetOuterAngle(double outer_angle) { outer_angle_ = outer_angle; }
	double OuterAngle() const { return outer_angle_; }

	void SetOuterGain(double outer_gain) { outer_gain_ = outer_gain; }
	double OuterGain() const { return outer_gain_; }

protected:
	double inner_angle_;
	double outer_angle_;
	double outer_gain_;
};

// Abstract base class for panning a mono or stereo source.
class PLATFORM_EXPORT Panner {
	USING_FAST_MALLOC(Panner);
	WTF_MAKE_NONCOPYABLE(Panner);

public:
	// This values are used in histograms and should not be renumbered or deleted.
	enum { kPanningModelEqualPower = 0, kPanningModelHRTF = 1 };

	typedef unsigned PanningModel;

	static std::unique_ptr<Panner> Create(PanningModel,
		float sample_rate,
		HRTFDatabaseLoader*);

	virtual ~Panner() = default;

	virtual void Pan(double azimuth,
		double elevation,
		const AudioBus* input_bus,
		AudioBus* output_bus,
		size_t frames_to_process,
		AudioBus::ChannelInterpretation) = 0;

	virtual void PanWithSampleAccurateValues(double* azimuth,
		double* elevation,
		const AudioBus* input_bus,
		AudioBus* output_bus,
		size_t frames_to_process,
		AudioBus::ChannelInterpretation) = 0;

	virtual void Reset() = 0;

	virtual double TailTime() const = 0;
	virtual double LatencyTime() const = 0;
	virtual bool RequiresTailProcessing() const = 0;

protected:
	Panner(PanningModel model) : panning_model_(model) {}

	PanningModel panning_model_;
};

// Common type of stereo panner as found in normal audio mixing equipment.
class PLATFORM_EXPORT EqualPowerPanner final : public Panner{
 public:
  EqualPowerPanner(float sample_rate);

  void Pan(double azimuth,
		   double elevation,
		   const AudioBus* input_bus,
		   AudioBus* output_buf,
		   size_t frames_to_process,
		   AudioBus::ChannelInterpretation) override;
  void PanWithSampleAccurateValues(double* azimuth,
								   double* elevation,
								   const AudioBus* input_bus,
								   AudioBus* output_bus,
								   size_t frames_to_process,
								   AudioBus::ChannelInterpretation) override;

  void Reset() override {}

  double TailTime() const override { return 0; }
  double LatencyTime() const override { return 0; }
  bool RequiresTailProcessing() const override { return false; }

 private:
  void CalculateDesiredGain(double& desired_gain_l,
							double& desired_gain_r,
							double azimuth,
							int number_of_channels);
};

//namespace AudioUtilities {
//
//// Standard functions for converting to and from decibel values from linear.
//double linearToDecibels(double);
//double decibelsToLinear(double);
//
//// timeConstant is the time it takes a first-order linear time-invariant system
//// to reach the value 1 - 1/e (around 63.2%) given a step input response.
//// discreteTimeConstantForSampleRate() will return the discrete time-constant for the specific sampleRate.
//double discreteTimeConstantForSampleRate(double timeConstant, double sampleRate);
//
//} // AudioUtilites

float CalculateDistanceConeGain(
	const Vector3& position,
	const Vector3& orientation,
	const Vector3& listener_position,
	DistanceEffect* distance_effect,
	ConeEffect* cone_effect);

void CalculateAzimuthElevation(
	double* out_azimuth,
	double* out_elevation,
	const Vector3& position,
	const Vector3& listener_position,
	const Vector3& listener_forward,
	const Vector3& listener_up);

} // namespace blink
} // namespace detail
} // namespace ln

