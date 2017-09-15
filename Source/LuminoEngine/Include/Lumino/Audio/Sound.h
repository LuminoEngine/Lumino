
#pragma once
#include <Lumino/Math/Vector3.h>
#include "../Animation/EasingValue.h"
#include "Common.h"
#include "Detail.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

class Sound;
typedef Ref<Sound>	SoundPtr;

/**
	@brief	音声の再生、制御を行います。
*/
LN_CLASS()
class Sound
    : public Object
{
	LN_OBJECT;
public:


	/**
		@brief	Sound クラスのインスタンスを作成します。
	*/
	static SoundPtr create(const Char* filePath);

	/**
		@brief	Sound クラスのインスタンスを作成します。
	*/
	static SoundPtr create(Stream* stream, SoundLoadingMode loadingMode);

public:
	
	/**
		@brief		この音声の音量を設定します。
		@param[in]	volume	: 音量 (0.0～1.0。初期値は 1.0)
	*/
	LN_METHOD(Property)
	void setVolume(float volume);

	/**
		@brief		この音声の音量を取得します。
	*/
	LN_METHOD(Property)
	float getVolume() const;

	/**
		@brief		この音声のピッチ (音高) を設定します。
		@param[in]	volume	: ピッチ (0.5～2.0。初期値は 1.0)
	*/
	LN_METHOD(Property)
	void setPitch(float pitch);

	/**
		@brief		この音声のピッチ (音高) を取得します。
	*/
	LN_METHOD(Property)
	float getPitch() const;

	/**
		@brief		ループ再生の有無を設定します。
		@param[in]	enabled		: ループ再生するか
	*/
	LN_METHOD(Property)
	void setLoopEnabled(bool enabled);

	/**
		@brief		ループ再生が有効かを確認します。
	*/
	LN_METHOD(Property)
	bool isLoopEnabled() const;

	/**
		@brief		ループ範囲を設定します。
		@param[in]	begin		: ループ範囲の開始サンプル
		@param[in]	length		: ループ範囲のサンプル数
		@details	MIDI の場合、ループ範囲はミュージックタイム単位 (四分音符ひとつ分を 768 で表す) で指定します。 
	*/
	LN_METHOD()
	void SetLoopRange(uint32_t begin, uint32_t length);

	/**
		@brief		この音声の再生を開始します。
	*/
	LN_METHOD()
	void play();

	/**
		@brief		この音声の再生を停止します。
	*/
	LN_METHOD()
	void stop();

	/**
		@brief		この音声の再生を一時停止します。
	*/
	LN_METHOD()
	void pause();

	/**
		@brief		一時停止中の再生を再開します。
	*/
	LN_METHOD()
	void Resume();

	/**
		@brief		サウンドを 3D 音源として再生するかを設定します。(規定値:false)
		@details	設定は play() の前に行う必要があります。
	*/
	void set3DEnabled(bool enabled);

	/**
		@brief		この音声が 3D 音声であるかを確認します。
	*/
	bool is3DEnabled() const;

	/**
		@brief		3D 音声としての位置を設定します。
	*/
	void setEmitterPosition(const Vector3& position);

	/**
		@brief		3D 音声としての位置を取得します。
	*/
	const Vector3& getEmitterPosition() const;
	
	/**
		@brief		3D 音声としての速度を設定します。
	*/
	void setEmitterVelocity(const Vector3& velocity);

	/**
		@brief		3D 音声としての位置を取得します。
	*/
	const Vector3& getEmitterVelocity() const;

	/**
		@brief		3D 音声としての減衰距離を設定します。(default:100)
	*/
	void setEmitterMaxDistance(float distance);

	/**
		@brief		音声データの全サンプル数を取得します。
		@details	MIDI の場合はミュージックタイム単位 (四分音符ひとつ分が 768) で、
					1度でも play() で再生を開始していないと取得できません。
	*/
	int64_t getTotalSamples() const;

	/**
		@brief		現在の再生サンプル数を取得します。
	*/
	int64_t getPlayedSamples() const;

	/**
		@brief		サンプリングレートを取得します。
		@details	MIDI の場合は 768 を返します。
	*/
	int getSamplingRate() const;

	/**
		@brief		この音声の現在の再生状態を取得します。
	*/
	SoundPlayingState getPlayingState() const;

	/**
		@brief		音声データの再生方法を設定します。(規定値:Unknown)
		@details	設定は play() の前に行う必要があります。
	*/
	void SetPlayingMode(SoundPlayingMode mode);
	
	/**
		@brief		音声データの再生方法を取得します。
	*/
	SoundPlayingMode getPlayingMode() const;

	/**
		@brief		音量のフェードを開始します。
		@param[in]	targetVolume	: フェード先音量
		@param[in]	time			: 変化にかける時間 (秒)
		@param[in]	state			: 音量フェード完了時の動作
		@details	現在の音量から targetVolume へ音量の遷移を行います。現在の音量は getVolume() で取得できる値です。
					フェード中は音量が変更され、getVolume() で取得できる値が変わります。
	*/
	void fadeVolume(float targetVolume, double time, SoundFadeBehavior behavior);

	/**
		@brief		音量のフェード中であるかを確認します。
	*/
	bool isVolumeFading() const;

LN_INTERNAL_ACCESS:
	Sound();
	virtual ~Sound();
	LN_METHOD()
	void initialize(const StringRef& filePath);
	void initialize(Stream* stream, SoundLoadingMode loadingMode);
	void initialize(detail::AudioStream* audioStream);
	void createAudioPlayerSync();
	void polling(float elapsedTime);

	void setGameAudioFlags(uint32_t flags) { m_gameAudioFlags = flags; }
	uint32_t getGameAudioFlags() const { return m_gameAudioFlags; }
	detail::AudioStream* getAudioStream() const { return m_audioStream; }

private:
	detail::AudioManager*		m_manager;
	Mutex						m_mutex;
	detail::AudioStream*		m_audioStream;
	detail::AudioPlayer*		m_audioPlayer;
	SoundPlayingMode			m_playingMode;

	Mutex						m_playerStateLock;	// TODO: 性質上、スピンロックの方が効率がいいかもしれない
	detail::AudioPlayerState	m_playerState;
	bool						m_is3DSound;
	Vector3						m_position;
	Vector3						m_velocity;
	float						m_maxDistance;

	uint32_t					m_gameAudioFlags;

	EasingValue<float, double>	m_fadeValue;
	SoundFadeBehavior			m_fadeBehavior;
	bool						m_fading;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
