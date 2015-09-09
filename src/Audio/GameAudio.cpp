
#include "../Internal.h"
#include <Lumino/Audio/AudioManager.h>
#include <Lumino/Audio/Sound.h>
#include <Lumino/Audio/GameAudio.h>
#include "AudioHelper.h"

namespace Lumino
{
namespace Audio
{

enum GameAudioFlags
{
	GameAudioFlags_SE = 0x01,
};

//=============================================================================
// GameAudio
//=============================================================================
#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GameAudio::GameAudio(AudioManager* mamager)
	: mManager(mamager)
	, mBGM(NULL)
	, mBGS(NULL)
	, mME(NULL)
	, mBGMVolume(0)
	, mBGMPitch(0)
	, mBGSVolume(0)
	, mBGSPitch(0)
	, mBGMFadeOutTime(500)
	, mBGMFadeInTime(1000)
	, mMEPlaying(false)
	, mBGMRestart(false)
	, mBGSRestart(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GameAudio::~GameAudio()
{
	StopBGM(0);
	StopBGS(0);
	StopME();
	StopSE();

	// 再生終了後の解放リストに入っているサウンドを解放
	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end; ++itr)
	{
		(*itr)->Release();
	}
	mReleaseAtPlayEndList.clear();

	LN_SAFE_RELEASE(mBGM);
	LN_SAFE_RELEASE(mBGS);
	LN_SAFE_RELEASE(mME);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayBGM( const TCHAR* filePath, int volume, int pitch, int fadeTime )
{
	// 演奏再開チェック
	//if ( !mBGMRestart && mBGM )
	//{
	//	if ( mBGMName == filename_ )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> sound(Sound::Create(filePath, mManager));
	//mManager->createSound( filePath, SOUNDPLAYTYPE_STREAMING, false ) );

	PlayBGMFromSound(sound, volume, pitch, fadeTime);

	// ファイル名記憶
	mBGMName = filePath;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayBGMFromSound( Sound* sound, int volume, int pitch, int fadeTime )
{
	LN_CHECK_ARGS(sound != NULL);

	// 演奏再開チェック
	// TODO:フェードアウト中に再開すると無音になる
	//if ( !mBGMRestart && mBGM )
	//{
	//	if ( sound_ == mBGM && volume_ == mBGMVolume && pitch_ == mBGMPitch )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> prev_bgm( mBGM, false );

	{
		Threading::MutexScopedLock lock(mLock);

		mBGM = sound;
		LN_SAFE_ADDREF( mBGM );

		mBGM->SetPitch( pitch );
		mBGM->SetLoopEnabled( true );

		mBGMVolume = volume;
		mBGMPitch  = pitch;

		// ME が再生中ではないかを調べる
		bool me_not_play = true;	
		if ( mME )
		{
			SoundPlayState state = mME->GetSoundPlayState();
			if (state == SoundPlayState_Playing)
			{
				me_not_play = false;
			}
		}

		// ME 再生中ではない場合
		if ( me_not_play )
		{
			if ( fadeTime > 0 )
			{
				mBGM->SetVolume( 0 );
				mBGM->Play();
				mBGM->FadeVolume( volume, fadeTime, SOUNDFADE_CONTINUE );
			}
			else
			{
				mBGM->SetVolume( volume );
				mBGM->Play();
			}
		}
		// ME 再生中の場合は、一度再生するけどすぐに一時停止する ( ME 終了後に再開 )
		else
		{
			mBGM->Play();
			mBGM->Pause();
		}
	}


	// フェード時間がある場合
	if ( fadeTime > 0 )
	{
		if ( prev_bgm != NULL )
		{
			// ひとつ前のBGMは、fade_time_ 後に停止、解放するようにする
			prev_bgm->SetLoopEnabled( false );
			prev_bgm->FadeVolume( 0, fadeTime, SOUNDFADE_STOP_RESET );
			PushReleaseAtPlayEndList( prev_bgm );
		}
	}
	// フェード時間がない場合はすぐに停止
	else
	{
		if (prev_bgm != NULL)
		{
			prev_bgm->Stop();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::StopBGM( int fadeTime )
{
	if ( mBGM )
	{
		if ( fadeTime > 0 )
		{
			// フェード終了後に停止して、音量等を元に戻す
			mBGM->FadeVolume( 0, fadeTime, SOUNDFADE_STOP_RESET );
		}
		else
		{
			mBGM->Stop();
			LN_SAFE_RELEASE( mBGM );
		}

		mBGMName = _T("");
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayBGS( const TCHAR* filePath, int volume, int pitch, int fadeTime )
{
	// 演奏再開チェック
	//if ( !mBGSRestart && mBGS )
	//{
	//	if ( mBGSName == filename_ )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> sound(Sound::Create(filePath, mManager));

	PlayBGSFromSound( sound, volume, pitch, fadeTime );
		
	// ファイル名記憶
	mBGSName = filePath;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayBGSFromSound( Sound* sound, int volume, int pitch, int fadeTime )
{
	// 演奏再開チェック
	//if ( !mBGSRestart && mBGS )
	//{
	//	if ( sound_ == mBGS && volume_ == mBGSVolume && pitch_ == mBGSPitch )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> prev_bgs( mBGS, false );

	{
		Threading::MutexScopedLock lock(mLock);

		mBGS = sound;
		LN_SAFE_ADDREF( mBGS );

		mBGS->SetPitch( pitch );
		mBGS->SetLoopEnabled( true );

		mBGSVolume = volume;
		mBGSPitch  = pitch;

		if ( fadeTime > 0 )
		{
			mBGS->SetVolume( 0 );
			mBGS->Play();
			mBGS->FadeVolume( volume, fadeTime, SOUNDFADE_CONTINUE );
		}
		else
		{
			mBGS->SetVolume( volume );
			mBGS->Play();
		}
	}

	// フェード時間がある場合
	if ( fadeTime > 0 )
	{
		if ( prev_bgs != NULL )
		{
			// ひとつ前のBGSは、fade_time_ 後に停止、解放するようにする
			prev_bgs->SetLoopEnabled( false );
			prev_bgs->FadeVolume( 0, fadeTime, SOUNDFADE_STOP_RESET );
			PushReleaseAtPlayEndList( prev_bgs );
		}
	}
	// フェード時間がない場合はすぐに停止
	else
	{
		if (prev_bgs != NULL)
		{
			prev_bgs->Stop();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::StopBGS( int fadeTime )
{
	if ( mBGS )
	{
		if ( fadeTime > 0 )
		{
			// フェード終了後に停止して、音量等を元に戻す
			mBGS->FadeVolume( 0, fadeTime, SOUNDFADE_STOP_RESET );
		}
		else
		{
			mBGS->Stop();
		}

		mBGSName = _T("");
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayME( const TCHAR* filePath, int volume, int pitch )
{	   
	RefPtr<Sound> sound(Sound::Create(filePath, mManager));
	PlayMEFromSound( sound, volume, pitch );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayMEFromSound( Sound* sound, int volume, int pitch )
{
	StopME();

	mME = sound;
	LN_SAFE_ADDREF( mME );

	mME->SetVolume( volume );
	mME->SetPitch( pitch );
	mME->SetLoopEnabled( false );

	bool flag = false;  // BGM があり、再生されているかを示すフラグ

	// BGM がある場合
	if ( mBGM )
	{
		SoundPlayState state = mBGM->GetSoundPlayState();

		// 再生されている場合
		if (state == SoundPlayState_Playing)
		{
			flag = true;
		}
	}

	// BGM が再生中の場合
	if ( flag )
	{
		// フェードアウト時間がある場合
		if ( mBGMFadeOutTime > 0 )
		{
			// フェードアウト後、一時停止する
			mBGM->FadeVolume( 0, mBGMFadeOutTime, SOUNDFADE_PAUSE );
		}
		// フェードアウト時間がない場合
		else
		{
			// すぐに一時停止
			mBGM->Pause(  );
		}
	}
	// BGM がない場合はすぐ演奏開始
	else
	{
		mME->Play();
		mMEPlaying = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::StopME()
{
	if ( mME )
	{
		mME->Stop();
		LN_SAFE_RELEASE( mME );
			
		// BGM があって、一時停止中の場合は再開
		if ( mBGM )
		{
			SoundPlayState state = mBGM->GetSoundPlayState();

			if (state == SoundPlayState_Playing)
			{
				mBGM->FadeVolume( mBGMVolume, mBGMFadeInTime, SOUNDFADE_CONTINUE );
				mBGM->Resume;
			}
		}

		mMEPlaying = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlaySE( const TCHAR* filePath, int volume, int pitch )
{
	RefPtr<Sound> sound(Sound::Create(filePath, mManager));
	sound->SetLoadingType(SoundLoadingType::OnMemory);

	// ボリューム・ピッチ設定
	sound->SetVolume( volume );
	sound->SetPitch( pitch );

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	AudioHelper::SetGameAudioFlags(sound, GameAudioFlags_SE);
	PushReleaseAtPlayEndList( sound );

	// 再生
	sound->SetLoopEnabled( false );
	sound->Play();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlaySE3D( const TCHAR* filePath, const Vector3& position, float distance, int volume, int pitch )
{
	// サウンド作成
	RefPtr<Sound> sound(Sound::Create(filePath, mManager));
	sound->SetLoadingType(SoundLoadingType::OnMemory);
	sound->Set3DEnabled(true);

	// 位置・ボリューム・ピッチ設定
	sound->SetPosition( position );
	sound->SetMaxDistance( distance );
	sound->SetVolume( volume );
	sound->SetPitch( pitch );

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	AudioHelper::SetGameAudioFlags(sound, GameAudioFlags_SE);
	PushReleaseAtPlayEndList( sound );
		
	// 再生
	sound->SetLoopEnabled( false );
	sound->Play();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlaySEFromSound( Sound* srcSound, int volume, int pitch )
{
	// 受け取った Sound が持っているソースをもとに新しい Sound を作成
	RefPtr<Sound> sound(AudioHelper::CreateSound(mManager, AudioHelper::GetAudioStream(srcSound)));
	sound->SetLoadingType(SoundLoadingType::OnMemory);
	sound->Set3DEnabled(srcSound->Is3DEnabled());

	// 位置・ボリューム・ピッチ設定
	sound->SetVolume( volume );
	sound->SetPitch( pitch );
	if (srcSound->Is3DEnabled())
	{
		sound->SetPosition( srcSound->GetPosition() );
	}

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	AudioHelper::SetGameAudioFlags(sound, GameAudioFlags_SE);
	PushReleaseAtPlayEndList( sound );
		
	// 再生
	sound->SetLoopEnabled( false );
	sound->Play();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::StopSE()
{
	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end;)
	{
		if (AudioHelper::GetGameAudioFlags(*itr) & GameAudioFlags_SE)
		{
			(*itr)->Stop();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::SetMEFadeState( int begin, int end )
{
	if ( begin >= 0 )
	{
		mBGMFadeOutTime = begin;
	}
	if ( end >= 0 )
	{
		mBGMFadeInTime = end;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::SetBGMVolume( int volume, int fadeTime )
{
	Threading::MutexScopedLock lock(mLock );

	// GameAudio 内では SOUNDFADE_STOP_RESET == フェードアウト中
	if ( mBGM && mBGM->getFadeState() != SOUNDFADE_STOP_RESET )
	{
		mBGMVolume = volume;
		mBGM->FadeVolume( volume, fadeTime, SOUNDFADE_CONTINUE );
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::SetBGSVolume( int volume, int fadeTime )
{
	Threading::MutexScopedLock lock(mLock);

	// GameAudio 内では SOUNDFADE_STOP_RESET == フェードアウト中
	if ( mBGS && mBGS->getFadeState() != SOUNDFADE_STOP_RESET )
	{
		mBGSVolume = volume;
		mBGS->FadeVolume( volume, fadeTime, SOUNDFADE_CONTINUE );
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//Sound* GameAudio::GetInternalGameSound( InternalGameSound type )
//{
//	switch ( type )
//	{
//		case InternalGameSound_BGM:
//			return mBGM;
//		case InternalGameSound_BGS:
//			return mBGS;
//		case InternalGameSound_ME:
//			return mME;
//	}
//	return NULL;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::Polling()
{
	Threading::MutexScopedLock lock(mLock);

	// 演奏する ME がある場合
	if ( mME )
	{
		SoundPlayState mestate = mME->GetSoundPlayState();

		// BGM がある場合
		if ( mBGM )
		{
			SoundPlayState bgmstate = mBGM->GetSoundPlayState();

			// BGMのフェードアウトが終わって一時停止状態になっている場合
			if (bgmstate == SoundPlayState_Pausing)
			{
				// ME 再生開始
				if ( !mMEPlaying )
				{
					mME->SetLoopEnabled( false );
					mME->Play();
					mMEPlaying = true;
				}
				// ME の再生が終了した場合
				else if (mestate != SoundPlayState_Playing)
				{
					// ME 再生中に BGM がストップしたとかで解放されている場合はなにもしない
					if ( mBGM )
					{
						mBGM->FadeVolume( mBGMVolume, mBGMFadeInTime, SOUNDFADE_CONTINUE );
						mBGM->Resume();
					}
					LN_SAFE_RELEASE( mME );
					mMEPlaying = false;
				}
			}
		}
		// BGM がない場合
		else
		{
			// ME が終了した場合
			if (mestate != SoundPlayState_Playing)
			{
				LN_SAFE_RELEASE( mME );
				mMEPlaying = false;
			}
		}
	}

	/// 再生終了している音声はリストから外して解放する
	
	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for ( ; itr != end;  )
	{
		SoundPlayState state = (*itr)->GetSoundPlayState;
		if (state != SoundPlayState_Playing)
		{
			(*itr)->Release();
			itr = mReleaseAtPlayEndList.erase( itr );
			end = mReleaseAtPlayEndList.end();
		}
		else
		{
			++itr;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PushReleaseAtPlayEndList( Sound* sound )
{
	if ( sound )
	{
		Threading::MutexScopedLock lock( mLock );
		mReleaseAtPlayEndList.push_back( sound );
		LN_SAFE_ADDREF( sound );
	}
}
#endif
} // namespace Audio
} // namespace Lumino
