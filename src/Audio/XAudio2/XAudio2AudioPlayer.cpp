
#include "../Internal.h"
//#include "../../AudioUtil.h"
//#include "../../Resource/AudioSource.h"
#include <Lumino/Base/Exception.h>
#include "../AudioStream.h"
#include "../AudioUtils.h"
#include "XAudio2AudioDevice.h"
#include "XAudio2AudioPlayer.h"

namespace Lumino
{
namespace Audio
{

//=============================================================================
// XAudio2AudioPlayerBase 
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XAudio2AudioPlayerBase::XAudio2AudioPlayerBase(XAudio2AudioDevice* device)
	: AudioPlayer(device)
	, m_XAudio2AudioDevice(device)
	, mEmitterState(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XAudio2AudioPlayerBase::~XAudio2AudioPlayerBase()
{
    LN_SAFE_DELETE( mEmitterState );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2AudioPlayerBase::Initialize(AudioStream* audioStream, bool enable3d)
{
	AudioPlayer::Initialize(audioStream, enable3d);

	if (enable3d)
	{
		mEmitterState = LN_NEW EmitterState(m_decoder->GetWaveFormat()->Channels);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2AudioPlayerBase::setPosition(const Vector3& pos)
{
    if ( mEmitterState )
    {
        mEmitterState->Position = pos;
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2AudioPlayerBase::setVelocity(const Vector3& v)
{
    if ( mEmitterState )
    {
        mEmitterState->Velocity = v;
    }
}


//==============================================================================
// XAudio2OnMemoryAudioPlayer
//==============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XAudio2OnMemoryAudioPlayer::XAudio2OnMemoryAudioPlayer(XAudio2AudioDevice* device)
	: XAudio2AudioPlayerBase(device)
	, mSourceVoice(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XAudio2OnMemoryAudioPlayer::~XAudio2OnMemoryAudioPlayer()
{
	if ( mSourceVoice )
	{
		mSourceVoice->Stop();
		mSourceVoice->FlushSourceBuffers();
		mSourceVoice->DestroyVoice();
		mSourceVoice = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2OnMemoryAudioPlayer::Initialize(AudioStream* audioStream, bool enable3d)
{
	XAudio2AudioPlayerBase::Initialize(audioStream, enable3d);

	// オンメモリ再生用に内部に持ってるバッファを埋める
	m_decoder->FillOnmemoryBuffer();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2OnMemoryAudioPlayer::SetVolume( int volume )
{
    XAudio2AudioPlayerBase::SetVolume( volume );
    if ( mSourceVoice )
	{
		LN_COMCALL(mSourceVoice->SetVolume(0.01f * mVolume));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2OnMemoryAudioPlayer::SetPitch( int pitch )
{
    XAudio2AudioPlayerBase::SetPitch( pitch );
    if ( mSourceVoice )
	{
		LN_COMCALL(mSourceVoice->SetFrequencyRatio(0.01f * mPitch));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint64_t XAudio2OnMemoryAudioPlayer::getPlayedSamples() const
{
    if ( mSourceVoice )
    {
        XAUDIO2_VOICE_STATE voice_state;
		mSourceVoice->GetState( &voice_state );
		return voice_state.SamplesPlayed;
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2OnMemoryAudioPlayer::play()
{
    stop();

    // XAudio のソースボイス作成
	const WaveFormat* ln_format = m_decoder->GetWaveFormat();
	WAVEFORMATEX format;
	AudioUtils::ConvertLNWaveFormatToWAVEFORMATEX(*ln_format, &format);

	LN_COMCALL(m_XAudio2AudioDevice->GetXAudio2()->CreateSourceVoice(&mSourceVoice, &format));

	LN_COMCALL(mSourceVoice->FlushSourceBuffers());
        
    // SourceVoiceに送信するデータ
    XAUDIO2_BUFFER submit = { 0 };
	submit.AudioBytes = m_decoder->GetOnmemoryPCMBufferSize();	// バッファのバイト数
	submit.pAudioData = m_decoder->GetOnmemoryPCMBuffer();		// バッファの先頭アドレス
	submit.Flags = XAUDIO2_END_OF_STREAM;

	// ループ再生する場合
	if ( mIsLoop )
	{
		submit.LoopCount	= XAUDIO2_LOOP_INFINITE;	// 無限ループ
		submit.LoopBegin	= mLoopBegin;
		submit.LoopLength	= mLoopLength;
	}
	// ループ再生しない場合
	else
	{
		submit.LoopBegin = XAUDIO2_NO_LOOP_REGION;
	}

	// ボイスキューにデータを送る
	LN_COMCALL(mSourceVoice->SubmitSourceBuffer(&submit));
	LN_COMCALL(mSourceVoice->SetVolume(mVolume * 0.01f));
	LN_COMCALL(mSourceVoice->SetFrequencyRatio(mPitch * 0.01f));

	// 再生開始
	LN_COMCALL(mSourceVoice->Start());

	mIsPlaying = true;
	mIsPausing = false;

	polling();	// 3D オーディオの計算を行うため
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2OnMemoryAudioPlayer::stop()
{
    if ( mSourceVoice )
	{
		LN_COMCALL(mSourceVoice->Stop());
		LN_COMCALL(mSourceVoice->FlushSourceBuffers());\

		mSourceVoice->DestroyVoice();
		mSourceVoice = NULL;
	}

	mIsPlaying = false;
	mIsPausing = false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2OnMemoryAudioPlayer::pause( bool isPause )
{
	// 再生中の場合
	if ( mSourceVoice && mIsPlaying )
	{
		// 一時停止する場合で、現在一時停止していない場合
		if ( isPause && !mIsPausing )
		{
			LN_COMCALL(mSourceVoice->Stop(0));
			mIsPausing = true;
		}
		// 一時停止を解除する場合で、現在一時停止している場合
		else if ( !isPause && mIsPausing )
		{
			LN_COMCALL(mSourceVoice->Start(0));
			mIsPausing = false;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool XAudio2OnMemoryAudioPlayer::polling()
{
	// 再生中ではない場合は中断
	if ( !mSourceVoice || !mIsPlaying || mIsPausing )
	{
		return false;
	}

	// ループ再生ではない場合
	if ( !mIsLoop )
	{
		XAUDIO2_VOICE_STATE state;
		mSourceVoice->GetState( &state );

		// ボイスキューが空になった場合
		if ( state.BuffersQueued == 0 )
		{
			stop();
			return false;
		}
	}

    // 3D オーディオの更新
    if ( mEmitterState != NULL)
    {
        m_XAudio2AudioDevice->CalcEmitterState( mEmitterState );

		LN_COMCALL(mSourceVoice->SetFrequencyRatio(mEmitterState->DSPSettings.DopplerFactor * (mPitch * 0.01f)));

		LN_COMCALL(mSourceVoice->SetOutputMatrix(
			m_XAudio2AudioDevice->GetMasteringVoice(),   // NULL でもいいみたいだけど一応
			mEmitterState->DSPSettings.SrcChannelCount,
			mEmitterState->DSPSettings.DstChannelCount,
			mEmitterState->DSPSettings.pMatrixCoefficients ));
    }

    return true;
}


//==============================================================================
// XAudio2StreamingAudioPlayer
//==============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XAudio2StreamingAudioPlayer::XAudio2StreamingAudioPlayer(XAudio2AudioDevice* device)
	: XAudio2AudioPlayerBase(device)
	, mSourceVoice(NULL)
	, mPrimaryAudioData(NULL)
	, mSecondaryAudioData(NULL)
	, mAudioDataBufferSize(0)
	, mReadCursor(0)
	, mLoopBeginBlockOffset(0)
	, mCursorBeginPos(0)
	, mCursorEndPos(0)
	, mWriteSampleNum(0)
	, mEOF(false)
	, mPassedLoopBeginSample(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XAudio2StreamingAudioPlayer::~XAudio2StreamingAudioPlayer()
{
    if ( mSourceVoice )
	{
		mSourceVoice->Stop();
		mSourceVoice->FlushSourceBuffers();
		mSourceVoice->DestroyVoice();
		mSourceVoice = NULL;
	}

	LN_SAFE_DELETE_ARRAY( mPrimaryAudioData );
	LN_SAFE_DELETE_ARRAY(mSecondaryAudioData);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2StreamingAudioPlayer::Initialize(AudioStream* audioStream, bool enable3d)
{
	XAudio2AudioPlayerBase::Initialize(audioStream, enable3d);

	// 1 秒分のバッファサイズ取得
	mAudioDataBufferSize = m_decoder->GetBytesPerSec();

	// 1 秒のバッファをふたつ用意
	mPrimaryAudioData	= LN_NEW uint8_t[ mAudioDataBufferSize ];
	mSecondaryAudioData = LN_NEW uint8_t[ mAudioDataBufferSize ];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2StreamingAudioPlayer::SetVolume( int volume )
{
    XAudio2AudioPlayerBase::SetVolume( volume );
    if ( mSourceVoice )
	{
		LN_COMCALL(mSourceVoice->SetVolume(0.01f * mVolume));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2StreamingAudioPlayer::SetPitch(int pitch)
{
	XAudio2AudioPlayerBase::SetPitch(pitch);
    if ( mSourceVoice )
	{
		LN_COMCALL(mSourceVoice->SetFrequencyRatio(0.01f * mPitch));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint64_t XAudio2StreamingAudioPlayer::getPlayedSamples() const
{
    if ( mSourceVoice )
    {
        XAUDIO2_VOICE_STATE voice_state;
		mSourceVoice->GetState( &voice_state );
		return voice_state.SamplesPlayed;
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2StreamingAudioPlayer::play()
{
    stop();

    // ソースボイス作成
	const WaveFormat* ln_format = m_decoder->GetWaveFormat();
	WAVEFORMATEX format;
    AudioUtils::ConvertLNWaveFormatToWAVEFORMATEX( *ln_format, &format );

	LN_COMCALL(m_XAudio2AudioDevice->GetXAudio2()->CreateSourceVoice(&mSourceVoice, &format));
	LN_COMCALL(mSourceVoice->FlushSourceBuffers());

	// デコード状態をリセットする ( MP3 用 )
	m_decoder->Reset();

	// ストリーミング読み込み位置リセット
	mReadCursor = 0;

    // とりあえずソースデータをカーソルの終端にする。
    // ファイル内から読むのは _addNextBuffer() で
	mCursorEndPos = m_decoder->GetSourceDataSize();
        
    mWriteSampleNum = 0;
    mCursorBeginPos = 0;
    mLoopBeginBlockOffset = 0;

	// 最初のデータをキューに追加する
	_addNextBuffer();

	LN_COMCALL(mSourceVoice->SetVolume(mVolume * 0.01f));
	LN_COMCALL(mSourceVoice->SetFrequencyRatio(mPitch * 0.01f));

	// 再生開始
	LN_COMCALL(mSourceVoice->Start());

	mIsPlaying = true;
	mIsPausing = false;

	polling();	// 3D オーディオの計算を行うため
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2StreamingAudioPlayer::stop()
{
	if ( mSourceVoice )
	{
		LN_COMCALL(mSourceVoice->Stop());
		LN_COMCALL(mSourceVoice->FlushSourceBuffers());

		mSourceVoice->DestroyVoice();
		mSourceVoice = NULL;
	}

	mIsPlaying = false;
	mIsPausing = false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2StreamingAudioPlayer::pause( bool isPause )
{
	// 再生中の場合
	if ( mSourceVoice && mIsPlaying )
	{
		// 一時停止する場合で、現在一時停止していない場合
		if ( isPause && !mIsPausing )
		{
			LN_COMCALL(mSourceVoice->Stop(0));
			mIsPausing = true;
		}
		// 一時停止を解除する場合で、現在一時停止している場合
		else if ( !isPause && mIsPausing )
		{
			LN_COMCALL(mSourceVoice->Start(0));
			mIsPausing = false;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool XAudio2StreamingAudioPlayer::polling()
{
	// 再生中ではない場合は中断
	if ( !mSourceVoice || !mIsPlaying || mIsPausing )
	{
		return false;
	}

	// 状態の取得
	XAUDIO2_VOICE_STATE voice_state;
	mSourceVoice->GetState( &voice_state );

	// 再生が終了しておらず、キューにデータがふたつたまっていない場合
	if ( !mEOF && voice_state.BuffersQueued < 2 )
	{
		// 読み込みカーソルが終端を超えている場合
		if ( mReadCursor >= mCursorEndPos )
		{
			// ループ再生の場合、次にキューに追加するデータはループ領域の先頭であるため、読み込みカーソル位置を先頭に戻す
			if ( mIsLoop )
			{
				mReadCursor = mCursorBeginPos;
				mCursorEndPos = m_decoder->GetSourceDataSize();
			}
			// ループ再生しない場合はファイルの最後まで読み切ったことを示すフラグを ON
			else
			{
				mEOF = true;
			}
		}

		// ファイルの最後まで読み切っていない場合
		if ( !mEOF )
		{
			// 次のデータを再生キューに追加
			_addNextBuffer();
		}
	}

	// 非ループ再生で、ファイルの最後まで読み切っている場合 ( 再生終了している場合 )
	if ( !mIsLoop && mEOF )
	{
		// キューが完全になくなった場合、停止とする
		if ( voice_state.BuffersQueued == 0 )
		{
			stop();
			return false;
		}
	}

    // 3D オーディオの更新
    if ( mEmitterState )
    {
        m_XAudio2AudioDevice->CalcEmitterState( mEmitterState );

		LN_COMCALL(mSourceVoice->SetFrequencyRatio(mEmitterState->DSPSettings.DopplerFactor * (mPitch * 0.01f)));

		LN_COMCALL(mSourceVoice->SetOutputMatrix(
			m_XAudio2AudioDevice->GetMasteringVoice(),   // NULL でもいいみたいだけど一応
			mEmitterState->DSPSettings.SrcChannelCount,
			mEmitterState->DSPSettings.DstChannelCount,
			mEmitterState->DSPSettings.pMatrixCoefficients));
    }

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2StreamingAudioPlayer::_addNextBuffer()
{
	Threading::MutexScopedLock lock(mLock);
#if 0
    // 読み込み位置に移動
	mAudioSource->seek( mReadCursor );

	// データ読み込み
	uint32_t read_size;		// 呼んだサイズ
	uint32_t write_size;		// デコードして書き込んだサイズ
	mAudioSource->read( mSecondaryAudioData, mAudioDataBufferSize, &read_size, &write_size );
#endif

	// データ読み込み
	uint32_t read_size;		// 呼んだサイズ
	uint32_t write_size;		// デコードして書き込んだサイズ
	m_decoder->Read(mReadCursor, mSecondaryAudioData, mAudioDataBufferSize, &read_size, &write_size);

	// 実際に元データから読んだデータ分、カーソルを進める
	mReadCursor += read_size;

    // 書きこんだサイズを加算
    mWriteSampleNum += write_size;

    uint8_t* data_buffer = mSecondaryAudioData;

    // ループ上で扱うサンプル数は、チャンネル数関係無しになってる。
    // それを正しい値に変換するため、実際の 1 サンプルあたりのバイト数を計算する
    uint32_t smp_size =
		m_decoder->GetWaveFormat()->Channels *
		m_decoder->GetWaveFormat()->BitsPerSample / 8;

    if ( mIsLoop )
    {
        if ( mLoopBegin == 0 && mLoopLength == 0 )
        {
            mCursorBeginPos = 0;
			mCursorEndPos = m_decoder->GetSourceDataSize();
        }
        else
        {
            // 読んだデータ内にループ領域の開始位置が含まれている場合
            if ( mPassedLoopBeginSample && mReadCursor - read_size == mCursorBeginPos )
            {
                data_buffer = ( data_buffer + mLoopBeginBlockOffset );
                write_size -= mLoopBeginBlockOffset; 
            }
            // ループ領域の開始位置が未設定で、ループ領域の開始サンプルを超えた場合
            else if ( mLoopBegin > 0 && !mPassedLoopBeginSample && mWriteSampleNum > ( mLoopBegin * smp_size ) )
            {
                // カーソルはブロック単位。直前の read() のひとつ前を指す
                mCursorBeginPos = mReadCursor - read_size;

                // ブロック先頭からのサンプル数
                mLoopBeginBlockOffset = write_size - ( mWriteSampleNum - ( mLoopBegin * smp_size ) );

                mPassedLoopBeginSample = true;
            }

            //printf( "%d %d %d\n", mLoopBegin, mLoopLength, ( mLoopBegin + mLoopLength ) );
            //printf( "b:%d l:%d n:%d a:%d\n", mLoopBegin, mLoopLength, mWriteSampleNum, ( mLoopBegin + mLoopLength ) * smp_size );

            // 終端サンプル数を超えた場合、そこが終端
            if ( mLoopLength > 0 && mWriteSampleNum > ( mLoopBegin + mLoopLength ) * smp_size )
            {
                // 終端。polling() でも検出する
                mCursorEndPos = mReadCursor;

                write_size -= ( mWriteSampleNum - ( mLoopBegin + mLoopLength ) * smp_size );

                mWriteSampleNum = mCursorBeginPos;
            }
        }
    }
        
	// SourceVoice に送信するデータの設定
	XAUDIO2_BUFFER buffer = { 0 };

	// 現在位置がファイルの終端に達している場合
	if ( !mIsLoop && mReadCursor >= mCursorEndPos )//mAudioSource->getSourceDataSize() )
	{
		// フラグ追加 ( 最後まで演奏した時、GetState で取得する演奏時間をリセットする )
		buffer.Flags = XAUDIO2_END_OF_STREAM;
	}

	buffer.AudioBytes = write_size;
	buffer.pAudioData = data_buffer;

	//static int a = 0;
	//LRefString str;
	//str.format( "test%d.pcm", a);
	//FileIO::File::writeAllBytes( str.c_str(), buffer.pAudioData, buffer.AudioBytes );
	//++a;

	// SourceVoice にデータを送る
	LN_COMCALL(mSourceVoice->SubmitSourceBuffer(&buffer));

	// primary と secondary の入れ替え
	uint8_t* temp = mPrimaryAudioData;
	mPrimaryAudioData = mSecondaryAudioData;
	mSecondaryAudioData = temp;
}

} // namespace Audio
} // namespace Lumino
