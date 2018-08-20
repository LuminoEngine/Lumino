#pragma once
#include "AudioDevice.hpp"	// for IAudioDeviceRenderCallback

namespace ln {
namespace detail {
class CoreAudioNode;
class CoreAudioInputPin;
class CoreAudioOutputPin;
class AudioDecoder;

//enum class Channel : int
//{
//	First = 0,
//	Left = 0,
//	Right = 1,
//	Center = 2, // center and mono are the same
//	Mono = 2,
//	LFE = 3,
//	SurroundLeft = 4,
//	SurroundRight = 5,
//};

// CoreAudioNode 間の音声データの受け渡しに使用するバッファ。
// データは 浮動小数点サンプルで、範囲 -1.0 ~ +1.0 となっている。
class CoreAudioChannel
	: public Object
{
public:
	CoreAudioChannel();
	virtual ~CoreAudioChannel() = default;
	void initialize(size_t length);

	float* mutableData() { clearSilentFlag();  return m_data.data(); }	// Direct access to PCM sample data. clears silent flag.
	const float* constData() const { return m_data.data(); }
	size_t length() const { return m_data.size(); }

	void setSilentAndZero();
	void clearSilentFlag() { m_isSilent = false; }
	bool isSilent() const { return m_isSilent; }

	//void clear();
	void copyTo(float* buffer, size_t bufferLength, size_t stride) const;
	void copyFrom(const float* buffer, size_t bufferLength, size_t stride);
	void copyFrom(const CoreAudioChannel* ch);
	void sumFrom(const CoreAudioChannel* ch);


	// chromium interface
	float* MutableData() { return mutableData(); }
	const float* Data() const { return constData(); }


private:
	std::vector<float> m_data;
	bool m_isSilent;
};

// CoreAudioNode 間の音声データの受け渡しに使用する CoreAudioChannel のコレクション。
class CoreAudioBus
	: public Object
{
public:
	enum
	{
		kLayoutCanonical = 0
	};

	enum ChannelInterpretation {
		kSpeakers,
		kDiscrete,
	};


	enum {
		kChannelLeft = 0,
		kChannelRight = 1,
		kChannelCenter = 2,  // center and mono are the same
		kChannelMono = 2,
		kChannelLFE = 3,
		kChannelSurroundLeft = 4,
		kChannelSurroundRight = 5,
	};

	CoreAudioBus();
	virtual ~CoreAudioBus() = default;
	void initialize(int channelCount, size_t length);

	size_t length() const { return m_validLength; }	// フレーム数
	//void setValidLength(size_t length) { m_validLength = length; }
	//size_t fullLength() const { return m_channels[0]->length(); }

	int channelCount() const { return m_channels.size(); }
	int numberOfChannels() const { return m_channels.size(); }

	CoreAudioChannel* channel(int index) const { return m_channels[index]; }
	CoreAudioChannel* channelByType(unsigned  type);
	const CoreAudioChannel* channelByType(unsigned  type) const;

	void setSilentAndZero();	// set silent flag, and zero clear buffers if needed. if set a valid samples in process(), please call clearSilentFlag()
	void clearSilentFlag();
	bool isSilent() const;	// return true if all child true.


	void mergeToChannelBuffers(float* buffer, size_t length);
	void separateFrom(const float* buffer, size_t length, int channelCount);
	void sumFrom(const CoreAudioBus* bus);

	void copyWithGainFrom(const CoreAudioBus& source_bus, float gain);
	bool topologyMatches(const CoreAudioBus& bus) const;

	// chromium interface
	int NumberOfChannels() const { return m_channels.size(); }
	CoreAudioChannel* Channel(int index) const { return channel(index); }
	CoreAudioChannel* ChannelByType(unsigned  type) { return channelByType(type); }


private:
	List<Ref<CoreAudioChannel>> m_channels;
	size_t m_validLength;

	int m_layout = kLayoutCanonical;
};

class AudioContextCore
	: public RefObject
{
public:
	const AudioListenerParams& listener() const { return m_listener; }

private:
	AudioListenerParams m_listener;
};

class CoreAudioInputPin
	: public Object
{
public:
	CoreAudioInputPin();
	virtual ~CoreAudioInputPin() = default;
	void initialize(int channels);

	CoreAudioBus* bus() const;

	CoreAudioBus* pull();

	// TODO: internal
	void setOwnerNode(CoreAudioNode* node) { m_ownerNode = node; }
	void addLinkOutput(CoreAudioOutputPin* output);


private:

	CoreAudioNode * m_ownerNode;
	Ref<CoreAudioBus> m_summingBus;	// Total output
	List<Ref<CoreAudioOutputPin>> m_connectedOutputPins;
};

class CoreAudioOutputPin
	: public Object
{
public:
	CoreAudioOutputPin();
	virtual ~CoreAudioOutputPin() = default;
	void initialize(int channels);

	CoreAudioBus* bus() const;

	// process() から呼び出してはならない
	CoreAudioBus* pull();


	// TODO: internal
	void setOwnerNode(CoreAudioNode* node) { m_ownerNode = node; }
	void addLinkInput(CoreAudioInputPin* input);

private:
	CoreAudioNode* m_ownerNode;
	Ref<CoreAudioBus> m_resultBus;	// result of m_ownerNode->process()
	List<Ref<CoreAudioInputPin>> m_connectedInputPins;
};


class CoreAudioNode
	: public Object
{
public:
	// 1度の process で処理するサンプル数。
	// = 各 Audio API に送り込む 1 回分のサンプル数。
	// 1 チャンネル分。
	// レート 44100 の場合はだいたい 1/16 秒くらい。そのくらいに１度 process が実行される。
	// XAudio では 128 の倍数である必要があるので注意。https://msdn.microsoft.com/ja-jp/library/microsoft.directx_sdk.xaudio2.xaudio2_buffer(v=vs.85).aspx
	// 値を小さくするほど (高レベルAPIとしての) 演奏開始から実際に音が鳴るまでの遅延が少なくなるが、process の回数 (ノードをたどる回数) が増えるので処理は重くなる。
	static const int ProcessingSizeInFrames = 2048;

	CoreAudioNode(AudioContextCore* context);
	virtual ~CoreAudioNode() = default;
	void initialize();

	AudioContextCore* context() const { return m_context; }

	CoreAudioInputPin* inputPin(int index) const;
	CoreAudioOutputPin* outputPin(int index) const;
	void processIfNeeded();

	// in=1, out=1 用のユーティリティ
	static void connect(CoreAudioNode* outputSide, CoreAudioNode* inputSide);

protected:
	// Do not call after object initialzation.
	CoreAudioInputPin* addInputPin(int channels);
	// Do not call after object initialzation.
	CoreAudioOutputPin* addOutputPin(int channels);

	void pullInputs();

	// output(x) へ書き込む。要素数は自分で addOutputPin() した数だけ。
	// input は pull 済み。データを取り出すだけでよい。
	// output(x) のバッファクリアは process() の実装側で行う。例えば消音したい場合は必ず process 無いでクリア (setSilentAndZero() を呼び出す)
	virtual void process() = 0;

private:
	AudioContextCore* m_context;
	List<Ref<CoreAudioInputPin>> m_inputPins;
	List<Ref<CoreAudioOutputPin>> m_outputPins;
};

class CoreAudioSourceNode
	: public CoreAudioNode
{
public:

protected:
	virtual void process() override;

public:
	CoreAudioSourceNode(AudioContextCore* context);
	virtual ~CoreAudioSourceNode() = default;
	void initialize(const Ref<AudioDecoder>& decoder);

	void setPlaybackRate(float rate);

	bool loop() const { return false; }

	
	void start();
	void stop();
	void reset();	// TODO: internal
	void finish();	// TODO: internal

private:
	unsigned numberOfChannels() const;
	void resetSourceBuffers();
	double calculatePitchRate();
	bool renderSilenceAndFinishIfNotLooping(CoreAudioBus * bus, unsigned index, size_t framesToProcess);
	void updatePlayingState();

	Ref<AudioDecoder> m_decoder;
	std::vector<float> m_readBuffer;
	Ref<CoreAudioBus> m_sourceBus;

	// Current playback position.
	// Since it's floating-point, it interpolate sub-samples.
	double m_virtualReadIndex;

	float m_playbackRate;
	size_t m_readFrames;
	size_t m_seekFrame;

	// scheduling
	enum class PlayingState
	{
		None,
		Stopped,
		Playing,
		Pausing,
	};
	PlayingState m_playingState;
	PlayingState m_requestedPlayingState;
	bool m_resetRequested;
};

namespace blink { 
class Panner;
class DistanceEffect;
class ConeEffect;
}

class CoreAudioPannerNode
	: public CoreAudioNode
{
public:

protected:
	virtual void process() override;

public:
	CoreAudioPannerNode(AudioContextCore* context);
	virtual ~CoreAudioPannerNode() = default;
	void initialize();

private:
	void azimuthElevation(double* outAzimuth, double* outElevation);
	float distanceConeGain();

	AudioEmitterParams m_emitter;

	std::shared_ptr<blink::Panner> m_panner;
	std::shared_ptr<blink::DistanceEffect> m_distanceEffect;
	std::shared_ptr<blink::ConeEffect> m_coneEffect;
};

class CoreAudioDestinationNode
	: public CoreAudioNode
	, public IAudioDeviceRenderCallback
{
public:
	CoreAudioDestinationNode(AudioContextCore* context);
	virtual ~CoreAudioDestinationNode() = default;
	void initialize();

protected:
	virtual void render(float* outputBuffer, int length) override;
	virtual void process() override;

private:
};

class Audio3DModule
{
public:
	template <typename T>
	static void fixNANs(T& x)
	{
		if (std::isnan(T(x)) || std::isinf(x)) x = T(0);
	}

