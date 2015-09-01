
#include <Lumino/Base/Exception.h>
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/IO/BinaryReader.h>
#include "Internal.h"
#include "AudioUtils.h"
#include "Mp3Decoder.h"

namespace Lumino
{
namespace Audio
{

//=============================================================================
// Mp3Decoder
//=============================================================================
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Mp3Decoder::Mp3Decoder()
	: m_stream(NULL)
	, m_sourceDataSize(0)
	, m_dataOffset(0)
	, m_totalTime(0)
	, m_onmemoryPCMBuffer(NULL)
	, m_onmemoryPCMBufferSize(0)
	, m_totalSamples(0)
	, m_mutex()
	, m_hACMStream(NULL)
	, m_id3vTagFieldSize(0)
	, m_streamingPCMBufferSize(0)
	, m_mp3SourceBufferParSec()
	, m_resetFlag(true)
{
	memset(&m_waveFormat, 0, sizeof(m_waveFormat));
	memset(&m_acmMP3WaveFormat, 0, sizeof(m_acmMP3WaveFormat));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Mp3Decoder::~Mp3Decoder()
{
	if (m_hACMStream)
	{
		acmStreamClose(m_hACMStream, 0);
		m_hACMStream = NULL;
	}

	LN_SAFE_DELETE_ARRAY(m_onmemoryPCMBuffer);
	LN_SAFE_RELEASE(m_stream);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Mp3Decoder::Create(Stream* stream)
{
	LN_THROW(stream != NULL, ArgumentException);
	m_stream = stream;
	m_stream->AddRef();

	// ファイルポインタを先頭に戻す
	m_stream->Seek(0, SeekOrigin_Begin);

	m_dataOffset = 0;

	// ID3v 確認
	CheckId3v();
		
	// PCM フォーマット取得
	GetPCMFormat();

	// mp3 を PCM にデコードした時の wave フォーマットを取得
	MPEGLAYER3WAVEFORMAT* mp3_format = &m_acmMP3WaveFormat;
	WAVEFORMATEX wav_fmt_ex;
	wav_fmt_ex.wFormatTag = WAVE_FORMAT_PCM;
	MMRESULT mmr = acmFormatSuggest(NULL, &mp3_format->wfx, &wav_fmt_ex, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);
	LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);

	// ACM 変換ストリームを開く ( mp3 → wave )
	//HACMSTREAM* acm = mACMStreamHandle;//(HACMSTREAM*)&mACMStreamHandle;
	mmr = acmStreamOpen(&m_hACMStream, NULL, &mp3_format->wfx, &wav_fmt_ex, NULL, 0, 0, 0);
	LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);

	// WAVEFORMATEX → Audio::WaveFormat
	AudioUtils::ConvertWAVEFORMATEXToLNWaveFormat(wav_fmt_ex, &m_waveFormat);

	// 全体を変換した時の PCM サイズを m_onmemoryPCMBufferSize に格納
    DWORD pcm_size = 0;
	mmr = acmStreamSize(m_hACMStream, m_sourceDataSize, &pcm_size, ACM_STREAMSIZEF_SOURCE);
	LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);
    m_onmemoryPCMBufferSize = pcm_size;

	// 1 秒分の mp3 データを変換した時の、最適な転送先 PCM バッファサイズを取得する
	DWORD wave_buffer_size;
	mmr = acmStreamSize(m_hACMStream, mp3_format->wfx.nAvgBytesPerSec, &wave_buffer_size, ACM_STREAMSIZEF_SOURCE);
	LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);
	m_streamingPCMBufferSize = wave_buffer_size;

	//HACMSTREAM acm2;
	//MMRESULT m = acmStreamOpen( &acm2, NULL, &mp3_format->wfx, &mWaveFormat, NULL, 0, 0, 0 );
	//LASSERT_STRONG_RETURN( (m!=0), false, "acmStreamOpen"  );
	//acmStreamSize( m_hACMStream, mMP3BufferSize, &mMP3BufferSize, ACM_STREAMSIZEF_SOURCE );
	//acmStreamClose( acm2, 0 );

