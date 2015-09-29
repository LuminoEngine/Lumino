
#include "Internal.h"
#include <Lumino/Audio/AudioManager.h>
#include <Lumino/Audio/Sound.h>
#include <Lumino/IO/FileStream.h>
#include "AudioStream.h"
#include "AudioPlayer.h"
#ifdef LN_OS_WIN32
#include "DirectMusic/DirectMusic.h"
#include "DirectMusic/DirectMusicAudioPlayer.h"
#endif

namespace Lumino
{
LN_NAMESPACE_AUDIO_BEGIN

//=============================================================================
// Sound
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(Sound, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound* Sound::Create(const TCHAR* filePath, AudioManagerImpl* manager)
{
	manager = (manager) ? manager : Internal::AudioManager;
	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath, true));

	//RefPtr<FileStream> stream(LN_NEW FileStream(filePath, FileOpenMode::Read | FileOpenMode::Deferring));
	return manager->CreateSound(stream, CacheKey(PathName(filePath)), SoundLoadingMode::ASync);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound* Sound::Create(Stream* stream, SoundLoadingMode loadingMode)
{
	return Internal::AudioManager->CreateSound(stream, CacheKey::Null, loadingMode);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound::Sound(AudioManagerImpl* manager, AudioStream* stream/*, SoundPlayType playerType, bool is3DSound*/)
	: m_manager(manager)
	, m_audioStream(stream)
	, m_audioPlayer(NULL)
	, m_playingMode(SoundPlayingMode::Unknown)
	, m_volume(1.0)
	, m_pitch(1.0)
	, m_loopEnabled(false)
	, m_loopBegin(0)
	, m_loopLength(0)
	, m_is3DSound(false)
	, m_position(0, 0, 0)
	, m_velocity(0, 0, 0)
	, m_maxDistance(100)
	, m_playState(SoundPlayingState::Stopped)
	, m_gameAudioFlags(0)
	, m_fadeValue()
	, m_fadeBehavior(SoundFadeBehavior::Continue)
	, m_fading(false)
	/*
	, mOrginalVolume(100)
	, mFadeState(SoundFadeState_NotFading)
	, mFlags(0)
	, mGroupFlags(0)
	, mIsFading(false)*/
{
	LN_SAFE_ADDREF(m_manager);
	LN_SAFE_ADDREF(m_audioStream);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound::~Sound()
{
	LN_SAFE_RELEASE(m_audioStream);
	LN_SAFE_RELEASE(m_audioPlayer);
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetVolume(float volume)
{
	Threading::MutexScopedLock lock(m_mutex);
	m_volume = Math::Clamp(volume, 0.0f, 1.0f);
	if (m_audioPlayer != NULL) {
		m_audioPlayer->SetVolume(m_volume);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Sound::GetVolume() const
{
	return m_volume;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetPitch(float pitch)
{
	m_pitch = Math::Clamp(pitch, 0.5f, 2.0f);
	if (m_audioPlayer != NULL) {
		m_audioPlayer->SetPitch(m_pitch);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Sound::GetPitch() const
{
	return m_pitch;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetLoopEnabled(bool enabled)
{
	m_loopEnabled = enabled;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->loop(enabled);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Sound::IsLoopEnabled() const
{
	return m_loopEnabled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetLoopRange(uint32_t begin, uint32_t length)
{
	m_loopBegin = begin;
	m_loopLength = length;

	if (m_audioPlayer != NULL) {
		m_audioPlayer->setLoopState(begin, length);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Play()
{
	Threading::MutexScopedLock lock(m_mutex);
	m_playState = SoundPlayingState::Playing;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->play();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Stop()
{
	Threading::MutexScopedLock lock(m_mutex);
	m_playState = SoundPlayingState::Stopped;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->stop();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Pause()
{
	Threading::MutexScopedLock lock(m_mutex);
	m_playState = SoundPlayingState::Pausing;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->pause(true);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Resume()
{
	Threading::MutexScopedLock lock(m_mutex);
	if (m_playState == SoundPlayingState::Pausing)
	{
		m_playState = SoundPlayingState::Playing;
		if (m_audioPlayer != NULL) {
			m_audioPlayer->pause(false);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Sound::fadeVolume( int targetVolume, int time, SoundFadeState state )
//{
//	// 即時更新
//	if ( time == 0 ) {
//		setVolume( targetVolume );
//	}
//
//	Threading::ScopedLock lock( mLock );
//
//	// 現在の音量から volume_ へのフェード
//    mFadeValue.start( static_cast< int >( m_audioPlayer->getVolume() ), targetVolume, static_cast< double >( time ) * 0.001 );	
//	mFadeState = state;
//	mIsFading = true;
//}
//


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Set3DEnabled(bool enabled)
{
	m_is3DSound = enabled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Sound::Is3DEnabled() const
{
	return m_is3DSound;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetEmitterPosition(const Vector3& pos)
{
	m_position = pos;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setPosition(m_position);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector3& Sound::GetEmitterPosition() const
{
    return m_audioPlayer->getPosition();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetEmitterVelocity(const Vector3& v)
{
	m_velocity = v;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setPosition(m_velocity);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector3& Sound::GetEmitterVelocity() const
{
	return m_velocity;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetEmitterMaxDistance(float distance)
{
	m_maxDistance = distance;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setEmitterDistance(m_maxDistance);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int64_t Sound::GetTotalSamples() const
{
	// TODO: 生成完了まで待つべき？
	if (m_audioStream->CheckCreated() && m_audioStream->GetFormat() == StreamFormat_Midi) {
#ifdef LN_OS_WIN32
		if (m_audioPlayer != NULL) {
			return static_cast<DirectMusicAudioPlayer*>(m_audioPlayer)->getTotalTime();
		}
		return 0;
#else
		LN_THROW(0, NotImplementedException);
#endif
	}
	if (m_audioStream->CheckCreated()) {
		return m_audioStream->GetDecoder()->GetTotalUnits();
	}
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int64_t Sound::GetPlayedSamples() const
{
	if (m_audioPlayer != NULL) {
		return m_audioPlayer->getPlayedSamples();
	}
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Sound::GetSamplingRate() const
{
	if (m_audioStream->CheckCreated() && m_audioStream->GetFormat() == StreamFormat_Midi) {
#ifdef LN_OS_WIN32
		return DirectMusicManager::MusicTimeBase;
#else
		LN_THROW(0, NotImplementedException);
#endif
	}
	if (m_audioStream->CheckCreated()) {
		return m_audioStream->GetDecoder()->GetWaveFormat()->SamplesPerSec;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SoundPlayingState Sound::GetPlayingState() const
{
	return m_playState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetPlayingMode(SoundPlayingMode mode)
{
	m_playingMode = mode;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SoundPlayingMode Sound::GetPlayingMode() const
{
	return m_playingMode;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::FadeVolume(float targetVolume, double time, SoundFadeBehavior behavior)
{
	// 即時更新
	if (time <= 0) {
		SetVolume(targetVolume);
	}

	Threading::MutexScopedLock lock(m_mutex);

	// 現在の音量から volume_ へのフェード
	targetVolume = Math::Clamp(targetVolume, 0.0f, 1.0f);
	m_fadeValue.Start(GetVolume(), targetVolume, time);
	m_fadeBehavior = behavior;
	m_fading = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Sound::IsVolumeFading() const
{
	return m_fading;
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Sound::setGroupFlag( lnU32 flag )
//{
//	Threading::ScopedLock lock( mLock );
//    mGroupFlags = flag;
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//lnU32 Sound::getGroupFlag()
//{
//	Threading::ScopedLock lock( mLock );
//    return mGroupFlags;
//}

//-----------------------------------------------------------------------------
// 同期的な生成モード時、MAnager から呼ばれる。
//-----------------------------------------------------------------------------
void Sound::CreateAudioPlayerSync()
{
	LN_CHECK_STATE(m_audioPlayer == NULL && m_audioStream->CheckCreated());
	m_audioPlayer = m_manager->CreateAudioPlayer(m_audioStream, m_playingMode, m_is3DSound);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Polling(float elapsedTime)
{
	// stream が準備できていれば Player を作成する
	if (m_audioPlayer == NULL && m_audioStream->CheckCreated())
	{
		m_audioPlayer = m_manager->CreateAudioPlayer(m_audioStream, m_playingMode, m_is3DSound);
		m_audioPlayer->SetVolume(m_volume);
		m_audioPlayer->SetPitch(m_pitch);
		m_audioPlayer->loop(m_loopEnabled);
		m_audioPlayer->setLoopState(m_loopBegin, m_loopLength);
		m_audioPlayer->setPosition(m_position);
		m_audioPlayer->setVelocity(m_velocity);
		m_audioPlayer->setEmitterDistance(m_maxDistance);
		switch (m_playState)
		{
		default:
		case SoundPlayingState::Stopped:
			break;
		case SoundPlayingState::Playing:
			m_audioPlayer->play();
			break;
		case SoundPlayingState::Pausing:
			m_audioPlayer->play();
			m_audioPlayer->pause(true);
			break;
		}
	}

	if (m_audioPlayer != NULL)
	{
		if (!m_audioPlayer->polling()) {
			m_playState = SoundPlayingState::Stopped;
		}
	}

	Threading::MutexScopedLock lock(m_mutex);

    // フェード中の場合
	if (m_fading)
	{
		m_fadeValue.AdvanceTime(elapsedTime);

		if (m_audioPlayer != NULL) {
			m_volume = m_fadeValue.GetValue();
			m_audioPlayer->SetVolume(m_volume);
		}

		// フェード完了
		if (m_fadeValue.IsFinished())
		{
			m_fading = false;

			// フェード終了時の動作によって分岐
			switch (m_fadeBehavior)
			{
			// 継続する場合
			case SoundFadeBehavior::Continue:
				break;
			// 停止する場合
			case SoundFadeBehavior::Stop:
			case SoundFadeBehavior::StopReset:
				if (m_audioPlayer != NULL) {
					m_audioPlayer->stop();
				}
				m_playState = SoundPlayingState::Stopped;
				break;
			// 一時停止する場合
			case SoundFadeBehavior::Pause:
			case SoundFadeBehavior::PauseReset:
				if (m_audioPlayer != NULL) {
					m_audioPlayer->pause(true);
				}
				m_playState = SoundPlayingState::Pausing;
				break;
			}

			// 音量を元に戻す
			if (m_fadeBehavior == SoundFadeBehavior::StopReset || SoundFadeBehavior::StopReset == SoundFadeBehavior::PauseReset)
			{
				m_volume = m_fadeValue.GetStartValue();	// SetVolume() を呼ぶとデッドロックするのでここでセット
				if (m_audioPlayer != NULL) {
					m_audioPlayer->SetVolume(m_volume);
				}

			}
		}
	}
}

LN_NAMESPACE_AUDIO_END
} // namespace Lumino
