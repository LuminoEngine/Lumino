
#include "../Internal.h"
#include <Lumino/Audio/Sound.h>
#include "AudioManager.h"
#include "GameAudioImpl.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

enum GameAudioFlags
{
	GameAudioFlags_SE = 0x01,
};

//==============================================================================
// GameAudioImpl
//==============================================================================

//------------------------------------------------------------------------------
GameAudioImpl::GameAudioImpl(AudioManager* mamager)
	: mManager(mamager)
	, mBGM(NULL)
	, mBGS(NULL)
	, mME(NULL)
	, mBGMVolume(0)
	, mBGMPitch(0)
	, mBGSVolume(0)
	, mBGSPitch(0)
	, mBGMFadeOutTime(0.5)
	, mBGMFadeInTime(1.0)
	, mMEPlaying(false)
	, mBGMRestart(false)
	, mBGSRestart(false)
{
}

//------------------------------------------------------------------------------
GameAudioImpl::~GameAudioImpl()
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

//------------------------------------------------------------------------------
void GameAudioImpl::PlayBGM(const TCHAR* filePath, float volume, float pitch, double fadeTime)
{
	// 演奏再開チェック
	//if ( !mBGMRestart && mBGM )
	//{
	//	if ( mBGMName == filename_ )
	//	{
	//		return LN_OK;
	//	}
	//}

	auto sound = Sound::CreateInternal(mManager, filePath);
	//mManager->createSound( filePath, SOUNDPLAYTYPE_STREAMING, false ) );

	PlayBGMFromSound(sound, volume, pitch, fadeTime);

	// ファイル名記憶
	mBGMName = filePath;
}