	//// ストリーミング再生用のバッファのサイズを取得 ( mp3_format->wfx.nAvgBytesPerSec は仮のサイズ )
	//mMP3SourceBufferSizeParSec = ;

	//
	//mMP3SourceBufferParSec = LN_NEW byte_t[ mMP3SourceBufferSizeParSec ];
	// ストリーミング再生用の 1 秒分の mp3 データのサイズ分、メモリを確保
	m_mp3SourceBufferParSec.Resize(mp3_format->wfx.nAvgBytesPerSec);

	// 全体の再生時間を計算する
	double t = static_cast< double >(m_onmemoryPCMBufferSize) / (static_cast< double >(m_waveFormat.AvgBytesPerSec) * 0.001);
	m_totalTime = static_cast< uint32_t >(t);

    // 全体の再生サンプル数を求める
	uint32_t one_channel_bits = (m_onmemoryPCMBufferSize / m_waveFormat.Channels) * 8;	// 1チャンネルあたりの総ビット数
	m_totalSamples = one_channel_bits / m_waveFormat.BitsPerSample;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Mp3Decoder::FillOnmemoryBuffer()
{
	Threading::MutexScopedLock lock(m_mutex);

	if (m_onmemoryPCMBuffer == NULL)
	{
		MMRESULT mmr;

		// mp3 データ全体を格納するバッファを作成して読み込む
		//lnU8* mp3_buffer = (lnU8*)malloc(m_sourceDataSize);
		//LN_THROW_SystemCall(mp3_buffer);
		ByteBuffer mp3_buffer(m_sourceDataSize);

		m_stream->Seek(m_dataOffset, SeekOrigin_Begin);
		size_t read_size = m_stream->Read(mp3_buffer.GetData(), m_sourceDataSize);
		LN_THROW(read_size == m_sourceDataSize, InvalidFormatException);

		// 全体を変換した時の PCM サイズを mPCMSize に格納
		DWORD pcm_size = 0;
		mmr = acmStreamSize(m_hACMStream, m_sourceDataSize, &pcm_size, ACM_STREAMSIZEF_SOURCE);
		LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);
		m_onmemoryPCMBufferSize = pcm_size;

		// 取得したサイズでバッファ確保
		m_onmemoryPCMBuffer = LN_NEW byte_t[m_onmemoryPCMBufferSize];

		// ACM ヘッダに変換バッファ設定
		ACMSTREAMHEADER ash;
		ZeroMemory(&ash, sizeof(ACMSTREAMHEADER));
		ash.cbStruct = sizeof(ACMSTREAMHEADER);
		ash.pbSrc = mp3_buffer.GetData();
		ash.cbSrcLength = m_sourceDataSize;
		ash.pbDst = (LPBYTE)m_onmemoryPCMBuffer;
		ash.cbDstLength = m_onmemoryPCMBufferSize;

		// コンバート実行
		mmr = acmStreamPrepareHeader(m_hACMStream, &ash, 0);
		LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);
		mmr = acmStreamConvert(m_hACMStream, &ash, 0);
		LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);
		mmr = acmStreamUnprepareHeader(m_hACMStream, &ash, 0);
		LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);

		// 実際に PCM バッファに書き込んだデータサイズを記憶する
		m_onmemoryPCMBufferSize = ash.cbDstLengthUsed;

		LN_SAFE_RELEASE(m_stream);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Mp3Decoder::Read(uint32_t seekPos, void* buffer, uint32_t buffer_size, uint32_t* out_read_size, uint32_t* out_write_size)
{
	LN_THROW(m_stream != NULL, InvalidOperationException);	// オンメモリ再生とストリーミング再生で同じ AudioStream を共有したときにぶつかる
	Threading::MutexScopedLock lock(m_mutex);

	//if (m_onmemoryPCMBuffer)
	//{
	//	AudioSourceBase::read(buffer, buffer_size, out_read_size, out_write_size);
	//}
	//else
	//{

		m_stream->Seek(m_dataOffset + seekPos, SeekOrigin_Begin);

		ZeroMemory(buffer, buffer_size);

		// ファイルからデータ読み込み
		size_t read_size = m_stream->Read(m_mp3SourceBufferParSec.GetData(), m_mp3SourceBufferParSec.GetSize());

		DWORD src_length = m_mp3SourceBufferParSec.GetSize();

		// 実際に読み込んだサイズが、読むべきサイズよりも小さかった場合
		if (read_size < m_mp3SourceBufferParSec.GetSize())
		{
			// とりあえず、読み込めたサイズ分コンバートする
			src_length = read_size;
		}

		// ACM ヘッダに変換バッファ設定
		ACMSTREAMHEADER ash;
		ZeroMemory(&ash, sizeof(ACMSTREAMHEADER));
		ash.cbStruct = sizeof(ACMSTREAMHEADER);
		ash.pbSrc = m_mp3SourceBufferParSec.GetData();
		ash.cbSrcLength = src_length;
		ash.pbDst = (LPBYTE)buffer;
		ash.cbDstLength = buffer_size;

		// コンバート実行
		MMRESULT mmr = acmStreamPrepareHeader(m_hACMStream, &ash, 0);
		LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);

		DWORD acm_flag = (m_resetFlag == true) ? ACM_STREAMCONVERTF_START : ACM_STREAMCONVERTF_BLOCKALIGN;
		mmr = acmStreamConvert(m_hACMStream, &ash, acm_flag);
		LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);

		mmr = acmStreamUnprepareHeader(m_hACMStream, &ash, 0);
		LN_THROW(mmr == 0, InvalidOperationException, _T("MMRESULT:%u"), mmr);

		// コンバートした結果、実際に使った領域を返す
		*out_read_size = ash.cbSrcLengthUsed;
		*out_write_size = ash.cbDstLengthUsed;

		m_resetFlag = false;
	//}
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void Mp3Decoder::CheckId3v()
{
	// とりあえず最初に、ファイルサイズを mp3 データ全体のサイズとする
	m_sourceDataSize = (uint32_t)m_stream->GetLength();

	// とりあえず ID3v2 としてヘッダ部分を読み込む
	ID3v2Header header;
	int read_size = m_stream->Read(&header, sizeof(ID3v2Header));
	LN_THROW(read_size == sizeof(ID3v2Header), InvalidFormatException, "mp3 file size is invalid.");

	// Id3v2 形式の場合
	if (header.ID[0] == 'I' && header.ID[1] == 'D' && header.ID[2] == '3')
	{
		// タグサイズ取得
		m_id3vTagFieldSize = ((header.Size[0] << 21) | (header.Size[1] << 14) | (header.Size[2] << 7) | (header.Size[3])) + 10;

		// 音声データがある位置は、タグの場所の次からとする
		m_dataOffset = m_id3vTagFieldSize;

		// 音声データ本体のサイズは、ファイル全体のサイズからタグの分を引いたもの
		m_sourceDataSize -= m_id3vTagFieldSize;
	}
	// Id3v2 形式以外 ( Id3v1 ) の場合
	else
	{
		// 終端のタグ情報がない
		LN_THROW(m_stream->GetLength() >= 128, InvalidFormatException, "not found mp3 tag.");

		// タグなし　データがある場所はファイルの先頭から
		m_id3vTagFieldSize = 0;
		m_dataOffset = 0;

		// ファイル終端から 128 バイト戻ったところを調べる
		byte_t data[3];
		m_stream->Seek(-128, SeekOrigin_End);
		read_size = m_stream->Read(data, 3);
		LN_THROW(read_size == 3, InvalidFormatException, "not found mp3 tag.");

		//printf( "%c %c %c %c\n", data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ] );

		// 'TAG' が見つかった
		if (data[0] == 'T' && data[1] == 'A' && data[2] == 'G')
		{
			// mp3 データ部分のサイズは、全体からタグの分を引いたもの
			m_sourceDataSize -= 128;
		}
	}

	// 念のため、ファイルポインタを先頭に戻しておく
	m_stream->Seek(0, SeekOrigin_Begin);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void Mp3Decoder::GetPCMFormat()
{
	BYTE  index;
	BYTE  version;
	BYTE  channel;
	BYTE  padding;
	WORD  wBlockSize;
	DWORD dwBitRate;
	DWORD dwSampleRate;
	DWORD dwBitTableLayer3[][16] = {
		{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0 },
		{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 }
	};
	DWORD dwSampleTable[][3] = {
		{ 44100, 48000, 32000 },
		{ 22050, 24000, 16000 }
	};

	// フレームヘッダ部分を読み込む
	byte_t data[4];
	m_stream->Seek(m_id3vTagFieldSize, SeekOrigin_Begin);
	m_stream->Read(data, 4);

	// data がフレームヘッダを指しているか調べる
	if (data[0] != 0xff || data[1] >> 5 != 0x07)
	{
		// 先頭になければガンガン進めながら探す
		int rs;
		int ends = m_stream->GetLength();
		while (true)
		{
			rs = m_stream->Read(data, 4);
			LN_THROW(rs == 4, InvalidFormatException, "not found mp3 frame header.");

			if (data[0] == 0xff && data[1] >> 5 == 0x07)
			{
				break;
			}
			m_stream->Seek(-3, SeekOrigin_Current);
		}
	}

	// MP3 のバージョンは？
	switch (data[1] >> 3 & 0x03)
	{
	case 3:
		version = 1;
		break;
	case 2:
		version = 2;
		break;
	default:
		LN_THROW(0, InvalidFormatException, "not found mp3 frame header.");
		break;
	}

	// レイヤー 3 ？
	if ((data[1] >> 1 & 0x03) != 1)
	{
		LN_THROW(0, InvalidFormatException, "( ( data[1] >> 1 & 0x03 ) != 1 )");
	}

	// テーブルで定義したビットレートのうち、当てはまるものを選ぶ
	index = data[2] >> 4;
	dwBitRate = dwBitTableLayer3[version - 1][index];


	// 同じように、サンプリングレートを選ぶ
	index = data[2] >> 2 & 0x03;
	dwSampleRate = dwSampleTable[version - 1][index];

	// パディングの取得
	padding = data[2] >> 1 & 0x01;

	// チャンネル数の取得
	channel = ((data[3] >> 6) == 3) ? 1 : 2;

	wBlockSize = (WORD)((1152 * dwBitRate * 1000 / dwSampleRate) / 8) + padding;

	// MPEGLAYER3WAVEFORMAT 構造体にいろいろ格納する
	MPEGLAYER3WAVEFORMAT* format = &m_acmMP3WaveFormat;

	format->wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
	format->wfx.nChannels = channel;
	format->wfx.nSamplesPerSec = dwSampleRate;
	format->wfx.nAvgBytesPerSec = (dwBitRate * 1000) / 8;
	format->wfx.nBlockAlign = 1;
	format->wfx.wBitsPerSample = 0;
	format->wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;

	format->wID = MPEGLAYER3_ID_MPEG;
	format->fdwFlags = padding ? MPEGLAYER3_FLAG_PADDING_ON : MPEGLAYER3_FLAG_PADDING_OFF;
	format->nBlockSize = wBlockSize;
	format->nFramesPerBlock = 1;
	format->nCodecDelay = 0x571;
}

} // namespace Audio
} // namespace Lumino
