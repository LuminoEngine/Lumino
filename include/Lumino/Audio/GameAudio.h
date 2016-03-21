
#pragma once
#include <list>
#include "Common.h"

LN_NAMESPACE_BEGIN
class Vector3;

LN_NAMESPACE_AUDIO_BEGIN

/*
	@brief	
*/
class GameAudio final
{
public:

	/// BGM を演奏する
	static void PlayBGM(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// 指定された Sound を BGM として演奏する
	static void PlayBGMFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// BGM の演奏を停止する
	static void StopBGM(double fadeTime = 0.0);

	/// BGS ( 環境音 ) を演奏する
	static void PlayBGS(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// 指定された Sound を BGS として演奏する
	static void PlayBGSFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// BGS の演奏を停止する
	static void StopBGS(double fadeTime = 0.0);

	// ME ( 効果音楽 ) を演奏する
	static void PlayME(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f);

	/// 指定された Sound を ME として演奏する
	static void PlayMEFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

	/// ME の演奏を停止する
	static void StopME();

	/// SE を演奏する
	static void PlaySE(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f);

	/// SE を 3D 空間上で演奏する
	static void PlaySE3D(const TCHAR* filePath, const Vector3& position, float distance, float volume = 1.0f, float pitch = 1.0f);

	/// 指定された Sound を SE として演奏する
	static void PlaySEFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

	/// 全ての SE の演奏を停止する
	static void StopSE();

	/// ME 演奏時の BGM のフェード時間を設定する
	static void SetMEFadeState(double begin, double end);

	/// 再生中のBGMの音量を設定する (フェードアウト中は無効)
	static void SetBGMVolume(float volume, double fadeTime = 0.0);

	/// 再生中のBGSの音量を設定する (フェードアウト中は無効)
	static void SetBGSVolume(float volume, double fadeTime = 0.0);

	/// 同名 BGM の演奏再開フラグの設定
	static void SetEnableBGMRestart(bool flag);

	/// 同名 BGS の演奏再開フラグの設定
	static void SetEnableBGSRestart(bool flag);

};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
