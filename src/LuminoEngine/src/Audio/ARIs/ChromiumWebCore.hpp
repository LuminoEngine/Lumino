// License: BSD 3 Clause
// Copyright (C) 2010, Google Inc. All rights reserved.
// Copyright (C) 2018+, The Lumino Authors. All rights reserved.

// chromium/third_party/blink/renderer/platform
// chromium/third_party/blink/renderer/modules/webaudio

#pragma once

#include "ARIAudioBus.hpp"

#define PLATFORM_EXPORT
#define USING_FAST_MALLOC(x)
#define WTF_MAKE_NONCOPYABLE(x)
#define DISALLOW_COPY_AND_ASSIGN(x)

#define ASSERT assert
#define NOTREACHED LN_UNREACHABLE
#define DCHECK LN_DCHECK
#define SECURITY_DCHECK LN_DCHECK
#define WTF_HEAP_PROFILER_TYPE_NAME(x) nullptr

#define CHECK LN_CHECK
#define CHECK_EQ(x, y) CHECK((x) == (y))
#define CHECK_NE(x, y) CHECK((x) != (y))
#define CHECK_LE(x, y) CHECK((x) <= (y))
#define CHECK_LT(x, y) CHECK((x) < (y))
#define CHECK_GE(x, y) CHECK((x) >= (y))
#define CHECK_GT(x, y) CHECK((x) > (y))

namespace ln {
namespace detail {

namespace WTF {
	class Partitions {
	public:
		static void* FastMalloc(size_t n, const char* type_name) {
			return malloc(n);
		}
		static void FastFree(void* p) {
			free(p);
		}
	};
}

namespace blink {
using AudioBus = ARIAudioBus;

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

// https://github.com/chromium/chromium/blob/master/third_party/blink/renderer/platform/audio/audio_array.h

template <typename T>
class AudioArray {
	USING_FAST_MALLOC(AudioArray);
	WTF_MAKE_NONCOPYABLE(AudioArray);

public:
	AudioArray() : allocation_(nullptr), aligned_data_(nullptr), size_(0) {}
	explicit AudioArray(size_t n)
		: allocation_(nullptr), aligned_data_(nullptr), size_(0) {
		Allocate(n);
	}

	~AudioArray() { WTF::Partitions::FastFree(allocation_); }

	// It's OK to call allocate() multiple times, but data will *not* be copied
	// from an initial allocation if re-allocated. Allocations are
	// zero-initialized.
	void Allocate(size_t n) {
		// Although n is a size_t, its true limit is max unsigned because we use
		// unsigned in zeroRange() and copyToRange(). Also check for integer
		// overflow.
		CHECK_LE(n, std::numeric_limits<unsigned>::max() / sizeof(T));

		unsigned initial_size = sizeof(T) * n;

#if defined(ARCH_CPU_X86_FAMILY) || defined(WTF_USE_WEBAUDIO_FFMPEG) || \
    defined(WTF_USE_WEBAUDIO_OPENMAX_DL_FFT)
		const size_t kAlignment = 32;
#else
		const size_t kAlignment = 16;
#endif

		if (allocation_)
			WTF::Partitions::FastFree(allocation_);

		bool is_allocation_good = false;

		while (!is_allocation_good) {
			// Initially we try to allocate the exact size, but if it's not aligned
			// then we'll have to reallocate and from then on allocate extra.
			static size_t extra_allocation_bytes = 0;

			// Again, check for integer overflow.
			CHECK_GE(initial_size + extra_allocation_bytes, initial_size);

			T* allocation = static_cast<T*>(WTF::Partitions::FastMalloc(
				initial_size + extra_allocation_bytes,
				WTF_HEAP_PROFILER_TYPE_NAME(AudioArray<T>)));
			CHECK(allocation);

			T* aligned_data = AlignedAddress(allocation, kAlignment);

			if (aligned_data == allocation || extra_allocation_bytes == kAlignment) {
				allocation_ = allocation;
				aligned_data_ = aligned_data;
				size_ = n;
				is_allocation_good = true;
				Zero();
			}
			else {
				// always allocate extra after the first alignment failure.
				extra_allocation_bytes = kAlignment;
				WTF::Partitions::FastFree(allocation);
			}
		}
	}

	T* Data() { return aligned_data_; }
	const T* Data() const { return aligned_data_; }
	size_t size() const { return size_; }

