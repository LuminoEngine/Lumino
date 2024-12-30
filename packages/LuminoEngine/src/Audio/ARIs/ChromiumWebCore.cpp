
#include "Internal.hpp"
#include <float.h>
#include "ChromiumWebCore.hpp"



namespace ln {
namespace detail {
namespace blink {

using namespace std;

constexpr double kPiDouble = 3.14159265358979323846;
constexpr double kPiOverTwoDouble = kPiDouble / 2;
constexpr double kTwoPiDouble = kPiDouble * 2.0;

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
// SincResampler

// Input buffer layout, dividing the total buffer into regions (r0 - r5):
//
// |----------------|-----------------------------------------|----------------|
//
//                                     blockSize + kernelSize / 2
//                   <--------------------------------------------------------->
//                                                r0
//
//   kernelSize / 2   kernelSize / 2          kernelSize / 2     kernelSize / 2
// <---------------> <--------------->       <---------------> <--------------->
//         r1                r2                      r3               r4
//
//                                                     blockSize
//                                    <---------------------------------------->
//                                                         r5

// The Algorithm:
//
// 1) Consume input frames into r0 (r1 is zero-initialized).
// 2) Position kernel centered at start of r0 (r2) and generate output frames
//    until kernel is centered at start of r4, or we've finished generating
//    all the output frames.
// 3) Copy r3 to r1 and r4 to r2.
// 4) Consume input frames into r5 (zero-pad if we run out of input).
// 5) Goto (2) until all of input is consumed.
//
// note: we're glossing over how the sub-sample handling works with
// m_virtualSourceIndex, etc.


SincResampler::SincResampler(double scale_factor,
	unsigned kernel_size,
	unsigned number_of_kernel_offsets)
	: scale_factor_(scale_factor),
	kernel_size_(kernel_size),
	number_of_kernel_offsets_(number_of_kernel_offsets),
	kernel_storage_(kernel_size_ * (number_of_kernel_offsets_ + 1)),
	virtual_source_index_(0),
	block_size_(512),
	// See input buffer layout above.
	input_buffer_(block_size_ + kernel_size_),
	source_(nullptr),
	source_frames_available_(0),
	source_provider_(nullptr),
	is_buffer_primed_(false) {
	InitializeKernel();
}

void SincResampler::InitializeKernel() {
	// Blackman window parameters.
	double alpha = 0.16;
	double a0 = 0.5 * (1.0 - alpha);
	double a1 = 0.5;
	double a2 = 0.5 * alpha;

	// sincScaleFactor is basically the normalized cutoff frequency of the
	// low-pass filter.
	double sinc_scale_factor = scale_factor_ > 1.0 ? 1.0 / scale_factor_ : 1.0;

	// The sinc function is an idealized brick-wall filter, but since we're
	// windowing it the transition from pass to stop does not happen right away.
	// So we should adjust the lowpass filter cutoff slightly downward to avoid
	// some aliasing at the very high-end.
	// FIXME: this value is empirical and to be more exact should vary depending
	// on m_kernelSize.
	sinc_scale_factor *= 0.9;

	int n = kernel_size_;
	int half_size = n / 2;

	// Generates a set of windowed sinc() kernels.
	// We generate a range of sub-sample offsets from 0.0 to 1.0.
	for (unsigned offset_index = 0; offset_index <= number_of_kernel_offsets_;
		++offset_index) {
		double subsample_offset =
			static_cast<double>(offset_index) / number_of_kernel_offsets_;

		for (int i = 0; i < n; ++i) {
			// Compute the sinc() with offset.
			double s =
				sinc_scale_factor * kPiDouble * (i - half_size - subsample_offset);
			double sinc = !s ? 1.0 : std::sin(s) / s;
			sinc *= sinc_scale_factor;

			// Compute Blackman window, matching the offset of the sinc().
			double x = (i - subsample_offset) / n;
			double window = a0 - a1 * std::cos(kTwoPiDouble * x) +
				a2 * std::cos(kTwoPiDouble * 2.0 * x);

			// Window the sinc() function and store at the correct offset.
			kernel_storage_[i + offset_index * kernel_size_] = sinc * window;
		}
	}
}

void SincResampler::ConsumeSource(float* buffer,
	unsigned number_of_source_frames) {
	DCHECK(source_provider_);
	if (!source_provider_)
		return;

	source_provider_->ProvideInput(buffer, number_of_source_frames);
}

namespace {

