#pragma once

namespace ln {
class DiagnosticsManager;

namespace detail {

enum class PCMFormat
{
	U8,
	S8,
	S16L,	// L = little endian
	S24L,
	S32L,
	S64L,
	Float,
	Double,
};

struct AudioDataInfo
{
	int channelCount;
	int sampleRate;
	double totalSeconds;
	size_t frameSize; // samples の、1フレームの要素数 (channelCount * bits per sample)
	size_t totalSamples;	// 全サンプル数。ステレオの場合、複数チャンネル込み。
	//std::vector<float> samples;
	PCMFormat sourceFormat;
	size_t byteParSample;
};

// TODO: できるだけキャッシュして共有して使いたい。シークは read でやる。できるだけmutable な状態を持たない。
class AudioDecoder
	: public RefObject
{
public:
	virtual ~AudioDecoder() = default;
	virtual const AudioDataInfo& audioDataInfo() const = 0;
	virtual void seekToSample(size_t sampleNumber) = 0;
	virtual size_t read2(float* buffer, uint32_t requestFrames) = 0;	// チャンネル数2の時に frames=1 で呼び出すと、2サンプル取り出す
	virtual void reset() = 0;

	static void convertToFloat32(float* dst, const void* src, const size_t length, PCMFormat format);
	static void convertFromFloat32(void * dst, const float * src, const size_t length, PCMFormat format);

	//virtual void getLoopState(uint32_t* begin, uint32_t* length) const = 0;
private:
};

class WaveDecoder
	: public AudioDecoder
{
public:
	virtual ~WaveDecoder() = default;
	void initialize(Stream* stream, DiagnosticsManager* diag);
	virtual const AudioDataInfo& audioDataInfo() const override;
	virtual void seekToSample(uint32_t sampleNumber) override;
	virtual uint32_t read2(float* buffer, uint32_t requestFrames) override;
	virtual void reset() override;

private:
	Ref<Stream> m_stream;
	AudioDataInfo m_info;
	size_t m_pcmDataOffset;
	size_t m_pcmDataLength;
	size_t m_bitDepth;
	size_t m_seekSample;
	std::vector<byte_t> m_workBuffer;
};

} // namespace detail
} // namespace ln

