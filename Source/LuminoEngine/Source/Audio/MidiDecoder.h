
#pragma once
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/IO/BinaryReader.h>
#include "AudioStream.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

// MIDI データ AudioStream
class MidiDecoder
	: public AudioDecoder
{
public:
	MidiDecoder();
	virtual ~MidiDecoder();
	
	// メモリ上に展開された MIDI データの取得
	const byte_t* GetSourceData() const { return m_midiFileData.GetData(); }

	// 音量を正規化して読み込むかの設定 ( fillBufferAndReleaseStream() の前に呼ぶこと )
	void SetEnableVolumeNormalize(bool flag) { m_volumeNormalize = flag; }

public:
	// AudioDecoder interface
	virtual void				Create(Stream* stream) override;
	virtual StreamFormat		GetSourceFormat() const override { return StreamFormat_Midi; }
	virtual const WaveFormat*	GetWaveFormat() const override { return NULL; }
	virtual uint32_t			GetSourceDataSize() const override { return m_midiFileData.GetSize(); }
	virtual uint32_t			GetTotalUnits() const override { return 0; }
	virtual byte_t*				GetOnmemoryPCMBuffer() const override { return NULL; }
	virtual uint32_t			GetOnmemoryPCMBufferSize() const override { return 0; }
	virtual uint32_t			GetBytesPerSec() const override { return 0; }
	virtual void				GetLoopState(uint32_t* begin, uint32_t* length) const override;
	virtual void				FillOnmemoryBuffer() override;
	virtual void				Read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize) override;
	virtual void				Reset() override;

private:
	// midi データの中の必要なデータをチェックする
	void SearchData();

	// デルタタイムの読み込み
	uint32_t ReadDelta(BinaryReader& reader);

	// トラック内の CC111 検索
	bool SearchTrack(BinaryReader& reader, uint32_t* cc111_time);

	// Midi ファイルのヘッダ
	struct MidiHeader
	{
		uint8_t			chunktype[4];		// チャンクタイプ (MThd)
		uint32_t		length;				// データ長
		uint16_t		format;				// フォーマットタイプ
		uint16_t		numtrack;			// トラック数
		uint16_t		timebase;			// タイムベース
	};

	// ボリュームチェンジの位置と値 (正規化に使う)
	struct VolumeEntry
	{
		uint32_t		position;			// ボリュームチェンジの値の位置 [ 00 B0 07 46 ] の 07
		uint32_t		volume;				// ボリューム値
	};

	Stream*				m_stream;			// 入力ストリーム
	ByteBuffer			m_midiFileData;		// MIDI ファイル全体のデータ (m_stream をすべてメモリに読み込んだバッファ)
	Mutex				m_mutex;
	uint32_t			m_cc111Time;
	uint32_t			m_baseTime;
	List<VolumeEntry>	m_volumeEntryList;
	bool				m_volumeNormalize;
	
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
