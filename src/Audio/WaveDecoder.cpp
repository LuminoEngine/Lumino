
#include <Lumino/Base/Exception.h>
#include <Lumino/IO/BinaryReader.h>
#include "WaveDecoder.h"

namespace Lumino
{
namespace Audio
{

/// wave ファイルのヘッダ
struct WaveFileHeader
{
	uint32_t	RIFF;
	uint32_t	Size;
	uint32_t	WaveHeader;
};

//=============================================================================
// WaveDecoder
//=============================================================================
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WaveDecoder::WaveDecoder()
	: mInStream(NULL)
	, mSourceDataSize(0)
	, mDataOffset(0)
	, mPCMDataSize(0)
	, mTotalTime(0)
	, mOnmemoryPCMBuffer(NULL)
	, mOnmemoryPCMBufferSize(0)
	, mTotalSamples(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WaveDecoder::~WaveDecoder()
{
	LN_SAFE_DELETE_ARRAY(mOnmemoryPCMBuffer);
	LN_SAFE_RELEASE(mInStream);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WaveDecoder::Create(Stream* stream)
{
	LN_THROW(stream != NULL, ArgumentException);
	mInStream = stream;
	mInStream->AddRef();

	// 念のためファイルポインタを先頭に戻す
	mInStream->Seek(0, SeekOrigin_Begin);

	BinaryReader reader(mInStream);

	//-------------------------------------------------------------
	// ヘッダ読み込んで念のためフォーマットチェック

	WaveFileHeader rh;
	reader.Read(&rh, sizeof(WaveFileHeader));
	if (memcmp(&rh.RIFF, "RIFF", 4) != 0 ||
		memcmp(&rh.WaveHeader, "WAVE", 4) != 0)
	{
		LN_THROW(0, InvalidFormatException);
	}

	//-------------------------------------------------------------
	// 各チャンクチェック

	char chunk[4];
	while (reader.Read(chunk, 4) == 4)
	{
		if (strncmp(chunk, "fmt ", 4) == 0)
		{
			uint32_t chunkSize = reader.ReadUInt32();	// チャンクサイズ

			mWaveFormat.FormatTag = reader.ReadUInt16();
			mWaveFormat.Channels = reader.ReadUInt16();
			mWaveFormat.SamplesPerSec = reader.ReadUInt32();
			mWaveFormat.AvgBytesPerSec = reader.ReadUInt32();
			mWaveFormat.BlockAlign = reader.ReadUInt16();
			mWaveFormat.BitsPerSample = reader.ReadUInt16();

			// 拡張部分のあるファイルの場合は読みとばす
			if (chunkSize > 16) {
				reader.Seek(chunkSize - 16);
				//mWaveFormat.EXSize = FileIO::readU16( mInFile );
				//mInFile->seek( mWaveFormat.EXSize, SEEK_CUR );
			}
			else {
				mWaveFormat.EXSize = 0;
			}
		}
		else if (strncmp(chunk, "data", 4) == 0)
		{
			uint32_t chunkSize = reader.ReadUInt32();

			// ファイルポインタの現在位置 (data チャンク内のデータ位置) を記憶
			mDataOffset = reader.GetPosition();

			// 元データのサイズは data チャンク内のデータのサイズ
			mOnmemoryPCMBufferSize = mPCMDataSize = chunkSize;
			mSourceDataSize = mOnmemoryPCMBufferSize;

			// 全体の再生時間を計算する
			double t = static_cast< double >(mPCMDataSize) / (static_cast< double >(mWaveFormat.AvgBytesPerSec) * 0.001);
			mTotalTime = static_cast< uint32_t >(t);

			// 全体の再生サンプル数
			uint32_t one_channel_bits = (mOnmemoryPCMBufferSize / mWaveFormat.Channels) * 8;	// 1チャンネルあたりの総ビット数
			mTotalSamples = one_channel_bits / mWaveFormat.BitsPerSample;

			break;
		}
		// "fmt " と "data" 以外はすべて読み飛ばす
		else
		{
			uint32_t chunkSize = reader.ReadUInt32();
			reader.Seek(chunkSize);
		}
	}

	// data チャンクは見つかっているはず
	LN_THROW(mDataOffset != 0, InvalidFormatException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WaveDecoder::FillOnmemoryBuffer()
{
	Threading::MutexScopedLock lock(m_mutex);

	if (mOnmemoryPCMBuffer == NULL)
	{
		// オンメモリ再生するときに必要なバッファのサイズは元データのサイズと同じ
		mOnmemoryPCMBufferSize = mPCMDataSize;

		// メモリ確保
		mOnmemoryPCMBuffer = LN_NEW byte_t[mOnmemoryPCMBufferSize];

		// 現在のシーク位置を覚えておく
		int64_t old_seek = mInStream->GetPosition();

		// ファイルポインタをデータがある場所の先頭にセット
		mInStream->Seek(mDataOffset, SeekOrigin_Begin);

		// 全部読み込み
		int size = mInStream->Read(mOnmemoryPCMBuffer, mOnmemoryPCMBufferSize);

		// 読み込んだサイズが変な場合はエラー
		LN_THROW(size == mOnmemoryPCMBufferSize, InvalidOperationException, "read file size is incorrect.\nThere is a possibility that the file is corrupted.");

		// シーク位置を元に戻す
		mInStream->Seek(old_seek, SeekOrigin_Begin);

		// もういらない
		LN_SAFE_RELEASE(mInStream);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WaveDecoder::Read(uint32_t seekPos, void* buffer, uint32_t buffer_size, uint32_t* out_read_size, uint32_t* out_write_size)
{
	LN_THROW(mInStream != NULL, InvalidOperationException);	// オンメモリ再生とストリーミング再生で同じ AudioStream を共有したときにぶつかる
	Threading::MutexScopedLock lock(m_mutex);

	mInStream->Seek(mDataOffset + seekPos, SeekOrigin_Begin);

	if (!buffer || !(buffer_size > 0) || !(out_read_size) || !(out_write_size)) {
		return;
	}

	// 読み込むサイズ
	uint32_t read_size = buffer_size;
	// ソースのサイズを超えている場合はソースサイズ分読む
	if (mInStream->GetPosition() + buffer_size > mDataOffset + mPCMDataSize)
	{
		read_size = (mDataOffset + mPCMDataSize) - mInStream->GetPosition();
	}

	size_t size = mInStream->Read(buffer, read_size);

	// 元データから読み込んだサイズと、buffer_ へ書き込んだサイズは同じ
	*out_read_size = static_cast<uint32_t>(size);
	*out_write_size = static_cast<uint32_t>(size);
}


} // namespace Audio
} // namespace Lumino
