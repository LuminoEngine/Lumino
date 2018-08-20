
#include "Internal.hpp"
#include "CoreAudioNode.hpp"
#include "AudioDecoder.hpp"	// for CoreAudioSourceNode
#include "ChromiumWebCore.hpp"

namespace ln {
namespace detail {

static void vadd(const float* source1P, const float* source2P, float* destP, size_t framesToProcess)
{
	int n = framesToProcess;
	while (n--) {
		*destP = *source1P + *source2P;
		source1P++;
		source2P++;
		destP++;
	}
}

void vsmul(const float* sourceP, int sourceStride, const float* scale, float* destP, int destStride, size_t framesToProcess)
{
	int n = framesToProcess;
	float k = *scale;
	while (n--) {
		*destP = k * *sourceP;
		sourceP += sourceStride;
		destP += destStride;
	}
}

void vclip(const float* sourceP, int sourceStride, const float* lowThresholdP, const float* highThresholdP, float* destP, int destStride, size_t framesToProcess)
{
	int n = framesToProcess;
	float lowThreshold = *lowThresholdP;
	float highThreshold = *highThresholdP;
	while (n--) {
		*destP = std::max(std::min(*sourceP, highThreshold), lowThreshold);
		sourceP += sourceStride;
		destP += destStride;
	}
}

//==============================================================================
// CoreAudioChannel

CoreAudioChannel::CoreAudioChannel()
	: m_isSilent(true)
{
}

void CoreAudioChannel::initialize(size_t length)
{
	Object::initialize();
	m_data.resize(length);
}

void CoreAudioChannel::setSilentAndZero()
{
	if (!m_isSilent)
	{
		memset(mutableData(), 0, sizeof(float) * length());
		m_isSilent = true;
	}
}

void CoreAudioChannel::copyTo(float* buffer, size_t bufferLength, size_t stride) const
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

void CoreAudioChannel::copyFrom(const float * buffer, size_t bufferLength, size_t stride)
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

void CoreAudioChannel::copyFrom(const CoreAudioChannel* ch)
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

void CoreAudioChannel::sumFrom(const CoreAudioChannel * ch)
{
	if (ch->isSilent()) {
		return;
	}

	if (isSilent()) {
		// optimize for first time.
		copyFrom(ch);
	}
	else {
		vadd(constData(), ch->constData(), mutableData(), length());
	}
}

//==============================================================================
// CoreAudioBus

const unsigned kMaxBusChannels = 32;

CoreAudioBus::CoreAudioBus()
{
}

void CoreAudioBus::initialize(int channelCount, size_t length)
{
	Object::initialize();
	for (int i = 0; i < channelCount; ++i)
	{
		m_channels.add(newObject<CoreAudioChannel>(length));
	}
	m_validLength = length;
}

CoreAudioChannel* CoreAudioBus::channelByType(unsigned channel_type)
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

const CoreAudioChannel* CoreAudioBus::channelByType(unsigned type) const
{
	return const_cast<CoreAudioBus*>(this)->channelByType(type);
}

void CoreAudioBus::setSilentAndZero()
{
	for (auto& ch : m_channels) {
		ch->setSilentAndZero();
	}
}

void CoreAudioBus::clearSilentFlag()
{
	for (auto& ch : m_channels) {
		ch->clearSilentFlag();
	}
}

bool CoreAudioBus::isSilent() const
{
	for (auto& ch : m_channels) {
		if (!ch->isSilent()) {
			return false;
		}
	}
	return true;
}

void CoreAudioBus::mergeToChannelBuffers(float* buffer, size_t length)
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

void CoreAudioBus::separateFrom(const float * buffer, size_t length, int channelCount)
{
	assert(m_channels.size() == 2);
	assert(m_channels[0]->length() * 2 >= length);	// length が少ない分にはOK。多いのはあふれるのでNG

	for (int i = 0; i < m_channels.size(); i++)
	{
		m_channels[i]->copyFrom(buffer + i, length - i, m_channels.size());
	}
}

void CoreAudioBus::sumFrom(const CoreAudioBus* bus)
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

void CoreAudioBus::copyWithGainFrom(const CoreAudioBus& source_bus, float gain)
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
			vsmul(sources[channel_index], 1, &gain, destinations[channel_index], 1,
				frames_to_process);
		}
	}
}