	// BufferSourceProvider is an AudioSourceProvider wrapping an in-memory buffer.

	class BufferSourceProvider final : public AudioSourceProvider {
	public:
		BufferSourceProvider(const float* source, size_t number_of_source_frames)
			: source_(source), source_frames_available_(number_of_source_frames) {}

		// Consumes samples from the in-memory buffer.
		void ProvideInput(float* mutableData, size_t frames_to_process) override {
			DCHECK(source_);
			DCHECK(mutableData);
			if (!source_ || !mutableData)
				return;

			float* buffer = mutableData;

			// Clamp to number of frames available and zero-pad.
			size_t frames_to_copy =
				std::min(source_frames_available_, frames_to_process);
			memcpy(buffer, source_, sizeof(float) * frames_to_copy);

			// Zero-pad if necessary.
			if (frames_to_copy < frames_to_process)
				memset(buffer + frames_to_copy, 0,
					sizeof(float) * (frames_to_process - frames_to_copy));

			source_frames_available_ -= frames_to_copy;
			source_ += frames_to_copy;
		}

	private:
		const float* source_;
		size_t source_frames_available_;
	};

}  // namespace

void SincResampler::Process(const float* source,
	float* destination,
	unsigned number_of_source_frames,
	unsigned number_of_output_destination_frames) {
	// Resample an in-memory buffer using an AudioSourceProvider.
	BufferSourceProvider source_provider(source, number_of_source_frames);

#if 0
	unsigned number_of_destination_frames =
		static_cast<unsigned>(std::ceil(static_cast<float>(number_of_source_frames) / scale_factor_));
	//float* last = destination + number_of_destination_frames - 1;
#else
	unsigned number_of_destination_frames =
		static_cast<unsigned>(number_of_source_frames / scale_factor_);
	float* last = destination + number_of_destination_frames;
#endif
	unsigned remaining = number_of_destination_frames;



	auto* firstd = destination;
	while (remaining) {
		unsigned frames_this_time = std::min(remaining, block_size_);
		Process(&source_provider, destination, frames_this_time);

		destination += frames_this_time;
		remaining -= frames_this_time;
	}

	if (number_of_destination_frames < number_of_output_destination_frames) {

		float realRemaining = (static_cast<float>(number_of_source_frames) / scale_factor_) - number_of_destination_frames;
		assert(realRemaining < 1.0f);

		*last = Math::lerp(*(last - 1), source[number_of_source_frames - 1], realRemaining);
		
	}

	//float mmin = 0;
	//float mmax = 0;
	//for (int i = 0; i < 2048; i++) {
	//	mmin = std::min(firstd[i], mmin);
	//	mmax = std::max(firstd[i], mmax);
	//}
	//printf("mm: %f %f\n", mmin, mmax);
	//*last = *(last - 1);

	//memset(last - 32, 0, sizeof(float) *  32);

	//printf("%d\n", destination - firstd);
	//memset(firstd, 0, sizeof(float) *  (number_of_destination_frames + 1));
	//*last = 0;
	//*destination = 0;
	//memset(firstd, 0, sizeof(float) *  (number_of_destination_frames + 1));
	//memcpy(firstd, source, sizeof(float) *  number_of_source_frames);
}

void SincResampler::Process(AudioSourceProvider* source_provider,
	float* destination,
	size_t frames_to_process) {
	bool is_good = source_provider && block_size_ > kernel_size_ &&
		input_buffer_.size() >= block_size_ + kernel_size_ &&
		!(kernel_size_ % 2);
	DCHECK(is_good);
	if (!is_good)
		return;

	source_provider_ = source_provider;

	unsigned number_of_destination_frames = frames_to_process;

	// Setup various region pointers in the buffer (see diagram above).
	float* r0 = input_buffer_.Data() + kernel_size_ / 2;
	float* r1 = input_buffer_.Data();
	float* r2 = r0;
	float* r3 = r0 + block_size_ - kernel_size_ / 2;
	float* r4 = r0 + block_size_;
	float* r5 = r0 + kernel_size_ / 2;

	// Step (1)
	// Prime the input buffer at the start of the input stream.
	if (!is_buffer_primed_) {
		ConsumeSource(r0, block_size_ + kernel_size_ / 2);
		is_buffer_primed_ = true;
	}

	// Step (2)

	while (number_of_destination_frames) {
		while (virtual_source_index_ < block_size_) {
			// m_virtualSourceIndex lies in between two kernel offsets so figure out
			// what they are.
			int source_index_i = static_cast<int>(virtual_source_index_);
			double subsample_remainder = virtual_source_index_ - source_index_i;

			double virtual_offset_index =
				subsample_remainder * number_of_kernel_offsets_;
			int offset_index = static_cast<int>(virtual_offset_index);

			float* k1 = kernel_storage_.Data() + offset_index * kernel_size_;
			float* k2 = k1 + kernel_size_;

			// Initialize input pointer based on quantized m_virtualSourceIndex.
			float* input_p = r1 + source_index_i;

			// We'll compute "convolutions" for the two kernels which straddle
			// m_virtualSourceIndex
			float sum1 = 0;
			float sum2 = 0;

			// Figure out how much to weight each kernel's "convolution".
			double kernel_interpolation_factor = virtual_offset_index - offset_index;

			// Generate a single output sample.
			int n = kernel_size_;

#define CONVOLVE_ONE_SAMPLE() \
  do {                        \
    input = *input_p++;       \
    sum1 += input * *k1;      \
    sum2 += input * *k2;      \
    ++k1;                     \
    ++k2;                     \
  } while (0)

			{
				float input;

#if defined(ARCH_CPU_X86_FAMILY)
				// If the sourceP address is not 16-byte aligned, the first several
				// frames (at most three) should be processed seperately.
				while ((reinterpret_cast<uintptr_t>(input_p) & 0x0F) && n) {
					CONVOLVE_ONE_SAMPLE();
					n--;
				}

				// Now the inputP is aligned and start to apply SSE.
				float* end_p = input_p + n - n % 4;
				__m128 m_input;
				__m128 m_k1;
				__m128 m_k2;
				__m128 mul1;
				__m128 mul2;

				__m128 sums1 = _mm_setzero_ps();
				__m128 sums2 = _mm_setzero_ps();
				bool k1_aligned = !(reinterpret_cast<uintptr_t>(k1) & 0x0F);
				bool k2_aligned = !(reinterpret_cast<uintptr_t>(k2) & 0x0F);

#define LOAD_DATA(l1, l2)           \
  do {                              \
    m_input = _mm_load_ps(input_p); \
    m_k1 = _mm_##l1##_ps(k1);       \
    m_k2 = _mm_##l2##_ps(k2);       \
  } while (0)

#define CONVOLVE_4_SAMPLES()          \
  do {                                \
    mul1 = _mm_mul_ps(m_input, m_k1); \
    mul2 = _mm_mul_ps(m_input, m_k2); \
    sums1 = _mm_add_ps(sums1, mul1);  \
    sums2 = _mm_add_ps(sums2, mul2);  \
    input_p += 4;                     \
    k1 += 4;                          \
    k2 += 4;                          \
  } while (0)

				if (k1_aligned && k2_aligned) {  // both aligned
					while (input_p < end_p) {
						LOAD_DATA(load, load);
						CONVOLVE_4_SAMPLES();
					}
				}
				else if (!k1_aligned && k2_aligned) {  // only k2 aligned
					while (input_p < end_p) {
						LOAD_DATA(loadu, load);
						CONVOLVE_4_SAMPLES();
					}
				}
				else if (k1_aligned && !k2_aligned) {  // only k1 aligned
					while (input_p < end_p) {
						LOAD_DATA(load, loadu);
						CONVOLVE_4_SAMPLES();
					}
				}
				else {  // both non-aligned
					while (input_p < end_p) {
						LOAD_DATA(loadu, loadu);
						CONVOLVE_4_SAMPLES();
					}
				}

				// Summarize the SSE results to sum1 and sum2.
				float* group_sum_p = reinterpret_cast<float*>(&sums1);
				sum1 +=
					group_sum_p[0] + group_sum_p[1] + group_sum_p[2] + group_sum_p[3];
				group_sum_p = reinterpret_cast<float*>(&sums2);
				sum2 +=
					group_sum_p[0] + group_sum_p[1] + group_sum_p[2] + group_sum_p[3];

				n %= 4;
				while (n) {
					CONVOLVE_ONE_SAMPLE();
					n--;
				}
#else
				// FIXME: add ARM NEON optimizations for the following. The scalar
				// code-path can probably also be optimized better.

				// Optimize size 32 and size 64 kernels by unrolling the while loop.
				// A 20 - 30% speed improvement was measured in some cases by using this
				// approach.

				if (n == 32) {
					CONVOLVE_ONE_SAMPLE();  // 1
					CONVOLVE_ONE_SAMPLE();  // 2
					CONVOLVE_ONE_SAMPLE();  // 3
					CONVOLVE_ONE_SAMPLE();  // 4
					CONVOLVE_ONE_SAMPLE();  // 5
					CONVOLVE_ONE_SAMPLE();  // 6
					CONVOLVE_ONE_SAMPLE();  // 7
					CONVOLVE_ONE_SAMPLE();  // 8
					CONVOLVE_ONE_SAMPLE();  // 9
					CONVOLVE_ONE_SAMPLE();  // 10
					CONVOLVE_ONE_SAMPLE();  // 11
					CONVOLVE_ONE_SAMPLE();  // 12
					CONVOLVE_ONE_SAMPLE();  // 13
					CONVOLVE_ONE_SAMPLE();  // 14
					CONVOLVE_ONE_SAMPLE();  // 15
					CONVOLVE_ONE_SAMPLE();  // 16
					CONVOLVE_ONE_SAMPLE();  // 17
					CONVOLVE_ONE_SAMPLE();  // 18
					CONVOLVE_ONE_SAMPLE();  // 19
					CONVOLVE_ONE_SAMPLE();  // 20
					CONVOLVE_ONE_SAMPLE();  // 21
					CONVOLVE_ONE_SAMPLE();  // 22
					CONVOLVE_ONE_SAMPLE();  // 23
					CONVOLVE_ONE_SAMPLE();  // 24
					CONVOLVE_ONE_SAMPLE();  // 25
					CONVOLVE_ONE_SAMPLE();  // 26
					CONVOLVE_ONE_SAMPLE();  // 27
					CONVOLVE_ONE_SAMPLE();  // 28
					CONVOLVE_ONE_SAMPLE();  // 29
					CONVOLVE_ONE_SAMPLE();  // 30
					CONVOLVE_ONE_SAMPLE();  // 31
					CONVOLVE_ONE_SAMPLE();  // 32
				}
				else if (n == 64) {
					CONVOLVE_ONE_SAMPLE();  // 1
					CONVOLVE_ONE_SAMPLE();  // 2
					CONVOLVE_ONE_SAMPLE();  // 3
					CONVOLVE_ONE_SAMPLE();  // 4
					CONVOLVE_ONE_SAMPLE();  // 5
					CONVOLVE_ONE_SAMPLE();  // 6
					CONVOLVE_ONE_SAMPLE();  // 7
					CONVOLVE_ONE_SAMPLE();  // 8
					CONVOLVE_ONE_SAMPLE();  // 9
					CONVOLVE_ONE_SAMPLE();  // 10
					CONVOLVE_ONE_SAMPLE();  // 11
					CONVOLVE_ONE_SAMPLE();  // 12
					CONVOLVE_ONE_SAMPLE();  // 13
					CONVOLVE_ONE_SAMPLE();  // 14
					CONVOLVE_ONE_SAMPLE();  // 15
					CONVOLVE_ONE_SAMPLE();  // 16
					CONVOLVE_ONE_SAMPLE();  // 17
					CONVOLVE_ONE_SAMPLE();  // 18
					CONVOLVE_ONE_SAMPLE();  // 19
					CONVOLVE_ONE_SAMPLE();  // 20
					CONVOLVE_ONE_SAMPLE();  // 21
					CONVOLVE_ONE_SAMPLE();  // 22
					CONVOLVE_ONE_SAMPLE();  // 23
					CONVOLVE_ONE_SAMPLE();  // 24
					CONVOLVE_ONE_SAMPLE();  // 25
					CONVOLVE_ONE_SAMPLE();  // 26
					CONVOLVE_ONE_SAMPLE();  // 27
					CONVOLVE_ONE_SAMPLE();  // 28
					CONVOLVE_ONE_SAMPLE();  // 29
					CONVOLVE_ONE_SAMPLE();  // 30
					CONVOLVE_ONE_SAMPLE();  // 31
					CONVOLVE_ONE_SAMPLE();  // 32
					CONVOLVE_ONE_SAMPLE();  // 33
					CONVOLVE_ONE_SAMPLE();  // 34
					CONVOLVE_ONE_SAMPLE();  // 35
					CONVOLVE_ONE_SAMPLE();  // 36
					CONVOLVE_ONE_SAMPLE();  // 37
					CONVOLVE_ONE_SAMPLE();  // 38
					CONVOLVE_ONE_SAMPLE();  // 39
					CONVOLVE_ONE_SAMPLE();  // 40
					CONVOLVE_ONE_SAMPLE();  // 41
					CONVOLVE_ONE_SAMPLE();  // 42
					CONVOLVE_ONE_SAMPLE();  // 43
					CONVOLVE_ONE_SAMPLE();  // 44
					CONVOLVE_ONE_SAMPLE();  // 45
					CONVOLVE_ONE_SAMPLE();  // 46
					CONVOLVE_ONE_SAMPLE();  // 47
					CONVOLVE_ONE_SAMPLE();  // 48
					CONVOLVE_ONE_SAMPLE();  // 49
					CONVOLVE_ONE_SAMPLE();  // 50
					CONVOLVE_ONE_SAMPLE();  // 51
					CONVOLVE_ONE_SAMPLE();  // 52
					CONVOLVE_ONE_SAMPLE();  // 53
					CONVOLVE_ONE_SAMPLE();  // 54
					CONVOLVE_ONE_SAMPLE();  // 55
					CONVOLVE_ONE_SAMPLE();  // 56
					CONVOLVE_ONE_SAMPLE();  // 57
					CONVOLVE_ONE_SAMPLE();  // 58
					CONVOLVE_ONE_SAMPLE();  // 59
					CONVOLVE_ONE_SAMPLE();  // 60
					CONVOLVE_ONE_SAMPLE();  // 61
					CONVOLVE_ONE_SAMPLE();  // 62
					CONVOLVE_ONE_SAMPLE();  // 63
					CONVOLVE_ONE_SAMPLE();  // 64
				}
				else {
					while (n--) {
						// Non-optimized using actual while loop.
						CONVOLVE_ONE_SAMPLE();
					}
				}
#endif
			}
#undef CONVOLVE_ONE_SAMPLE

			// Linearly interpolate the two "convolutions".
			double result = (1.0 - kernel_interpolation_factor) * sum1 +
				kernel_interpolation_factor * sum2;

			*destination++ = result;

			// Advance the virtual index.
			virtual_source_index_ += scale_factor_;

			--number_of_destination_frames;
			if (!number_of_destination_frames)
				return;
		}

		// Wrap back around to the start.
		virtual_source_index_ -= block_size_;

		// Step (3) Copy r3 to r1 and r4 to r2.
		// This wraps the last input frames back to the start of the buffer.
		memcpy(r1, r3, sizeof(float) * (kernel_size_ / 2));
		memcpy(r2, r4, sizeof(float) * (kernel_size_ / 2));

		// Step (4)
		// Refresh the buffer with more input.
		ConsumeSource(r5, block_size_);
	}
}



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
	source_listener.mutatingNormalize();

	// Align axes
	Vector3 listener_right = Vector3::cross(listener_forward, listener_up);
	listener_right.mutatingNormalize();

	Vector3 listener_forward_norm = listener_forward;
	listener_forward_norm.mutatingNormalize();

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

