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

//==============================================================================
// Sound
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sound, Object);

//------------------------------------------------------------------------------
SoundPtr Sound::create(const Char* filePath)
{
	auto ptr = SoundPtr::makeRef();
	ptr->initialize(filePath);
	return ptr;
	//return CreateInternal(detail::AudioManager::getInstance(), filePath);
}

//------------------------------------------------------------------------------
SoundPtr Sound::create(Stream* stream, SoundLoadingMode loadingMode)
{
	auto ptr = SoundPtr::makeRef();
	ptr->initialize(stream, loadingMode);
	return ptr;
	//return CreateInternal(detail::AudioManager::getInstance(), stream, loadingMode);
}

////------------------------------------------------------------------------------
//SoundPtr Sound::CreateInternal(detail::AudioManager* manager, const StringRef& filePath)
//{
//}
//
////------------------------------------------------------------------------------
//SoundPtr Sound::CreateInternal(detail::AudioManager* manager, Stream* stream, SoundLoadingMode loadingMode)
//{
//}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
Sound::~Sound()
{
	LN_SAFE_RELEASE(m_audioStream);
	LN_SAFE_RELEASE(m_audioPlayer);
}

//------------------------------------------------------------------------------
void Sound::initialize(const StringRef& filePath)
{
	detail::AudioManager* manager = detail::AudioManager::getInstance();
	Ref<Stream> stream(manager->getFileManager()->createFileStream(filePath, true), false);
	Ref<detail::AudioStream> audioStream(manager->createAudioStream(stream, CacheKey(filePath), SoundLoadingMode::ASync), false);


	//manager->createSound(stream, CacheKey(PathName(filePath)), SoundLoadingMode::ASync)
	//return SoundPtr(, false);


	if (LN_CHECK_ARG(manager != nullptr)) return;
	if (LN_CHECK_ARG(stream != nullptr)) return;
	m_manager = manager;
	LN_REFOBJ_SET(m_audioStream, audioStream);
	m_manager->addSound(this);
}

//------------------------------------------------------------------------------
void Sound::initialize(Stream* stream, SoundLoadingMode loadingMode)
{
	detail::AudioManager* manager = detail::AudioManager::getInstance();
	//Ref<Stream> stream(manager->getFileManager()->createFileStream(filePath, true), false);
	Ref<detail::AudioStream> audioStream(manager->createAudioStream(stream, CacheKey::Null, SoundLoadingMode::ASync), false);

	//SoundPtr(manager->createSound(stream, CacheKey::Null, loadingMode), false);

	if (LN_CHECK_ARG(manager != nullptr)) return;
	if (LN_CHECK_ARG(stream != nullptr)) return;
	m_manager = manager;
	LN_REFOBJ_SET(m_audioStream, audioStream);

	if (loadingMode == SoundLoadingMode::Sync) {
		createAudioPlayerSync();
	}
	m_manager->addSound(this);
}

//------------------------------------------------------------------------------
void Sound::initialize(detail::AudioStream* audioStream)
{
	detail::AudioManager* manager = detail::AudioManager::getInstance();
	if (LN_CHECK_ARG(manager != nullptr)) return;
	if (LN_CHECK_ARG(audioStream != nullptr)) return;
	m_manager = manager;
	LN_REFOBJ_SET(m_audioStream, audioStream);
	m_manager->addSound(this);
}

//------------------------------------------------------------------------------
void Sound::setVolume(float volume)
{
	MutexScopedLock lock(m_playerStateLock);
	m_playerState.setVolume(Math::clamp(volume, 0.0f, 1.0f));
}

//------------------------------------------------------------------------------
float Sound::getVolume() const
{
	return m_playerState.getVolume();
}

//------------------------------------------------------------------------------
void Sound::setPitch(float pitch)
{
	MutexScopedLock lock(m_playerStateLock);
	m_playerState.setPitch(Math::clamp(pitch, 0.5f, 2.0f));
}

//------------------------------------------------------------------------------
float Sound::getPitch() const
{
	return m_playerState.getPitch();
}

//------------------------------------------------------------------------------
void Sound::setLoopEnabled(bool enabled)
{
	MutexScopedLock lock(m_playerStateLock);
	m_playerState.setLoopEnabled(enabled);
}

//------------------------------------------------------------------------------
bool Sound::isLoopEnabled() const
{
	return m_playerState.isLoopEnabled();
}

