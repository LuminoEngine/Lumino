
#include "Internal.h"
#include <Lumino/Base/Exception.h>
#include <Lumino/Math/MathUtils.h>
#include "AudioStream.h"
#include "AudioDevice.h"
#include "AudioPlayer.h"

namespace Lumino
{
namespace Audio
{

//=============================================================================
// AudioPlayer 
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioPlayer::AudioPlayer(AudioDevice* device)
	: mDevice(device)
	, m_audioStream(NULL)
	, mVolume(100)
	, mPitch(100)
	, mLoopBegin(0)
	, mLoopLength(0)
	, mIsPlaying(false)
	, mIsPausing(false)
	, mIsLoop(false)
{
	LN_SAFE_ADDREF(mDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioPlayer::~AudioPlayer()
{
	LN_SAFE_RELEASE(m_audioStream);
	LN_SAFE_RELEASE(mDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioPlayer::Initialize(AudioStream* audioStream, bool enable3d)
{ 
	LN_THROW(audioStream, ArgumentException);
	LN_REFOBJ_SET(m_audioStream, audioStream);
	m_decoder = m_audioStream->GetDecoder();

    // ソースからループ位置を取得
    uint32_t loop_begin, loop_length;
	m_decoder->GetLoopState(&loop_begin, &loop_length);
    mLoopBegin  = loop_begin;
	mLoopLength = loop_length;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioPlayer::SetVolume( int volume )
{
    mVolume = volume;
	mVolume = LN_CLAMP(mVolume, 0, 100);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioPlayer::SetPitch( int pitch )
{
	mPitch = pitch;
	mPitch = LN_CLAMP(mPitch, 50, 200);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioPlayer::setLoopState(uint32_t loop_begin, uint32_t loop_length)
{
    if ( loop_begin == 0 && loop_length == 0 )
    {
        // ソースからループ位置を取得して設定する
		uint32_t begin, length;
		m_decoder->GetLoopState(&begin, &length);
        mLoopBegin  = begin;
		mLoopLength = length;
    }
    else
    {
        mLoopBegin = loop_begin;
		mLoopLength = loop_length;
    }
}

} // namespace Audio
} // namespace Lumino
