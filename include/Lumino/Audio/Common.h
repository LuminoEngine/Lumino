/*
	@file	Common.h
*/
#pragma once

#include <Lumino/Math/Vector3.h>
#include <Lumino/Base/Common.h>

#define LN_MUSIC_TIME_BASE	768

namespace Lumino
{
namespace Audio
{

/// 音声ファイルフォーマット
enum StreamFormat
{
	StreamFormat_Unknown = 0,		///< 不明なファイル
	StreamFormat_Wave,				///< WAVE
	StreamFormat_Mp3,				///< MP3
	StreamFormat_Ogg,				///< OGG
	StreamFormat_Midi,				///< MIDI

	StreamFormat_Max,				///< (terminator)
};

/// PCM フォーマット
struct WaveFormat
{
	uint16_t	FormatTag;
	uint16_t	Channels;
	uint32_t	SamplesPerSec;
	uint32_t	AvgBytesPerSec;
	uint16_t	BlockAlign;
	uint16_t	BitsPerSample;
	uint16_t	EXSize;
};

/// DirectMusic の初期化方法
enum DirectMusicInitMode
{
	DirectMusicInitMode_NotUse = 0,			///< DirectMusic を使用しない
	DirectMusicInitMode_Normal,				///< 通常
	DirectMusicInitMode_ThreadWait,			///< 別スレッドで初期化して、再生時に未完了の場合は待つ
	DirectMusicInitMode_ThreadRequest,		///< 別スレッドで初期化して、再生時に未完了の場合は再生を予約する

	DirectMusicInitMode_MAX,
};

/// 再生方法 (Player の種類)
enum SoundPlayType
{
	SoundPlayType_Unknown = 0,		///< 不明な再生方法 (自動選択)
	//SoundPlayType_Auto,				///< 自動選択 ( デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります )
	SoundPlayType_OnMemory,			///< オンメモリ
	SoundPlayType_Streaming,	    ///< ストリーミング
	SoundPlayType_Midi,  			///< SMF

	SoundPlayType_Max,
};

/// 音声の再生状態
enum SoundPlayState
{
	SoundPlayState_Stopped = 0,		///< 停止中
	SoundPlayState_Playing,			///< 再生中
	SoundPlayState_Pausing,			///< 一時停止中

	SoundPlayState_Max,
};

/// 3D サウンドリスナー
struct SoundListener
{
	Vector3		Position;
	Vector3		Direction;
	Vector3		UpDirection;
	Vector3		Velocity;

	SoundListener()
		: Position(0, 0, 0)
		, Direction(0, 0, 1.0f)
		, UpDirection(0, 1.0f, 0)
		, Velocity(0, 0, 0)
	{}
};

} // namespace Audio
} // namespace Lumino