// Returns true if the channel count and frame-size match.
bool CoreAudioBus::topologyMatches(const CoreAudioBus& bus) const
{
	if (NumberOfChannels() != bus.NumberOfChannels())
		return false;  // channel mismatch

					   // Make sure source bus has enough frames.
	if (length() > bus.length())
		return false;  // frame-size mismatch

	return true;
}

//==============================================================================
// CoreAudioOutputPin

CoreAudioInputPin::CoreAudioInputPin()
	: m_ownerNode(nullptr)
{
}

void CoreAudioInputPin::initialize(int channels)
{
	Object::initialize();
	m_summingBus = newObject<CoreAudioBus>(channels, CoreAudioNode::ProcessingSizeInFrames);
}

CoreAudioBus* CoreAudioInputPin::bus() const
{
	return m_summingBus;
}

CoreAudioBus* CoreAudioInputPin::pull()
{
	m_summingBus->setSilentAndZero();

	for (auto& output : m_connectedOutputPins)
	{
		CoreAudioBus* bus = output->pull();
		m_summingBus->sumFrom(bus);
	}

	return m_summingBus;
}

void CoreAudioInputPin::addLinkOutput(CoreAudioOutputPin * output)
{
	m_connectedOutputPins.add(output);
}

//==============================================================================
// CoreAudioOutputPin

CoreAudioOutputPin::CoreAudioOutputPin()
	: m_ownerNode(nullptr)
{
}

void CoreAudioOutputPin::initialize(int channels)
{
	Object::initialize();
	m_resultBus = newObject<CoreAudioBus>(channels, CoreAudioNode::ProcessingSizeInFrames);
}

CoreAudioBus * CoreAudioOutputPin::bus() const
{
	return m_resultBus;
}

CoreAudioBus * CoreAudioOutputPin::pull()
{
	m_ownerNode->processIfNeeded();
	return m_resultBus;
}

void CoreAudioOutputPin::addLinkInput(CoreAudioInputPin * input)
{
	m_connectedInputPins.add(input);
}

//==============================================================================
// CoreAudioNode

CoreAudioNode::CoreAudioNode(AudioContextCore* context)
	: m_context(context)
{
}

void CoreAudioNode::initialize()
{
	Object::initialize();
}

CoreAudioInputPin * CoreAudioNode::inputPin(int index) const
{
	return m_inputPins[index];
}

CoreAudioOutputPin * CoreAudioNode::outputPin(int index) const
{
	return m_outputPins[index];
}

void CoreAudioNode::pullInputs()
{
	for (auto & in : m_inputPins)
	{
		in->pull();
	}
}

void CoreAudioNode::processIfNeeded()
{
	pullInputs();
	process();
}

void CoreAudioNode::connect(CoreAudioNode * outputSide, CoreAudioNode * inputSide)
{
	outputSide->outputPin(0)->addLinkInput(inputSide->inputPin(0));
	inputSide->inputPin(0)->addLinkOutput(outputSide->outputPin(0));
}

CoreAudioInputPin* CoreAudioNode::addInputPin(int channels)
{
	auto pin = newObject<CoreAudioInputPin>(channels);
	pin->setOwnerNode(this);
	m_inputPins.add(pin);
	return pin;
}

CoreAudioOutputPin* CoreAudioNode::addOutputPin(int channels)
{
	auto pin = newObject<CoreAudioOutputPin>(channels);
	pin->setOwnerNode(this);
	m_outputPins.add(pin);
	return pin;
}

//==============================================================================
// CoreAudioDestinationNode

CoreAudioSourceNode::CoreAudioSourceNode(AudioContextCore* context)
	: CoreAudioNode(context)
	, m_virtualReadIndex(0)
	, m_playbackRate(1.0f)
	, m_seekFrame(0)
	, m_playingState(PlayingState::Stopped)
	, m_requestedPlayingState(PlayingState::None)
	, m_resetRequested(false)
{
}

void CoreAudioSourceNode::initialize(const Ref<AudioDecoder>& decoder)
{
	CoreAudioNode::initialize();

	// TODO:
	//auto decoder = makeRef<WaveDecoder>();
	//decoder->initialize(FileStream::create(filePath), diag);
	m_decoder = decoder;
	//tmpBuffer.resize(CoreAudioNode::ProcessingSizeInFrames * m_masterChannels);

	unsigned numChannels = m_decoder->audioDataInfo().channelCount;
	auto* pin = addOutputPin(numChannels);
	//m_readBuffer.resize(pin->bus()->length() * numChannels);

	resetSourceBuffers();
}

