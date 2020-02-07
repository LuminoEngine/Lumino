#pragma once
#include "../AudioDevice.hpp"	// for IAudioDeviceRenderCallback
#include <LuminoEngine/Audio/AudioBus.hpp>

#include <float.h>	// for FLT_EPSILON

namespace ln {
namespace detail {
class AudioNodeCore;
class CoreAudioInputPin;
class CoreAudioOutputPin;
class AudioDecoder;

namespace blink {
	class Panner;
	class DistanceEffect;
	class ConeEffect;
	class SincResampler;
}

//class AudioContextCore
//	: public RefObject
//{
//public:
//	AudioContextCore();
//	void init();
//	void dispose();
//	const Ref<AudioDevice>& device() const { return m_device; }
//	const AudioListenerParams& listener() const { return m_listener; }
//
//private:
//	Ref<AudioDevice> m_device;
//	AudioListenerParams m_listener;
//};

class PropagationParameters
{
public:
	PropagationParameters();

	int finalSamplingRate() const { return m_finalSamplingRate; }

	void setFinalSamplingRate(int rate) { m_finalSamplingRate = rate; }

private:
	int m_finalSamplingRate;
};

class CoreAudioInputPin
	: public RefObject
{
public:
	CoreAudioInputPin(int channels);
	virtual ~CoreAudioInputPin() = default;

	AudioBus* bus() const;

	AudioBus* pull();

	// TODO: internal
	void setOwnerNode(AudioNodeCore* node) { m_ownerNode = node; }
	void addLinkOutput(CoreAudioOutputPin* output);
	void removeLinkOutput(CoreAudioOutputPin* output);
	

	const List<Ref<CoreAudioOutputPin>>& connectedOutputPins() const { return m_connectedOutputPins; }
	void disconnect(int index);
	void disconnectAll();

private:

	AudioNodeCore * m_ownerNode;
	Ref<AudioBus> m_summingBus;	// Total output
	List<Ref<CoreAudioOutputPin>> m_connectedOutputPins;
};

class CoreAudioOutputPin
	: public RefObject
{
public:
	CoreAudioOutputPin(int channels);
	virtual ~CoreAudioOutputPin() = default;

	AudioBus* bus() const;

	// process() から呼び出してはならない
	AudioBus* pull();


	// TODO: internal
	void setOwnerNode(AudioNodeCore* node) { m_ownerNode = node; }
	void addLinkInput(CoreAudioInputPin* input);
	void removeLinkInput(CoreAudioInputPin* input);

	const List<Ref<CoreAudioInputPin>>& connectedInputPins() const { return m_connectedInputPins; }
	void disconnect(int index);
	void disconnectAll();

private:
	AudioNodeCore* m_ownerNode;
	Ref<AudioBus> m_resultBus;	// result of m_ownerNode->process()
	List<Ref<CoreAudioInputPin>> m_connectedInputPins;
};


class AudioNodeCore
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

	AudioNodeCore(AudioDevice* context);
	virtual ~AudioNodeCore() = default;
	void init();

	AudioDevice* context() const { return m_context; }

	CoreAudioInputPin* inputPin(int index) const;
	CoreAudioOutputPin* outputPin(int index) const;
	void processIfNeeded();

	// in=1, out=1 用のユーティリティ
	static void connect(AudioNodeCore* outputSide, AudioNodeCore* inputSide);

	void disconnectAllInputSide();
	void disconnectAllOutputSide();

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
	AudioDevice * m_context;
	List<Ref<CoreAudioInputPin>> m_inputPins;
	List<Ref<CoreAudioOutputPin>> m_outputPins;
};

class CoreAudioPannerNode
	: public AudioNodeCore
{
public:

protected:
	virtual void process() override;

public:
	CoreAudioPannerNode(AudioDevice* context);
	virtual ~CoreAudioPannerNode() = default;
	void init();

private:
	void azimuthElevation(double* outAzimuth, double* outElevation);
	float distanceConeGain();

	AudioEmitterParams m_emitter;

	std::shared_ptr<blink::Panner> m_panner;
	std::shared_ptr<blink::DistanceEffect> m_distanceEffect;
	std::shared_ptr<blink::ConeEffect> m_coneEffect;
};

class CoreAudioDestinationNode
	: public AudioNodeCore
	, public IAudioDeviceRenderCallback
{
public:
	CoreAudioDestinationNode(AudioDevice* context);
	virtual ~CoreAudioDestinationNode() = default;
	void init();

protected:
	virtual void render(float* outputBuffer, int length) override;
	virtual void process() override;

private:
	PropagationParameters m_propagationParameters;
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

