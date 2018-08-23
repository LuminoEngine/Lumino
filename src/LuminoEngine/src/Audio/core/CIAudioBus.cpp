
#include "Internal.hpp"
#include "CIAudioBus.hpp"
#include "../ChromiumWebCore.hpp"
#include "../blink/VectorMath.h"

namespace ln {
namespace detail {

//==============================================================================
// CIAudioChannel

CIAudioChannel::CIAudioChannel(size_t length)
	: m_isSilent(true)
{
	m_data.resize(length);
}

void CIAudioChannel::setSilentAndZero()
{
	if (!m_isSilent)
	{
		memset(mutableData(), 0, sizeof(float) * length());
		m_isSilent = true;
	}
}

void CIAudioChannel::copyTo(float* buffer, size_t bufferLength, size_t stride) const
{
	const float* src = constData();
	if (stride == 1) {
		memcpy(buffer, src, length());
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

void CIAudioChannel::copyFrom(const float * buffer, size_t bufferLength, size_t stride)
{
	float* dst = mutableData();
	if (stride == 1) {
		memcpy(dst, buffer, length());
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

void CIAudioChannel::copyFrom(const CIAudioChannel* ch)
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

void CIAudioChannel::sumFrom(const CIAudioChannel * ch)
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

//==============================================================================
// CIAudioBus

const unsigned kMaxBusChannels = 32;

CIAudioBus::CIAudioBus()
{
}

void CIAudioBus::initialize2(int channelCount, size_t length, int sampleRate)
{
	for (int i = 0; i < channelCount; ++i)
	{
		m_channels.add(makeRef<CIAudioChannel>(length));
	}
	m_validLength = length;
	m_sampleRate = sampleRate;
}

CIAudioChannel* CIAudioBus::channelByType(unsigned channel_type)
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

const CIAudioChannel* CIAudioBus::channelByType(unsigned type) const
{
	return const_cast<CIAudioBus*>(this)->channelByType(type);
}

void CIAudioBus::setSilentAndZero()
{
	for (auto& ch : m_channels) {
		ch->setSilentAndZero();
	}
}

void CIAudioBus::clearSilentFlag()
{
	for (auto& ch : m_channels) {
		ch->clearSilentFlag();
	}
}

bool CIAudioBus::isSilent() const
{
	for (auto& ch : m_channels) {
		if (!ch->isSilent()) {
			return false;
		}
	}
	return true;
}

void CIAudioBus::mergeToChannelBuffers(float* buffer, size_t length)
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

void CIAudioBus::separateFrom(const float * buffer, size_t length, int channelCount)
{
	assert(m_channels.size() == 2);
	assert(m_channels[0]->length() * 2 >= length);	// length が少ない分にはOK。多いのはあふれるのでNG

	for (int i = 0; i < m_channels.size(); i++)
	{
		m_channels[i]->copyFrom(buffer + i, length - i, m_channels.size());
	}
}

void CIAudioBus::sumFrom(const CIAudioBus* bus)
{
	int thisChannelCount = channelCount();
	if (thisChannelCount == bus->channelCount())
	{
		for (int i = 0; i < thisChannelCount; i++)
		{
			channel(i)->sumFrom(bus->channel(i));
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

void CIAudioBus::copyWithGainFrom(const CIAudioBus& source_bus, float gain)
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
void CIAudioBus::copyBySampleRateConverting(
	const CIAudioBus* source_bus,
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
	const CIAudioBus* resampler_source_bus = source_bus;

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

		blink::SincResampler resampler(sample_rate_ratio);
		resampler.Process(source, destination, source_length, channel(i)->length());
	}

	clearSilentFlag();
	setSampleRate(new_sample_rate);
}

// Returns true if the channel count and frame-size match.
bool CIAudioBus::topologyMatches(const CIAudioBus& bus) const
{
	if (NumberOfChannels() != bus.NumberOfChannels())
		return false;  // channel mismatch

					   // Make sure source bus has enough frames.
	if (length() > bus.length())
		return false;  // frame-size mismatch

	return true;
}

} // namespace detail
} // namespace ln