void CoreAudioSourceNode::setPlaybackRate(float rate)
{
	m_playbackRate = rate;
	resetSourceBuffers();
}

void CoreAudioSourceNode::start()
{
	m_requestedPlayingState = PlayingState::Playing;
}

void CoreAudioSourceNode::stop()
{
	m_requestedPlayingState = PlayingState::Stopped;
	m_resetRequested = true;
}

void CoreAudioSourceNode::reset()
{
	m_seekFrame = 0;
}

void CoreAudioSourceNode::finish()
{
	m_requestedPlayingState = PlayingState::Stopped;
}

unsigned CoreAudioSourceNode::numberOfChannels() const
{
	return outputPin(0)->bus()->channelCount();
}

void CoreAudioSourceNode::resetSourceBuffers()
{
	CoreAudioOutputPin* pin = outputPin(0);

	m_readFrames = pin->bus()->length() * m_playbackRate;

	unsigned numChannels = m_decoder->audioDataInfo().channelCount;

	m_readBuffer.resize(m_readFrames * numChannels);

	m_sourceBus = newObject<CoreAudioBus>(numChannels, m_readFrames);
}

double CoreAudioSourceNode::calculatePitchRate()
{
	// TODO: doppler from associatd panner.
	
	double totalRate = m_playbackRate;

	// zero rate is illegal
	if (totalRate <= 0.0) {
		totalRate = 1.0;
	}

	return totalRate;
}

void CoreAudioSourceNode::process()
{
	updatePlayingState();


	CoreAudioBus* result = outputPin(0)->bus();

	if (m_playingState != PlayingState::Playing) {
		result->setSilentAndZero();
		return;
	}


	//size_t bufferLength = //result->length();
	unsigned numChannels = m_decoder->audioDataInfo().channelCount;

	size_t bufferLength = m_decoder->read2(m_readBuffer.data(), m_readFrames);
	size_t readSamples = bufferLength * numChannels;
	m_sourceBus->separateFrom(m_readBuffer.data(), readSamples, numChannels);



	double pitchRate = calculatePitchRate();

	// 

	unsigned endFrame = bufferLength;

	//if (m_virtualReadIndex >= endFrame)
		m_virtualReadIndex = 0; // reset to start


	unsigned writeIndex = 0;

	double virtualReadIndex = m_virtualReadIndex;

	double virtualDeltaFrames = bufferLength;	// Number of frames processed this time process().
	double virtualEndFrame = bufferLength;
	// TODO: loop

	{
		int framesToProcess = result->length();
		while (framesToProcess--)
		{
			unsigned readIndex = static_cast<unsigned>(virtualReadIndex);
			double factor = virtualReadIndex - readIndex;

			// For linear interpolation we need the next sample-frame too.
			unsigned readIndex2 = readIndex + 1;

			if (readIndex2 >= bufferLength)
			{
				if (loop()) {
					// Make sure to wrap around at the end of the buffer.
					readIndex2 = static_cast<unsigned>(virtualReadIndex + 1 - virtualDeltaFrames);
				}
				else
					readIndex2 = readIndex;
			}


			// Final sanity check on buffer access.
			// FIXME: as an optimization, try to get rid of this inner-loop check and put assertions and guards before the loop.
			if (readIndex >= bufferLength || readIndex2 >= bufferLength)
				break;

			// Linear interpolation.
			for (unsigned i = 0; i < numChannels; ++i)
			{
				float * destination = result->channel(i)->mutableData();
				const float * source = m_sourceBus->channel(i)->constData();

				double sample1 = source[readIndex];
				double sample2 = source[readIndex2];
				double sample = (1.0 - factor) * sample1 + factor * sample2;

				destination[writeIndex] = static_cast<float>(sample);

				//if (sample > 1.0) {
				//	printf("");
				//}
			}
			writeIndex++;

			virtualReadIndex += pitchRate;

			// Wrap-around, retaining sub-sample position since virtualReadIndex is floating-point.
			if (virtualReadIndex >= virtualEndFrame)
			{
				virtualReadIndex -= virtualDeltaFrames;
				if (renderSilenceAndFinishIfNotLooping(result, writeIndex, framesToProcess))
					break;
			}
		}
	}

	//printf("writeIndex:%d\n", writeIndex);

	result->clearSilentFlag();

	m_virtualReadIndex = virtualReadIndex;
}

