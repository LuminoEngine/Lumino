
#pragma once
#include "AudioStream.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

// Wave データ AudioStream
class WaveDecoder
	: public AudioDecoder
{
public:
	WaveDecoder();
	virtual ~WaveDecoder();

public:
	virtual void Create(Stream* stream) override;
	virtual StreamFormat GetSourceFormat() const override { return StreamFormat_Wave; }
	virtual const WaveFormat* GetWaveFormat() const override { return &m_waveFormat; }
	virtual uint32_t GetSourceDataSize() const override { return m_sourceDataSize; }
	virtual uint32_t GetTotalUnits() const override { return m_totalSamples; }
	virtual byte_t* GetOnmemoryPCMBuffer() const override { return m_onmemoryPCMBuffer; }
	virtual uint32_t GetOnmemoryPCMBufferSize() const override { return m_onmemoryPCMBufferSize; }
	virtual uint32_t GetBytesPerSec() const override { return m_waveFormat.avgBytesPerSec; }
	virtual void GetLoopState(uint32_t* begin, uint32_t* length) const override { *begin = 0; *length = 0; }
	virtual void FillOnmemoryBuffer() override;
	virtual void Read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize) override;
	virtual void Reset() override {}

private:
	Stream*			m_stream;					// 入力ストリーム
	WaveFormat		m_waveFormat;				// PCM 用フォーマット
	uint32_t		m_sourceDataSize;			// 音声データ部分のサイズ
	uint64_t		m_dataOffset;				// ストリーム内の PCM データの先頭までのオフセットバイト数
	uint32_t		m_pcmDataSize;				// ストリーム内の PCM データのサイズ
	uint32_t		m_totalTime;				// 全体の再生時間 (ミリ秒)
	uint8_t*		m_onmemoryPCMBuffer;		// オンメモリ再生用のデータを読み込むバッファ
	uint32_t		m_onmemoryPCMBufferSize;	// mOnmemoryPCMBuffer のサイズ (今のところ m_dataOffset と同じ)
	uint32_t		m_totalSamples;				// 全体の再生サンプル数
	Mutex			m_mutex;	// TODO: 必要あるかな？

};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
