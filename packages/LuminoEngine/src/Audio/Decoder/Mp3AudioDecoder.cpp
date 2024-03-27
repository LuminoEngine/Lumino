
#include "Internal.hpp"

#ifdef LN_OS_WIN32
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "Mp3AudioDecoder.hpp"

#pragma comment(lib, "msacm32.lib")

namespace ln {
namespace detail {

Mp3AudioDecoder::Mp3AudioDecoder()
	: m_acmMP3WaveFormat(new MPEGLAYER3WAVEFORMAT())
{
}

void Mp3AudioDecoder::init(Stream* stream, DiagnosticsManager* diag)
{
	if (LN_REQUIRE(stream)) return;
	m_stream = stream;

	// ID3v 確認
	if (!checkId3v()) {
		return;
	}

	// PCM フォーマット取得
	if (!getPCMFormat()) {
		return;
	}
}

const AudioDataInfo& Mp3AudioDecoder::audioDataInfo() const
{
	return m_info;
}

//void Mp3AudioDecoder::seekToFrame(size_t frameNumber)
//{
//    LN_NOTIMPLEMENTED();
//}

uint32_t Mp3AudioDecoder::read(size_t seekFrameNumber, float* buffer, uint32_t requestFrames)
{
	//m_stream->seek(m_dataOffset + seekPos, SeekOrigin::Begin);

	//// ファイルからデータ読み込み
	//size_t read_size = m_stream->read(m_mp3SourceBufferParSec.getData(), m_mp3SourceBufferParSec.getSize());

	//DWORD src_length = m_mp3SourceBufferParSec.getSize();

	//// 実際に読み込んだサイズが、読むべきサイズよりも小さかった場合
	//if (read_size < m_mp3SourceBufferParSec.getSize())
	//{
	//	// とりあえず、読み込めたサイズ分コンバートする
	//	src_length = read_size;
	//}

	//// ACM ヘッダに変換バッファ設定
	//ACMSTREAMHEADER ash;
	//ZeroMemory(&ash, sizeof(ACMSTREAMHEADER));
	//ash.cbStruct = sizeof(ACMSTREAMHEADER);
	//ash.pbSrc = m_mp3SourceBufferParSec.getData();
	//ash.cbSrcLength = src_length;
	//ash.pbDst = (LPBYTE)buffer;
	//ash.cbDstLength = buffer_size;

	//// コンバート実行
	//MMRESULT mmr = acmStreamPrepareHeader(m_hACMStream, &ash, 0);
	//if (LN_ENSURE(mmr == 0, _T("MMRESULT:%_TT("), mmr)) return;

	//DWORD acm_flag = (m_resetFlag == true) ? ACM_STREAMCONVERTF_START : ACM_STREAMCONVERTF_BLOCKALIGN;
	//mmr = acmStreamConvert(m_hACMStream, &ash, acm_flag);
	//if (LN_ENSURE(mmr == 0, _T("MMRESULT:%_TT("), mmr)) return;

	//mmr = acmStreamUnprepareHeader(m_hACMStream, &ash, 0);
	//if (LN_ENSURE(mmr == 0, _T("MMRESULT:%_TT("), mmr)) return;

	//// コンバートした結果、実際に使った領域を返す
	//*out_read_size = ash.cbSrcLengthUsed;
	//*out_write_size = ash.cbDstLengthUsed;

	//m_resetFlag = false;

	return 0;
}

void Mp3AudioDecoder::reset()
{
}

bool Mp3AudioDecoder::checkId3v()
{
	// ID3v2 形式のヘッダ情報
	struct ID3v2Header
	{
		uint8_t	ID[3];
		uint8_t	Version[2];
		uint8_t	Flag;
		uint8_t	Size[4];
	};

	// とりあえず最初に、ファイルサイズを mp3 データ全体のサイズとする
	m_sourceDataSize = (uint32_t)m_stream->length();

	// とりあえず ID3v2 としてヘッダ部分を読み込む
	ID3v2Header header;
	int read_size = m_stream->read(&header, sizeof(ID3v2Header));
	if (read_size != sizeof(ID3v2Header)) {
		m_diag->reportError(_TT("mp3 file size is invalid."));
		return false;
	}

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
		if (m_sourceDataSize < 128) {
			m_diag->reportError(_TT("not found mp3 tag. (file ending)"));
			return false;
		}

		// タグなし　データがある場所はファイルの先頭から
		m_id3vTagFieldSize = 0;
		m_dataOffset = 0;

		// ファイル終端から 128 バイト戻ったところを調べる
		byte_t data[3];
		m_stream->seek(-128, SeekOrigin::End);
		read_size = m_stream->read(data, 3);
		if (read_size != 3) {
			m_diag->reportError(_TT("not found mp3 tag."));
			return false;
		}

		// 'TAG' が見つかった
		if (data[0] == 'T' && data[1] == 'A' && data[2] == 'G')
		{
			// mp3 データ部分のサイズは、全体からタグの分を引いたもの
			m_sourceDataSize -= 128;
		}
	}

	// 念のため、ファイルポインタを先頭に戻しておく
	m_stream->seek(0, SeekOrigin::Begin);

	return true;
}

bool Mp3AudioDecoder::getPCMFormat()
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
	m_stream->seek(m_id3vTagFieldSize, SeekOrigin::Begin);
	m_stream->read(data, 4);

	// data がフレームヘッダを指しているか調べる
	if (data[0] != 0xff || data[1] >> 5 != 0x07)
	{
		// 先頭になければガンガン進めながら探す
		int rs;
		int64_t ends = m_stream->length();
		while (true)
		{
			rs = m_stream->read(data, 4);
			if (rs != 4) {
				m_diag->reportError(_TT("not found mp3 frame header."));
				return false;
			}

			if (data[0] == 0xff && data[1] >> 5 == 0x07)
			{
				break;
			}
			m_stream->seek(-3, SeekOrigin::Current);
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
		m_diag->reportError(_TT("invalid mp3 version."));
		return false;
	}

	// レイヤー 3 ？
	if ((data[1] >> 1 & 0x03) != 1)
	{
		m_diag->reportError(_TT("invalid mp3 layer."));
		return false;
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

	// mp3 v1 layerIII
	// https://qiita.com/tanakah/items/3828df5329a408082462
	// TODO: other http://www.cactussoft.co.jp/Sarbo/divMPeg3UnmanageStruct.html
	wBlockSize = (WORD)((1152 * dwBitRate * 1000 / dwSampleRate) / 8) + padding;

	// MPEGLAYER3WAVEFORMAT 構造体にいろいろ格納する
	MPEGLAYER3WAVEFORMAT* format = m_acmMP3WaveFormat;

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

	return true;
}

} // namespace detail
} // namespace ln

#endif
