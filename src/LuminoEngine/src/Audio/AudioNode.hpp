#pragma once

namespace ln {
namespace detail {

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

private:
	List<Ref<AudioChannel>> m_channels;
};

class AudioInputPin
	: public Object
{
public:
	AudioInputPin();
	virtual ~AudioInputPin() = default;
	void initialize();

private:
};

class AudioOutputPin
	: public Object
{
public:
	AudioOutputPin();
	virtual ~AudioOutputPin() = default;
	void initialize();

private:

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

protected:
	// Do not call after object initialzation.
	void addInputPin();
	// Do not call after object initialzation.
	void addOutputPin();

private:
	List<Ref<AudioInputPin>> m_inputPins;
	List<Ref<AudioOutputPin>> m_outputPins;
};

class AudioDestinationNode
	: public AudioNode
{
public:
	AudioDestinationNode();
	virtual ~AudioDestinationNode() = default;
	void initialize();

private:
};

} // namespace detail
} // namespace ln

