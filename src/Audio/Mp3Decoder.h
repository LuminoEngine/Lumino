/*
	@file	Mp3Decoder.h
*/
#pragma once

#include <Lumino/Base/ByteBuffer.h>
#include "AudioStream.h"

namespace Lumino
{
namespace Audio
{

/// MP3 データ AudioStream
///
/// この AudioStream は、ストリーミング再生で共有することはできない。
/// これは、デコードには WinAPI を使用しているが、これは元の MP3 データに対して
///	シーケンシャルにアクセスしなければならないため。
/// つまり、ストリーミングの Player クラス 2 つ以上から同時に Read (デコード処理) が呼ばれると、
/// ストリームのシーク位置が前に戻ったりする。
/// その状態でデコード API を呼び出すと、音が飛んだりする。
class Mp3Decoder
	: public AudioDecoder
{
public:
	Mp3Decoder();
	virtual ~Mp3Decoder();

public:
	virtual void Create(Stream* stream);
	virtual StreamFormat GetSourceFormat() const { return StreamFormat_Mp3; }
	virtual const WaveFormat* GetWaveFormat() const { return &m_waveFormat; }
	virtual uint32_t GetSourceDataSize() const { return m_sourceDataSize; }
	virtual uint32_t GetTotalUnits() const { return m_totalSamples; }
	virtual byte_t* GetOnmemoryPCMBuffer() const { return m_onmemoryPCMBuffer; }
	virtual uint32_t GetOnmemoryPCMBufferSize() const { return m_onmemoryPCMBufferSize; }
	virtual uint32_t GetBytesPerSec() const { return m_streamingPCMBufferSize; }
	virtual void GetLoopState(uint32_t* begin, uint32_t* length) const { *begin = 0; *length = 0; }
	virtual void FillOnmemoryBuffer();
	virtual void Read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize);
	virtual void Reset() { m_resetFlag = true; }

private:

	/// どのフォーマットの mp3 か調べて、データまでのオフセットやタグフィールドのサイズをメンバに格納
	void CheckId3v();

	/// mp3 の PCM フォーマットを調べてメンバに格納する
	void GetPCMFormat();

private:

	/// ID3v2 形式のヘッダ情報
	struct ID3v2Header
	{
		uint8_t	ID[3];
		uint8_t	Version[2];
		uint8_t	Flag;
		uint8_t	Size[4];
	};

private:
	Stream*					m_stream;					///< 入力ストリーム
	WaveFormat				m_waveFormat;				///< ライブラリ用 PCM 用フォーマット
	uint32_t				m_sourceDataSize;			///< 音声データ部分のサイズ
	uint64_t				m_dataOffset;				///< ストリーム内の PCM データの先頭までのオフセットバイト数
	uint32_t				m_totalTime;				///< 全体の再生時間 ( ミリ秒 )
	uint8_t*				m_onmemoryPCMBuffer;		///< オンメモリ再生用のデータを読み込むバッファ
	uint32_t				m_onmemoryPCMBufferSize;	///< mOnmemoryPCMBuffer のサイズ ( 今のところ mDataOffset と同じ )
	uint32_t				m_totalSamples;				///< 全体の再生サンプル数
	Threading::Mutex		m_mutex;

	MPEGLAYER3WAVEFORMAT	m_acmMP3WaveFormat;
	HACMSTREAM				m_hACMStream;
	size_t					m_id3vTagFieldSize;			///< Id3v2 形式の場合のタグフィールド(ヘッダ情報)部分のサイズ
	uint32_t				m_streamingPCMBufferSize;	///< 1 秒分の mp3 データを変換した時の、最適な転送先 PCM バッファサイズ
	ByteBuffer				m_mp3SourceBufferParSec;	///< デコード時にファイルから読む 1 秒分の mp3 データを一時的に格納するバッファ
	bool					m_resetFlag;				///< デコード状態のリセットを要求するフラグ ( read() でのデコード時のフラグ指定に使う )

};

} // namespace Audio
} // namespace Lumino
