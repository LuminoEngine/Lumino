#pragma once
#include "Common.hpp"
#include <LuminoFramework/Audio/AudioBus.hpp>

#include <float.h>	// for FLT_EPSILON

namespace ln {
namespace detail {

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

class ARINode
	: public Object
{
public:
	//class StagingData
	//{
	//protected:
	//	detail::AudioRWMutex& mutex() { return m_mutex; }

	//private:
	//	detail::AudioRWMutex m_mutex;
	//};


	// 1度の process で処理するサンプル数。
	// = 各 Audio API に送り込む 1 回分のサンプル数。
	// 1 チャンネル分。
	// レート 44100 の場合はだいたい 1/16 秒くらい。そのくらいに１度 process が実行される。
	// XAudio では 128 の倍数である必要があるので注意。https://msdn.microsoft.com/ja-jp/library/microsoft.directx_sdk.xaudio2.xaudio2_buffer(v=vs.85).aspx
	// 値を小さくするほど (高レベルAPIとしての) 演奏開始から実際に音が鳴るまでの遅延が少なくなるが、process の回数 (ノードをたどる回数) が増えるので処理は重くなる。
	static const int ProcessingSizeInFrames = 2048;

	ARINode(AudioDevice* context, AudioNode* frontNode);
	virtual ~ARINode() = default;
	void init();

	AudioDevice* context() const { return m_context; }
	AudioNode* frontNode() const { return m_frontNode; }

	bool isInputConnected() const;
	bool isOutputConnected() const;
	ARIInputPin* inputPin(int index) const;
	ARIOutputPin* outputPin(int index) const;
	void processIfNeeded();

	// in=1, out=1 用のユーティリティ
	static void connect(ARINode* outputSide, ARINode* inputSide);

	void disconnectAllInputSide();
	void disconnectAllOutputSide();
	void disconnectAll();

	bool m_marked = false;

protected:
	// Do not call after object initialzation.
	ARIInputPin* addInputPin(int channels);
	// Do not call after object initialzation.
	ARIOutputPin* addOutputPin(int channels);

	void pullInputs();

	virtual void onCommit() = 0;

	// output(x) へ書き込む。要素数は自分で addOutputPin() した数だけ。
	// input は pull 済み。データを取り出すだけでよい。
	// output(x) のバッファクリアは process() の実装側で行う。例えば消音したい場合は必ず process 無いでクリア (setSilentAndZero() を呼び出す)
	virtual void process() = 0;

private:
	AudioDevice* m_context;
	AudioNode* m_frontNode;
	List<Ref<ARIInputPin>> m_inputPins;
	List<Ref<ARIOutputPin>> m_outputPins;

	friend class ::ln::AudioContext;	// for onCcommit
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

