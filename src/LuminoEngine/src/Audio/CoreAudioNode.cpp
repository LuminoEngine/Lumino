
#include "Internal.hpp"
#include "CoreAudioNode.hpp"
#include "AudioDecoder.hpp"	// for CoreAudioSourceNode
#include "ChromiumWebCore.hpp"
#include "blink/VectorMath.h"

namespace ln {
namespace detail {

//==============================================================================
// CoreAudioOutputPin

CoreAudioInputPin::CoreAudioInputPin(int channels)
	: m_ownerNode(nullptr)
{
	m_summingBus = makeRef<CIAudioBus>();
	m_summingBus->initialize2(channels, CoreAudioNode::ProcessingSizeInFrames);
}

CIAudioBus* CoreAudioInputPin::bus() const
{
	return m_summingBus;
}

CIAudioBus* CoreAudioInputPin::pull()
{
	m_summingBus->setSilentAndZero();

	for (auto& output : m_connectedOutputPins)
	{
		CIAudioBus* bus = output->pull();
		m_summingBus->sumFrom(bus);
	}

	return m_summingBus;
}

void CoreAudioInputPin::addLinkOutput(CoreAudioOutputPin * output)
{
	m_connectedOutputPins.add(output);
}

void CoreAudioInputPin::removeLinkOutput(CoreAudioOutputPin * output)
{
	m_connectedOutputPins.remove(output);
}

void CoreAudioInputPin::disconnect(int index)
{
	auto& otherPin = m_connectedOutputPins[index];
	otherPin->removeLinkInput(this);
	m_connectedOutputPins.removeAt(index);
}

void CoreAudioInputPin::disconnectAll()
{
	for (auto& otherPin : m_connectedOutputPins) {
		otherPin->removeLinkInput(this);
	}
	m_connectedOutputPins.clear();
}

//==============================================================================
// CoreAudioOutputPin

CoreAudioOutputPin::CoreAudioOutputPin(int channels)
	: m_ownerNode(nullptr)
{
	m_resultBus = makeRef<CIAudioBus>();
	m_resultBus->initialize2(channels, CoreAudioNode::ProcessingSizeInFrames);
}

CIAudioBus * CoreAudioOutputPin::bus() const
{
	return m_resultBus;
}

CIAudioBus * CoreAudioOutputPin::pull()
{
	m_ownerNode->processIfNeeded();
	return m_resultBus;
}

void CoreAudioOutputPin::addLinkInput(CoreAudioInputPin * input)
{
	m_connectedInputPins.add(input);
}

void CoreAudioOutputPin::removeLinkInput(CoreAudioInputPin * input)
{
	m_connectedInputPins.remove(input);
}

void CoreAudioOutputPin::disconnect(int index)
{
	auto& otherPin = m_connectedInputPins[index];
	otherPin->removeLinkOutput(this);
	m_connectedInputPins.removeAt(index);
}

void CoreAudioOutputPin::disconnectAll()
{
	for (auto& otherPin : m_connectedInputPins) {
		otherPin->removeLinkOutput(this);
	}
	m_connectedInputPins.clear();
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

void CoreAudioNode::disconnectAllInputSide()
{
	for (auto& pin : m_inputPins) {
		pin->disconnectAll();
	}
}

void CoreAudioNode::disconnectAllOutputSide()
{
	for (auto& pin : m_outputPins) {
		pin->disconnectAll();
	}
}

CoreAudioInputPin* CoreAudioNode::addInputPin(int channels)
{
	auto pin = makeRef<CoreAudioInputPin>(channels);
	pin->setOwnerNode(this);
	m_inputPins.add(pin);
	return pin;
}

CoreAudioOutputPin* CoreAudioNode::addOutputPin(int channels)
{
	auto pin = makeRef<CoreAudioOutputPin>(channels);
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

	m_sourceBus = makeRef<CIAudioBus>();
	m_sourceBus->initialize2(numChannels, m_readFrames);
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


	CIAudioBus* result = outputPin(0)->bus();

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

bool CoreAudioSourceNode::renderSilenceAndFinishIfNotLooping(CIAudioBus * bus, unsigned index, size_t framesToProcess)
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



	CIAudioBus* destination = outputPin(0)->bus();

	if (!m_panner.get()) {
		destination->setSilentAndZero();
		return;
	}

	CIAudioBus* source = inputPin(0)->bus();
	if (!source) {
		destination->setSilentAndZero();
		return;
	}

	double azimuth;
	double elevation;
	azimuthElevation(&azimuth, &elevation);
	//printf("azimuth:%f\n", azimuth);

	m_panner->Pan(azimuth, elevation, source, destination, destination->length(), CIAudioBus::kSpeakers);

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
	CIAudioBus* bus = inputPin(0)->pull();

	// Clamp values at 0db (i.e., [-1.0, 1.0])
	const float kLowThreshold = -1.0f;
	const float kHighThreshold = 1.0f;
	for (unsigned i = 0; i < bus->numberOfChannels(); ++i)
	{
		CIAudioChannel * channel = bus->channel(i);
		::blink::VectorMath::vclip(channel->constData(), 1, &kLowThreshold, &kHighThreshold, channel->mutableData(), 1, channel->length());
	}

	bus->mergeToChannelBuffers(outputBuffer, length);
}

void CoreAudioDestinationNode::process()
{
	LN_UNREACHABLE();
}


} // namespace detail
} // namespace ln

