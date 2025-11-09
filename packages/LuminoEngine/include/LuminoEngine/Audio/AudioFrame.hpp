#pragma once

namespace ln {
namespace a2 {
class AudioFrameChannel;

/**
 * 音響処理の単位となる音声データのフレームを表します。
 * 
 * AudioFrame は AudioFrameChannel のコレクションで、各チャンネルごとの音声データを保持します。
 * 
 * AudioFrame                |<-- 2048 floats -->|
 * - AudioFrameChannel[0](L) |...................|
 * - AudioFrameChannel[1](R) |...................|
 * 
 * データ構造は chromium(blink) や godot を参考にしていますが、
 * これはどちらかというと chromium 寄りです。
 * chromium では AudioBus というクラスに対応しますが、Web で検索すると
 * AudioBus という名前はどちらかというと godot で使われているような
 * ミキサーに近い概念を指すことが多いようでしたので、このような名前にしています。
 * 
 * godot を参考にしていますが godot の AudioFrame は L-R の１サンプリング分の
 * 構造体であり、それとは異なる点に注意してください。理由は次の通りです。
 * - chromium の他、いくつかのサウンドプログラミングのサンプルでは
 *   ch 別の配列を使っているのが多いように見えたため。
 * - 波形編集ソフトの見た目と同じようなイメージが出来ると考えたため。
 * - godot では 3D オーディオの計算などで 2ch では足りない場合に
 *   audio_vector という固有の概念の配列を使って対応していたが、
 *   それが直感的にわかりにくいと感じたため。
 */
class AudioFrame : public RefObject {

    // 1度の process で処理するサンプル数。
    // = 各 Audio API に送り込む 1 回分のサンプル数。
    // 1 チャンネル分。
    // レート 44100 の場合はだいたい 1/16 秒くらい。そのくらいに１度 process が実行される。
    // XAudio では 128 の倍数である必要があるので注意。https://msdn.microsoft.com/ja-jp/library/microsoft.directx_sdk.xaudio2.xaudio2_buffer(v=vs.85).aspx
    // 値を小さくするほど (高レベルAPIとしての) 演奏開始から実際に音が鳴るまでの遅延が少なくなるが、process の回数 (ノードをたどる回数) が増えるので処理は重くなる。
    static const int PROCESSING_SIZE_IN_FRAMES = 2048;

    // NOTE: v0.10.0 までの ARIAudioBus に相当するクラス。
    //	通常、 1秒分の音声データを保持するバッファ。
    //	chome(blink) の Bus に相当する。
    //	godot だと AudioFrame は L-R の１サンプリング分の構造体なので、godot を参考にする際は注意。

    // NOTE: インターリーブ配列とするかどうか
    //  インターリーブ配列は、例えば 2ch の場合、LRLRLRLR... というようにデータが並ぶ。
    //  これは Master からサウンドドライバに流す音声データの形式。
    //  godot はこの形式だが、 chome(blink) や WebAudio、 LoSound は非インターリーブ配列を使っている。
    //  godot は常に 2ch を前提としているため、音声フィルタは全てそれを前提に作られている。
    //  インターリーブ配列の方がドライバに送る際に変換が不要であるため、効率が良い。
    //  一般的なのは非インターリーブ配列の方だと思う。

	// AudioEffectDistortionInstance::process
};

/**
 * 
 */
class AudioFrameChannel : public URefObject {};

} // namespace a2
} // namespace ln


#if 0
namespace ln {

//namespace blink {
//	class SincResampler;
//}

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

// The buffer used for data stream between a audio nodes.
// Data is floating point, and range is -1.0 ~ +1.0
class AudioChannel
	: public RefObject
{
public:
	AudioChannel(size_t length);
	virtual ~AudioChannel() = default;

	float* mutableData() { clearSilentFlag();  return m_data.data(); }	// Direct access to PCM sample data. clears silent flag.
	const float* constData() const { return m_data.data(); }
	size_t length() const { return m_data.size(); }

	void setSilentAndZero();
	void clearSilentFlag() { m_isSilent = false; }
	bool isSilent() const { return m_isSilent; }

	//void clear();
	void copyTo(float* buffer, size_t bufferLength, size_t stride) const;
	void copyFrom(const float* buffer, size_t bufferLength, size_t stride);
	void copyFrom(const AudioChannel* ch);
	void sumFrom(const AudioChannel* ch);
    void fillZero(size_t start, size_t length); // isSilent は変化しない

	// chromium interface
	float* MutableData() { return mutableData(); }
	const float* Data() const { return constData(); }


private:
	std::vector<float> m_data;
	bool m_isSilent;
};

// collection of a audio channels.
class ARIAudioBus
	: public RefObject
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

	ARIAudioBus();
	virtual ~ARIAudioBus() = default;
	void initialize2(int channelCount, size_t length, int sampleRate = 0);

	size_t length() const { return m_validLength; }	// フレーム数
	//void setValidLength(size_t length) { m_validLength = length; }
	//size_t fullLength() const { return m_channels[0]->length(); }
	int sampleRate() const { return m_sampleRate; }
	void setSampleRate(int value) { m_sampleRate = value; }

	int channelCount() const { return m_channels.size(); }
	int numberOfChannels() const { return m_channels.size(); }

	AudioChannel* channel(int index) const { return m_channels[index]; }
	AudioChannel* channelByType(unsigned  type);
	const AudioChannel* channelByType(unsigned  type) const;

	void setSilentAndZero();	// set silent flag, and zero clear buffers if needed. if set a valid samples in process(), please call clearSilentFlag()
	void clearSilentFlag();
	bool isSilent() const;	// return true if all child true.

    void fillZero(size_t start, size_t length);
	void mergeToChannelBuffers(float* buffer, size_t length);
	void separateFrom(const float* buffer, size_t length, int channelCount);
	void sumFrom(const ARIAudioBus* sourceBus);

    void copyFrom(ARIAudioBus* source);
	void copyWithGainFrom(const ARIAudioBus& source_bus, float gain);
	void copyBySampleRateConverting(const ARIAudioBus* source_bus, int new_sample_rate);
	bool topologyMatches(const ARIAudioBus& bus) const;

	// chromium interface
	int NumberOfChannels() const { return m_channels.size(); }
	AudioChannel* Channel(int index) const { return channel(index); }
	AudioChannel* ChannelByType(unsigned  type) { return channelByType(type); }


private:
    void sumFromByUpMixing(const ARIAudioBus* sourceBus);
    void sumFromByDownMixing(const ARIAudioBus* sourceBus);
    void discreteSumFrom(const ARIAudioBus* sourceBus);

	List<Ref<AudioChannel>> m_channels;
	size_t m_validLength;
	int m_sampleRate;

	int m_layout = kLayoutCanonical;
};

} // namespace ln

#endif
