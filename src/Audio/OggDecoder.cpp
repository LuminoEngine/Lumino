
#include <Lumino/Base/Exception.h>
#include <Lumino/IO/BinaryReader.h>
#include "OggDecoder.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// OggDecoder
//==============================================================================
	
//------------------------------------------------------------------------------
OggDecoder::OggDecoder()
	: m_stream(nullptr)
	, m_waveFormat()
	, m_sourceDataSize(0)
	, m_totalTime(0)
	, m_onmemoryPCMBuffer(nullptr)
	, m_onmemoryPCMBufferSize(0)
	, m_totalSamples(0)
	, m_mutex()
	, m_oggVorbisFile()
	, m_loopStart(0)
	, m_loopLength(0)
{
}

//------------------------------------------------------------------------------
OggDecoder::~OggDecoder()
{
	LN_SAFE_DELETE_ARRAY(m_onmemoryPCMBuffer);
	ov_clear(&m_oggVorbisFile);
	LN_SAFE_RELEASE(m_stream);
}

//------------------------------------------------------------------------------
void OggDecoder::Create(Stream* stream)
{
	if (LN_CHECKEQ_ARG(stream == nullptr)) return;
	
	LN_REFOBJ_SET(m_stream, stream);
	m_stream->Seek(0, SeekOrigin_Begin);

	// コールバック関数
	ov_callbacks callbacks = {
		readOggCallback,
		seekOggCallback,
		closeOggCallback,
		tellOggCallback };

	// コールバックを使って Ogg ファイルオープン
	int err = ov_open_callbacks(m_stream, &m_oggVorbisFile, 0, 0, callbacks);

	// エラーが発生した場合
	if (err != 0)
	{
		const char* es = "";
		switch (err)
		{
		case OV_EREAD:		es = "OV_EREAD"; break;
		case OV_ENOTVORBIS:	es = "OV_ENOTVORBIS"; break;
		case OV_EVERSION:	es = "OV_EVERSION"; break;
		case OV_EBADHEADER:	es = "OV_EBADHEADER"; break;
		case OV_EFAULT:		es = "OV_EFAULT"; break;
		}
		LN_THROW(0, InvalidFormatException, "ov_open_callbacks %s\n", es);
	}

	// Ogg ファイルの情報取得
	vorbis_info* ogg_info = ov_info(&m_oggVorbisFile, -1);

	int bits_per_sample = WORD_BITS;	// とりあえず 16bit 固定
	m_waveFormat.formatTag = 1;	// PCM
	m_waveFormat.channels = ogg_info->channels;
	m_waveFormat.samplesPerSec = ogg_info->rate;
	m_waveFormat.avgBytesPerSec = ogg_info->rate * bits_per_sample / 8 * ogg_info->channels;
	m_waveFormat.blockAlign = bits_per_sample / 8 * ogg_info->channels;
	m_waveFormat.bitsPerSample = bits_per_sample;
	m_waveFormat.exSize = 0;

	ogg_int64_t total = ov_pcm_total(&m_oggVorbisFile, -1);
	LN_THROW((total != OV_EINVAL), InvalidFormatException, "ov_pcm_total %d\n", total);

	// オンメモリに展開する時に必要な PCM サイズ
	m_onmemoryPCMBufferSize = static_cast<uint32_t>(total)* WORD_SIZE * m_waveFormat.channels;		// 2 は 16bit なので

	m_sourceDataSize = m_onmemoryPCMBufferSize;

	// 念のため先頭にシーク
	ov_time_seek(&m_oggVorbisFile, 0.0);

	// 再生時間
	double t = static_cast<double>(m_onmemoryPCMBufferSize) / (static_cast<double>(m_waveFormat.avgBytesPerSec) * 0.001);
	m_totalTime = static_cast<uint32_t>(t);

	// 全体の再生サンプル数を求める
	uint32_t one_channel_bits = (m_onmemoryPCMBufferSize / m_waveFormat.channels) * 8;	// 1チャンネルあたりの総ビット数
	m_totalSamples = one_channel_bits / m_waveFormat.bitsPerSample;

	// ファイルに埋め込まれている "LOOPSTART" "LOOPLENGTH" コメントを探す
	vorbis_comment* ogg_comment = ov_comment(&m_oggVorbisFile, -1);
	char* c;
	char buf[20];
	for (int i = 0; i < ogg_comment->comments; ++i)
	{
		if (ogg_comment->comment_lengths[i] >= 9)
		{
			c = ogg_comment->user_comments[i];

			if (memcmp(c, "LOOPSTART", 9) == 0)
			{
				memset(buf, 0, sizeof(buf));
				memcpy(buf, (c + 10), (ogg_comment->comment_lengths[i] - 10));
				m_loopStart = static_cast<uint32_t>(atoi(buf));
			}
			else if (memcmp(c, "LOOPLENGTH", 10) == 0)
			{
				memset(buf, 0, sizeof(buf));
				memcpy(buf, (c + 11), (ogg_comment->comment_lengths[i] - 11));
				m_loopLength = static_cast<uint32_t>(atoi(buf));
			}
		}
	}
}

