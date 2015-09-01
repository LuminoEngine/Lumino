/*
	@file	XAudio2AudioPlayer.h
*/
#pragma once
//
//#include "../../../Threading/Mutex.h"
#include "XAudio2AudioDevice.h"
#include "../AudioPlayer.h"
//#include "../../Interface.h"

namespace Lumino
{
namespace Audio
{
class XAudio2AudioDevice;

/// XAudio2 Player ベースクラス
class XAudio2AudioPlayerBase
    : public AudioPlayer
{
public:
	XAudio2AudioPlayerBase(XAudio2AudioDevice* device);
    virtual ~XAudio2AudioPlayerBase();

public:
	virtual void			Initialize(AudioStream* audioStream, bool enable3d);
    virtual bool			is3DSound() { return ( mEmitterState != NULL ); }
    virtual void			setPosition( const Vector3& pos );
    virtual const Vector3&	getPosition() { return mEmitterState->Position; }
    virtual void			setVelocity( const Vector3& v );
	virtual void			setEmitterDistance(float distance) { if (mEmitterState) mEmitterState->Distance = distance; }
	virtual float			getEmitterDistance() const { return ( mEmitterState ) ? mEmitterState->Distance : 0; }

public:

	/// IXAudio2SourceVoice の取得
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const = 0;

	/// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

protected:
	XAudio2AudioDevice*    m_XAudio2AudioDevice;	///< 管理クラス
    EmitterState*			mEmitterState;			///< 3D 音源の情報 (3D音源として初期化された場合に作成される)
};

/// オンメモリデータの再生クラス
class XAudio2OnMemoryAudioPlayer
    : public XAudio2AudioPlayerBase
{
public:
	XAudio2OnMemoryAudioPlayer(XAudio2AudioDevice* device);
	virtual ~XAudio2OnMemoryAudioPlayer();

public:
	virtual void		Initialize(AudioStream* audioStream, bool enable3d);
	virtual void		SetVolume( int volume );
	virtual void		SetPitch( int pitch );
    virtual uint64_t	getPlayedSamples() const;
	virtual void		play();
	virtual void		stop();
	virtual void		pause( bool is_pause_ );
	virtual bool        polling();

public:
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

private:
	IXAudio2SourceVoice*	    mSourceVoice;
};


///	ストリーミングでの再生を扱うクラス
///
///		開始位置指定を実装する場合、ループ位置計算の都合で
///		ストリーミングは制限とする方向で。
///		(デコード時のブロックサイズがまちまちなので、一度流さないとわからない)
class XAudio2StreamingAudioPlayer
    : public XAudio2AudioPlayerBase
{
public:
	XAudio2StreamingAudioPlayer(XAudio2AudioDevice* device);
	virtual ~XAudio2StreamingAudioPlayer();

public:
	virtual void		Initialize(AudioStream* audioStream, bool enable3d);
	virtual void		SetVolume( int volume );
	virtual void		SetPitch( int pitch );
	virtual uint64_t	getPlayedSamples() const;
	virtual void		play();
	virtual void		stop();
	virtual void		pause( bool is_pause_ );
	virtual bool		polling();

public:
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

private:

	/// セカンダリに新しいデータを書き込んで、再生キューに追加する
	void _addNextBuffer();

private:
	IXAudio2SourceVoice*    mSourceVoice;
	Threading::Mutex		mLock;

	uint8_t*				mPrimaryAudioData;		///< ダブルバッファリング用プライマリ
	uint8_t*				mSecondaryAudioData;	///< ダブルバッファリング用セカンダリ
	uint32_t				    mAudioDataBufferSize;	///< プライマリバッファとセランダリバッファのサイズ ( 1 秒分 )
	uint32_t				    mReadCursor;

    uint32_t                   mLoopBeginBlockOffset;
    uint32_t                   mCursorBeginPos;
    uint32_t                   mCursorEndPos;

    uint32_t                   mWriteSampleNum;
	bool					mEOF;			///< ファイルの終端まで再生したフラグ
    bool                    mPassedLoopBeginSample;
};

} // namespace Audio
} // namespace Lumino
