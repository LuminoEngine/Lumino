/*
	@file	AudioPlayer.h
*/
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Math/Vector3.h>
#include <Lumino/Audio/Common.h>
//#include "../../Base/Container/NodeList.h"
//#include "../Interface.h"
//#include "../Types.h"

namespace Lumino
{
namespace Audio
{
class AudioDevice;
class AudioStream;
class AudioDecoder;

/// AudioPlayer
class AudioPlayer
    : public RefObject
    //, public Base::NodeList< AudioPlayerBase >::Node
{
public:
	AudioPlayer(AudioDevice* device);
	virtual ~AudioPlayer();

public:
	virtual void Initialize(AudioStream* audioStream, bool enable3d);

public:
	virtual AudioStream*		GetAudioStream() const { return m_audioStream; }
    virtual void				SetVolume( int volume );
    virtual int					getVolume() const { return static_cast< int >( mVolume ); }
    virtual void				SetPitch( int pitch );
	virtual int					getPitch() const { return static_cast< int >( mPitch ); }
	virtual void				setLoopState(uint32_t loop_begin, uint32_t loop_length);
    virtual bool				isPlaying() const { return mIsPlaying; }
    virtual bool				isPausing() const { return mIsPausing; }
	virtual SoundPlayState		getPlayState() const { return (mIsPausing) ? SoundPlayState_Pausing : ((mIsPlaying) ? SoundPlayState_Playing : SoundPlayState_Stopped); }
    virtual void				loop( bool enableLoop ) { mIsLoop = enableLoop; }
    virtual bool				isLoop() const { return mIsLoop; }

public:

	/// 再生したサンプル数の取得 ( Midi の場合はミュージックタイム )
    virtual uint64_t getPlayedSamples() const = 0;

	/// 再生
	virtual void play() = 0;

	/// 停止
	virtual void stop() = 0;

	/// 一時停止
	virtual void pause( bool isPause ) = 0;

	/// ポーリング更新
	virtual bool polling() = 0;

	/// 3D 音源かを判定する
	virtual bool is3DSound() = 0;

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
	
protected:
	AudioDevice*		mDevice;
	AudioStream*		m_audioStream;
	AudioDecoder*		m_decoder;
    int				    mVolume;		///< 音量 ( 0 ～ 100 )
	int				    mPitch;			///< ピッチ ( 50 ～ 200 )
	uint32_t			mLoopBegin;		///< ループされる領域の最初のサンプル (Midi なら ミュージックタイム単位)
	uint32_t			mLoopLength;	///< ループ領域の長さ (サンプル数単位)  (Midi なら ミュージックタイム単位)
	bool			    mIsPlaying;
	bool			    mIsPausing;
	bool			    mIsLoop;
};

} // namespace Audio
} // namespace Lumino
