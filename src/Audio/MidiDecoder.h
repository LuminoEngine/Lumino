
#pragma once
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/IO/BinaryReader.h>
#include "AudioStream.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

/// MIDI データ AudioStream
class MidiDecoder
	: public AudioDecoder
{
public:
	MidiDecoder();
	virtual ~MidiDecoder();

public:
	/// メモリ上に展開された MIDI データの取得
	const byte_t* GetSourceData() const { return m_midiFileData.GetData(); }

	/// 音量を正規化して読み込むかの設定 ( fillBufferAndReleaseStream() の前に呼ぶこと )
	void SetEnableVolumeNormalize(bool flag) { m_volumeNormalize = flag; }

public:
	virtual void				Create(Stream* stream);
	virtual StreamFormat		GetSourceFormat() const { return StreamFormat_Midi; }
	virtual const WaveFormat*	GetWaveFormat() const { return NULL; }
	virtual uint32_t			GetSourceDataSize() const { return m_midiFileData.GetSize(); }
	virtual uint32_t			GetTotalUnits() const { printf("Midi::getTotalUnits() Undefined."); return 0; }
	virtual byte_t*				GetOnmemoryPCMBuffer() const { return NULL; }
	virtual uint32_t			GetOnmemoryPCMBufferSize() const { return 0; }
	virtual uint32_t			GetBytesPerSec() const { return 0; }
	virtual void				GetLoopState(uint32_t* begin, uint32_t* length) const;
	virtual void				FillOnmemoryBuffer();
	virtual void				Read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize);
	virtual void				Reset();

private:

	/// midi データの中の必要なデータをチェックする
	void SearchData();

	/// デルタタイムの読み込み
	uint32_t ReadDelta(BinaryReader& reader);

	/// トラック内の CC111 検索
	bool SearchTrack(BinaryReader& reader, uint32_t* cc111_time);

private:

	/// Midi ファイルのヘッダ
	struct MidiHeader
	{
		uint8_t		mChunktype[4];	///< チャンクタイプ (MThd)
		uint32_t	mLength;		///< データ長
		uint16_t	mFormat;		///< フォーマットタイプ
		uint16_t	mNumtrack;		///< トラック数
		uint16_t	mTimebase;		///< タイムベース
	};

	/// ボリュームチェンジの位置と値 (正規化に使う)
	struct VolumeEntry
	{
		uint32_t	mPosition;		///< ボリュームチェンジの値の位置 [ 00 B0 07 46 ] の 07
		uint32_t	mVolume;		///< ボリューム値
	};

private:
	Stream*					m_stream;		///< 入力ストリーム
	ByteBuffer				m_midiFileData;	///< MIDI ファイル全体のデータ (m_stream をすべてメモリに読み込んだバッファ)
	Mutex					m_mutex;
	uint32_t				m_cc111Time;
	uint32_t				m_baseTime;
	Array<VolumeEntry>		m_volumeEntryList;
	bool					m_volumeNormalize;

};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
