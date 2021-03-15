
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioBus.hpp>
#include "ARIs/ChromiumWebCore.hpp"
#include "ARIs/VectorMath.h"

#if 0

namespace ln {

//==============================================================================
// AudioChannel

AudioChannel::AudioChannel(size_t length)
	: m_isSilent(true)
{
	m_data.resize(length);
}

void AudioChannel::setSilentAndZero()
{
    memset(mutableData(), 0, sizeof(float) * length());
	if (!m_isSilent)
	{
		m_isSilent = true;
	}
}

void AudioChannel::copyTo(float* buffer, size_t bufferLength, size_t stride) const
{
	const float* src = constData();
	if (stride == 1) {
		memcpy(buffer, src, sizeof(float) * length());
	}
	else {
		size_t n = length();
		while (n--) {
			*buffer = *src;
			src++;
			buffer += stride;
		}
	}
}

void AudioChannel::copyFrom(const float * buffer, size_t bufferLength, size_t stride)
{
	float* dst = mutableData();
	if (stride == 1) {
		memcpy(dst, buffer, sizeof(float) * length());
	}
	else {
		size_t n = length();
		while (n--) {
			*dst = *buffer;
			dst++;
			buffer += stride;
		}
	}
}

void AudioChannel::copyFrom(const AudioChannel* ch)
{
	bool isSafe = (ch && ch->length() >= length());
	assert(isSafe);
	if (!isSafe) {
		return;
	}

	if (ch->isSilent()) {
		setSilentAndZero();
		return;
	}

	memcpy(mutableData(), ch->constData(), sizeof(float) * length());
}

void AudioChannel::sumFrom(const AudioChannel * ch)
{
	if (ch->isSilent()) {
		return;
	}

	if (isSilent()) {
		// optimize for first time.
		copyFrom(ch);
	}
	else {
		::blink::VectorMath::vadd(constData(), 1, ch->constData(), 1, mutableData(), 1, length());
	}
}

void AudioChannel::fillZero(size_t start, size_t length)
{
    if (LN_REQUIRE(start + length <= m_data.size())) return;
    memset(m_data.data() + start, 0, sizeof(float) * length);
}

//==============================================================================
// AudioBus

const unsigned kMaxBusChannels = 32;

ARIAudioBus::ARIAudioBus()
{
}

void ARIAudioBus::initialize2(int channelCount, size_t length, int sampleRate)
{
	for (int i = 0; i < channelCount; ++i)
	{
		m_channels.add(makeRef<AudioChannel>(length));
	}
	m_validLength = length;
	m_sampleRate = sampleRate;
}

AudioChannel* ARIAudioBus::channelByType(unsigned channel_type)
{
	// For now we only support canonical channel layouts...
	if (m_layout != kLayoutCanonical)
		return nullptr;

	switch (NumberOfChannels()) {
	case 1:  // mono
		if (channel_type == kChannelMono || channel_type == kChannelLeft)
			return Channel(0);
		return nullptr;

	case 2:  // stereo
		switch (channel_type) {
		case kChannelLeft:
			return Channel(0);
		case kChannelRight:
			return Channel(1);
		default:
			return nullptr;
		}

	case 4:  // quad
		switch (channel_type) {
		case kChannelLeft:
			return Channel(0);
		case kChannelRight:
			return Channel(1);
		case kChannelSurroundLeft:
			return Channel(2);
		case kChannelSurroundRight:
			return Channel(3);
		default:
			return nullptr;
		}

	case 5:  // 5.0
		switch (channel_type) {
		case kChannelLeft:
			return Channel(0);
		case kChannelRight:
			return Channel(1);
		case kChannelCenter:
			return Channel(2);
		case kChannelSurroundLeft:
			return Channel(3);
		case kChannelSurroundRight:
			return Channel(4);
		default:
			return nullptr;
		}

	case 6:  // 5.1
		switch (channel_type) {
		case kChannelLeft:
			return Channel(0);
		case kChannelRight:
			return Channel(1);
		case kChannelCenter:
			return Channel(2);
		case kChannelLFE:
			return Channel(3);
		case kChannelSurroundLeft:
			return Channel(4);
		case kChannelSurroundRight:
			return Channel(5);
		default:
			return nullptr;
		}
	}

	LN_UNREACHABLE();
	return nullptr;
}

const AudioChannel* ARIAudioBus::channelByType(unsigned type) const
{
	return const_cast<ARIAudioBus*>(this)->channelByType(type);
}

void ARIAudioBus::setSilentAndZero()
{
	for (auto& ch : m_channels) {
		ch->setSilentAndZero();
	}
}

void ARIAudioBus::clearSilentFlag()
{
	for (auto& ch : m_channels) {
		ch->clearSilentFlag();
	}
}

bool ARIAudioBus::isSilent() const
{
	for (auto& ch : m_channels) {
		if (!ch->isSilent()) {
			return false;
		}
	}
	return true;
}

void ARIAudioBus::fillZero(size_t start, size_t length)
{
    for (int i = 0; i < m_channels.size(); i++) {
        m_channels[i]->fillZero(start, length);
    }
}

void ARIAudioBus::mergeToChannelBuffers(float* buffer, size_t length)
{
	assert(m_channels.size() == 2);
	assert(m_channels[0]->length() * 2 == length);

	if (isSilent()) {
		memset(buffer, 0, sizeof(float) * length);
		return;
	}

	for (int i = 0; i < m_channels.size(); i++)
	{
		m_channels[i]->copyTo(buffer + i, length - i, m_channels.size());
	}
}

void ARIAudioBus::separateFrom(const float * buffer, size_t length, int channelCount)
{
	//assert(m_channels.size() == 2);
	//assert(m_channels[0]->length() * 2 >= length);	// length が少ない分にはOK。多いのはあふれるのでNG

	for (int i = 0; i < m_channels.size(); i++)
	{
		m_channels[i]->copyFrom(buffer + i, length - i, channelCount);
	}
}

void ARIAudioBus::sumFrom(const ARIAudioBus* sourceBus)
{
    if (sourceBus == this) {
        return;
    }

	int dstChannels = channelCount();
    int srcChannels = sourceBus->channelCount();
	if (dstChannels == sourceBus->channelCount())
	{
		for (int i = 0; i < dstChannels; i++)
		{
			channel(i)->sumFrom(sourceBus->channel(i));
		}
	}
	else
	{
        if (srcChannels < dstChannels)
            sumFromByUpMixing(sourceBus);
        else
            sumFromByDownMixing(sourceBus);
	}
}

void ARIAudioBus::copyFrom(ARIAudioBus* source)
{
    if (source == this)
        return;

    unsigned numberOfSourceChannels = source->numberOfChannels();
    unsigned numberOfDestinationChannels = numberOfChannels();

    if (numberOfDestinationChannels == numberOfSourceChannels) {
        for (unsigned i = 0; i < numberOfSourceChannels; ++i)
            channel(i)->copyFrom(source->channel(i));
    }
    else {
        LN_NOTIMPLEMENTED();
    }
}

void ARIAudioBus::copyWithGainFrom(const ARIAudioBus& source_bus, float gain)
{
	if (!topologyMatches(source_bus)) {
		LN_UNREACHABLE();
		setSilentAndZero();
		return;
	}

	if (source_bus.isSilent()) {
		setSilentAndZero();
		return;
	}

	unsigned number_of_channels = this->NumberOfChannels();
	LN_DCHECK(number_of_channels < kMaxBusChannels);
	if (number_of_channels > kMaxBusChannels)
		return;

	// If it is copying from the same bus and no need to change gain, just return.
	if (this == &source_bus && gain == 1)
		return;

	const float* sources[kMaxBusChannels];
	float* destinations[kMaxBusChannels];

	for (unsigned i = 0; i < number_of_channels; ++i) {
		sources[i] = source_bus.Channel(i)->Data();
		destinations[i] = Channel(i)->MutableData();
	}

	unsigned frames_to_process = length();

	// Handle gains of 0 and 1 (exactly) specially.
	if (gain == 1) {
		for (unsigned channel_index = 0; channel_index < number_of_channels;
			++channel_index) {
			memcpy(destinations[channel_index], sources[channel_index],
				frames_to_process * sizeof(*destinations[channel_index]));
		}
	}
	else if (gain == 0) {
		for (unsigned channel_index = 0; channel_index < number_of_channels;
			++channel_index) {
			memset(destinations[channel_index], 0,
				frames_to_process * sizeof(*destinations[channel_index]));
		}
	}
	else {
		for (unsigned channel_index = 0; channel_index < number_of_channels;
			++channel_index) {
			::blink::VectorMath::vsmul(sources[channel_index], 1, &gain, destinations[channel_index], 1, frames_to_process);
		}
	}
}
void ARIAudioBus::copyBySampleRateConverting(
	const ARIAudioBus* source_bus,
	int new_sample_rate) {
	// sourceBus's sample-rate must be known.
	LN_DCHECK(source_bus);
	LN_DCHECK(source_bus->sampleRate());
	if (!source_bus || !source_bus->sampleRate())
		return;

	double source_sample_rate = source_bus->sampleRate();
	double destination_sample_rate = new_sample_rate;
	double sample_rate_ratio = source_sample_rate / destination_sample_rate;
	unsigned number_of_source_channels = source_bus->NumberOfChannels();

	//if (number_of_source_channels == 1)
	//	mix_to_mono = false;  // already mono

	if (source_sample_rate == destination_sample_rate) {
		// No sample-rate conversion is necessary.
		//if (mix_to_mono)
		//	return AudioBus::CreateByMixingToMono(source_bus);

		// Return exact copy.
		//return AudioBus::CreateBufferFromRange(source_bus, 0, source_bus->length());
		LN_NOTIMPLEMENTED();return;
	}

	if (source_bus->isSilent()) {
		//scoped_refptr<AudioBus> silent_bus = Create(
		//	number_of_source_channels, source_bus->length() / sample_rate_ratio);
		//silent_bus->SetSampleRate(new_sample_rate);
		//return silent_bus;
		LN_NOTIMPLEMENTED(); return;
	}

	// First, mix to mono (if necessary) then sample-rate convert.
/*	const AudioBus* resampler_source_bus;
	scoped_refptr<AudioBus> mixed_mono_bus;
	if (mix_to_mono) {
		mixed_mono_bus = AudioBus::CreateByMixingToMono(source_bus);
		resampler_source_bus = mixed_mono_bus.get();
	}
	else {
		// Directly resample without down-mixing.
		resampler_source_bus = source_bus;
	}*/
	const ARIAudioBus* resampler_source_bus = source_bus;

	// Calculate destination length based on the sample-rates.
	int source_length = resampler_source_bus->length();
//#if 1
//	int destination_length = std::ceil(static_cast<float>(source_length) / sample_rate_ratio);
//#else
//	int destination_length = source_length / sample_rate_ratio;
//#endif

	// Create destination bus with same number of channels.
	unsigned number_of_destination_channels =
		resampler_source_bus->NumberOfChannels();
	//scoped_refptr<AudioBus> destination_bus =
	//	Create(number_of_destination_channels, destination_length);

	// Sample-rate convert each channel.
	for (unsigned i = 0; i < number_of_destination_channels; ++i) {
		const float* source = resampler_source_bus->Channel(i)->Data();
		float* destination = channel(i)->mutableData();

		detail::blink::SincResampler resampler(sample_rate_ratio);
		resampler.Process(source, destination, source_length, channel(i)->length());
	}

	clearSilentFlag();
	setSampleRate(new_sample_rate);
}

// Returns true if the channel count and frame-size match.
bool ARIAudioBus::topologyMatches(const ARIAudioBus& bus) const
{
	if (NumberOfChannels() != bus.NumberOfChannels())
		return false;  // channel mismatch

					   // Make sure source bus has enough frames.
	if (length() > bus.length())
		return false;  // frame-size mismatch

	return true;
}

// チャンネル数の多い bus から少ない bus への合成。
// 例) dst=1ch, src=2ch
// https://github.com/chromium/chromium/blob/ff00deceb553460d047c7544d2494ffe8ab31d91/third_party/blink/renderer/platform/audio/audio_bus.cc#L292
void ARIAudioBus::sumFromByUpMixing(const ARIAudioBus* sourceBus)
{
    unsigned number_of_source_channels = sourceBus->NumberOfChannels();
    unsigned number_of_destination_channels = NumberOfChannels();

    if ((number_of_source_channels == 1 && number_of_destination_channels == 2) ||
        (number_of_source_channels == 1 && number_of_destination_channels == 4)) {
        // Up-mixing: 1 -> 2, 1 -> 4
        //   output.L = input
        //   output.R = input
        //   output.SL = 0 (in the case of 1 -> 4)
        //   output.SR = 0 (in the case of 1 -> 4)
        const AudioChannel* source_l = sourceBus->channelByType(kChannelLeft);
        channelByType(kChannelLeft)->sumFrom(source_l);
        channelByType(kChannelRight)->sumFrom(source_l);
    }
    else if (number_of_source_channels == 1 &&
        number_of_destination_channels == 6) {
        // Up-mixing: 1 -> 5.1
        //   output.L = 0
        //   output.R = 0
        //   output.C = input (put in center channel)
        //   output.LFE = 0
        //   output.SL = 0
        //   output.SR = 0
        channelByType(kChannelCenter)->sumFrom(sourceBus->channelByType(kChannelLeft));
    }
    else if ((number_of_source_channels == 2 &&
        number_of_destination_channels == 4) ||
        (number_of_source_channels == 2 &&
            number_of_destination_channels == 6)) {
        // Up-mixing: 2 -> 4, 2 -> 5.1
        //   output.L = input.L
        //   output.R = input.R
        //   output.C = 0 (in the case of 2 -> 5.1)
        //   output.LFE = 0 (in the case of 2 -> 5.1)
        //   output.SL = 0
        //   output.SR = 0
        channelByType(kChannelLeft)->sumFrom(sourceBus->channelByType(kChannelLeft));
        channelByType(kChannelRight)->sumFrom(sourceBus->channelByType(kChannelRight));
    }
    else if (number_of_source_channels == 4 &&
        number_of_destination_channels == 6) {
        // Up-mixing: 4 -> 5.1
        //   output.L = input.L
        //   output.R = input.R
        //   output.C = 0
        //   output.LFE = 0
        //   output.SL = input.SL
        //   output.SR = input.SR
        channelByType(kChannelLeft)->sumFrom(sourceBus->channelByType(kChannelLeft));
        channelByType(kChannelRight)->sumFrom(sourceBus->channelByType(kChannelRight));
        channelByType(kChannelSurroundLeft)->sumFrom(sourceBus->channelByType(kChannelSurroundLeft));
        channelByType(kChannelSurroundRight)->sumFrom(sourceBus->channelByType(kChannelSurroundRight));
    }
    else {
        // All other cases, fall back to the discrete sum. This will silence the
        // excessive channels.
        discreteSumFrom(sourceBus);
    }
}

// チャンネル数の少ない bus から多い bus への合成。
// 例) dst=2ch, src=1ch
void ARIAudioBus::sumFromByDownMixing(const ARIAudioBus* sourceBus)
{
    unsigned number_of_source_channels = sourceBus->NumberOfChannels();
    unsigned number_of_destination_channels = NumberOfChannels();

    if (number_of_source_channels == 2 && number_of_destination_channels == 1) {
        // Down-mixing: 2 -> 1
        //   output = 0.5 * (input.L + input.R)
        const float* source_l = sourceBus->channelByType(kChannelLeft)->Data();
        const float* source_r = sourceBus->channelByType(kChannelRight)->Data();

        float* destination = channelByType(kChannelLeft)->MutableData();
        float scale = 0.5;

        blink::VectorMath::vsma(source_l, 1, &scale, destination, 1, length());
        blink::VectorMath::vsma(source_r, 1, &scale, destination, 1, length());
    }
    else if (number_of_source_channels == 4 &&
        number_of_destination_channels == 1) {
        // Down-mixing: 4 -> 1
        //   output = 0.25 * (input.L + input.R + input.SL + input.SR)
        const float* source_l = sourceBus->channelByType(kChannelLeft)->Data();
        const float* source_r = sourceBus->channelByType(kChannelRight)->Data();
        const float* source_sl =
            sourceBus->channelByType(kChannelSurroundLeft)->Data();
        const float* source_sr =
            sourceBus->channelByType(kChannelSurroundRight)->Data();

        float* destination = channelByType(kChannelLeft)->MutableData();
        float scale = 0.25;

        blink::VectorMath::vsma(source_l, 1, &scale, destination, 1, length());
        blink::VectorMath::vsma(source_r, 1, &scale, destination, 1, length());
        blink::VectorMath::vsma(source_sl, 1, &scale, destination, 1, length());
        blink::VectorMath::vsma(source_sr, 1, &scale, destination, 1, length());
    }
    else if (number_of_source_channels == 6 &&
        number_of_destination_channels == 1) {
        // Down-mixing: 5.1 -> 1
        //   output = sqrt(1/2) * (input.L + input.R) + input.C
        //            + 0.5 * (input.SL + input.SR)
        const float* source_l = sourceBus->channelByType(kChannelLeft)->Data();
        const float* source_r = sourceBus->channelByType(kChannelRight)->Data();
        const float* source_c = sourceBus->channelByType(kChannelCenter)->Data();
        const float* source_sl =
            sourceBus->channelByType(kChannelSurroundLeft)->Data();
        const float* source_sr =
            sourceBus->channelByType(kChannelSurroundRight)->Data();

        float* destination = channelByType(kChannelLeft)->MutableData();
        float scale_sqrt_half = sqrtf(0.5);
        float scale_half = 0.5;

        blink::VectorMath::vsma(source_l, 1, &scale_sqrt_half, destination, 1, length());
        blink::VectorMath::vsma(source_r, 1, &scale_sqrt_half, destination, 1, length());
        blink::VectorMath::vadd(source_c, 1, destination, 1, destination, 1, length());
        blink::VectorMath::vsma(source_sl, 1, &scale_half, destination, 1, length());
        blink::VectorMath::vsma(source_sr, 1, &scale_half, destination, 1, length());
    }
    else if (number_of_source_channels == 4 &&
        number_of_destination_channels == 2) {
        // Down-mixing: 4 -> 2
        //   output.L = 0.5 * (input.L + input.SL)
        //   output.R = 0.5 * (input.R + input.SR)
        const float* source_l = sourceBus->channelByType(kChannelLeft)->Data();
        const float* source_r = sourceBus->channelByType(kChannelRight)->Data();
        const float* source_sl =
            sourceBus->channelByType(kChannelSurroundLeft)->Data();
        const float* source_sr =
            sourceBus->channelByType(kChannelSurroundRight)->Data();

        float* destination_l = channelByType(kChannelLeft)->MutableData();
        float* destination_r = channelByType(kChannelRight)->MutableData();
        float scale_half = 0.5;

        blink::VectorMath::vsma(source_l, 1, &scale_half, destination_l, 1, length());
        blink::VectorMath::vsma(source_sl, 1, &scale_half, destination_l, 1, length());
        blink::VectorMath::vsma(source_r, 1, &scale_half, destination_r, 1, length());
        blink::VectorMath::vsma(source_sr, 1, &scale_half, destination_r, 1, length());
    }
    else if (number_of_source_channels == 6 &&
        number_of_destination_channels == 2) {
        // Down-mixing: 5.1 -> 2
        //   output.L = input.L + sqrt(1/2) * (input.C + input.SL)
        //   output.R = input.R + sqrt(1/2) * (input.C + input.SR)
        const float* source_l = sourceBus->channelByType(kChannelLeft)->Data();
        const float* source_r = sourceBus->channelByType(kChannelRight)->Data();
        const float* source_c = sourceBus->channelByType(kChannelCenter)->Data();
        const float* source_sl =
            sourceBus->channelByType(kChannelSurroundLeft)->Data();
        const float* source_sr =
            sourceBus->channelByType(kChannelSurroundRight)->Data();

        float* destination_l = channelByType(kChannelLeft)->MutableData();
        float* destination_r = channelByType(kChannelRight)->MutableData();
        float scale_sqrt_half = sqrtf(0.5);

        blink::VectorMath::vadd(source_l, 1, destination_l, 1, destination_l, 1, length());
        blink::VectorMath::vsma(source_c, 1, &scale_sqrt_half, destination_l, 1, length());
        blink::VectorMath::vsma(source_sl, 1, &scale_sqrt_half, destination_l, 1, length());
        blink::VectorMath::vadd(source_r, 1, destination_r, 1, destination_r, 1, length());
        blink::VectorMath::vsma(source_c, 1, &scale_sqrt_half, destination_r, 1, length());
        blink::VectorMath::vsma(source_sr, 1, &scale_sqrt_half, destination_r, 1, length());
    }
    else if (number_of_source_channels == 6 &&
        number_of_destination_channels == 4) {
        // Down-mixing: 5.1 -> 4
        //   output.L = input.L + sqrt(1/2) * input.C
        //   output.R = input.R + sqrt(1/2) * input.C
        //   output.SL = input.SL
        //   output.SR = input.SR
        const float* source_l = sourceBus->channelByType(kChannelLeft)->Data();
        const float* source_r = sourceBus->channelByType(kChannelRight)->Data();
        const float* source_c = sourceBus->channelByType(kChannelCenter)->Data();

        float* destination_l = channelByType(kChannelLeft)->MutableData();
        float* destination_r = channelByType(kChannelRight)->MutableData();
        float scale_sqrt_half = sqrtf(0.5);

        blink::VectorMath::vadd(source_l, 1, destination_l, 1, destination_l, 1, length());
        blink::VectorMath::vsma(source_c, 1, &scale_sqrt_half, destination_l, 1, length());
        blink::VectorMath::vadd(source_r, 1, destination_r, 1, destination_r, 1, length());
        blink::VectorMath::vsma(source_c, 1, &scale_sqrt_half, destination_r, 1, length());
        channel(2)->sumFrom(sourceBus->channel(4));
        channel(3)->sumFrom(sourceBus->channel(5));
    }
    else {
        // All other cases, fall back to the discrete sum. This will perform
        // channel-wise sum until the destination channels run out.
        discreteSumFrom(sourceBus);
    }
}

// チャンネル数の差による特別な合成などは行わず、単純に詰めるようにコピーする。
// 主に、合成処理のフォールバック用に使う。
void ARIAudioBus::discreteSumFrom(const ARIAudioBus* sourceBus)
{
    unsigned number_of_source_channels = sourceBus->NumberOfChannels();
    unsigned number_of_destination_channels = NumberOfChannels();

    if (number_of_destination_channels < number_of_source_channels) {
        // Down-mix by summing channels and dropping the remaining.
        for (unsigned i = 0; i < number_of_destination_channels; ++i)
            channel(i)->sumFrom(sourceBus->Channel(i));
    }
    else if (number_of_destination_channels > number_of_source_channels) {
        // Up-mix by summing as many channels as we have.
        for (unsigned i = 0; i < number_of_source_channels; ++i)
            channel(i)->sumFrom(sourceBus->Channel(i));
    }
}

} // namespace ln

#endif