//------------------------------------------------------------------------------
void Sound::SetLoopRange(uint32_t begin, uint32_t length)
{
	MutexScopedLock lock(m_playerStateLock);
	m_playerState.setLoopBegin(begin);
	m_playerState.setLoopLength(length);
}

//------------------------------------------------------------------------------
void Sound::play()
{
	MutexScopedLock lock(m_playerStateLock);
	m_playerState.setPlayingState(SoundPlayingState::Playing);
}

//------------------------------------------------------------------------------
void Sound::stop()
{
	MutexScopedLock lock(m_playerStateLock);
	m_playerState.setPlayingState(SoundPlayingState::Stopped);
}

//------------------------------------------------------------------------------
void Sound::pause()
{
	MutexScopedLock lock(m_playerStateLock);
	m_playerState.setPlayingState(SoundPlayingState::Pausing);
}

//------------------------------------------------------------------------------
void Sound::Resume()
{
	MutexScopedLock lock(m_playerStateLock);
	if (m_playerState.getPlayingState() == SoundPlayingState::Pausing)
	{
		m_playerState.setPlayingState(SoundPlayingState::Playing);
	}
}

//------------------------------------------------------------------------------
void Sound::set3DEnabled(bool enabled)
{
	m_is3DSound = enabled;
}

//------------------------------------------------------------------------------
bool Sound::is3DEnabled() const
{
	return m_is3DSound;
}

//------------------------------------------------------------------------------
void Sound::setEmitterPosition(const Vector3& pos)
{
	m_position = pos;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setPosition(m_position);
	}
}

//------------------------------------------------------------------------------
const Vector3& Sound::getEmitterPosition() const
{
    return m_audioPlayer->getPosition();
}

//------------------------------------------------------------------------------
void Sound::setEmitterVelocity(const Vector3& v)
{
	m_velocity = v;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setVelocity(m_velocity);
	}
}

//------------------------------------------------------------------------------
const Vector3& Sound::getEmitterVelocity() const
{
	return m_velocity;
}

//------------------------------------------------------------------------------
void Sound::setEmitterMaxDistance(float distance)
{
	m_maxDistance = distance;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setEmitterDistance(m_maxDistance);
	}
}

//------------------------------------------------------------------------------
int64_t Sound::getTotalSamples() const
{
	// TODO: 生成完了まで待つべき？
	if (m_audioStream->checkCreated() && m_audioStream->getFormat() == StreamFormat_Midi) {
#ifdef LN_OS_WIN32
		if (m_audioPlayer != NULL) {
			return static_cast<detail::DirectMusicAudioPlayer*>(m_audioPlayer)->getTotalTime();
		}
		return 0;
#else
		LN_THROW(0, NotImplementedException);
#endif
	}
	if (m_audioStream->checkCreated()) {
		return m_audioStream->getDecoder()->getTotalUnits();
	}
	return 0;
}

//------------------------------------------------------------------------------
int64_t Sound::getPlayedSamples() const
{
	if (m_audioPlayer != NULL) {
		return m_audioPlayer->getPlayedSamples();
	}
	return 0;
}

//------------------------------------------------------------------------------
int Sound::getSamplingRate() const
{
	if (m_audioStream->checkCreated() && m_audioStream->getFormat() == StreamFormat_Midi) {
#ifdef LN_OS_WIN32
		return detail::DirectMusicManager::MusicTimeBase;
#else
		LN_THROW(0, NotImplementedException);
#endif
	}
	if (m_audioStream->checkCreated()) {
		return m_audioStream->getDecoder()->getWaveFormat()->samplesPerSec;
	}
	return 0;
}

//------------------------------------------------------------------------------
SoundPlayingState Sound::getPlayingState() const
{
	return m_playerState.getPlayingState();
}

//------------------------------------------------------------------------------
void Sound::SetPlayingMode(SoundPlayingMode mode)
{
	m_playingMode = mode;
}

//------------------------------------------------------------------------------
SoundPlayingMode Sound::getPlayingMode() const
{
	return m_playingMode;
}

//------------------------------------------------------------------------------
void Sound::fadeVolume(float targetVolume, double time, SoundFadeBehavior behavior)
{
	MutexScopedLock lock(m_playerStateLock);

	// 現在の音量から targetVolume への遷移
	targetVolume = Math::clamp(targetVolume, 0.0f, 1.0f);
	m_fadeValue.start(getVolume(), targetVolume, time);
	m_fadeBehavior = behavior;
	m_fading = true;
}

