
#include "Internal.hpp"
#include "../Decoder/AudioDecoder.hpp"
#include "../Backend/AudioDevice.hpp"
#include "ChromiumWebCore.hpp"
#include "ARIAudioBus.hpp"
#include "ARIOutputPin.hpp"
#include "ARISourceNode.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ARISourceNode

ARISourceNode::ARISourceNode(AudioDevice* context, AudioNode* frontNode)
	: ARINode(context, frontNode)
	, m_virtualReadIndex(0)
	, m_playbackRate(1.0f)
	, m_seekFrame(0)
	, m_playingState(State::Stopped)
	//, m_requestedPlayingState(State::None)
	, m_resetRequested(false)
    , m_loop(false)
{
}

void ARISourceNode::init(const Ref<AudioDecoder>& decoder)
{
	ARINode::init();

	// TODO:
	//auto decoder = makeRef<WaveDecoder>();
	//decoder->init(FileStream::create(filePath), diag);
	m_decoder = decoder;
	//tmpBuffer.resize(ARINode::ProcessingSizeInFrames * m_masterChannels);

	unsigned numChannels = m_decoder->audioDataInfo().channelCount;
	auto* pin = addOutputPin(numChannels);
	//m_readBuffer.resize(pin->bus()->length() * numChannels);


	staging.playbackRate = 1.0f;
	staging.requestedState = ln::PlayingState::NoChanged;
	staging.resetRequire = false;
	staging.loop = false;
	

	resetSourceBuffers();
}

void ARISourceNode::setPlaybackRate(float rate)
{
	m_playbackRate = rate;
	resetSourceBuffers();
}

void ARISourceNode::start()
{
    m_playingState = State::Playing;
	//m_requestedPlayingState = State::Playing;
}

void ARISourceNode::stop()
{
    m_playingState = State::Stopped;
	//m_requestedPlayingState = State::Stopped;
	m_resetRequested = true;
}

void ARISourceNode::reset()
{
	m_seekFrame = 0;
}

void ARISourceNode::finish()
{
    m_playingState = State::Stopped;
	//m_requestedPlayingState = State::Stopped;
}

unsigned ARISourceNode::numberOfChannels() const
{
	return outputPin(0)->bus()->channelCount();
}

void ARISourceNode::resetSourceBuffers()
{
	ARIOutputPin* pin = outputPin(0);

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
		m_resamplingBus = makeRef<ARIAudioBus>();
		m_resamplingBus->initialize2(numChannels, m_readFrames, m_decoder->audioDataInfo().sampleRate);
	}

	m_sourceBus = makeRef<ARIAudioBus>();
	m_sourceBus->initialize2(numChannels, baseFrames);



}

double ARISourceNode::calculatePitchRate()
{
	// TODO: doppler from associatd panner.
	
	double totalRate = m_playbackRate;

	// zero rate is illegal
	if (totalRate <= 0.0) {
		totalRate = 1.0;
	}

	return totalRate;
}

void ARISourceNode::onCommit()
{
	detail::ScopedWriteLock lock(staging.m_mutex);

	if (staging.resetRequire) {
		reset();
		staging.resetRequire = false;
	}

	switch (staging.requestedState)
	{
	case ln::PlayingState::NoChanged:
		break;
	case  ln::PlayingState::Stop:
		stop();
		break;
	case  ln::PlayingState::Play:
		reset();
		start();
		break;
	case  ln::PlayingState::Pause:
		LN_NOTIMPLEMENTED();
		break;
	default:
		break;
	}
	staging.requestedState = ln::PlayingState::NoChanged;

	setPlaybackRate(staging.playbackRate);
	setLoop(staging.loop);
}

// https://github.com/chromium/chromium/blob/ba96c018682416a7b2ec77876404b14322aa1b54/third_party/blink/renderer/modules/webaudio/audio_buffer_source_node.cc
void ARISourceNode::process()
{
	updatePlayingState();


	ARIAudioBus* result = outputPin(0)->bus();

	if (m_playingState != State::Playing) {
		result->setSilentAndZero();
		return;
	}


	//size_t bufferLength = //result->length();
	unsigned numChannels = m_decoder->audioDataInfo().channelCount;

    size_t readFrames = 0;
    {
        if (loop()) {
            size_t requestLength = m_readFrames;
            float* buf = m_readBuffer.data();
            do
            {
                readFrames = m_decoder->read(m_seekFrame, buf, requestLength);
				m_seekFrame += readFrames;
                if (readFrames < requestLength) {    // EOF
					m_seekFrame = 0;
                }
                requestLength -= readFrames;
                buf += readFrames;
            } while (requestLength > 0);

            readFrames = m_readFrames;
        }
        else {
            readFrames = m_decoder->read(m_seekFrame, m_readBuffer.data(), m_readFrames);
			m_seekFrame += readFrames;
            if (readFrames == 0) {    // EOF
                result->setSilentAndZero();
                return;
            }
        }


    }



	size_t readSamples = readFrames * numChannels;


    //printf("bufferLength:%d , result->length():%d", bufferLength, result->length());

	double pitchRate = calculatePitchRate();

	// 

	unsigned endFrame = readFrames;

	//if (m_virtualReadIndex >= endFrame)
		m_virtualReadIndex = 0; // reset to start


	unsigned writeIndex = 0;

	double virtualReadIndex = m_virtualReadIndex;

	double virtualDeltaFrames = readFrames;	// Number of frames processed this time process().
	double virtualEndFrame = readFrames;
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

        int remain = result->length() - readFrames;
		renderSilenceAndFinishIfNotLooping(result, readFrames, remain);
        //if (remain > 0) {
        //    //result->fillZero(bufferLength, remain);
        //    //result->fillZero(0, result->length());
        //    //printf("eof\n");
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

			if (readIndex2 >= readFrames)
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
			if (readIndex >= readFrames || readIndex2 >= readFrames)
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

// bus が持っているすべての Channel のバッファを、
// startIndex から framesToProcess 個分、0 で埋める。
// 非ループ再生時に要求された bus サイズに対して Decoder から呼んだサイズが足りないときに、足りない分を埋めるのに使う。
bool ARISourceNode::renderSilenceAndFinishIfNotLooping(ARIAudioBus* bus, unsigned startIndex, size_t framesToProcess)
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
				memset(bus->channel(i)->mutableData() + startIndex, 0, sizeof(float) * framesToProcess);
			}
			finish();
		}

		return true;
	}
	return false;
}

void ARISourceNode::updatePlayingState()
{
	if (m_resetRequested) {
		reset();
	}

	//m_playingState = m_requestedPlayingState;
}

} // namespace detail
} // namespace ln

