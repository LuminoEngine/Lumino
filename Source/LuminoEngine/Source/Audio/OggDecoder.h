
#pragma once
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include "AudioStream.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

// Ogg 音声用 Decoder
class OggDecoder
	: public AudioDecoder
{
public:
	OggDecoder();
	virtual ~OggDecoder();

public:
	virtual void Create(Stream* stream) override;
	virtual StreamFormat GetSourceFormat() const override { return StreamFormat_Ogg; }
	virtual const WaveFormat* GetWaveFormat() const override { return &m_waveFormat; }
	virtual uint32_t GetSourceDataSize() const override { return m_sourceDataSize; }
	virtual uint32_t GetTotalUnits() const override { return m_totalSamples; }
	virtual byte_t* GetOnmemoryPCMBuffer() const override { return m_onmemoryPCMBuffer; }
	virtual uint32_t GetOnmemoryPCMBufferSize() const override { return m_onmemoryPCMBufferSize; }
	virtual uint32_t GetBytesPerSec() const override { return m_waveFormat.avgBytesPerSec; }
	virtual void GetLoopState(uint32_t* begin, uint32_t* length) const override { *begin = m_loopStart; *length = m_loopLength; }
	virtual void FillOnmemoryBuffer() override;
	virtual void Read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize) override;
	virtual void Reset() override {}

public:	// 以下は ogg API からのコールバックとして登録する関数

	// ogg 用 read コールバック
	static size_t readOggCallback(void* buffer, size_t element_size, size_t count, void* stream);

	// ogg 用 seek コールバック
	static int seekOggCallback(void* stream, ogg_int64_t offset, int whence);

	// ogg 用 close コールバック
	static int closeOggCallback(void* stream);

    // ogg 用 tell コールバック
	static long tellOggCallback(void* stream);

private:
	static const int WORD_SIZE = sizeof(uint16_t);	// 1サンプルあたりのビット数
	static const int WORD_BITS = WORD_SIZE * 8;

	Stream*			m_stream;					// 入力ストリーム
	WaveFormat		m_waveFormat;				// PCM 用フォーマット
	uint32_t		m_sourceDataSize;			// 音声データ部分のサイズ
	uint32_t		m_totalTime;				// 全体の再生時間 (ミリ秒)
	byte_t*			m_onmemoryPCMBuffer;		// オンメモリ再生用のデータを読み込むバッファ
	uint32_t		m_onmemoryPCMBufferSize;	// mOnmemoryPCMBuffer のサイズ (今のところ m_dataOffset と同じ)
	uint32_t		m_totalSamples;				// 全体の再生サンプル数
	Mutex			m_mutex;	// TODO: 必要無いかな？

	OggVorbis_File	m_oggVorbisFile;			// オープン済みの Ogg ファイル
	uint32_t		m_loopStart;				// ループ領域の先頭サンプル数
	uint32_t		m_loopLength;				// ループ領域の長さ (サンプル数)

};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
