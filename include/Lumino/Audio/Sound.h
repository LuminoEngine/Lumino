/*
	@file	Sound.h
*/
#pragma once

#include <Lumino/Math/Vector3.h>
#include "../Animation/EasingValue.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
class AudioManagerImpl;
class AudioStream;
class AudioPlayer;

/**
	@brief	音声の再生、制御を行います。
*/
class Sound
    : public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief	Sound クラスのインスタンスを作成します。
	*/
	static Sound* Create(const TCHAR* filePath, AudioManagerImpl* manager = NULL);

	/**
		@brief	Sound クラスのインスタンスを作成します。
	*/
	static Sound* Create(Stream* stream, SoundLoadingMode loadingMode);

public:
	
	/**
		@brief		この音声の音量を設定します。
		@param[in]	volume	: 音量 (0.0～1.0。初期値は 1.0)
	*/
	void SetVolume(float volume);

	/**
		@brief		この音声の音量を取得します。
	*/
	float GetVolume() const;

	/**
		@brief		この音声のピッチ (音高) を設定します。
		@param[in]	volume	: ピッチ (0.5～2.0。初期値は 1.0)
	*/
	void SetPitch(float pitch);

	/**
		@brief		この音声のピッチ (音高) を取得します。
	*/
	float GetPitch() const;

	/**
		@brief		ループ再生の有無を設定します。
		@param[in]	enabled		: ループ再生するか
	*/
	void SetLoopEnabled(bool enabled);

	/**
		@brief		ループ再生が有効かを確認します。
	*/
	bool IsLoopEnabled() const;

	/**
		@brief		ループ範囲を設定します。
		@param[in]	begin		: ループ範囲の開始サンプル
		@param[in]	length		: ループ範囲のサンプル数
		@details	MIDI の場合、ループ範囲はミュージックタイム単位 (四分音符ひとつ分を 768 で表す) で指定します。 
	*/
	void SetLoopRange(uint32_t begin, uint32_t length);

	/**
		@brief		この音声の再生を開始します。
	*/
	void Play();

	/**
		@brief		この音声の再生を停止します。
	*/
	void Stop();

	/**
		@brief		この音声の再生を一時停止します。
	*/
	void Pause();

	/**
		@brief		一時停止中の再生を再開します。
	*/
	void Resume();

	/**
		@brief		サウンドを 3D 音源として再生するかを設定します。(規定値:false)
		@details	設定は Play() の前に行う必要があります。
	*/
	void Set3DEnabled(bool enabled);

	/**
		@brief		この音声が 3D 音声であるかを確認します。
	*/
	bool Is3DEnabled() const;

	/**
		@brief		3D 音声としての位置を設定します。
	*/
	void SetEmitterPosition(const Vector3& position);

	/**
		@brief		3D 音声としての位置を取得します。
	*/
	const Vector3& GetEmitterPosition() const;
	
	/**
		@brief		3D 音声としての速度を設定します。
	*/
	void SetEmitterVelocity(const Vector3& velocity);

	/**
		@brief		3D 音声としての位置を取得します。
	*/
	const Vector3& GetEmitterVelocity() const;

	/**
		@brief		3D 音声としての減衰距離を設定します。(default:100)
	*/
	void SetEmitterMaxDistance(float distance);

	/**
		@brief		音声データの全サンプル数を取得します。
		@details	MIDI の場合はミュージックタイム単位 (四分音符ひとつ分が 768) で、
					1度でも Play() で再生を開始していないと取得できません。
	*/
	int64_t GetTotalSamples() const;

	/**
		@brief		現在の再生サンプル数を取得します。
	*/
	int64_t GetPlayedSamples() const;

	/**
		@brief		サンプリングレートを取得します。
		@details	MIDI の場合は 768 を返します。
	*/
	int GetSamplingRate() const;

	/**
		@brief		この音声の現在の再生状態を取得します。
	*/
	SoundPlayingState GetPlayingState() const;

	/**
		@brief		音声データの再生方法を設定します。(規定値:Unknown)
		@details	設定は Play() の前に行う必要があります。
	*/
	void SetPlayingMode(SoundPlayingMode mode);
	
	/**
		@brief		音声データの再生方法を取得します。
	*/
	SoundPlayingMode GetPlayingMode() const;

	/**
		@brief		音量のフェードを開始します。
		@param[in]	targetVolume	: フェード先音量
		@param[in]	time			: 変化にかける時間 (秒)
		@param[in]	state			: 音量フェード完了時の動作
	*/
	void FadeVolume(float targetVolume, double time, SoundFadeBehavior behavior);

	/**
		@brief		音量のフェード中であるかを確認します。
	*/
	bool IsVolumeFading() const;

public:
	Sound(AudioManagerImpl* manager, AudioStream* stream);
	virtual ~Sound();
	void CreateAudioPlayerSync();
	void Polling(float elapsedTime);

private:
	AudioManagerImpl*			m_manager;
	Threading::Mutex			m_mutex;
	AudioStream*				m_audioStream;
    AudioPlayer*				m_audioPlayer;
	SoundPlayingMode			m_playingMode;

	float						m_volume;
	float						m_pitch;
	bool						m_loopEnabled;
	uint32_t					m_loopBegin;
	uint32_t					m_loopLength;
	bool						m_is3DSound;
	Vector3						m_position;
	Vector3						m_velocity;
	float						m_maxDistance;
	SoundPlayingState			m_playState;		// AudioPlayer の遅延作成に対応する

	uint32_t					m_gameAudioFlags;

	EasingValue<float, double>	m_fadeValue;
	SoundFadeBehavior			m_fadeBehavior;
	bool						m_fading;

	friend class AudioHelper;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
