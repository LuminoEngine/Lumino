
#include "Internal.hpp"
#include "CoreAudioNode.hpp"
#include "AudioDecoder.hpp"	// for CoreAudioSourceNode
#include "ChromiumWebCore.hpp"
#include "blink/VectorMath.h"

namespace ln {
namespace detail {

////==============================================================================
//// AudioContextCore
//
//AudioContextCore::AudioContextCore()
//{
//}
//
//void AudioContextCore::init()
//{
//#ifdef LN_USE_SDL
//	auto device = makeRef<SDLAudioDevice>();
//	device->init();
//	m_device = device;
//#else
//	auto device = makeRef<ALAudioDevice>();
//	device->init();
//	m_device = device;
//#endif
//}
//
//void AudioContextCore::dispose()
//{
//	if (m_device)
//	{
//		m_device->dispose();
//		m_device.reset();
//	}
//}
//
//==============================================================================
// PropagationParameters

PropagationParameters::PropagationParameters()
	: m_finalSamplingRate(0)
{}

//==============================================================================
// CoreAudioOutputPin

CoreAudioInputPin::CoreAudioInputPin(int channels)
	: m_ownerNode(nullptr)
{
	m_summingBus = makeRef<AudioBus>();
	m_summingBus->initialize2(channels, CoreAudioNode::ProcessingSizeInFrames);
}

AudioBus* CoreAudioInputPin::bus() const
{
	return m_summingBus;
}

AudioBus* CoreAudioInputPin::pull()
{
	m_summingBus->setSilentAndZero();

	for (auto& output : m_connectedOutputPins)
	{
		AudioBus* bus = output->pull();
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
	m_resultBus = makeRef<AudioBus>();
	m_resultBus->initialize2(channels, CoreAudioNode::ProcessingSizeInFrames);
}

AudioBus * CoreAudioOutputPin::bus() const
{
	return m_resultBus;
}

AudioBus * CoreAudioOutputPin::pull()
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

CoreAudioNode::CoreAudioNode(AudioDevice* context)
	: m_context(context)
{
}

void CoreAudioNode::init()
{
	Object::init();
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

CoreAudioSourceNode::CoreAudioSourceNode(AudioDevice* context)
	: CoreAudioNode(context)
	, m_virtualReadIndex(0)
	, m_playbackRate(1.0f)
	, m_seekFrame(0)
	, m_playingState(PlayingState::Stopped)
	//, m_requestedPlayingState(PlayingState::None)
	, m_resetRequested(false)
    , m_loop(false)
{
}

void CoreAudioSourceNode::init(const Ref<AudioDecoder>& decoder)
{
	CoreAudioNode::init();

	// TODO:
	//auto decoder = makeRef<WaveDecoder>();
	//decoder->init(FileStream::create(filePath), diag);
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
    m_playingState = PlayingState::Playing;
	//m_requestedPlayingState = PlayingState::Playing;
}

void CoreAudioSourceNode::stop()
{
    m_playingState = PlayingState::Stopped;
	//m_requestedPlayingState = PlayingState::Stopped;
	m_resetRequested = true;
}

void CoreAudioSourceNode::reset()
{
	m_seekFrame = 0;
}

void CoreAudioSourceNode::finish()
{
    m_playingState = PlayingState::Stopped;
	//m_requestedPlayingState = PlayingState::Stopped;
}

unsigned CoreAudioSourceNode::numberOfChannels() const
{
	return outputPin(0)->bus()->channelCount();
}

void CoreAudioSourceNode::resetSourceBuffers()
{
	CoreAudioOutputPin* pin = outputPin(0);

	size_t baseFrames = pin->bus()->length();
	m_readFrames = baseFrames;


	int sourceSampleRate = m_decoder->audioDataInfo().sampleRate;
	int destinationSampleRate = context()->deviceSamplingRate();
	if (sourceSampleRate != destinationSampleRate)
	{
		float scale = static_cast<float>(sourceSampleRate) / static_cast<float>(destinationSampleRate);
		m_resampler = std::make_unique<blink::SincResampler>(scale);
		m_readFrames *= scale;
	}

	m_readFrames *= m_playbackRate;
	baseFrames *= m_playbackRate;

	unsigned numChannels = m_decoder->audioDataInfo().channelCount;

	m_readBuffer.resize(m_readFrames * numChannels);
	if (m_resampler) {
		m_resamplingBus = makeRef<AudioBus>();
		m_resamplingBus->initialize2(numChannels, m_readFrames, m_decoder->audioDataInfo().sampleRate);
	}

	m_sourceBus = makeRef<AudioBus>();
	m_sourceBus->initialize2(numChannels, baseFrames);



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

// https://github.com/chromium/chromium/blob/ba96c018682416a7b2ec77876404b14322aa1b54/third_party/blink/renderer/modules/webaudio/audio_buffer_source_node.cc
void CoreAudioSourceNode::process()
{
	updatePlayingState();


	AudioBus* result = outputPin(0)->bus();

	if (m_playingState != PlayingState::Playing) {
		result->setSilentAndZero();
		return;
	}


	//size_t bufferLength = //result->length();
	unsigned numChannels = m_decoder->audioDataInfo().channelCount;

    size_t bufferLength = 0;
    {
        if (loop()) {
            size_t requestLength = m_readFrames;
            float* buf = m_readBuffer.data();
            do
            {
                bufferLength = m_decoder->read2(buf, requestLength);
                if (bufferLength < requestLength) {    // EOF
                    m_decoder->seekToFrame(0);
                }
                requestLength -= bufferLength;
                buf += bufferLength;
            } while (requestLength > 0);

            bufferLength = m_readFrames;
        }
        else {
            size_t bufferLength = m_decoder->read2(m_readBuffer.data(), m_readFrames);
            if (bufferLength == 0) {    // EOF
                result->setSilentAndZero();
                return;
            }
        }


    }



	size_t readSamples = bufferLength * numChannels;


    //printf("bufferLength:%d , result->length():%d", bufferLength, result->length());

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

	if (Math::nearEqual(m_playbackRate, 1.0f))
	{
		if (m_resampler) {
			m_resamplingBus->separateFrom(m_readBuffer.data(), readSamples, numChannels);
			result->copyBySampleRateConverting(m_resamplingBus, context()->deviceSamplingRate());
		}
		else {
			result->separateFrom(m_readBuffer.data(), readSamples, numChannels);
		}

        //int remain = result->length() - bufferLength;
        //if (remain > 0) {
        //    //result->fillZero(bufferLength, remain);
        //    result->fillZero(0, result->length());
        //    printf("eof\n");
        //}
	}
	else
	{
		m_sourceBus->separateFrom(m_readBuffer.data(), readSamples, numChannels);

        // writeIndex は result の Bus の書き込みインデックス。以下 while で 1 ずつインクリメントしていく。
        // readIndex は m_sourceBus の読み込みインデックス。以下 while で pitchRate ずつインクリメントしていく。
        // 例えば pitch=1.2 で処理を進めると・・・
        //   1 回目は writeIndex, readIndex 共に 0 なので、最初のサンプルがそのまま使われる。
        //   2 回目は readIndex が 1.2 となっている。このとき、readIndex+1 のフレームを 0.2 で補間した値を writeIndex に書き込む。

		int framesToProcess = result->length();
		while (framesToProcess--)
		{
			unsigned readIndex = static_cast<unsigned>(virtualReadIndex);
			double factor = virtualReadIndex - readIndex;

			// For linear interpolation we need the next sample-frame too.
			unsigned readIndex2 = readIndex + 1;

			if (readIndex2 >= bufferLength)
			{
#if 0           // TODO: loop() と書いてあるが、サンプル数が result->length() より小さいときのみという条件も入れておかないとプチノイズが乗る
				if (loop()) {
					// Make sure to wrap around at the end of the buffer.
					readIndex2 = static_cast<unsigned>(virtualReadIndex + 1 - virtualDeltaFrames);
				}
				else
#endif
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

        printf("");
	}

	//printf("writeIndex:%d\n", writeIndex);

	result->clearSilentFlag();

	m_virtualReadIndex = virtualReadIndex;
}

bool CoreAudioSourceNode::renderSilenceAndFinishIfNotLooping(AudioBus * bus, unsigned index, size_t framesToProcess)
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

	//m_playingState = m_requestedPlayingState;
}

//==============================================================================
// CoreAudioPannerNode

CoreAudioPannerNode::CoreAudioPannerNode(AudioDevice* context)
	: CoreAudioNode(context)
{
}

void CoreAudioPannerNode::init()
{
	CoreAudioNode::init();

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



	AudioBus* destination = outputPin(0)->bus();

	if (!m_panner.get()) {
		destination->setSilentAndZero();
		return;
	}

	AudioBus* source = inputPin(0)->bus();
	if (!source) {
		destination->setSilentAndZero();
		return;
	}

	double azimuth;
	double elevation;
	azimuthElevation(&azimuth, &elevation);
	//printf("azimuth:%f\n", azimuth);

	m_panner->Pan(azimuth, elevation, source, destination, destination->length(), AudioBus::kSpeakers);

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

CoreAudioDestinationNode::CoreAudioDestinationNode(AudioDevice* context)
	: CoreAudioNode(context)
{
}

void CoreAudioDestinationNode::init()
{
	CoreAudioNode::init();
	addInputPin(2);
}

void CoreAudioDestinationNode::render(float * outputBuffer, int length)
{


    ////AudioChannel* ch1 = output->channel(0);
    ////AudioChannel* ch2 = output->channel(1);
    ////float* data1 = ch1->mutableData();
    ////float* data2 = ch2->mutableData();
    //static float step = 0.0;
    //static float frequency = 440;

    //for (int i = 0; i < length / 2; i++) {
    //    float v = std::sin(2.0 * Math::PI * step);
    //    outputBuffer[(i*2) + 0] = outputBuffer[(i * 2) + 1] = v;
    //    step += frequency / context()->deviceSamplingRate();//4096;
    //}

    //return;


	AudioBus* bus = inputPin(0)->pull();

	// Clamp values at 0db (i.e., [-1.0, 1.0])
	const float kLowThreshold = -1.0f;
	const float kHighThreshold = 1.0f;
	for (unsigned i = 0; i < bus->numberOfChannels(); ++i)
	{
		AudioChannel * channel = bus->channel(i);
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

