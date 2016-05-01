/*
	[2016/5/1] フェード時の Volume は、Sound::Volume を直接変更する？
		した方が良いと思う。
		フェードの開始値は現在の Volume 値にした方が、
		フェード中の値からさらにフェードを開始するときにやり易い。
*/
#include "Internal.h"
#include <Lumino/Audio/Sound.h>
#include <Lumino/IO/FileStream.h>
#include "AudioManager.h"
#include "AudioStream.h"
#include "AudioPlayer.h"
#ifdef LN_OS_WIN32
#include "DirectMusic/DirectMusic.h"
#include "DirectMusic/DirectMusicAudioPlayer.h"
#endif

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

//=============================================================================
// Sound
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sound, tr::ReflectionObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SoundPtr Sound::Create(const TCHAR* filePath)
{
	return CreateInternal(Internal::AudioManager, filePath);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SoundPtr Sound::Create(Stream* stream, SoundLoadingMode loadingMode)
{
	return CreateInternal(Internal::AudioManager, stream, loadingMode);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SoundPtr Sound::CreateInternal(AudioManagerImpl* manager, const StringRef& filePath)
{
	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath, true), false);
	return SoundPtr(manager->CreateSound(stream, CacheKey(PathName(filePath)), SoundLoadingMode::ASync), false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SoundPtr Sound::CreateInternal(AudioManagerImpl* manager, Stream* stream, SoundLoadingMode loadingMode)
{
	return SoundPtr(manager->CreateSound(stream, CacheKey::Null, loadingMode), false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound::Sound()
	: m_manager(nullptr)
	, m_audioStream(nullptr)
	, m_audioPlayer(nullptr)
	, m_playingMode(SoundPlayingMode::Unknown)
	, m_is3DSound(false)
	, m_position(0, 0, 0)
	, m_velocity(0, 0, 0)
	, m_maxDistance(100)
	, m_gameAudioFlags(0)
	, m_fadeValue()
	, m_fadeBehavior(SoundFadeBehavior::Continue)
	, m_fading(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound::~Sound()
{
	LN_SAFE_RELEASE(m_audioStream);
	LN_SAFE_RELEASE(m_audioPlayer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Initialize(AudioManagerImpl* manager, AudioStream* stream)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	LN_CHECK_ARGS_RETURN(stream != nullptr);
	m_manager = manager;
	LN_REFOBJ_SET(m_audioStream, stream);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetVolume(float volume)
{
	Threading::MutexScopedLock lock(m_playerStateLock);
	m_playerState.SetVolume(Math::Clamp(volume, 0.0f, 1.0f));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Sound::GetVolume() const
{
	return m_playerState.GetVolume();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetPitch(float pitch)
{
	Threading::MutexScopedLock lock(m_playerStateLock);
	m_playerState.SetPitch(Math::Clamp(pitch, 0.5f, 2.0f));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Sound::GetPitch() const
{
	return m_playerState.GetPitch();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetLoopEnabled(bool enabled)
{
	Threading::MutexScopedLock lock(m_playerStateLock);
	m_playerState.SetLoopEnabled(enabled);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Sound::IsLoopEnabled() const
{
	return m_playerState.IsLoopEnabled();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetLoopRange(uint32_t begin, uint32_t length)
{
	Threading::MutexScopedLock lock(m_playerStateLock);
	m_playerState.SetLoopBegin(begin);
	m_playerState.SetLoopLength(length);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Play()
{
	Threading::MutexScopedLock lock(m_playerStateLock);
	m_playerState.SetPlayingState(SoundPlayingState::Playing);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Stop()
{
	Threading::MutexScopedLock lock(m_playerStateLock);
	m_playerState.SetPlayingState(SoundPlayingState::Stopped);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Pause()
{
	Threading::MutexScopedLock lock(m_playerStateLock);
	m_playerState.SetPlayingState(SoundPlayingState::Pausing);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Resume()
{
	Threading::MutexScopedLock lock(m_playerStateLock);
	if (m_playerState.GetPlayingState() == SoundPlayingState::Pausing)
	{
		m_playerState.SetPlayingState(SoundPlayingState::Playing);
	}
}

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
		m_audioPlayer->setVelocity(m_velocity);
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
		return m_audioPlayer->GetPlayedSamples();
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
		return m_audioStream->GetDecoder()->GetWaveFormat()->samplesPerSec;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SoundPlayingState Sound::GetPlayingState() const
{
	return m_playerState.GetPlayingState();
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
	//if (time <= 0) {
	//	SetVolume(targetVolume);
	//}

	Threading::MutexScopedLock lock(m_playerStateLock);

	// 現在の音量から targetVolume への遷移
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

//-----------------------------------------------------------------------------
// 同期的な生成モード時、Manager から呼ばれる。
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
	}

	//-----------------------------------------------------------
	// ステートの同期

	// まず、AudioPlayer へ設定するステートを作る。
	// 直接設定しないのは、Mutex のロック時間を極力少なくするため。
	// ↓のスコープ内で時間のかかる処理は絶対NG。
	detail::AudioPlayerState newState;
	{
		Threading::MutexScopedLock lock(m_playerStateLock);

		// フェード中の場合 (m_playerState の Volume,PlayingState 上書き)
		if (m_fading)
		{
			m_fadeValue.AdvanceTime(elapsedTime);
			m_playerState.SetVolume(m_fadeValue.GetValue());

			//if (m_audioPlayer != NULL)
			//{
			//	m_volume = m_fadeValue.GetValue();
			//	m_audioPlayer->SetVolume(m_volume);
			//}

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
					m_playerState.SetPlayingState(SoundPlayingState::Stopped);
					break;
					// 一時停止する場合
				case SoundFadeBehavior::Pause:
				case SoundFadeBehavior::PauseReset:
					m_playerState.SetPlayingState(SoundPlayingState::Pausing);
					break;
				}

				// 音量を元に戻す
				if (m_fadeBehavior == SoundFadeBehavior::StopReset || SoundFadeBehavior::StopReset == SoundFadeBehavior::PauseReset)
				{
					m_playerState.SetVolume(m_fadeValue.GetStartValue());

					//m_volume = m_fadeValue.GetStartValue();	// SetVolume() を呼ぶとデッドロックするのでここでセット
					//if (m_audioPlayer != NULL) {
					//	m_audioPlayer->SetVolume(m_volume);
					//}

				}
			}
		}

		if (m_playerState.GetModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_Volume)
		{
			newState.SetVolume(m_playerState.GetVolume());
		}
		if (m_playerState.GetModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_Pitch)
		{
			newState.SetPitch(m_playerState.GetPitch());
		}
		if (m_playerState.GetModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopEnabled)
		{
			newState.SetLoopEnabled(m_playerState.IsLoopEnabled());
		}
		if (m_playerState.GetModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopBegin)
		{
			newState.SetLoopBegin(m_playerState.GetLoopBegin());
		}
		if (m_playerState.GetModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopLength)
		{
			newState.SetLoopLength(m_playerState.GetLoopLength());
		}
		if (m_playerState.GetModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_PlayingState)
		{
			// 再生状態が Sound 側から変更されているので、Player へ設定する。
			// Player 側から変更されている場合も Sound 側の状態を上書きする。
			newState.SetPlayingState(m_playerState.GetPlayingState());
		}
		else if (m_audioPlayer != nullptr && m_audioPlayer->GetPlayerState().GetModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_PlayingState)
		{
			// Player 側からのみ変更されている場合は Sound 側へ設定する。
			m_playerState.SetPlayingState(m_audioPlayer->GetPlayerState().GetPlayingState());
			m_playerState.SetModifiedFlags(m_playerState.GetModifiedFlags() & ~detail::AudioPlayerState::ModifiedFlags_PlayingState);
		}

	}

	if (m_audioPlayer != nullptr)
	{
		// AudioPlayer へ送り込む
		m_audioPlayer->CommitPlayerState(newState);
		m_playerState.SetModifiedFlags(detail::AudioPlayerState::ModifiedFlags_None);	// 同期完了

		// TODO: あとで playerstate に含めたい
		m_audioPlayer->setPosition(m_position);
		m_audioPlayer->setVelocity(m_velocity);
		m_audioPlayer->setEmitterDistance(m_maxDistance);

		m_audioPlayer->DoPolling();
	}
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
