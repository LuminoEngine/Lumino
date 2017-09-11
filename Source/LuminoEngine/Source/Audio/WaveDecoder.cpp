
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
void WaveDecoder::create(Stream* stream)
{
	if (LN_REQUIRE(stream != nullptr)) return;
	LN_REFOBJ_SET(m_stream, stream);

	// 念のためファイルポインタを先頭に戻す
	m_stream->seek(0, SeekOrigin_Begin);

	BinaryReader reader(m_stream);

	//-------------------------------------------------------------
	// ヘッダ読み込んで念のためフォーマットチェック

	WaveFileHeader rh;
	reader.read(&rh, sizeof(WaveFileHeader));
	if (memcmp(&rh.riff, "RIFF", 4) != 0 ||
		memcmp(&rh.waveHeader, "WAVE", 4) != 0)
	{
		LN_ENSURE_INVALID_FORMAT(0);
		return;
	}

	//-------------------------------------------------------------
	// 各チャンクチェック

	char chunk[4];
	while (reader.read(chunk, 4) == 4)
	{
		if (strncmp(chunk, "fmt ", 4) == 0)
		{
			uint32_t chunkSize = reader.readUInt32();	// チャンクサイズ

			m_waveFormat.formatTag = reader.readUInt16();
			m_waveFormat.channels = reader.readUInt16();
			m_waveFormat.samplesPerSec = reader.readUInt32();
			m_waveFormat.avgBytesPerSec = reader.readUInt32();
			m_waveFormat.blockAlign = reader.readUInt16();
			m_waveFormat.bitsPerSample = reader.readUInt16();

			// 拡張部分のあるファイルの場合は読みとばす
			if (chunkSize > 16) {
				reader.seek(chunkSize - 16);
				//mWaveFormat.EXSize = FileIO::readU16( mInFile );
				//mInFile->seek( mWaveFormat.EXSize, SEEK_CUR );
			}
			else {
				m_waveFormat.exSize = 0;
			}
		}
		else if (strncmp(chunk, "data", 4) == 0)
		{
			uint32_t chunkSize = reader.readUInt32();

			// ファイルポインタの現在位置 (data チャンク内のデータ位置) を記憶
			m_dataOffset = reader.getPosition();

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
			uint32_t chunkSize = reader.readUInt32();
			reader.seek(chunkSize);
		}
	}

	// data チャンクは見つかっているはず
	LN_ENSURE_INVALID_FORMAT(m_dataOffset != 0);
}

//------------------------------------------------------------------------------
void WaveDecoder::fillOnmemoryBuffer()
{
	MutexScopedLock lock(m_mutex);

	if (m_onmemoryPCMBuffer == NULL)
	{
		// オンメモリ再生するときに必要なバッファのサイズは元データのサイズと同じ
		m_onmemoryPCMBufferSize = m_pcmDataSize;

		// メモリ確保
		m_onmemoryPCMBuffer = LN_NEW byte_t[m_onmemoryPCMBufferSize];

		// 現在のシーク位置を覚えておく
		int64_t old_seek = m_stream->getPosition();

		// ファイルポインタをデータがある場所の先頭にセット
		m_stream->seek(m_dataOffset, SeekOrigin_Begin);

		// 全部読み込み
		int size = m_stream->read(m_onmemoryPCMBuffer, m_onmemoryPCMBufferSize);

		// 読み込んだサイズが変な場合はエラー
		if (LN_ENSURE_INVALID_FORMAT(size == m_onmemoryPCMBufferSize, "read file size is incorrect.\nThere is a possibility that the file is corrupted.")) return;

		// シーク位置を元に戻す
		m_stream->seek(old_seek, SeekOrigin_Begin);

		// もういらない
		LN_SAFE_RELEASE(m_stream);
	}
}

//------------------------------------------------------------------------------
void WaveDecoder::read(uint32_t seekPos, void* buffer, uint32_t buffer_size, uint32_t* out_read_size, uint32_t* out_write_size)
{
	if (LN_REQUIRE(m_stream != NULL)) return;	// オンメモリ再生とストリーミング再生で同じ AudioStream を共有したときにぶつかる
	MutexScopedLock lock(m_mutex);

	m_stream->seek(m_dataOffset + seekPos, SeekOrigin_Begin);

	if (!buffer || !(buffer_size > 0) || !(out_read_size) || !(out_write_size)) {
		return;
	}

	// 読み込むサイズ
	uint32_t read_size = buffer_size;
	// ソースのサイズを超えている場合はソースサイズ分読む
	if (((uint64_t)m_stream->getPosition()) + buffer_size > m_dataOffset + m_pcmDataSize)
	{
		read_size = (m_dataOffset + m_pcmDataSize) - m_stream->getPosition();
	}

	size_t size = m_stream->read(buffer, read_size);

	// 元データから読み込んだサイズと、buffer_ へ書き込んだサイズは同じ
	*out_read_size = static_cast<uint32_t>(size);
	*out_write_size = static_cast<uint32_t>(size);
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
