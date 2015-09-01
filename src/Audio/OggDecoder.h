/*
	@file	OggDecoder.h
*/
#pragma once

#include "../../external/libogg/include/ogg/ogg.h"
#include "../../external/libvorbis/include/vorbis/vorbisfile.h"
#include "AudioStream.h"

namespace Lumino
{
namespace Audio
{

/// Ogg 音声用 Decoder
class OggDecoder
	: public AudioDecoder
{
public:
	OggDecoder();
	virtual ~OggDecoder();

public:
	virtual void Create(Stream* stream);
	virtual StreamFormat GetSourceFormat() const { return StreamFormat_Ogg; }
	virtual const WaveFormat* GetWaveFormat() const { return &mWaveFormat; }
	virtual uint32_t GetSourceDataSize() const { return mSourceDataSize; }
	virtual uint32_t GetTotalUnits() const { return mTotalSamples; }
	virtual byte_t* GetOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }
	virtual uint32_t GetOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }
	virtual uint32_t GetBytesPerSec() const { return mWaveFormat.AvgBytesPerSec; }
	virtual void GetLoopState(uint32_t* begin, uint32_t* length) const { *begin = mLoopStart; *length = mLoopLength; }
	virtual void FillOnmemoryBuffer();
	virtual void Read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize);
	virtual void Reset() {}


public:	// 以下は ogg API からのコールバックとして登録する関数

	/// ogg 用 read コールバック
	static size_t readOggCallback(void* buffer, size_t element_size, size_t count, void* stream);

	/// ogg 用 seek コールバック
	static int seekOggCallback(void* stream, ogg_int64_t offset, int whence);

	/// ogg 用 close コールバック
	static int closeOggCallback(void* stream);

	/// ogg 用 tell コールバック
	static long tellOggCallback(void* stream);

private:
	static const int WORD_SIZE = sizeof(uint16_t);	/// ※1サンプルあたりのビット数
	static const int WORD_BITS = WORD_SIZE * 8;

	Stream*			m_stream;				///< 入力ストリーム
	WaveFormat		mWaveFormat;			///< PCM 用フォーマット
	uint32_t		mSourceDataSize;		///< 音声データ部分のサイズ
	//uint64_t		mDataOffset;			///< ストリーム内の PCM データの先頭までのオフセットバイト数
	uint32_t		mPCMDataSize;			///< ストリーム内の PCM データのサイズ
	uint32_t		mTotalTime;				///< 全体の再生時間 ( ミリ秒 )
	byte_t*			mOnmemoryPCMBuffer;		///< オンメモリ再生用のデータを読み込むバッファ
	uint32_t		mOnmemoryPCMBufferSize;	///< mOnmemoryPCMBuffer のサイズ ( 今のところ mDataOffset と同じ )
	uint32_t		mTotalSamples;          ///< 全体の再生サンプル数
	Threading::Mutex	m_mutex;

	OggVorbis_File		mOggVorbisFile;			///< オープン済みの Ogg ファイル
	uint32_t				mLoopStart;             ///< ループ領域の先頭サンプル数
	uint32_t				mLoopLength;            ///< ループ領域の長さ ( サンプル数 )

};

} // namespace Audio
} // namespace Lumino
