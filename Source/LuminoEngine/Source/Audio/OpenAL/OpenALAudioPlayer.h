
#pragma once
#include "OpenALAudioDevice.h"
#include "../AudioPlayer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

// OpenAL 用 AudioPlayer ベースクラス
class OpenALAudioPlayerBase
    : public AudioPlayer
{
public:
	OpenALAudioPlayerBase(OpenALAudioDevice* device);
    virtual ~OpenALAudioPlayerBase();

public:
	virtual void			Initialize(AudioStream* audioStream, bool enable3d) override;
    virtual bool			Is3DSound() override { return ( mEmitterState != NULL ); }
    virtual void			setPosition( const Vector3& pos ) override;
    virtual const Vector3&	getPosition() override { return mEmitterState->Position; }
    virtual void			setVelocity( const Vector3& v ) override;
	virtual void			setEmitterDistance(float distance) override { if (mEmitterState) mEmitterState->Distance = distance; }
	virtual float			getEmitterDistance() const override { return ( mEmitterState ) ? mEmitterState->Distance : 0; }

public:

	// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

protected:
	//OpenALAudioDevice*    m_OpenALAudioDevice;	// 管理クラス
    EmitterState*			mEmitterState;			// 3D 音源の情報 (3D音源として初期化された場合に作成される)
};

/// オンメモリデータの再生クラス
class OpenALOnMemoryAudioPlayer
    : public OpenALAudioPlayerBase
{
public:
	OpenALOnMemoryAudioPlayer(OpenALAudioDevice* device);
	virtual ~OpenALOnMemoryAudioPlayer();

public:
	virtual void		Initialize(AudioStream* audioStream, bool enable3d) override;
	virtual void		SetVolume(float volume) override;
	virtual void		SetPitch(float pitch) override;
    virtual uint64_t	GetPlayedSamples() const override;
	virtual void		Play() override;
	virtual void		Stop() override;
	virtual void		Pause( bool is_pause_ ) override;
	virtual bool        Polling() override;

	// OpenALAudioPlayerBase interface
    virtual IOpenALSourceVoice* getOpenALSourceVoice() const override { return mSourceVoice; }
    virtual EmitterState* getEmitterState() const override { return mEmitterState; }

private:
};


///	ストリーミングでの再生を扱うクラス
///
///		開始位置指定を実装する場合、ループ位置計算の都合で
///		ストリーミングは制限とする方向で。
///		(デコード時のブロックサイズがまちまちなので、一度流さないとわからない)
class OpenALStreamingAudioPlayer
    : public OpenALAudioPlayerBase
{
public:
	OpenALStreamingAudioPlayer(OpenALAudioDevice* device);
	virtual ~OpenALStreamingAudioPlayer();

public:
	virtual void		Initialize(AudioStream* audioStream, bool enable3d) override;
	virtual void		SetVolume(float volume) override;
	virtual void		SetPitch(float pitch) override;
	virtual uint64_t	GetPlayedSamples() const override;
	virtual void		Play() override;
	virtual void		Stop() override;
	virtual void		Pause( bool is_pause_ ) override;
	virtual bool		Polling() override;

	// OpenALAudioPlayerBase interface
    virtual IOpenALSourceVoice* getOpenALSourceVoice() const  override { return mSourceVoice; }
    virtual EmitterState* getEmitterState() const override { return mEmitterState; }

private:

	/// セカンダリに新しいデータを書き込んで、再生キューに追加する
	void _addNextBuffer();

private:
	Mutex					mLock;

	uint8_t*				mPrimaryAudioData;		///< ダブルバッファリング用プライマリ
	uint8_t*				mSecondaryAudioData;	///< ダブルバッファリング用セカンダリ
	uint32_t				mAudioDataBufferSize;	///< プライマリバッファとセランダリバッファのサイズ ( 1 秒分 )
	uint32_t				mReadCursor;

    uint32_t				mLoopBeginBlockOffset;
    uint32_t				mCursorBeginPos;
    uint32_t				mCursorEndPos;

    uint32_t				mWriteSampleNum;
	bool					mEOF;			///< ファイルの終端まで再生したフラグ
    bool                    mPassedLoopBeginSample;
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
