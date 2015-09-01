/*
	@file	WaveDecoder.h
*/
#pragma once

#include "AudioStream.h"

namespace Lumino
{
namespace Audio
{

/// Wave データ AudioStream
class WaveDecoder
	: public AudioDecoder
{
public:
	WaveDecoder();
	virtual ~WaveDecoder();

public:
	virtual void Create(Stream* stream);
	virtual StreamFormat GetSourceFormat() const { return StreamFormat_Wave; }
	virtual const WaveFormat* GetWaveFormat() const { return &mWaveFormat; }
	virtual uint32_t GetSourceDataSize() const { return mSourceDataSize; }
	virtual uint32_t GetTotalUnits() const { return mTotalSamples; }
	virtual byte_t* GetOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }
	virtual uint32_t GetOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }
	virtual uint32_t GetBytesPerSec() const { return mWaveFormat.AvgBytesPerSec; }
	virtual void GetLoopState(uint32_t* begin, uint32_t* length) const { *begin = 0; *length = 0; }
	virtual void FillOnmemoryBuffer();
	virtual void Read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize);
	virtual void Reset() {}

private:
	Stream*			mInStream;				///< 入力ストリーム
	WaveFormat		mWaveFormat;			///< PCM 用フォーマット
	uint32_t		mSourceDataSize;		///< 音声データ部分のサイズ
	uint64_t		mDataOffset;			///< ストリーム内の PCM データの先頭までのオフセットバイト数
	uint32_t		mPCMDataSize;			///< ストリーム内の PCM データのサイズ
	uint32_t		mTotalTime;				///< 全体の再生時間 ( ミリ秒 )
	uint8_t*		mOnmemoryPCMBuffer;		///< オンメモリ再生用のデータを読み込むバッファ
	uint32_t		mOnmemoryPCMBufferSize;	///< mOnmemoryPCMBuffer のサイズ ( 今のところ mDataOffset と同じ )
	uint32_t		mTotalSamples;          ///< 全体の再生サンプル数
	Threading::Mutex	m_mutex;

};

} // namespace Audio
} // namespace Lumino
