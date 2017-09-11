
#include <Lumino/Base/Exception.h>
#include <Lumino/IO/BinaryReader.h>
#include "MidiDecoder.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// MidiDecoder
//==============================================================================
	
//------------------------------------------------------------------------------
MidiDecoder::MidiDecoder()
	: m_stream(NULL)
	, m_midiFileData()
	, m_mutex()
	, m_cc111Time(0)
	, m_baseTime(0)
	, m_volumeEntryList()
	, m_volumeNormalize(true)
{
}

//------------------------------------------------------------------------------
MidiDecoder::~MidiDecoder()
{
	LN_SAFE_RELEASE(m_stream);
}

//------------------------------------------------------------------------------
void MidiDecoder::create(Stream* stream)
{
	LN_REFOBJ_SET(m_stream, stream);
	m_volumeEntryList.clear();

	searchData();
}

//------------------------------------------------------------------------------
void MidiDecoder::setLoopState(uint32_t* cc111time, uint32_t* base_time) const
{
	*cc111time = m_cc111Time;
	*base_time = m_baseTime;
}

//------------------------------------------------------------------------------
void MidiDecoder::fillOnmemoryBuffer()
{
	MutexScopedLock lock(m_mutex);

	if (m_stream != NULL)
	{
		m_midiFileData.resize((size_t)m_stream->getLength(), false);
		m_stream->seek(0, SeekOrigin_Begin);
		m_stream->read(m_midiFileData.getData(), m_midiFileData.getSize());

		if (m_volumeNormalize)
		{
			// ボリュームの最大値を探す
			uint32_t maxVolume = 0;
			for (VolumeEntry& v : m_volumeEntryList)
			{
				maxVolume = std::max(maxVolume, v.volume);
			}

			// MIDI データ内の最大ボリュームを 127 にするのに必要な値を求め、全てのボリュームに加算する
			int sub = 127 - maxVolume;
			for (VolumeEntry& v : m_volumeEntryList)
			{
				m_midiFileData[v.position] += sub;
			}
		}

		LN_SAFE_RELEASE(m_stream);
	}
}

//------------------------------------------------------------------------------
void MidiDecoder::read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize)
{
	LN_UNREACHABLE();
}

//------------------------------------------------------------------------------
void MidiDecoder::reset()
{
	LN_UNREACHABLE();
}
	
//------------------------------------------------------------------------------
void MidiDecoder::searchData()
{
	// ファイルポインタを先頭に戻しておく
	m_stream->seek(0, SeekOrigin_Begin);

	BinaryReader reader(m_stream);

	// Midi ファイルのヘッダ読み込み
	MidiHeader header;
	reader.read(&(header.chunktype), 4);
	header.length = reader.readUInt32(ByteOrder::Big);
	header.format = reader.readUInt16(ByteOrder::Big);
	header.numtrack = reader.readUInt16(ByteOrder::Big);
	header.timebase = reader.readUInt16(ByteOrder::Big);

	// ベースタイム格納
	m_baseTime = header.timebase;

	m_cc111Time = 0;
	uint32_t cc111time = 0;

	// トラックの数だけループして、cc111 とボリュームチェンジを探す
	for (int i = 0; i < header.numtrack; ++i)
	{
		searchTrack(reader, &cc111time);

		if (m_cc111Time == 0)
		{
			m_cc111Time = cc111time;
		}
	}
}
	
//------------------------------------------------------------------------------
uint32_t MidiDecoder::readDelta(BinaryReader& reader)
{
	uint8_t t;
	uint32_t dtime = 0;
	for (int i = 0; i < sizeof(uint32_t); ++i)
	{
		reader.read(&t, sizeof(uint8_t));
		dtime = (dtime << 7) | (t & 0x7f);

		// MSBが立っていないならば、次のバイトはデルタタイムではないので抜ける
		if (!(t & 0x80)) break;
	}
	return dtime;
}
	
