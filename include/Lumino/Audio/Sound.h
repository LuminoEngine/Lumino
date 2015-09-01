/*
	@file	AudioManager.h
*/
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Math/Vector3.h>
#include "Common.h"

namespace Lumino
{
namespace Audio
{
class AudioManager;
class AudioStream;
class AudioPlayer;

/**
	@brief	音声の再生、制御を行います。
*/
class Sound
    : public RefObject
{
public:

	/**
		@brief		
	*/
	static Sound* Create(const TCHAR* filePath, SoundPlayType playerType = SoundPlayType_Unknown, bool enable3D = false, AudioManager* manager = NULL);

public:
	
	/**
		@brief		この音声の音量を設定します。
		@param[in]	volume	: 音量 (0～100。初期値は 100)
	*/
	void SetVolume(int volume);

	/**
		@brief		この音声の音量を取得します。
	*/
	int GetVolume() const;

	/**
		@brief		この音声のピッチ (音高) を設定します。
		@param[in]	volume	: ピッチ (50～200。初期値は 100)
	*/
	void SetPitch(int pitch);

	/**
		@brief		この音声のピッチ (音高) を取得します。
	*/
	int GetPitch() const;

	/**
		@brief		ループ再生の有無を設定します。
		@param[in]	enabled		: ループ再生するか
	*/
	void SetLoopEnabled(bool enabled);

	/**
		@brief		ループ範囲を設定します。
		@param[in]	begin		: ループ範囲の開始サンプル
		@param[in]	length		: ループ範囲のサンプル数
		@details	MIDI の場合、ループ範囲はミュージックタイム単位 (四分音符ひとつ分を 768 で表す) で指定します。 
	*/
	void SetLoopRange(uint32_t begin, uint32_t length);

	/**
		@brief		ループ再生が有効かを確認します。
	*/
    bool IsLoop() const;

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
		@brief		この音声が 3D 音声であるかを確認します。
	*/
	bool Is3DSound() const;

	/**
		@brief		3D 音声としての位置を設定します。
	*/
	void SetPosition(const Vector3& position);

	/**
		@brief		3D 音声としての位置を取得します。
	*/
	const Vector3& GetPosition() const;
	
	/**
		@brief		3D 音声としての速度を設定します。
	*/
	void SetVelocity(const Vector3& velocity);

	/**
		@brief		3D 音声としての位置を取得します。
	*/
	const Vector3& GetVelocity() const;

	/**
		@brief		3D 音声としての減衰距離を設定します。
	*/
    void SetMaxDistance(float distance);

	/**
		@brief		音声データの全サンプル数を取得します。
		@details	MIDI の場合はミュージックタイム単位 (四分音符ひとつ分が 768) で、
					1度でも Play() で再生を開始していないと取得できません。
	*/
	void GetTotalSamples() const;

	/**
		@brief		現在の再生サンプル数を取得します。
	*/
	void GetCurrentSamples() const;

	/**
		@brief		サンプリング周波数を取得します。
		@details	MIDI の場合は 768 を返します。
	*/
	void GetSamplesPerSec() const;

	/**
		@brief		この音声の現在の再生状態を取得します。
	*/
	SoundPlayState GetSoundPlayState() const;


	///// 音量のフェード
	//void fadeVolume( int targetVolume, int time, SoundFadeState state );

	///// 音量フェード状態の取得
	//SoundFadeState getFadeState() const { return mFadeState; }


	///// グループの設定 (ビットフラグ)
	//void setGroupFlag( lnU32 flag );

	///// グループの取得
 //   lnU32 getGroupFlag();

	///// 内部に持っている音声再生クラスの取得
 //   AudioPlayerBase* getAudioPlayer() { return mAudioPlayer; }

	///// フェード状態の更新
 //   bool update( float elapsedTime );

public:
	Sound(AudioManager* manager, AudioStream* stream, SoundPlayType playerType, bool is3DSound);
	virtual ~Sound();
	void Polling(float elapsedTime);

private:
	AudioManager*				m_manager;
	AudioStream*				m_audioStream;
    AudioPlayer*				m_audioPlayer;
	SoundPlayType				m_playerType;

	int							m_volume;
	int							m_pitch;
	bool						m_loopEnabled;
	uint32_t					m_loopBegin;
	uint32_t					m_loopLength;
	bool						m_is3DSound;
	Vector3						m_position;
	Vector3						m_velocity;
	float						m_maxDistance;
	SoundPlayState				m_playState;

    //int 						mOrginalVolume;	        ///< setVolume() で設定される、元の音量
    //SoundFadeState				mFadeState;
    //Animation::FadeValue<int>	mFadeValue;
    //
    //lnU32						mFlags;                 ///< LNAudioPlayerFlag の値
    //lnU32						mGroupFlags;
    //bool						mIsFading;		        ///< フェードイン、またはフェードアウト中フラグ (TODO: 削除予定)
    //bool						mOnlyRefFromManager;    ///< Manager::deleteAudioPlayer() から操作される
};

} // namespace Audio
} // namespace Lumino
