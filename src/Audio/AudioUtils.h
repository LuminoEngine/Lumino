/*
	@file	AudioUtils.h
*/
#pragma once

#include <Lumino/IO/Stream.h>
#include <Lumino/Audio/Common.h>

namespace Lumino
{
namespace Audio
{
class AudioStream;

/// Wave データ AudioStream
class AudioUtils
{
public:

#ifdef _WIN32
	/// WaveFormat から WAVEFORMATEX への変換
	static void ConvertLNWaveFormatToWAVEFORMATEX(const WaveFormat& lnFmt, WAVEFORMATEX* wavFmt);

	/// WAVEFORMATEX から WaveFormat への変換
	static void ConvertWAVEFORMATEXToLNWaveFormat(const WAVEFORMATEX& wavFmt, WaveFormat* lnFmt);

	/// WAVEFORMATEX 構造体を標準出力
	static void PrintWAVEFORMATEX(const WAVEFORMATEX& wavFmt, const char* str = NULL);
#endif

	/// 音声データのフォーマットチェック
	///		扱える音声ファイルかどうかを調べて、その種類を返す。
	///		調べた後、ファイルポインタはファイルの先頭に戻る。
	///		MP3 かをチェックするのにファイル終端を基準に Seek することがある。
	static StreamFormat CheckFormat(Stream* stream);
	
	/// 要求している type と AudioStream から正しい type を返す
	static SoundPlayType CheckAudioPlayType(SoundPlayType type, AudioStream* audioStream, uint32_t limitSize);
};

} // namespace Audio
} // namespace Lumino