//------------------------------------------------------------------------------
bool MidiDecoder::searchTrack(BinaryReader& reader, uint32_t* cc111_time)
{
	// トラックチャンクのチェック
	uint8_t chunk[4];
	size_t read_size = reader.read(chunk, 4);
	if (LN_ENSURE_INVALID_FORMAT(read_size == 4)) return false;
	if (LN_ENSURE_INVALID_FORMAT(memcmp(chunk, "MTrk", 4) == 0)) return false;

	// トラックの長さ読み込み
	uint32_t   track_length;
	track_length = reader.readUInt32(ByteOrder::Big);

	uint8_t prev_state = 0; // ひとつ前のイベントのステータスバイトを記憶する変数
	uint8_t state;
	uint8_t data1;
	uint8_t data2;
	uint32_t track_time = 0;

	while (1)
	{
		// デルタタイムを読み込む
		track_time += readDelta(reader);

		// ステータスバイトを読み込む
		read_size = reader.read(&state, sizeof(uint8_t));
		if (LN_ENSURE_INVALID_FORMAT(read_size == sizeof(uint8_t))) return false;

		// ランニングステータスの場合
		if (!(state & 0x80))
		{
			// 一つ前のイベントのステータスバイトを代入
			state = prev_state;
			// ファイルポインタを一つ戻す
			reader.seek(-1);
		}

		// ステータスバイトを基にどのイベントか判別
		switch (state & 0xf0)
		{
			// データバイトが 2 バイトのイベント
		case 0x80:	// ノートオフ
		case 0x90:	// ノートオン
		case 0xA0:	// ポリフォニック・キー・プレッシャ
		case 0xE0:	// ピッチベンド
			reader.seek(2);
			break;

		case 0xB0:	// コントロールチェンジ
			read_size = reader.read(&data1, sizeof(uint8_t));
			if (LN_ENSURE_INVALID_FORMAT(read_size == sizeof(uint8_t))) return false;

			read_size = reader.read(&data2, sizeof(uint8_t));
			if (LN_ENSURE_INVALID_FORMAT(read_size == sizeof(uint8_t))) return false;

			// cc111
			if (data1 == 0x6F)
			{
				*cc111_time = track_time;
			}
			// ボリュームチェンジ
			else if (data1 == 0x07)
			{
				// データの位置とボリュームをキューに入れて保存
				VolumeEntry entry;
				entry.position = (uint32_t)reader.getPosition() - 1;
				entry.volume = data2;
				m_volumeEntryList.add(entry);
			}
			break;

			// データバイトが 1 バイトのイベント
		case 0xC0:	// プログラムチェンジ
		case 0xD0:	// チャンネルプレッシャ
			reader.seek(1);
			break;

			// データバイトが可変長のイベント
		case 0xF0:
			// SysExイベント
			if (state == 0xF0)
			{
				int data_length = 0;
				// データ長読み込み
				read_size = reader.read(&data_length, sizeof(uint8_t));
				if (LN_ENSURE_INVALID_FORMAT(read_size == sizeof(uint8_t))) return false;

				reader.seek(data_length);
			}
			// メタイベント
			else if (state == 0xFF)
			{
				uint8_t type;

				// typeの取得
				read_size = reader.read(&type, sizeof(uint8_t));
				if (LN_ENSURE_INVALID_FORMAT(read_size == sizeof(uint8_t))) return false;

				uint32_t data_length = -1;

				// type 別にデータバイトの長さを取得
				switch (type)
				{
				case 0x00:
					data_length = 2; break;
				case 0x01:
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
				case 0x06:
				case 0x07:
					break;
				case 0x20:
					data_length = 1; break;
				case 0x21:
					data_length = 1; break;
				case 0x2F:
					data_length = 0; break; // エンドオブトラック
				case 0x51:
					data_length = 3; break; // セットテンポ
				case 0x54:
					data_length = 5; break;
				case 0x58:
					data_length = 4; break;
				case 0x59:
					data_length = 2; break;
				case 0x7F:
					break;
				default:
					LN_UNREACHABLE();
					return false;
				}

				uint32_t temp = data_length;

				// データ長が固定の場合
				if (data_length != -1)
				{
					data_length = readDelta(reader);
					if (data_length != temp)
					{
						LN_ENSURE_INVALID_FORMAT(0);
						return false;
					}
				}
				else
				{
					// 任意のデータ長を取得
					data_length = readDelta(reader);
				}

				reader.seek(data_length);

				// トラックの終端が見つかった場合は終了
				if (type == 0x2F)
				{
					return true;
				}
			}

			break;

		default:
			// "invalid status byte."
			LN_UNREACHABLE();
			return false;

		}
		// 次のイベントが前のイベントのステータスバイトを確認できるように保存する
		prev_state = state;
	}
	return true;
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