//------------------------------------------------------------------------------
bool Sound::isVolumeFading() const
{
	return m_fading;
}

//------------------------------------------------------------------------------
// 同期的な生成モード時、Manager から呼ばれる。
//------------------------------------------------------------------------------
void Sound::createAudioPlayerSync()
{
	if (LN_CHECK_STATE(m_audioPlayer == NULL && m_audioStream->checkCreated())) return;
	m_audioPlayer = m_manager->createAudioPlayer(m_audioStream, m_playingMode, m_is3DSound);
}

//------------------------------------------------------------------------------
void Sound::polling(float elapsedTime)
{
	// TODO: 例外catch
	
	// stream が準備できていれば Player を作成する
	if (m_audioPlayer == NULL && m_audioStream->checkCreated())
	{
		m_audioPlayer = m_manager->createAudioPlayer(m_audioStream, m_playingMode, m_is3DSound);
	}

	//-----------------------------------------------------------
	// ステートの同期

	// まず、AudioPlayer へ設定するステートを作る。
	// 直接設定しないのは、Mutex のロック時間を極力少なくするため。
	// ↓のスコープ内で時間のかかる処理は絶対NG。
	detail::AudioPlayerState newState;
	{
		MutexScopedLock lock(m_playerStateLock);

		// フェード中の場合 (m_playerState の Volume,PlayingState 上書き)
		if (m_fading)
		{
			m_fadeValue.advanceTime(elapsedTime);
			m_playerState.setVolume(m_fadeValue.getValue());

			// フェード完了
			if (m_fadeValue.isFinished())
			{
				m_fading = false;

				// フェード終了時の動作によって分岐
				switch (m_fadeBehavior)
				{
					// 継続する場合
				case SoundFadeBehavior::Continue:
					break;
					// 停止する場合
				case SoundFadeBehavior::stop:
				case SoundFadeBehavior::StopReset:
					m_playerState.setPlayingState(SoundPlayingState::Stopped);
					break;
					// 一時停止する場合
				case SoundFadeBehavior::pause:
				case SoundFadeBehavior::PauseReset:
					m_playerState.setPlayingState(SoundPlayingState::Pausing);
					break;
				}

				// 音量を元に戻す
				if (m_fadeBehavior == SoundFadeBehavior::StopReset || SoundFadeBehavior::StopReset == SoundFadeBehavior::PauseReset)
				{
					m_playerState.setVolume(m_fadeValue.getStartValue());
				}
			}
		}

		if (m_playerState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_Volume)
		{
			newState.setVolume(m_playerState.getVolume());
		}
		if (m_playerState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_Pitch)
		{
			newState.setPitch(m_playerState.getPitch());
		}
		if (m_playerState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopEnabled)
		{
			newState.setLoopEnabled(m_playerState.isLoopEnabled());
		}
		if (m_playerState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopBegin)
		{
			newState.setLoopBegin(m_playerState.getLoopBegin());
		}
		if (m_playerState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_LoopLength)
		{
			newState.setLoopLength(m_playerState.getLoopLength());
		}
		if (m_playerState.getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_PlayingState)
		{
			// 再生状態が Sound 側から変更されているので、Player へ設定する。
			// Player 側から変更されている場合も Sound 側の状態を上書きする。
			newState.setPlayingState(m_playerState.getPlayingState());
		}
		else if (m_audioPlayer != nullptr && m_audioPlayer->getPlayerState().getModifiedFlags() & detail::AudioPlayerState::ModifiedFlags_PlayingState)
		{
			// Player 側からのみ変更されている場合は Sound 側へ設定する。
			m_playerState.setPlayingState(m_audioPlayer->getPlayerState().getPlayingState());
			m_playerState.setModifiedFlags(m_playerState.getModifiedFlags() & ~detail::AudioPlayerState::ModifiedFlags_PlayingState);
		}

	}

	// 出来上がった newState を AudioPlayer へ送り込む
	if (m_audioPlayer != nullptr)
	{
		m_audioPlayer->commitPlayerState(newState);
		m_playerState.setModifiedFlags(detail::AudioPlayerState::ModifiedFlags_None);	// 同期完了

		// TODO: あとで playerstate に含めたい
		m_audioPlayer->setPosition(m_position);
		m_audioPlayer->setVelocity(m_velocity);
		m_audioPlayer->setEmitterDistance(m_maxDistance);

		m_audioPlayer->doPolling();
	}
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
