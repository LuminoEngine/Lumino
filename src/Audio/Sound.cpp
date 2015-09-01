
#include <Lumino/Audio/AudioManager.h>
#include <Lumino/Audio/Sound.h>
#include <Lumino/IO/FileStream.h>
#include "Internal.h"
#include "AudioStream.h"
#include "AudioPlayer.h"

namespace Lumino
{
namespace Audio
{

//=============================================================================
// Sound
//=============================================================================

//LN_TYPE_INFO_ACCESS_IMPL(Sound);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound* Sound::Create(const TCHAR* filePath, SoundPlayType playerType, bool enable3D, AudioManager* manager)
{
	manager = (manager) ? manager : Internal::Manager;

	RefPtr<FileStream> stream(LN_NEW FileStream(filePath, FileOpenMode::Read | FileOpenMode::Deferring));
	return manager->CreateSound(stream, playerType, enable3D, CacheKey(PathName(filePath)));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound::Sound(AudioManager* manager, AudioStream* stream, SoundPlayType playerType, bool is3DSound)
	: m_manager(manager)
	, m_audioStream(stream)
	, m_audioPlayer(NULL)
	, m_playerType(playerType)
	, m_volume(100)
	, m_pitch(100)
	, m_loopEnabled(false)
	, m_loopBegin(0)
	, m_loopLength(0)
	, m_is3DSound(is3DSound)
	, m_position(0, 0, 0)
	, m_velocity(0, 0, 0)
	, m_maxDistance(0)
	, m_playState(SoundPlayState_Stopped)
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
void Sound::SetVolume(int volume)
{
	m_volume = volume;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->SetVolume(m_volume);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Sound::GetVolume() const
{
	return m_volume;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetPitch(int pitch)
{
	m_pitch = pitch;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->SetPitch(m_pitch);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Sound::GetPitch() const
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
bool Sound::IsLoop() const
{
	return m_loopEnabled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Play()
{
	m_playState = SoundPlayState_Playing;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->play();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Stop()
{
	m_playState = SoundPlayState_Stopped;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->play();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Pause()
{
	m_playState = SoundPlayState_Pausing;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->pause(true);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::Resume()
{
	if (m_playState == SoundPlayState_Pausing)
	{
		m_playState = SoundPlayState_Playing;
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
bool Sound::Is3DSound() const
{
	return m_is3DSound;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetPosition(const Vector3& pos)
{
	m_position = pos;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setPosition(m_position);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector3& Sound::GetPosition() const
{
    return m_audioPlayer->getPosition();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetVelocity(const Vector3& v)
{
	m_velocity = v;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setPosition(m_velocity);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector3& Sound::GetVelocity() const
{
	return m_velocity;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sound::SetMaxDistance(float distance)
{
	m_maxDistance = distance;
	if (m_audioPlayer != NULL) {
		m_audioPlayer->setEmitterDistance(m_maxDistance);
	}
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
//
//-----------------------------------------------------------------------------
void Sound::Polling(float elapsedTime)
{
	// stream が準備できていれば Player を作成する
	if (m_audioPlayer == NULL && m_audioStream->CheckCreated())
	{
		m_audioPlayer = m_manager->CreateAudioPlayer(m_audioStream, m_playerType, m_is3DSound);
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
		case Lumino::Audio::SoundPlayState_Stopped:
			break;
		case Lumino::Audio::SoundPlayState_Playing:
			m_audioPlayer->play();
			break;
		case Lumino::Audio::SoundPlayState_Pausing:
			m_audioPlayer->play();
			m_audioPlayer->pause(true);
			break;
		}
	}

	if (m_audioPlayer != NULL)
	{
		m_audioPlayer->polling();
	}


	//Threading::ScopedLock lock( mLock );
 //   bool r = m_audioPlayer->polling();

 //   // フェード中の場合
	//if ( mIsFading )
	//{
	//	mFadeValue.advanceTime( elapsedTime );
	//		
	//	m_audioPlayer->setVolume( mFadeValue.getValue() );

	//	// フェード完了
	//	if ( mFadeValue.isFinished() )
	//	{
	//		mIsFading = false; 

	//		// フェード終了時の動作によって分岐
	//		switch ( mFadeState )
	//		{
	//			// 継続する場合
	//			case SOUNDFADE_CONTINUE:
	//				break;
	//			// 停止する場合
	//			case SOUNDFADE_STOP:
	//				m_audioPlayer->stop();
	//				break;
	//			// 一時停止する場合
	//			case SOUNDFADE_PAUSE:
	//				m_audioPlayer->pause( true );
	//				break;
	//			// 停止して音量を戻す場合
	//			case SOUNDFADE_STOP_RESET:
	//				m_audioPlayer->stop();
	//				m_audioPlayer->setVolume( mFadeValue.getStartValue() );
	//				break;
	//			// 一時停止して音量を戻す場合
	//			case SOUNDFADE_PAUSE_RESET:
	//				m_audioPlayer->pause( true );
	//				m_audioPlayer->setVolume( mFadeValue.getStartValue() );
	//				break;
	//		}
	//		mFadeState = SoundFadeState_NotFading;
	//	}
	//}

    //return r;
}

} // namespace Audio
} // namespace Lumino
