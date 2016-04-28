
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Math/Vector3.h>
#include <Lumino/Audio/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
class AudioDevice;
class AudioStream;
class AudioDecoder;

/*
	別スレッドでのデコード・再生管理を行いたいので、別スレッドで処理中に各種ステートを変更してほしくない。
	Polling でステートを丸ごとコピーしておく。
	Sound クラスの持つステートを変更する関数は全て mutex 張らなければならない。読み取りは無くてもいい。
*/
class AudioPlayerState
{
public:
	enum ModifiedFlags
	{
		ModifiedFlags_None = 0x0000,
		ModifiedFlags_Volume = 0x0001,
		ModifiedFlags_Pitch = 0x0002,
		ModifiedFlags_LoopEnabled = 0x0004,
		ModifiedFlags_LoopBegin = 0x0008,
		ModifiedFlags_LoopLength = 0x0010,
		ModifiedFlags_PlayingState = 0x0020,
	};

public:
	LN_DISALLOW_COPY_AND_ASSIGN(AudioPlayerState);
	AudioPlayerState();

	void				SetModifiedFlags(uint32_t v) { m_modified = v; }
	uint32_t			GetModifiedFlags() const { return m_modified; }
	void				SetVolume(float v) { m_volume = v; m_modified |= ModifiedFlags_Volume; }
	float				GetVolume() const { return m_volume; }
	void				SetPitch(float v) { m_pitch = v; m_modified |= ModifiedFlags_Pitch; }
	float				GetPitch() const { return m_pitch; }
	void				SetLoopEnabled(bool v) { m_loopEnabled = v; m_modified |= ModifiedFlags_LoopEnabled; }
	bool				IsLoopEnabled() const { return m_loopEnabled; }
	void				SetLoopBegin(uint32_t v) { m_loopBegin = v; m_modified |= ModifiedFlags_LoopBegin; }
	uint32_t			GetLoopBegin() const { return m_loopBegin; }
	void				SetLoopLength(uint32_t v) { m_loopLength = v; m_modified |= ModifiedFlags_LoopLength; }
	uint32_t			GetLoopLength() const { return m_loopLength; }
	void				SetPlayingState(SoundPlayingState v) { m_playingState = v; m_modified |= ModifiedFlags_PlayingState; }
	SoundPlayingState	GetPlayingState() const { return m_playingState; }

private:
	uint32_t			m_modified;			// ModifiedFlags
	float				m_volume;			// 音量
	float				m_pitch;			// ピッチ
	bool			    m_loopEnabled;		// ループ有無
	uint32_t			m_loopBegin;		// ループされる領域の最初のサンプル (Midi なら ミュージックタイム単位)
	uint32_t			m_loopLength;		// ループ領域の長さ (サンプル数単位)  (Midi なら ミュージックタイム単位)
	SoundPlayingState	m_playingState;		// 再生状態
};

// AudioPlayer
class AudioPlayer
    : public RefObject
{
public:
	AudioPlayer(AudioDevice* device);
	virtual ~AudioPlayer();

public:
	virtual void Initialize(AudioStream* audioStream, bool enable3d);

public:
    virtual void SetVolume( float volume );
	virtual void SetPitch(float pitch);
	virtual void SetLoopState(uint32_t loop_begin, uint32_t loop_length);
    void SetLoopEnabled( bool enableLoop ) { mIsLoop = enableLoop; }

public:

	/// 再生したサンプル数の取得 ( Midi の場合はミュージックタイム )
    virtual uint64_t GetPlayedSamples() const = 0;

	/// 再生
	virtual void Play() = 0;

	/// 停止
	virtual void Stop() = 0;

	/// 一時停止
	virtual void Pause(bool isPause) = 0;

	/// 3D 音源かを判定する
	virtual bool Is3DSound() = 0;

	/// 3D 音源としての位置を設定する
    virtual void setPosition( const Vector3& pos ) = 0;

	/// 3D 音源としての位置を取得する
    virtual const Vector3& getPosition() = 0;

	/// 速度の設定
    virtual void setVelocity( const Vector3& v ) = 0;

	/// 3D 音源の影響範囲 (音の届く距離) の設定
    virtual void setEmitterDistance( float distance ) = 0;

	/// 3D 音源の影響範囲 (音の届く距離) の取得
	virtual float getEmitterDistance() const = 0;


	/// ポーリング更新
	virtual bool Polling() = 0;

protected:
	AudioDevice*		mDevice;
	AudioStream*		m_audioStream;
	AudioDecoder*		m_decoder;
	float				mVolume;		///< 音量
	float				mPitch;			///< ピッチ
	uint32_t			mLoopBegin;		///< ループされる領域の最初のサンプル (Midi なら ミュージックタイム単位)
	uint32_t			mLoopLength;	///< ループ領域の長さ (サンプル数単位)  (Midi なら ミュージックタイム単位)
	bool			    mIsPlaying;
	bool			    mIsPausing;
	bool			    mIsLoop;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
