#pragma once
#include "AudioDecoder.hpp"

struct mpeglayer3waveformat_tag;
typedef mpeglayer3waveformat_tag MPEGLAYER3WAVEFORMAT;

namespace ln {
namespace detail {

// MP3 構造
// http://www.cactussoft.co.jp/Sarbo/divMPeg3UnmanageHeader.html
class Mp3AudioDecoder
	: public AudioDecoder
{
public:
	Mp3AudioDecoder();
	void init(Stream* stream, DiagnosticsManager* diag);
	virtual const AudioDataInfo& audioDataInfo() const override;
    //virtual void seekToFrame(size_t frameNumber) override;
	virtual uint32_t read(size_t seekFrameNumber, float* buffer, uint32_t requestFrames) override;
	virtual void reset() override;

private:
	bool checkId3v();
	bool getPCMFormat();

	Ref<Stream> m_stream;
	DiagnosticsManager* m_diag;
	AudioDataInfo m_info;

	uint32_t m_sourceDataSize;	// 音声データ部分のサイズ
	uint64_t m_dataOffset;		// ストリーム内の PCM データの先頭までのオフセットバイト数
	size_t m_id3vTagFieldSize;	// Id3v2 形式の場合のタグフィールド(ヘッダ情報)部分のサイズ
	MPEGLAYER3WAVEFORMAT* m_acmMP3WaveFormat;
	bool m_resetFlag;			// デコード状態のリセットを要求するフラグ ( read() でのデコード時のフラグ指定に使う )
};

} // namespace detail
} // namespace ln

