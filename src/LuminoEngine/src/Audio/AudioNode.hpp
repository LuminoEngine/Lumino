#pragma once
#include "AudioDevice.hpp"	// for IAudioDeviceRenderCallback

namespace ln {
namespace detail {
class AudioNode;
class AudioInputPin;
class AudioOutputPin;
class AudioDecoder;

// AudioNode 間の音声データの受け渡しに使用するバッファ。
// データは 浮動小数点サンプルで、範囲 -1.0 ~ +1.0 となっている。
class AudioChannel
	: public Object
{
public:
	AudioChannel();
	virtual ~AudioChannel() = default;
	void initialize(size_t length);

	float* data() { return m_data.data(); }
	const float* data() const { return m_data.data(); }
	size_t length() const { return m_data.size(); }

	void clear();
	void copyTo(float* buffer, size_t bufferLength, size_t stride) const;
	void copyFrom(const float* buffer, size_t bufferLength, size_t stride);
	void sumFrom(const AudioChannel* ch);

private:
	std::vector<float> m_data;
};

// AudioNode 間の音声データの受け渡しに使用する AudioChannel のコレクション。
class AudioBus
	: public Object
{
public:
	AudioBus();
	virtual ~AudioBus() = default;
	void initialize(int channelCount, size_t length);

	size_t length() const { return m_channels[0]->length(); }	// フレーム数
	int channelCount() const { return m_channels.size(); }
	AudioChannel* channel(int index) const { return m_channels[index]; }
	void clear();
	void mergeToChannelBuffers(float* buffer, size_t length);
	void separateFrom(const float* buffer, size_t length, int channelCount);
	void sumFrom(const AudioBus* bus);

private:
	List<Ref<AudioChannel>> m_channels;
};

class AudioInputPin
	: public Object
{
public:
	AudioInputPin();
	virtual ~AudioInputPin() = default;
	void initialize(int channels);

	AudioBus* pull();

	// TODO: internal
	void setOwnerNode(AudioNode* node) { m_ownerNode = node; }
	void addLinkOutput(AudioOutputPin* output);

private:
	AudioNode * m_ownerNode;
	Ref<AudioBus> m_summingBus;	// Total output
	List<Ref<AudioOutputPin>> m_connectedOutputPins;
};

class AudioOutputPin
	: public Object
{
public:
	AudioOutputPin();
	virtual ~AudioOutputPin() = default;
	void initialize(int channels);

	AudioBus* bus() const;

	// process() から呼び出してはならない
	AudioBus* pull();


	// TODO: internal
	void setOwnerNode(AudioNode* node) { m_ownerNode = node; }
	void addLinkInput(AudioInputPin* input);

private:
	AudioNode* m_ownerNode;
	Ref<AudioBus> m_resultBus;	// result of m_ownerNode->process()
	List<Ref<AudioInputPin>> m_connectedInputPins;
};


class AudioNode
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

	AudioNode();
	virtual ~AudioNode() = default;

	AudioInputPin* inputPin(int index) const;
	AudioOutputPin* outputPin(int index) const;
	void processIfNeeded();

	// in=1, out=1 用のユーティリティ
	static void connect(AudioNode* outputSide, AudioNode* inputSide);

protected:
	// Do not call after object initialzation.
	AudioInputPin* addInputPin(int channels);
	// Do not call after object initialzation.
	AudioOutputPin* addOutputPin(int channels);

	void pullInputs();

	// output(x) へ書き込む。要素数は自分で addOutputPin() した数だけ。
	// input は pull 済み。データを取り出すだけでよい。
	virtual void process() = 0;

private:
	List<Ref<AudioInputPin>> m_inputPins;
	List<Ref<AudioOutputPin>> m_outputPins;
};

class AudioSourceNode
	: public AudioNode
{
public:

protected:
	virtual void process() override;

LN_CONSTRUCT_ACCESS:
	AudioSourceNode();
	virtual ~AudioSourceNode() = default;
	void initialize(const StringRef& filePath);

private:
	Ref<AudioDecoder> m_decoder;
	std::vector<float> m_readBuffer;
};

class AudioDestinationNode
	: public AudioNode
	, public IAudioDeviceRenderCallback
{
public:
	AudioDestinationNode();
	virtual ~AudioDestinationNode() = default;
	void initialize();

protected:
	virtual void render(float* outputBuffer, int length) override;
	virtual void process() override;

private:
};

} // namespace detail
} // namespace ln

