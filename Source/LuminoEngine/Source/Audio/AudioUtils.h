
#pragma once
#include <Lumino/IO/Stream.h>
#include <Lumino/Audio/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{
class AudioStream;

// Wave データ AudioStream
class AudioUtils
{
public:

#if defined(_WIN32)
	// WaveFormat から WAVEFORMATEX への変換
	static void convertLNWaveFormatToWAVEFORMATEX(const WaveFormat& lnFmt, WAVEFORMATEX* wavFmt);

	// WAVEFORMATEX から WaveFormat への変換
	static void convertWAVEFORMATEXToLNWaveFormat(const WAVEFORMATEX& wavFmt, WaveFormat* lnFmt);

	// WAVEFORMATEX 構造体を標準出力
	static void printWAVEFORMATEX(const WAVEFORMATEX& wavFmt, const char* str = NULL);
#endif

	// 音声データのフォーマットチェック
	//		扱える音声ファイルかどうかを調べて、その種類を返す。
	//		調べた後、ファイルポインタはファイルの先頭に戻る。
	//		MP3 かをチェックするのにファイル終端を基準に Seek することがある。
	static StreamFormat checkFormat(Stream* stream);
	
	// 要求している type と AudioStream から正しい type を返す
	static SoundPlayingMode checkAudioPlayType(SoundPlayingMode type, AudioStream* audioStream, uint32_t limitSize);
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
