
#include "Internal.hpp"
#include "ChromiumWebCore.hpp"


#define ASSERT assert
#define NOTREACHED LN_UNREACHABLE
#define DCHECK LN_DCHECK

namespace ln {
namespace detail {
namespace blink {

using namespace std;

constexpr double kPiDouble = 3.14159265358979323846;
constexpr double kPiOverTwoDouble = kPiDouble / 2;

double clampTo(double value, double minValue = DBL_MIN, double maxValue = DBL_MAX) { return (value > maxValue) ? maxValue : (value < minValue) ? minValue : value; }

float AngleBetween(const Vector3& x, const Vector3& y) {
	float x_length = x.length();
	float y_length = y.length();

	if (x_length && y_length) {
		float cos_angle = Vector3::dot(x, y) / (x_length * y_length);
		// Due to round-off |cosAngle| can have a magnitude greater than 1.  Clamp
		// the value to [-1, 1] before computing the angle.
		return acos(clampTo(cos_angle, -1.0, 1.0));
	}
	return 0;
}

constexpr double rad2deg(double r) {
	return r * 180.0 / kPiDouble;
}

template <typename T>
static void FixNANs(T& x)
{
	if (std::isnan(T(x)) || std::isinf(x)) x = T(0);
}

//==============================================================================
// DistanceEffect

DistanceEffect::DistanceEffect()
	: model_(kModelInverse),
	ref_distance_(1.0),
	max_distance_(10000.0),
	rolloff_factor_(1.0) {}

double DistanceEffect::Gain(double distance) {
	switch (model_) {
	case kModelLinear:
		return LinearGain(distance);
	case kModelInverse:
		return InverseGain(distance);
	case kModelExponential:
		return ExponentialGain(distance);
	}
	NOTREACHED();
	return 0.0;
}

double DistanceEffect::LinearGain(double distance) {
	// Clamp refDistance and distance according to the spec.
	double dref = std::min(ref_distance_, max_distance_);
	double dmax = std::max(ref_distance_, max_distance_);
	distance = clampTo(distance, dref, dmax);

	if (dref == dmax)
		return 1 - rolloff_factor_;

	// We want a gain that decreases linearly from m_refDistance to
	// m_maxDistance. The gain is 1 at m_refDistance.
	return (1.0 - clampTo(rolloff_factor_, 0.0, 1.0) * (distance - dref) /
		(dmax - dref));
}

double DistanceEffect::InverseGain(double distance) {
	if (ref_distance_ == 0)
		return 0;

	// Clamp distance according to spec
	distance = clampTo(distance, ref_distance_);

	return ref_distance_ / (ref_distance_ + clampTo(rolloff_factor_, 0.0) *
		(distance - ref_distance_));
}

double DistanceEffect::ExponentialGain(double distance) {
	if (ref_distance_ == 0)
		return 0;

	// Clamp distance according to spec
	distance = clampTo(distance, ref_distance_);

	return pow(distance / ref_distance_, -clampTo(rolloff_factor_, 0.0));
}

//==============================================================================
// ConeEffect

ConeEffect::ConeEffect()
	: inner_angle_(360.0), outer_angle_(360.0), outer_gain_(0.0) {}

double ConeEffect::Gain(Vector3 source_position,
	Vector3 source_orientation,
	Vector3 listener_position) {
	if (source_orientation.isZero() ||
		((inner_angle_ == 360.0) && (outer_angle_ == 360.0)))
		return 1.0;  // no cone specified - unity gain

	  // Source-listener vector
	Vector3 source_to_listener = listener_position - source_position;

	// Angle between the source orientation vector and the source-listener vector
	double angle = rad2deg(AngleBetween(source_to_listener, source_orientation));
	double abs_angle = fabs(angle);

	// Divide by 2.0 here since API is entire angle (not half-angle)
	double abs_inner_angle = fabs(inner_angle_) / 2.0;
	double abs_outer_angle = fabs(outer_angle_) / 2.0;
	double gain = 1.0;

	if (abs_angle <= abs_inner_angle) {
		// No attenuation
		gain = 1.0;
	}
	else if (abs_angle >= abs_outer_angle) {
		// Max attenuation
		gain = outer_gain_;
	}
	else {
		// Between inner and outer cones
		// inner -> outer, x goes from 0 -> 1
		double x =
			(abs_angle - abs_inner_angle) / (abs_outer_angle - abs_inner_angle);
		gain = (1.0 - x) + outer_gain_ * x;
	}

	return gain;
}

//==============================================================================
// Panner

std::unique_ptr<Panner> Panner::Create(PanningModel model,
	float sample_rate,
	HRTFDatabaseLoader* database_loader) {
	switch (model) {
	case kPanningModelEqualPower:
		return std::make_unique<EqualPowerPanner>(sample_rate);

	case kPanningModelHRTF:
		//return std::make_unique<HRTFPanner>(sample_rate, database_loader);

	default:
		NOTREACHED();
		return nullptr;
	}
}

//==============================================================================
// EqualPowerPanner
EqualPowerPanner::EqualPowerPanner(float sample_rate)
	: Panner(kPanningModelEqualPower) {}

void EqualPowerPanner::Pan(double azimuth,
	double /*elevation*/,
	const AudioBus* input_bus,
	AudioBus* output_bus,
	size_t frames_to_process,
	AudioBus::ChannelInterpretation) {
	bool is_input_safe = input_bus &&
		(input_bus->NumberOfChannels() == 1 ||
			input_bus->NumberOfChannels() == 2) &&
		frames_to_process <= input_bus->length();
	DCHECK(is_input_safe);
	if (!is_input_safe)
		return;

	unsigned number_of_input_channels = input_bus->NumberOfChannels();

	bool is_output_safe = output_bus && output_bus->NumberOfChannels() == 2 &&
		frames_to_process <= output_bus->length();
	DCHECK(is_output_safe);
	if (!is_output_safe)
		return;

	const float* source_l = input_bus->Channel(0)->Data();
	const float* source_r =
		number_of_input_channels > 1 ? input_bus->Channel(1)->Data() : source_l;
	float* destination_l =
		output_bus->ChannelByType(AudioBus::kChannelLeft)->MutableData();
	float* destination_r =
		output_bus->ChannelByType(AudioBus::kChannelRight)->MutableData();

	if (!source_l || !source_r || !destination_l || !destination_r)
		return;

	// Clamp azimuth to allowed range of -180 -> +180.
	azimuth = clampTo(azimuth, -180.0, 180.0);

	// Alias the azimuth ranges behind us to in front of us:
	// -90 -> -180 to -90 -> 0 and 90 -> 180 to 90 -> 0
	if (azimuth < -90)
		azimuth = -180 - azimuth;
	else if (azimuth > 90)
		azimuth = 180 - azimuth;

	double desired_pan_position;
	double desired_gain_l;
	double desired_gain_r;

	if (number_of_input_channels == 1) {  // For mono source case.
	  // Pan smoothly from left to right with azimuth going from -90 -> +90
	  // degrees.
		desired_pan_position = (azimuth + 90) / 180;
	}
	else {               // For stereo source case.
		if (azimuth <= 0) {  // from -90 -> 0
		  // sourceL -> destL and "equal-power pan" sourceR as in mono case
		  // by transforming the "azimuth" value from -90 -> 0 degrees into the
		  // range -90 -> +90.
			desired_pan_position = (azimuth + 90) / 90;
		}
		else {  // from 0 -> +90
	   // sourceR -> destR and "equal-power pan" sourceL as in mono case
	   // by transforming the "azimuth" value from 0 -> +90 degrees into the
	   // range -90 -> +90.
			desired_pan_position = azimuth / 90;
		}
	}

	desired_gain_l = std::cos(kPiOverTwoDouble * desired_pan_position);
	desired_gain_r = std::sin(kPiOverTwoDouble * desired_pan_position);

	int n = frames_to_process;

	if (number_of_input_channels == 1) {  // For mono source case.
		while (n--) {
			float input_l = *source_l++;

			*destination_l++ = static_cast<float>(input_l * desired_gain_l);
			*destination_r++ = static_cast<float>(input_l * desired_gain_r);
		}
	}
	else {               // For stereo source case.
		if (azimuth <= 0) {  // from -90 -> 0
			while (n--) {
				float input_l = *source_l++;
				float input_r = *source_r++;

				*destination_l++ =
					static_cast<float>(input_l + input_r * desired_gain_l);
				*destination_r++ = static_cast<float>(input_r * desired_gain_r);
			}
		}
		else {  // from 0 -> +90
			while (n--) {
				float input_l = *source_l++;
				float input_r = *source_r++;

				*destination_l++ = static_cast<float>(input_l * desired_gain_l);
				*destination_r++ =
					static_cast<float>(input_r + input_l * desired_gain_r);
			}
		}
	}
}

void EqualPowerPanner::CalculateDesiredGain(double& desired_gain_l,
	double& desired_gain_r,
	double azimuth,
	int number_of_input_channels) {
	// Clamp azimuth to allowed range of -180 -> +180.
	azimuth = clampTo(azimuth, -180.0, 180.0);

	// Alias the azimuth ranges behind us to in front of us:
	// -90 -> -180 to -90 -> 0 and 90 -> 180 to 90 -> 0
	if (azimuth < -90)
		azimuth = -180 - azimuth;
	else if (azimuth > 90)
		azimuth = 180 - azimuth;

	double desired_pan_position;

	if (number_of_input_channels == 1) {  // For mono source case.
	  // Pan smoothly from left to right with azimuth going from -90 -> +90
	  // degrees.
		desired_pan_position = (azimuth + 90) / 180;
	}
	else {               // For stereo source case.
		if (azimuth <= 0) {  // from -90 -> 0
		  // sourceL -> destL and "equal-power pan" sourceR as in mono case
		  // by transforming the "azimuth" value from -90 -> 0 degrees into the
		  // range -90 -> +90.
			desired_pan_position = (azimuth + 90) / 90;
		}
		else {  // from 0 -> +90
	   // sourceR -> destR and "equal-power pan" sourceL as in mono case
	   // by transforming the "azimuth" value from 0 -> +90 degrees into the
	   // range -90 -> +90.
			desired_pan_position = azimuth / 90;
		}
	}

	desired_gain_l = std::cos(kPiOverTwoDouble * desired_pan_position);
	desired_gain_r = std::sin(kPiOverTwoDouble * desired_pan_position);
}

void EqualPowerPanner::PanWithSampleAccurateValues(
	double* azimuth,
	double* /*elevation*/,
	const AudioBus* input_bus,
	AudioBus* output_bus,
	size_t frames_to_process,
	AudioBus::ChannelInterpretation) {
	bool is_input_safe = input_bus &&
		(input_bus->NumberOfChannels() == 1 ||
			input_bus->NumberOfChannels() == 2) &&
		frames_to_process <= input_bus->length();
	DCHECK(is_input_safe);
	if (!is_input_safe)
		return;

	unsigned number_of_input_channels = input_bus->NumberOfChannels();

	bool is_output_safe = output_bus && output_bus->NumberOfChannels() == 2 &&
		frames_to_process <= output_bus->length();
	DCHECK(is_output_safe);
	if (!is_output_safe)
		return;

	const float* source_l = input_bus->Channel(0)->Data();
	const float* source_r =
		number_of_input_channels > 1 ? input_bus->Channel(1)->Data() : source_l;
	float* destination_l =
		output_bus->ChannelByType(AudioBus::kChannelLeft)->MutableData();
	float* destination_r =
		output_bus->ChannelByType(AudioBus::kChannelRight)->MutableData();

	if (!source_l || !source_r || !destination_l || !destination_r)
		return;

	int n = frames_to_process;

	if (number_of_input_channels == 1) {  // For mono source case.
		for (int k = 0; k < n; ++k) {
			double desired_gain_l;
			double desired_gain_r;
			float input_l = *source_l++;

			CalculateDesiredGain(desired_gain_l, desired_gain_r, azimuth[k],
				number_of_input_channels);
			*destination_l++ = static_cast<float>(input_l * desired_gain_l);
			*destination_r++ = static_cast<float>(input_l * desired_gain_r);
		}
	}
	else {  // For stereo source case.
		for (int k = 0; k < n; ++k) {
			double desired_gain_l;
			double desired_gain_r;

			CalculateDesiredGain(desired_gain_l, desired_gain_r, azimuth[k],
				number_of_input_channels);
			if (azimuth[k] <= 0) {  // from -90 -> 0
				float input_l = *source_l++;
				float input_r = *source_r++;
				*destination_l++ =
					static_cast<float>(input_l + input_r * desired_gain_l);
				*destination_r++ = static_cast<float>(input_r * desired_gain_r);
			}
			else {  // from 0 -> +90
				float input_l = *source_l++;
				float input_r = *source_r++;
				*destination_l++ = static_cast<float>(input_l * desired_gain_l);
				*destination_r++ =
					static_cast<float>(input_r + input_l * desired_gain_r);
			}
		}
	}
}

//==============================================================================


//namespace AudioUtilities {
//
//double decibelsToLinear(double decibels)
//{
//	return pow(10.0, 0.05 * decibels);
//}
//
//double linearToDecibels(double linear)
//{
//	// It's not possible to calculate decibels for a zero linear value since it would be -Inf.
//	// -1000.0 dB represents a very tiny linear value in case we ever reach this case.
//	ASSERT(linear);
//	if (!linear)
//		return -1000.0;
//
//	return 20.0 * log10(linear);
//}
//
//double discreteTimeConstantForSampleRate(double timeConstant, double sampleRate)
//{
//	// hardcoded value is temporary build fix for Windows.
//	// FIXME: replace hardcode 2.718282 with M_E until the correct MathExtras.h solution is determined.
//	return 1.0 - pow(1.0 / 2.718282, 1.0 / (sampleRate * timeConstant));
//}
//
//} // AudioUtilites


//float distanceConeGain(
//	const AudioListenerParams& listener,
//	const AudioEmitterParams& emitter,
//	const DistanceEffect* distanceEffect,
//	const ConeEffect* coneEffect,
//	float* outDistanceGain,
//	float* outConeGain)
//{
//	Vector3 listenerPosition = listener.m_position;//listener()->position();
//
//	double listenerDistance = Vector3::distance(emitter.m_position, listenerPosition);
//	double distanceGain = distanceEffect->gain(listenerDistance);
//
//	*outDistanceGain = static_cast<float>(distanceGain);
//
//	// FIXME: could optimize by caching coneGain
//	double coneGain = coneEffect->gain(emitter.m_position, emitter.m_direction, listenerPosition);
//
//	*outConeGain = static_cast<float>(coneGain);
//
//	return float(distanceGain * coneGain);
//}

float CalculateDistanceConeGain(
	const Vector3& position,
	const Vector3& orientation,
	const Vector3& listener_position,
	DistanceEffect* distance_effect,
	ConeEffect* cone_effect) {
	double listener_distance = Vector3::distance(position, listener_position);
	double distance_gain = distance_effect->Gain(listener_distance);
	double cone_gain = cone_effect->Gain(position, orientation, listener_position);

	return float(distance_gain * cone_gain);
}

void CalculateAzimuthElevation(
	double* out_azimuth,
	double* out_elevation,
	const Vector3& position,
	const Vector3& listener_position,
	const Vector3& listener_forward,
	const Vector3& listener_up) {
	// Calculate the source-listener vector
	Vector3 source_listener = position - listener_position;

	// Quick default return if the source and listener are at the same position.
	if (source_listener.isZero()) {
		*out_azimuth = 0;
		*out_elevation = 0;
		return;
	}

	// normalize() does nothing if the length of |sourceListener| is zero.
	source_listener.normalize();

	// Align axes
	Vector3 listener_right = Vector3::cross(listener_forward, listener_up);
	listener_right.normalize();

	Vector3 listener_forward_norm = listener_forward;
	listener_forward_norm.normalize();

	Vector3 up = Vector3::cross(listener_right, listener_forward_norm);

	float up_projection = Vector3::dot(source_listener, up);

	Vector3 projected_source = source_listener - up_projection * up;

	double azimuth = rad2deg(AngleBetween(projected_source, listener_right));
	FixNANs(azimuth);  // avoid illegal values

	// Source  in front or behind the listener
	double front_back = Vector3::dot(projected_source, listener_forward_norm);
	if (front_back < 0.0)
		azimuth = 360.0 - azimuth;

	// Make azimuth relative to "front" and not "right" listener vector
	if ((azimuth >= 0.0) && (azimuth <= 270.0))
		azimuth = 90.0 - azimuth;
	else
		azimuth = 450.0 - azimuth;

	// Elevation
	double elevation = 90 - rad2deg(AngleBetween(source_listener, up));
	FixNANs(elevation);  // avoid illegal values

	if (elevation > 90.0)
		elevation = 180.0 - elevation;
	else if (elevation < -90.0)
		elevation = -180.0 - elevation;

	if (out_azimuth)
		*out_azimuth = azimuth;
	if (out_elevation)
		*out_elevation = elevation;
}

} // namespace blink
} // namespace detail
} // namespace ln

