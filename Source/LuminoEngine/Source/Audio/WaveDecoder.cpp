
#include <Lumino/Base/Exception.h>
#include <Lumino/IO/BinaryReader.h>
#include "WaveDecoder.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

// wave ファイルのヘッダ
struct WaveFileHeader
{
	uint32_t	riff;
	uint32_t	size;
	uint32_t	waveHeader;
};

//==============================================================================
// WaveDecoder
//==============================================================================
	
//------------------------------------------------------------------------------
WaveDecoder::WaveDecoder()
	: m_stream(nullptr)
	, m_waveFormat()
	, m_sourceDataSize(0)
	, m_dataOffset(0)
	, m_pcmDataSize(0)
	, m_totalTime(0)
	, m_onmemoryPCMBuffer(nullptr)
	, m_onmemoryPCMBufferSize(0)
	, m_totalSamples(0)
	, m_mutex()
{
}

//------------------------------------------------------------------------------
WaveDecoder::~WaveDecoder()
{
	LN_SAFE_DELETE_ARRAY(m_onmemoryPCMBuffer);
	LN_SAFE_RELEASE(m_stream);
}

//------------------------------------------------------------------------------
void WaveDecoder::Create(Stream* stream)
{
	if (LN_CHECK_ARG(stream != nullptr)) return;
	LN_REFOBJ_SET(m_stream, stream);

	// 念のためファイルポインタを先頭に戻す
	m_stream->Seek(0, SeekOrigin_Begin);

	BinaryReader reader(m_stream);

	//-------------------------------------------------------------
	// ヘッダ読み込んで念のためフォーマットチェック

	WaveFileHeader rh;
	reader.Read(&rh, sizeof(WaveFileHeader));
	if (memcmp(&rh.riff, "RIFF", 4) != 0 ||
		memcmp(&rh.waveHeader, "WAVE", 4) != 0)
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

			m_waveFormat.formatTag = reader.ReadUInt16();
			m_waveFormat.channels = reader.ReadUInt16();
			m_waveFormat.samplesPerSec = reader.ReadUInt32();
			m_waveFormat.avgBytesPerSec = reader.ReadUInt32();
			m_waveFormat.blockAlign = reader.ReadUInt16();
			m_waveFormat.bitsPerSample = reader.ReadUInt16();

			// 拡張部分のあるファイルの場合は読みとばす
			if (chunkSize > 16) {
				reader.Seek(chunkSize - 16);
				//mWaveFormat.EXSize = FileIO::readU16( mInFile );
				//mInFile->seek( mWaveFormat.EXSize, SEEK_CUR );
			}
			else {
				m_waveFormat.exSize = 0;
			}
		}
		else if (strncmp(chunk, "data", 4) == 0)
		{
			uint32_t chunkSize = reader.ReadUInt32();

			// ファイルポインタの現在位置 (data チャンク内のデータ位置) を記憶
			m_dataOffset = reader.GetPosition();

			// 元データのサイズは data チャンク内のデータのサイズ
			m_onmemoryPCMBufferSize = m_pcmDataSize = chunkSize;
			m_sourceDataSize = m_onmemoryPCMBufferSize;

			// 全体の再生時間を計算する
			double t = static_cast< double >(m_pcmDataSize) / (static_cast< double >(m_waveFormat.avgBytesPerSec) * 0.001);
			m_totalTime = static_cast< uint32_t >(t);

			// 全体の再生サンプル数
			uint32_t one_channel_bits = (m_onmemoryPCMBufferSize / m_waveFormat.channels) * 8;	// 1チャンネルあたりの総ビット数
			m_totalSamples = one_channel_bits / m_waveFormat.bitsPerSample;

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
	LN_THROW(m_dataOffset != 0, InvalidFormatException);
}

//------------------------------------------------------------------------------
void WaveDecoder::FillOnmemoryBuffer()
{
	MutexScopedLock lock(m_mutex);

	if (m_onmemoryPCMBuffer == NULL)
	{
		// オンメモリ再生するときに必要なバッファのサイズは元データのサイズと同じ
		m_onmemoryPCMBufferSize = m_pcmDataSize;

		// メモリ確保
		m_onmemoryPCMBuffer = LN_NEW byte_t[m_onmemoryPCMBufferSize];

		// 現在のシーク位置を覚えておく
		int64_t old_seek = m_stream->GetPosition();

		// ファイルポインタをデータがある場所の先頭にセット
		m_stream->Seek(m_dataOffset, SeekOrigin_Begin);

		// 全部読み込み
		int size = m_stream->Read(m_onmemoryPCMBuffer, m_onmemoryPCMBufferSize);

		// 読み込んだサイズが変な場合はエラー
		LN_THROW(size == m_onmemoryPCMBufferSize, InvalidOperationException, "read file size is incorrect.\nThere is a possibility that the file is corrupted.");

		// シーク位置を元に戻す
		m_stream->Seek(old_seek, SeekOrigin_Begin);

		// もういらない
		LN_SAFE_RELEASE(m_stream);
	}
}

//------------------------------------------------------------------------------
void WaveDecoder::Read(uint32_t seekPos, void* buffer, uint32_t buffer_size, uint32_t* out_read_size, uint32_t* out_write_size)
{
	LN_THROW(m_stream != NULL, InvalidOperationException);	// オンメモリ再生とストリーミング再生で同じ AudioStream を共有したときにぶつかる
	MutexScopedLock lock(m_mutex);

	m_stream->Seek(m_dataOffset + seekPos, SeekOrigin_Begin);

	if (!buffer || !(buffer_size > 0) || !(out_read_size) || !(out_write_size)) {
		return;
	}

	// 読み込むサイズ
	uint32_t read_size = buffer_size;
	// ソースのサイズを超えている場合はソースサイズ分読む
	if (((uint64_t)m_stream->GetPosition()) + buffer_size > m_dataOffset + m_pcmDataSize)
	{
		read_size = (m_dataOffset + m_pcmDataSize) - m_stream->GetPosition();
	}

	size_t size = m_stream->Read(buffer, read_size);

	// 元データから読み込んだサイズと、buffer_ へ書き込んだサイズは同じ
	*out_read_size = static_cast<uint32_t>(size);
	*out_write_size = static_cast<uint32_t>(size);
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