//------------------------------------------------------------------------------
void OggDecoder::FillOnmemoryBuffer()
{
	MutexScopedLock lock(m_mutex);

	if (m_onmemoryPCMBuffer == NULL && m_onmemoryPCMBufferSize > 0)
	{
		m_onmemoryPCMBuffer = LN_NEW byte_t[m_onmemoryPCMBufferSize];

		long read_size;
		long point = 0;
		uint32_t total = 0;
		char* temp_buffer[4096];
		int bitstream;

		// ファイルの先頭にシーク
		ov_time_seek(&m_oggVorbisFile, 0.0);

		// 全部読み込む
		while (1)
		{
			// 不定長～4096 バイトずつ読んでいく
			read_size = ov_read(&m_oggVorbisFile, (char*)temp_buffer, 4096, 0, WORD_SIZE, 1, &bitstream);

			// バッファにコピー
			memcpy(&m_onmemoryPCMBuffer[point], temp_buffer, read_size);

			total += read_size;

			// 最後まで読んだ場合は終了
			if (read_size == EOF || read_size == 0) break;

			point += read_size;
		}

		LN_SAFE_RELEASE(m_stream);
	}
}

//------------------------------------------------------------------------------
void OggDecoder::Read(uint32_t seekPos, void* buffer, uint32_t buffer_size, uint32_t* out_read_size, uint32_t* out_write_size)
{
    ov_pcm_seek(&m_oggVorbisFile, (seekPos / (WORD_SIZE * m_waveFormat.channels)));

    // 0 クリア
    memset(buffer, 0, buffer_size);
    *out_read_size = 0;

    long size = 0;
    int request_size = 4096;
    int bitstream = 0;
    uint32_t com_size = 0;
    char* byte_buffer = (char*)buffer;

    while (1)
    {
		size = ov_read(&m_oggVorbisFile, (char*)(byte_buffer + com_size), request_size, 0, WORD_SIZE, 1, &bitstream);
            
        // ファイルエンド
        if (size == 0)
        {
            break;
        }

        com_size += size;
            
        // バッファを全部埋めた場合
        if (com_size >= buffer_size)
        {
            break;
        }

        // バッファの残りが 4096 未満の場合
        if (buffer_size - com_size < 4096)
        {
            // 次に読むバイト数は、バッファの残り領域分だけ
            request_size = buffer_size - com_size;
        }
    }

    *out_read_size = com_size;
    *out_write_size = com_size;
}
	
//------------------------------------------------------------------------------
size_t OggDecoder::readOggCallback(void* buffer, size_t element_size, size_t count, void* stream)
{
	Stream* file = (Stream*)stream;
	size_t read_size = file->Read(buffer, element_size * count);
	return read_size;
}
	
//------------------------------------------------------------------------------
int OggDecoder::seekOggCallback(void* stream, ogg_int64_t offset, int whence)
{
	if (stream == NULL) return -1; // 異常の時は 0 以外の値を返す
	Stream* file = (Stream*)stream;
	file->Seek(static_cast< int >(offset), (SeekOrigin)whence);
	return 0;
}
	
//------------------------------------------------------------------------------
int OggDecoder::closeOggCallback(void* stream)
{
	return 0;	// InFile のデストラクタで閉じるので、ここでは何もしない。
}
	
//------------------------------------------------------------------------------
long OggDecoder::tellOggCallback(void* stream)
{
	Stream* file = (Stream*)stream;
	return (int)file->GetPosition();
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
