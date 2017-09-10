
#include "Internal.h"
#include <Lumino/Base/Exception.h>
#include <Lumino/Math/MathUtils.h>
#include "AudioStream.h"
#include "AudioDevice.h"
#include "AudioPlayer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// AudioPlayerState 
//==============================================================================
AudioPlayerState::AudioPlayerState()
	: m_modified(ModifiedFlags_None)
	, m_volume(1.0f)
	, m_pitch(1.0f)
	, m_loopEnabled(false)
	, m_loopBegin(0)
	, m_loopLength(0)
	, m_playingState(SoundPlayingState::Stopped)
{
}

//==============================================================================
// AudioPlayer 
//==============================================================================

//------------------------------------------------------------------------------
AudioPlayer::AudioPlayer(AudioDevice* device)
	: mDevice(device)
	, m_audioStream(NULL)
	, mVolume(1.0f)
	, mPitch(1.0f)
	, mLoopBegin(0)
	, mLoopLength(0)
	, mIsPlaying(false)
	, mIsPausing(false)
	, mIsLoop(false)
{
	LN_SAFE_ADDREF(mDevice);
}

//------------------------------------------------------------------------------
AudioPlayer::~AudioPlayer()
{
	LN_SAFE_RELEASE(m_audioStream);
	LN_SAFE_RELEASE(mDevice);
}

//------------------------------------------------------------------------------
void AudioPlayer::initialize(AudioStream* audioStream, bool enable3d)
{ 
	if (LN_REQUIRE(audioStream)) return;
	LN_REFOBJ_SET(m_audioStream, audioStream);
	m_decoder = m_audioStream->getDecoder();

    // ソースからループ位置を取得
    uint32_t loop_begin, loop_length;
	m_decoder->setLoopState(&loop_begin, &loop_length);
    mLoopBegin  = loop_begin;
	mLoopLength = loop_length;
}

//------------------------------------------------------------------------------
void AudioPlayer::commitPlayerState(const detail::AudioPlayerState& newState)
{
	if (newState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_Volume)
	{
		setVolume(newState.getVolume());
	}
	if (newState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_Pitch)
	{
		setPitch(newState.getPitch());
	}
	if (newState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopEnabled)
	{
		setLoopEnabled(newState.isLoopEnabled());
	}
	if ((newState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopBegin) ||
		(newState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopLength))
	{
		setLoopState(newState.getLoopBegin(), newState.getLoopLength());
	}
	if (newState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_PlayingState)
	{
		switch (newState.getPlayingState())
		{
		case SoundPlayingState::Stopped:
			stop();
			break;
		case SoundPlayingState::Playing:
			if (m_playerState.getPlayingState() == SoundPlayingState::Pausing)
			{
				pause(false);
			}
			else
			{
				play();
			}
			break;
		case SoundPlayingState::Pausing:
			pause(true);
			break;
		}
	}

	m_playerState = newState;
	m_playerState.setModifiedFlags(detail::AudioPlayerState::ModifiedFlags_None);	// 同期完了
}

//------------------------------------------------------------------------------
void AudioPlayer::doPolling()
{
	if (!polling())
	{
		m_playerState.setPlayingState(SoundPlayingState::Stopped);
	}
}

//------------------------------------------------------------------------------
void AudioPlayer::setVolume(float volume)
{
	mVolume = Math::clamp(volume, 0.0f, 1.0f);
}

//------------------------------------------------------------------------------
void AudioPlayer::setPitch(float pitch)
{
	mPitch = Math::clamp(pitch, 0.5f, 2.0f);
}

//------------------------------------------------------------------------------
void AudioPlayer::setLoopState(uint32_t loop_begin, uint32_t loop_length)
{
    if ( loop_begin == 0 && loop_length == 0 )
    {
        // ソースからループ位置を取得して設定する
		uint32_t begin, length;
		m_decoder->setLoopState(&begin, &length);
        mLoopBegin  = begin;
		mLoopLength = length;
    }
    else
    {
        mLoopBegin = loop_begin;
		mLoopLength = loop_length;
    }
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
