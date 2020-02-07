
#include "Internal.hpp"
#include "CoreAudioNode.hpp"
#include "../Decoder/AudioDecoder.hpp"	// for AudioSourceNodeCore
#include "ChromiumWebCore.hpp"
#include "../blink/VectorMath.h"

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
	m_summingBus->initialize2(channels, AudioNodeCore::ProcessingSizeInFrames);
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
	m_resultBus->initialize2(channels, AudioNodeCore::ProcessingSizeInFrames);
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
// AudioNodeCore

AudioNodeCore::AudioNodeCore(AudioDevice* context)
	: m_context(context)
{
}

void AudioNodeCore::init()
{
	Object::init();
}

CoreAudioInputPin * AudioNodeCore::inputPin(int index) const
{
	return m_inputPins[index];
}

CoreAudioOutputPin * AudioNodeCore::outputPin(int index) const
{
	return m_outputPins[index];
}

void AudioNodeCore::pullInputs()
{
	for (auto & in : m_inputPins)
	{
		in->pull();
	}
}

void AudioNodeCore::processIfNeeded()
{
	pullInputs();
	process();
}

void AudioNodeCore::connect(AudioNodeCore * outputSide, AudioNodeCore * inputSide)
{
	outputSide->outputPin(0)->addLinkInput(inputSide->inputPin(0));
	inputSide->inputPin(0)->addLinkOutput(outputSide->outputPin(0));
}

void AudioNodeCore::disconnectAllInputSide()
{
	for (auto& pin : m_inputPins) {
		pin->disconnectAll();
	}
}

void AudioNodeCore::disconnectAllOutputSide()
{
	for (auto& pin : m_outputPins) {
		pin->disconnectAll();
	}
}

CoreAudioInputPin* AudioNodeCore::addInputPin(int channels)
{
	auto pin = makeRef<CoreAudioInputPin>(channels);
	pin->setOwnerNode(this);
	m_inputPins.add(pin);
	return pin;
}

CoreAudioOutputPin* AudioNodeCore::addOutputPin(int channels)
{
	auto pin = makeRef<CoreAudioOutputPin>(channels);
	pin->setOwnerNode(this);
	m_outputPins.add(pin);
	return pin;
}

//==============================================================================
// CoreAudioPannerNode

CoreAudioPannerNode::CoreAudioPannerNode(AudioDevice* context)
	: AudioNodeCore(context)
{
}

void CoreAudioPannerNode::init()
{
	AudioNodeCore::init();

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
	: AudioNodeCore(context)
{
}

void CoreAudioDestinationNode::init()
{
	AudioNodeCore::init();
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