	T& at(size_t i) {
		// Note that although it is a size_t, m_size is now guaranteed to be
		// no greater than max unsigned. This guarantee is enforced in allocate().
		SECURITY_DCHECK(i < size());
		return Data()[i];
	}

	T& operator[](size_t i) { return at(i); }

	void Zero() {
		// This multiplication is made safe by the check in allocate().
		memset(this->Data(), 0, sizeof(T) * this->size());
	}

	void ZeroRange(unsigned start, unsigned end) {
		bool is_safe = (start <= end) && (end <= this->size());
		DCHECK(is_safe);
		if (!is_safe)
			return;

		// This expression cannot overflow because end - start cannot be
		// greater than m_size, which is safe due to the check in allocate().
		memset(this->Data() + start, 0, sizeof(T) * (end - start));
	}

	void CopyToRange(const T* source_data, unsigned start, unsigned end) {
		bool is_safe = (start <= end) && (end <= this->size());
		DCHECK(is_safe);
		if (!is_safe)
			return;

		// This expression cannot overflow because end - start cannot be
		// greater than m_size, which is safe due to the check in allocate().
		memcpy(this->Data() + start, source_data, sizeof(T) * (end - start));
	}

private:
	static T* AlignedAddress(T* address, intptr_t alignment) {
		intptr_t value = reinterpret_cast<intptr_t>(address);
		return reinterpret_cast<T*>((value + alignment - 1) & ~(alignment - 1));
	}

	T* allocation_;
	T* aligned_data_;
	size_t size_;
};

typedef AudioArray<float> AudioFloatArray;
typedef AudioArray<double> AudioDoubleArray;

// https://github.com/chromium/chromium/blob/master/third_party/blink/renderer/platform/audio/sinc_resampler.h

class AudioSourceProviderClient;

// Abstract base-class for a pull-model client.
class PLATFORM_EXPORT AudioSourceProvider {
	USING_FAST_MALLOC(AudioSourceProvider);
	DISALLOW_COPY_AND_ASSIGN(AudioSourceProvider);

public:
	AudioSourceProvider() = default;

	// provideInput() gets called repeatedly to render time-slices of a continuous
	// audio stream.
	virtual void ProvideInput(float* mutableData, size_t frames_to_process) = 0;

	// If a client is set, we call it back when the audio format is available or
	// changes.
	virtual void SetClient(AudioSourceProviderClient*) {};

	virtual ~AudioSourceProvider() = default;
};

// SincResampler is a high-quality sample-rate converter.
class PLATFORM_EXPORT SincResampler {
	USING_FAST_MALLOC(SincResampler);
	WTF_MAKE_NONCOPYABLE(SincResampler);

public:
	// scaleFactor == sourceSampleRate / destinationSampleRate
	// kernelSize can be adjusted for quality (higher is better)
	// numberOfKernelOffsets is used for interpolation and is the number of
	// sub-sample kernel shifts.
	SincResampler(double scale_factor,
		unsigned kernel_size = 32,
		unsigned number_of_kernel_offsets = 32);

	// Processes numberOfSourceFrames from source to produce numberOfSourceFrames
	// / scaleFactor frames in destination.
	void Process(const float* source,
		float* destination,
		unsigned number_of_source_frames,
		unsigned number_of_output_destination_frames);

	// Process with input source callback function for streaming applications.
	void Process(AudioSourceProvider*,
		float* destination,
		size_t frames_to_process);

protected:
	void InitializeKernel();
	void ConsumeSource(float* buffer, unsigned number_of_source_frames);

	double scale_factor_;
	unsigned kernel_size_;
	unsigned number_of_kernel_offsets_;

	// m_kernelStorage has m_numberOfKernelOffsets kernels back-to-back, each of
	// size m_kernelSize.  The kernel offsets are sub-sample shifts of a windowed
	// sinc() shifted from 0.0 to 1.0 sample.
	AudioFloatArray kernel_storage_;

	// m_virtualSourceIndex is an index on the source input buffer with sub-sample
	// precision.  It must be double precision to avoid drift.
	double virtual_source_index_;

	// This is the number of destination frames we generate per processing pass on
	// the buffer.
	unsigned block_size_;

	// Source is copied into this buffer for each processing pass.
	AudioFloatArray input_buffer_;

	const float* source_;
	unsigned source_frames_available_;

	// m_sourceProvider is used to provide the audio input stream to the
	// resampler.
	AudioSourceProvider* source_provider_;

	// The buffer is primed once at the very beginning of processing.
	bool is_buffer_primed_;
};

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