	inline static bool isZero(const Vector3 &v) { return fabsf(v.x) < FLT_EPSILON && fabsf(v.y) < FLT_EPSILON && fabsf(v.z) < FLT_EPSILON; }
	inline static float magnitude(const Vector3 &v) { return sqrtf(Vector3::dot(v, v)); }

	static float dopplerRate(const AudioListenerParams& listener, const AudioEmitterParams& emitter)
	{
		double dopplerShift = 1.0;

		// TODO: ソースもリスナーも変更されていない場合の最適化
		double dopplerFactor = listener.m_dopplerFactor;

		if (dopplerFactor > 0.0)
		{
			double speedOfSound = listener.m_speedOfSound;

			// Don't bother if both source and listener have no velocity
			bool sourceHasVelocity = !isZero(emitter.m_velocity);
			bool listenerHasVelocity = !isZero(listener.m_velocity);

			if (sourceHasVelocity || listenerHasVelocity)
			{
				// Calculate the source to listener vector
				Vector3 sourceToListener = emitter.m_position - listener.m_position;

				double sourceListenerMagnitude = magnitude(sourceToListener);

				double listenerProjection = Vector3::dot(sourceToListener, listener.m_velocity) / sourceListenerMagnitude;
				double sourceProjection = Vector3::dot(sourceToListener, emitter.m_velocity) / sourceListenerMagnitude;

				listenerProjection = -listenerProjection;
				sourceProjection = -sourceProjection;

				double scaledSpeedOfSound = speedOfSound / dopplerFactor;
				listenerProjection = std::min(listenerProjection, scaledSpeedOfSound);
				sourceProjection = std::min(sourceProjection, scaledSpeedOfSound);

				dopplerShift = ((speedOfSound - dopplerFactor * listenerProjection) / (speedOfSound - dopplerFactor * sourceProjection));
				fixNANs(dopplerShift); // avoid illegal values

				// Limit the pitch shifting to 4 octaves up and 3 octaves down.
				if (dopplerShift > 16.0)
					dopplerShift = 16.0;
				else if (dopplerShift < 0.125)
					dopplerShift = 0.125;
			}
		}

		return static_cast<float>(dopplerShift);
	}

};

} // namespace detail
} // namespace ln