bool CoreAudioSourceNode::renderSilenceAndFinishIfNotLooping(CoreAudioBus * bus, unsigned index, size_t framesToProcess)
{
	if (!loop())
	{
		// If we're not looping, then stop playing when we get to the end.

		if (framesToProcess > 0)
		{
			// We're not looping and we've reached the end of the sample data, but we still need to provide more output,
			// so generate silence for the remaining.
			for (unsigned i = 0; i < numberOfChannels(); ++i)
			{
				memset(bus->channel(i)->mutableData() + index, 0, sizeof(float) * framesToProcess);
			}
			finish();
		}

		return true;
	}
	return false;
}

void CoreAudioSourceNode::updatePlayingState()
{
	if (m_resetRequested) {
		reset();
	}

	m_playingState = m_requestedPlayingState;
}

//==============================================================================
// CoreAudioPannerNode

CoreAudioPannerNode::CoreAudioPannerNode(AudioContextCore* context)
	: CoreAudioNode(context)
{
}

void CoreAudioPannerNode::initialize()
{
	CoreAudioNode::initialize();

	unsigned numChannels = 2;
	addOutputPin(numChannels);
	addInputPin(numChannels);

	m_panner = blink::Panner::Create(blink::Panner::kPanningModelEqualPower, 0, nullptr);
	m_distanceEffect = std::make_shared<blink::DistanceEffect>();
	m_coneEffect = std::make_shared<blink::ConeEffect>();
}

void CoreAudioPannerNode::process()
{
	//test
	static float count = 0;
	count += 0.01;
	m_emitter.m_position = Vector3(cos(count), 0, sin(count));



	CoreAudioBus* destination = outputPin(0)->bus();

	if (!m_panner.get()) {
		destination->setSilentAndZero();
		return;
	}

	CoreAudioBus* source = inputPin(0)->bus();
	if (!source) {
		destination->setSilentAndZero();
		return;
	}

	double azimuth;
	double elevation;
	azimuthElevation(&azimuth, &elevation);
	//printf("azimuth:%f\n", azimuth);

	m_panner->Pan(azimuth, elevation, source, destination, destination->length(), CoreAudioBus::kSpeakers);

	float total_gain = distanceConeGain();
	//printf("tt:%f\n", total_gain);

	destination->copyWithGainFrom(*destination, total_gain);
}

void CoreAudioPannerNode::azimuthElevation(double* outAzimuth, double* outElevation)
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

float CoreAudioPannerNode::distanceConeGain()
{
	// TODO: dirty and cache

	float cached_distance_cone_gain_;

	cached_distance_cone_gain_ = blink::CalculateDistanceConeGain(
		m_emitter.m_position, m_emitter.m_direction, context()->listener().m_position, m_distanceEffect.get(), m_coneEffect.get());

	return cached_distance_cone_gain_;
}

//==============================================================================
// CoreAudioDestinationNode

CoreAudioDestinationNode::CoreAudioDestinationNode(AudioContextCore* context)
	: CoreAudioNode(context)
{
}

void CoreAudioDestinationNode::initialize()
{
	CoreAudioNode::initialize();
	addInputPin(2);
}

void CoreAudioDestinationNode::render(float * outputBuffer, int length)
{
	CoreAudioBus* bus = inputPin(0)->pull();

	// Clamp values at 0db (i.e., [-1.0, 1.0])
	const float kLowThreshold = -1.0f;
	const float kHighThreshold = 1.0f;
	for (unsigned i = 0; i < bus->numberOfChannels(); ++i)
	{
		CoreAudioChannel * channel = bus->channel(i);
		vclip(channel->constData(), 1, &kLowThreshold, &kHighThreshold, channel->mutableData(), 1, channel->length());
	}

	bus->mergeToChannelBuffers(outputBuffer, length);
}

void CoreAudioDestinationNode::process()
{
	LN_UNREACHABLE();
}


} // namespace detail
} // namespace ln

