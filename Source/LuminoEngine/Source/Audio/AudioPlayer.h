
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Math/Vector3.h>
#include <Lumino/Audio/Common.h>
#include <Lumino/Audio/Detail.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{
class AudioDevice;
class AudioStream;
class AudioDecoder;

// 音声の再生を行うクラス。
// 使う側は、複数のスレッドから同時にアクセスしないように注意すること。
class AudioPlayer
    : public RefObject
{
public:
	AudioPlayer(AudioDevice* device);
	virtual ~AudioPlayer();

public:
	virtual void initialize(AudioStream* audioStream, bool enable3d);

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

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
