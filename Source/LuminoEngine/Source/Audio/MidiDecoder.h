
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
	const byte_t* getSourceData() const { return m_midiFileData.getData(); }

	// 音量を正規化して読み込むかの設定 ( fillBufferAndReleaseStream() の前に呼ぶこと )
	void setEnableVolumeNormalize(bool flag) { m_volumeNormalize = flag; }

public:
	// AudioDecoder interface
	virtual void				create(Stream* stream) override;
	virtual StreamFormat		getSourceFormat() const override { return StreamFormat_Midi; }
	virtual const WaveFormat*	getWaveFormat() const override { return NULL; }
	virtual uint32_t			getSourceDataSize() const override { return m_midiFileData.getSize(); }
	virtual uint32_t			getTotalUnits() const override { return 0; }
	virtual byte_t*				getOnmemoryPCMBuffer() const override { return NULL; }
	virtual uint32_t			getOnmemoryPCMBufferSize() const override { return 0; }
	virtual uint32_t			getBytesPerSec() const override { return 0; }
	virtual void				setLoopState(uint32_t* begin, uint32_t* length) const override;
	virtual void				fillOnmemoryBuffer() override;
	virtual void				read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize) override;
	virtual void				reset() override;

private:
	// midi データの中の必要なデータをチェックする
	void searchData();

	// デルタタイムの読み込み
	uint32_t readDelta(BinaryReader& reader);

	// トラック内の CC111 検索
	bool searchTrack(BinaryReader& reader, uint32_t* cc111_time);

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