//------------------------------------------------------------------------------
void GameAudioImpl::PlayBGMFromSound(Sound* sound, float volume, float pitch, double fadeTime)
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

	RefPtr<Sound> prev_bgm(mBGM, false);

	{
		Threading::MutexScopedLock lock(mLock);

		mBGM = sound;
		LN_SAFE_ADDREF(mBGM);

		mBGM->SetPitch(pitch);
		mBGM->SetLoopEnabled(true);

		mBGMVolume = volume;
		mBGMPitch = pitch;

		// ME が再生中ではないかを調べる
		bool me_not_play = true;
		if (mME)
		{
			SoundPlayingState state = mME->GetPlayingState();
			if (state == SoundPlayingState::Playing)
			{
				me_not_play = false;
			}
		}

		// ME 再生中ではない場合
		if (me_not_play)
		{
			if (fadeTime > 0)
			{
				mBGM->SetVolume(0);
				mBGM->Play();
				mBGM->FadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
			}
			else
			{
				mBGM->SetVolume(volume);
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
	if (fadeTime > 0)
	{
		if (prev_bgm != nullptr)
		{
			// ひとつ前のBGMは、fade_time_ 後に停止、解放するようにする
			prev_bgm->SetLoopEnabled(false);
			prev_bgm->FadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
			PushReleaseAtPlayEndList(prev_bgm);
		}
	}
	// フェード時間がない場合はすぐに停止
	else
	{
		if (prev_bgm != nullptr)
		{
			prev_bgm->Stop();
		}
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::StopBGM(double fadeTime)
{
	if (mBGM)
	{
		if (fadeTime > 0)
		{
			// フェード終了後に停止して、音量等を元に戻す
			mBGM->FadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
		}
		else
		{
			mBGM->Stop();
			LN_SAFE_RELEASE(mBGM);
		}

		mBGMName = _T("");
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::PlayBGS(const TCHAR* filePath, float volume, float pitch, double fadeTime)
{
	// 演奏再開チェック
	//if ( !mBGSRestart && mBGS )
	//{
	//	if ( mBGSName == filename_ )
	//	{
	//		return LN_OK;
	//	}
	//}

	auto sound = Sound::CreateInternal(mManager, filePath);

	PlayBGSFromSound(sound, volume, pitch, fadeTime);

	// ファイル名記憶
	mBGSName = filePath;
}

//------------------------------------------------------------------------------
void GameAudioImpl::PlayBGSFromSound(Sound* sound, float volume, float pitch, double fadeTime)
{
	// 演奏再開チェック
	//if ( !mBGSRestart && mBGS )
	//{
	//	if ( sound_ == mBGS && volume_ == mBGSVolume && pitch_ == mBGSPitch )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> prev_bgs(mBGS, false);

	{
		Threading::MutexScopedLock lock(mLock);

		mBGS = sound;
		LN_SAFE_ADDREF(mBGS);

		mBGS->SetPitch(pitch);
		mBGS->SetLoopEnabled(true);

		mBGSVolume = volume;
		mBGSPitch = pitch;

		if (fadeTime > 0)
		{
			mBGS->SetVolume(0);
			mBGS->Play();
			mBGS->FadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
		}
		else
		{
			mBGS->SetVolume(volume);
			mBGS->Play();
		}
	}

	// フェード時間がある場合
	if (fadeTime > 0)
	{
		if (prev_bgs != nullptr)
		{
			// ひとつ前のBGSは、fade_time_ 後に停止、解放するようにする
			prev_bgs->SetLoopEnabled(false);
			prev_bgs->FadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
			PushReleaseAtPlayEndList(prev_bgs);
		}
	}
	// フェード時間がない場合はすぐに停止
	else
	{
		if (prev_bgs != nullptr)
		{
			prev_bgs->Stop();
		}
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::StopBGS(double fadeTime)
{
	if (mBGS)
	{
		if (fadeTime > 0)
		{
			// フェード終了後に停止して、音量等を元に戻す
			mBGS->FadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
		}
		else
		{
			mBGS->Stop();
		}

		mBGSName = _T("");
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::PlayME(const TCHAR* filePath, float volume, float pitch)
{
	auto sound = Sound::CreateInternal(mManager, filePath);
	PlayMEFromSound(sound, volume, pitch);
}

//------------------------------------------------------------------------------
void GameAudioImpl::PlayMEFromSound(Sound* sound, float volume, float pitch)
{
	StopME();

	mME = sound;
	LN_SAFE_ADDREF(mME);

	mME->SetVolume(volume);
	mME->SetPitch(pitch);
	mME->SetLoopEnabled(false);

	bool flag = false;  // BGM があり、再生されているかを示すフラグ

						// BGM がある場合
	if (mBGM)
	{
		SoundPlayingState state = mBGM->GetPlayingState();

		// 再生されている場合
		if (state == SoundPlayingState::Playing)
		{
			flag = true;
		}
	}

	// BGM が再生中の場合
	if (flag)
	{
		// フェードアウト時間がある場合
		if (mBGMFadeOutTime > 0)
		{
			// フェードアウト後、一時停止する
			mBGM->FadeVolume(0, mBGMFadeOutTime, SoundFadeBehavior::Pause);
		}
		// フェードアウト時間がない場合
		else
		{
			// すぐに一時停止
			mBGM->Pause();
		}
	}
	// BGM がない場合はすぐ演奏開始
	else
	{
		mME->Play();
		mMEPlaying = true;
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::StopME()
{
	if (mME)
	{
		mME->Stop();
		LN_SAFE_RELEASE(mME);

		// BGM があって、一時停止中の場合は再開
		if (mBGM)
		{
			SoundPlayingState state = mBGM->GetPlayingState();

			if (state == SoundPlayingState::Playing)
			{
				mBGM->FadeVolume(mBGMVolume, mBGMFadeInTime, SoundFadeBehavior::Continue);
				mBGM->Resume();
			}
		}

		mMEPlaying = false;
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::PlaySE(const TCHAR* filePath, float volume, float pitch)
{
	auto sound = Sound::CreateInternal(mManager, filePath);
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);

	// ボリューム・ピッチ設定
	sound->SetVolume(volume);
	sound->SetPitch(pitch);

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	sound->SetGameAudioFlags(GameAudioFlags_SE);
	PushReleaseAtPlayEndList(sound);

	// 再生
	sound->SetLoopEnabled(false);
	sound->Play();
}

//------------------------------------------------------------------------------
void GameAudioImpl::PlaySE3D(const TCHAR* filePath, const Vector3& position, float distance, float volume, float pitch)
{
	// サウンド作成
	auto sound = Sound::CreateInternal(mManager, filePath);
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);
	sound->Set3DEnabled(true);

	// 位置・ピッチ設定
	sound->SetEmitterPosition(position);
	sound->SetEmitterMaxDistance(distance);
	sound->SetVolume(volume);
	sound->SetPitch(pitch);

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	sound->SetGameAudioFlags(GameAudioFlags_SE);
	PushReleaseAtPlayEndList(sound);

	// 再生
	sound->SetLoopEnabled(false);
	sound->Play();
}

//------------------------------------------------------------------------------
void GameAudioImpl::PlaySEFromSound(Sound* srcSound, float volume, float pitch)
{
	// 受け取った Sound が持っているソースをもとに新しい Sound を作成
	auto sound = RefPtr<Sound>::MakeRef();
	sound->Initialize(mManager, srcSound->GetAudioStream());
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);
	sound->Set3DEnabled(srcSound->Is3DEnabled());

	// 位置・ボリューム・ピッチ設定
	sound->SetVolume(volume);
	sound->SetPitch(pitch);
	if (srcSound->Is3DEnabled())
	{
		sound->SetEmitterPosition(srcSound->GetEmitterPosition());
	}

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	sound->SetGameAudioFlags(GameAudioFlags_SE);
	PushReleaseAtPlayEndList(sound);

	// 再生
	sound->SetLoopEnabled(false);
	sound->Play();
}

//------------------------------------------------------------------------------
void GameAudioImpl::StopSE()
{
	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end; ++itr)
	{
		if ((*itr)->GetGameAudioFlags() & GameAudioFlags_SE)
		{
			(*itr)->Stop();
		}
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::SetMEFadeState(double begin, double end)
{
	if (begin >= 0)
	{
		mBGMFadeOutTime = begin;
	}
	if (end >= 0)
	{
		mBGMFadeInTime = end;
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::SetBGMVolume(float volume, double fadeTime)
{
	Threading::MutexScopedLock lock(mLock);

	if (mBGM != NULL)// && !mBGM->IsVolumeFading())
	{
		mBGMVolume = volume;
		mBGM->FadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::SetBGSVolume(float volume, double fadeTime)
{
	Threading::MutexScopedLock lock(mLock);

	// GameAudioImpl 内では SOUNDFADE_STOP_RESET == フェードアウト中
	if (mBGS != NULL)// && !mBGM->IsVolumeFading())
	{
		mBGSVolume = volume;
		mBGS->FadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
	}
}

//------------------------------------------------------------------------------
//Sound* GameAudioImpl::GetInternalGameSound( InternalGameSound type )
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

//------------------------------------------------------------------------------
void GameAudioImpl::Polling()
{
	Threading::MutexScopedLock lock(mLock);

	// 演奏する ME がある場合
	if (mME)
	{
		SoundPlayingState mestate = mME->GetPlayingState();

		// BGM がある場合
		if (mBGM)
		{
			SoundPlayingState bgmstate = mBGM->GetPlayingState();

			// BGMのフェードアウトが終わって一時停止状態になっている場合
			if (bgmstate == SoundPlayingState::Pausing)
			{
				// ME 再生開始
				if (!mMEPlaying)
				{
					mME->SetLoopEnabled(false);
					mME->Play();
					mMEPlaying = true;
				}
				// ME の再生が終了した場合
				else if (mestate != SoundPlayingState::Playing)
				{
					// ME 再生中に BGM がストップしたとかで解放されている場合はなにもしない
					if (mBGM)
					{
						mBGM->FadeVolume(mBGMVolume, mBGMFadeInTime, SoundFadeBehavior::Continue);
						mBGM->Resume();
					}
					LN_SAFE_RELEASE(mME);
					mMEPlaying = false;
				}
			}
		}
		// BGM がない場合
		else
		{
			// ME が終了した場合
			if (mestate != SoundPlayingState::Playing)
			{
				LN_SAFE_RELEASE(mME);
				mMEPlaying = false;
			}
		}
	}

	/// 再生終了している音声はリストから外して解放する

	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end; )
	{
		SoundPlayingState state = (*itr)->GetPlayingState();
		if (state != SoundPlayingState::Playing)
		{
			(*itr)->Release();
			itr = mReleaseAtPlayEndList.erase(itr);
			end = mReleaseAtPlayEndList.end();
		}
		else
		{
			++itr;
		}
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::PushReleaseAtPlayEndList(Sound* sound)
{
	if (sound)
	{
		Threading::MutexScopedLock lock(mLock);
		mReleaseAtPlayEndList.push_back(sound);
		LN_SAFE_ADDREF(sound);
	}
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
