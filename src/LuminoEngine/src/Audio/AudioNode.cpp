
#include "Internal.hpp"
#include "AudioNode.hpp"
#include "AudioDecoder.hpp"	// for AudioSourceNode
#include <Lumino/Engine/Diagnostics.hpp>

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
// AudioChannel

AudioChannel::AudioChannel()
{
}

void AudioChannel::initialize(size_t length)
{
	Object::initialize();
	m_data.resize(length);
}

void AudioChannel::clear()
{
	memset(data(), 0, sizeof(float) * length());
}

void AudioChannel::copyTo(float* buffer, size_t bufferLength, size_t stride) const
{
	const float* src = data();
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

void AudioChannel::copyFrom(const float * buffer, size_t bufferLength, size_t stride)
{
	float* dst = data();
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

void AudioChannel::sumFrom(const AudioChannel * ch)
{
	vadd(data(), ch->data(), data(), length());
}

//==============================================================================
// AudioBus

AudioBus::AudioBus()
{
}

void AudioBus::initialize(int channelCount, size_t length)
{
	Object::initialize();
	for (int i = 0; i < channelCount; ++i)
	{
		m_channels.add(newObject<AudioChannel>(length));
	}
}

void AudioBus::clear()
{
	for (auto& ch : m_channels)
	{
		ch->clear();
	}
}

void AudioBus::mergeToChannelBuffers(float* buffer, size_t length)
{
	assert(m_channels.size() == 2);
	assert(m_channels[0]->length() * 2 == length);

	for (int i = 0; i < m_channels.size(); i++)
	{
		m_channels[i]->copyTo(buffer + i, length - i, m_channels.size());
	}
}

void AudioBus::separateFrom(const float * buffer, size_t length, int channelCount)
{
	assert(m_channels.size() == 2);
	assert(m_channels[0]->length() * 2 == length);

	for (int i = 0; i < m_channels.size(); i++)
	{
		m_channels[i]->copyFrom(buffer + i, length - i, m_channels.size());
	}
}

void AudioBus::sumFrom(const AudioBus* bus)
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
// AudioOutputPin

AudioInputPin::AudioInputPin()
	: m_ownerNode(nullptr)
{
}

void AudioInputPin::initialize(int channels)
{
	Object::initialize();
	m_summingBus = newObject<AudioBus>(channels, AudioNode::ProcessingSizeInFrames);
}

AudioBus* AudioInputPin::pull()
{
	m_summingBus->clear();

	for (auto& output : m_connectedOutputPins)
	{
		AudioBus* bus = output->pull();
		m_summingBus->sumFrom(bus);
	}

	return m_summingBus;
}

void AudioInputPin::addLinkOutput(AudioOutputPin * output)
{
	m_connectedOutputPins.add(output);
}

//==============================================================================
// AudioOutputPin

AudioOutputPin::AudioOutputPin()
	: m_ownerNode(nullptr)
{
}

void AudioOutputPin::initialize(int channels)
{
	Object::initialize();
	m_resultBus = newObject<AudioBus>(channels, AudioNode::ProcessingSizeInFrames);
}

AudioBus * AudioOutputPin::bus() const
{
	return m_resultBus;
}

AudioBus * AudioOutputPin::pull()
{
	m_ownerNode->processIfNeeded();
	return m_resultBus;
}

void AudioOutputPin::addLinkInput(AudioInputPin * input)
{
	m_connectedInputPins.add(input);
}

//==============================================================================
// AudioNode

AudioNode::AudioNode()
{
}

AudioInputPin * AudioNode::inputPin(int index) const
{
	return m_inputPins[index];
}

AudioOutputPin * AudioNode::outputPin(int index) const
{
	return m_outputPins[index];
}

void AudioNode::pullInputs()
{
	for (auto & in : m_inputPins)
	{
		in->pull();
	}
}

void AudioNode::processIfNeeded()
{
	pullInputs();
	process();
}

void AudioNode::connect(AudioNode * outputSide, AudioNode * inputSide)
{
	outputSide->outputPin(0)->addLinkInput(inputSide->inputPin(0));
	inputSide->inputPin(0)->addLinkOutput(outputSide->outputPin(0));
}

AudioInputPin* AudioNode::addInputPin(int channels)
{
	auto pin = newObject<AudioInputPin>(channels);
	pin->setOwnerNode(this);
	m_inputPins.add(pin);
	return pin;
}

AudioOutputPin* AudioNode::addOutputPin(int channels)
{
	auto pin = newObject<AudioOutputPin>(channels);
	pin->setOwnerNode(this);
	m_outputPins.add(pin);
	return pin;
}

//==============================================================================
// AudioDestinationNode

AudioSourceNode::AudioSourceNode()
{
}

void AudioSourceNode::initialize(const StringRef & filePath)
{
	AudioNode::initialize();

	// TODO:
	auto decoder = makeRef<WaveDecoder>();
	auto diag = newObject<DiagnosticsManager>();
	decoder->initialize(FileStream::create(filePath), diag);
	m_decoder = decoder;
	//tmpBuffer.resize(AudioNode::ProcessingSizeInFrames * m_masterChannels);

	auto* pin = addOutputPin(m_decoder->audioDataInfo().channelCount);
	m_readBuffer.resize(pin->bus()->length() * m_decoder->audioDataInfo().channelCount);
}

void AudioSourceNode::process()
{
	AudioBus* result = outputPin(0)->bus();

	m_decoder->read2(m_readBuffer.data(), result->length());
	result->separateFrom(m_readBuffer.data(), m_readBuffer.size(), m_decoder->audioDataInfo().channelCount);
}

//==============================================================================
// AudioDestinationNode

AudioDestinationNode::AudioDestinationNode()
{
}

void AudioDestinationNode::initialize()
{
	AudioNode::initialize();
	addInputPin(2);
}

void AudioDestinationNode::render(float * outputBuffer, int length)
{
	AudioBus* bus = inputPin(0)->pull();
	bus->mergeToChannelBuffers(outputBuffer, length);
}

void AudioDestinationNode::process()
{
	LN_UNREACHABLE();
}


} // namespace detail
} // namespace ln

