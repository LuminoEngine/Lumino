
#include "Internal.hpp"
#include "CoreAudioNode.hpp"
#include "AudioDecoder.hpp"	// for CoreAudioSourceNode

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

CoreAudioNode::CoreAudioNode()
{
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

CoreAudioSourceNode::CoreAudioSourceNode()
	: m_virtualReadIndex(0)
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
	//m_readBuffer.resize(pin->bus()->validLength() * numChannels);

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

	m_readFrames = pin->bus()->validLength() * m_playbackRate;

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


	//size_t bufferLength = //result->validLength();
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
		int framesToProcess = result->validLength();
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
// CoreAudioDestinationNode

CoreAudioDestinationNode::CoreAudioDestinationNode()
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
	bus->mergeToChannelBuffers(outputBuffer, length);
}

void CoreAudioDestinationNode::process()
{
	LN_UNREACHABLE();
}


} // namespace detail
} // namespace ln

