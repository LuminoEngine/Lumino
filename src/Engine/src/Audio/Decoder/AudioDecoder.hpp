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
	uint32_t frameSize; // samples の、1フレームの要素数 (channelCount * byteParSample)
    uint32_t totalSamples;	// 全サンプル数。frameCount * channelCount
	//std::vector<float> samples;
	PCMFormat sourceFormat;
    uint32_t byteParSample;

    uint32_t totalFrameCount() const { return totalSamples / channelCount; }
};

// TODO: できるだけキャッシュして共有して使いたい。シークは read でやる。できるだけmutable な状態を持たない。
//
// リサンプリングはされないので注意。
// 例えば以下のようなフレーム数となる。
// - 3s,22050Hz=66150
// - 3s,32000Hz=96000
//
// TODO: キャッシングやリソース共有の単位となるが、実際に共有できるのはオンメモリに展開された PCM のみとする。
// ストリーミングデコード系 (MP3 や OGG) は不可能。
// ※できなくないが、たとえば 2つの SourceNode がひとつの decode シーケンスを使いまわすと、ブロック先頭を毎回探したり、最悪ファイル先頭から見直しとか非効率になる
//   ただそういったものの用途は主に BGM であり、頻繁に多重再生するかというとまずない。やりたければ Decoder 複数つくればいいだけ。
// なので、メモリ上に展開された短い PCM メモリ領域の共有を主な用途とする。
// ちなみに短い MP3 や OGG をオンメモリに展開して共有するのは OK.
class AudioDecoder
	: public RefObject
{
public:
	virtual ~AudioDecoder() = default;
	virtual const AudioDataInfo& audioDataInfo() const = 0;
	//virtual void seekToFrame(size_t frameNumber) = 0;
    // チャンネル数2の時に frames=1 で呼び出すと、2サンプル取り出す。ret は読んだフレーム数
	virtual uint32_t read(size_t seekFrameNumber, float* buffer, uint32_t requestFrames) = 0;
	virtual void reset() = 0;

	static void convertToFloat32(float* dst, const void* src, const size_t frameLength, PCMFormat format);
	static void convertFromFloat32(void * dst, const float * src, const size_t frameLength, PCMFormat format);

	//virtual void getLoopState(uint32_t* begin, uint32_t* length) const = 0;
private:
};

} // namespace detail
} // namespace ln

