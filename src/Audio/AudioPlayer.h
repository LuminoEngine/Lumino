
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Math/Vector3.h>
#include <Lumino/Audio/Common.h>
#include <Lumino/Audio/Detail.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
class AudioDevice;
class AudioStream;
class AudioDecoder;


/*
AudioPlayerState newState;
{
	ScopedLock lock(m_stateSync);
	if (m_state.GetModifiedFlags() & ModifiedFlags_Volume)
	{
		state.SetVolume(m_state.GetVolume());
	}
	if (m_state.GetModifiedFlags() & ModifiedFlags_Pitch)
	{
		state.SetPitch(m_state.GetPitch());
	}
	if (m_state.GetModifiedFlags() & ModifiedFlags_LoopEnabled)
	{
		state.SetLoopEnabled(m_state.IsLoopEnabled());
	}
	if (m_state.GetModifiedFlags() & ModifiedFlags_LoopBegin)
	{
		state.SetLoopBegin(m_state.GetLoopBegin());
	}
	if (m_state.GetModifiedFlags() & ModifiedFlags_LoopLength)
	{
		state.SetLoopLength(m_state.GetLoopLength());
	}
	if (m_state.GetModifiedFlags() & ModifiedFlags_PlayingState)
	{
		// 再生状態が Sound 側から変更されているので、Player へ設定する。
		// Player 側から変更されている場合も Sound 側の状態を上書きする。
		state.SetPlayingState(m_state.GetPlayingState());
	}
	else if (state.GetModifiedFlags() & ModifiedFlags_PlayingState)
	{
		// Player 側からのみ変更されている場合は Sound 側へ設定する。
		m_state.SetPlayingState(state.GetPlayingState());
	}
}

player->CommitState(state);
m_state.SetModifiedFlags(ModifiedFlags_None);

----

if (newState.GetModifiedFlags() & ModifiedFlags_Volume)
{
	SetVolume(newState.GetVolume());
}
if (newState.GetModifiedFlags() & ModifiedFlags_Pitch)
{
	SetPitch(newState.GetPitch());
}
if (newState.GetModifiedFlags() & ModifiedFlags_LoopEnabled)
{
	SetLoopEnabled(newState.IsLoopEnabled());
}
if ((newState.GetModifiedFlags() & ModifiedFlags_LoopBegin) || (newState.GetModifiedFlags() & ModifiedFlags_LoopLength))
{
	SetLoopState(newState.GetLoopBegin(), newState.GetLoopLength());
}
if (newState.GetModifiedFlags() & ModifiedFlags_PlayingState)
{
	switch (newState.GetPlayingState())
	{
	case SoundPlayingState::Stopped:
		Stop();
		break;
	case SoundPlayingState::Playing:
		if (m_state.GetPlayingState() == SoundPlayingState::Pausing)
		{
			Pause(false);
		}
		else
		{
			Play();
		}
		break;
	case SoundPlayingState::Pausing:
		Pause(true);
		break;
	}
}
m_state = state;
*/

// 音声の再生を行うクラス。
// 使う側は、複数のスレッドから同時にアクセスしないように注意すること。
class AudioPlayer
    : public RefObject
{
public:
	AudioPlayer(AudioDevice* device);
	virtual ~AudioPlayer();

public:
	virtual void Initialize(AudioStream* audioStream, bool enable3d);

public:
	void CommitPlayerState(const detail::AudioPlayerState& newState);
	const detail::AudioPlayerState& GetPlayerState() const { return m_playerState; }

	void DoPolling();

protected:


	virtual void SetVolume(float volume);
	virtual void SetPitch(float pitch);
	virtual void SetLoopState(uint32_t loop_begin, uint32_t loop_length);
	void SetLoopEnabled(bool enableLoop) { mIsLoop = enableLoop; }



	/// 再生
	virtual void Play() = 0;

	/// 停止
	virtual void Stop() = 0;

	/// 一時停止
	virtual void Pause(bool isPause) = 0;

public:	// TODO: protected にしたい

		/// 再生したサンプル数の取得 ( Midi の場合はミュージックタイム )
	virtual uint64_t GetPlayedSamples() const = 0;

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

	detail::AudioPlayerState	m_playerState;

	bool			    mIsPlaying;
	bool			    mIsPausing;
	bool			    mIsLoop;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
