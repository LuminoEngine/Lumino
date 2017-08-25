
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
	stopBGM(0);
	stopBGS(0);
	stopME();
	stopSE();

	// 再生終了後の解放リストに入っているサウンドを解放
	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end; ++itr)
	{
		(*itr)->release();
	}
	mReleaseAtPlayEndList.clear();

	LN_SAFE_RELEASE(mBGM);
	LN_SAFE_RELEASE(mBGS);
	LN_SAFE_RELEASE(mME);
}

//------------------------------------------------------------------------------
void GameAudioImpl::playBGM(const Char* filePath, float volume, float pitch, double fadeTime)
{
	// 演奏再開チェック
	//if ( !mBGMRestart && mBGM )
	//{
	//	if ( mBGMName == filename_ )
	//	{
	//		return LN_OK;
	//	}
	//}

	auto sound = createSound(filePath);
	//mManager->createSound( filePath, SOUNDPLAYTYPE_STREAMING, false ) );

	playBGMFromSound(sound, volume, pitch, fadeTime);

	// ファイル名記憶
	mBGMName = filePath;
}

//------------------------------------------------------------------------------
void GameAudioImpl::playBGMFromSound(Sound* sound, float volume, float pitch, double fadeTime)
{
	if (LN_CHECK_ARG(sound != nullptr)) return;

	// 演奏再開チェック
	// TODO:フェードアウト中に再開すると無音になる
	//if ( !mBGMRestart && mBGM )
	//{
	//	if ( sound_ == mBGM && volume_ == mBGMVolume && pitch_ == mBGMPitch )
	//	{
	//		return LN_OK;
	//	}
	//}

	Ref<Sound> prev_bgm(mBGM, false);

	{
		MutexScopedLock lock(mLock);

		mBGM = sound;
		LN_SAFE_ADDREF(mBGM);

		mBGM->setPitch(pitch);
		mBGM->setLoopEnabled(true);

		mBGMVolume = volume;
		mBGMPitch = pitch;

		// ME が再生中ではないかを調べる
		bool me_not_play = true;
		if (mME)
		{
			SoundPlayingState state = mME->getPlayingState();
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
				mBGM->setVolume(0);
				mBGM->play();
				mBGM->fadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
			}
			else
			{
				mBGM->setVolume(volume);
				mBGM->play();
			}
		}
		// ME 再生中の場合は、一度再生するけどすぐに一時停止する ( ME 終了後に再開 )
		else
		{
			mBGM->play();
			mBGM->pause();
		}
	}


	// フェード時間がある場合
	if (fadeTime > 0)
	{
		if (prev_bgm != nullptr)
		{
			// ひとつ前のBGMは、fade_time_ 後に停止、解放するようにする
			prev_bgm->setLoopEnabled(false);
			prev_bgm->fadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
			pushReleaseAtPlayEndList(prev_bgm);
		}
	}
	// フェード時間がない場合はすぐに停止
	else
	{
		if (prev_bgm != nullptr)
		{
			prev_bgm->stop();
		}
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::stopBGM(double fadeTime)
{
	if (mBGM)
	{
		if (fadeTime > 0)
		{
			// フェード終了後に停止して、音量等を元に戻す
			mBGM->fadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
		}
		else
		{
			mBGM->stop();
			LN_SAFE_RELEASE(mBGM);
		}

		mBGMName = _T("");
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::playBGS(const Char* filePath, float volume, float pitch, double fadeTime)
{
	// 演奏再開チェック
	//if ( !mBGSRestart && mBGS )
	//{
	//	if ( mBGSName == filename_ )
	//	{
	//		return LN_OK;
	//	}
	//}

	auto sound = createSound(filePath);

	playBGSFromSound(sound, volume, pitch, fadeTime);

	// ファイル名記憶
	mBGSName = filePath;
}

//------------------------------------------------------------------------------
void GameAudioImpl::playBGSFromSound(Sound* sound, float volume, float pitch, double fadeTime)
{
	// 演奏再開チェック
	//if ( !mBGSRestart && mBGS )
	//{
	//	if ( sound_ == mBGS && volume_ == mBGSVolume && pitch_ == mBGSPitch )
	//	{
	//		return LN_OK;
	//	}
	//}

	Ref<Sound> prev_bgs(mBGS, false);

	{
		MutexScopedLock lock(mLock);

		mBGS = sound;
		LN_SAFE_ADDREF(mBGS);

		mBGS->setPitch(pitch);
		mBGS->setLoopEnabled(true);

		mBGSVolume = volume;
		mBGSPitch = pitch;

		if (fadeTime > 0)
		{
			mBGS->setVolume(0);
			mBGS->play();
			mBGS->fadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
		}
		else
		{
			mBGS->setVolume(volume);
			mBGS->play();
		}
	}

	// フェード時間がある場合
	if (fadeTime > 0)
	{
		if (prev_bgs != nullptr)
		{
			// ひとつ前のBGSは、fade_time_ 後に停止、解放するようにする
			prev_bgs->setLoopEnabled(false);
			prev_bgs->fadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
			pushReleaseAtPlayEndList(prev_bgs);
		}
	}
	// フェード時間がない場合はすぐに停止
	else
	{
		if (prev_bgs != nullptr)
		{
			prev_bgs->stop();
		}
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::stopBGS(double fadeTime)
{
	if (mBGS)
	{
		if (fadeTime > 0)
		{
			// フェード終了後に停止して、音量等を元に戻す
			mBGS->fadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
		}
		else
		{
			mBGS->stop();
		}

		mBGSName = _T("");
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::playME(const Char* filePath, float volume, float pitch)
{
	auto sound = createSound(filePath);
	playMEFromSound(sound, volume, pitch);
}

//------------------------------------------------------------------------------
void GameAudioImpl::playMEFromSound(Sound* sound, float volume, float pitch)
{
	stopME();

	mME = sound;
	LN_SAFE_ADDREF(mME);

	mME->setVolume(volume);
	mME->setPitch(pitch);
	mME->setLoopEnabled(false);

	bool flag = false;  // BGM があり、再生されているかを示すフラグ

						// BGM がある場合
	if (mBGM)
	{
		SoundPlayingState state = mBGM->getPlayingState();

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
			mBGM->fadeVolume(0, mBGMFadeOutTime, SoundFadeBehavior::pause);
		}
		// フェードアウト時間がない場合
		else
		{
			// すぐに一時停止
			mBGM->pause();
		}
	}
	// BGM がない場合はすぐ演奏開始
	else
	{
		mME->play();
		mMEPlaying = true;
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::stopME()
{
	if (mME)
	{
		mME->stop();
		LN_SAFE_RELEASE(mME);

		// BGM があって、一時停止中の場合は再開
		if (mBGM)
		{
			SoundPlayingState state = mBGM->getPlayingState();

			if (state == SoundPlayingState::Playing)
			{
				mBGM->fadeVolume(mBGMVolume, mBGMFadeInTime, SoundFadeBehavior::Continue);
				mBGM->Resume();
			}
		}

		mMEPlaying = false;
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::playSE(const Char* filePath, float volume, float pitch)
{
	auto sound = createSound(filePath);
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);

	// ボリューム・ピッチ設定
	sound->setVolume(volume);
	sound->setPitch(pitch);

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	sound->setGameAudioFlags(GameAudioFlags_SE);
	pushReleaseAtPlayEndList(sound);

	// 再生
	sound->setLoopEnabled(false);
	sound->play();
}

//------------------------------------------------------------------------------
void GameAudioImpl::playSE3D(const Char* filePath, const Vector3& position, float distance, float volume, float pitch)
{
	// サウンド作成
	auto sound = createSound(filePath);
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);
	sound->set3DEnabled(true);

	// 位置・ピッチ設定
	sound->setEmitterPosition(position);
	sound->setEmitterMaxDistance(distance);
	sound->setVolume(volume);
	sound->setPitch(pitch);

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	sound->setGameAudioFlags(GameAudioFlags_SE);
	pushReleaseAtPlayEndList(sound);

	// 再生
	sound->setLoopEnabled(false);
	sound->play();
}

//------------------------------------------------------------------------------
void GameAudioImpl::playSEFromSound(Sound* srcSound, float volume, float pitch)
{
	// 受け取った Sound が持っているソースをもとに新しい Sound を作成
	auto sound = createSound(srcSound->getAudioStream());
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);
	sound->set3DEnabled(srcSound->is3DEnabled());

	// 位置・ボリューム・ピッチ設定
	sound->setVolume(volume);
	sound->setPitch(pitch);
	if (srcSound->is3DEnabled())
	{
		sound->setEmitterPosition(srcSound->getEmitterPosition());
	}

	// 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
	sound->setGameAudioFlags(GameAudioFlags_SE);
	pushReleaseAtPlayEndList(sound);

	// 再生
	sound->setLoopEnabled(false);
	sound->play();
}

//------------------------------------------------------------------------------
void GameAudioImpl::stopSE()
{
	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end; ++itr)
	{
		if ((*itr)->getGameAudioFlags() & GameAudioFlags_SE)
		{
			(*itr)->stop();
		}
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::setMEFadeState(double begin, double end)
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
void GameAudioImpl::setBGMVolume(float volume, double fadeTime)
{
	MutexScopedLock lock(mLock);

	if (mBGM != NULL)// && !mBGM->isVolumeFading())
	{
		mBGMVolume = volume;
		mBGM->fadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::setBGSVolume(float volume, double fadeTime)
{
	MutexScopedLock lock(mLock);

	// GameAudioImpl 内では SOUNDFADE_STOP_RESET == フェードアウト中
	if (mBGS != NULL)// && !mBGM->isVolumeFading())
	{
		mBGSVolume = volume;
		mBGS->fadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
	}
}

//------------------------------------------------------------------------------
void GameAudioImpl::polling()
{
	MutexScopedLock lock(mLock);

	// 演奏する ME がある場合
	if (mME)
	{
		SoundPlayingState mestate = mME->getPlayingState();

		// BGM がある場合
		if (mBGM)
		{
			SoundPlayingState bgmstate = mBGM->getPlayingState();

			// BGMのフェードアウトが終わって一時停止状態になっている場合
			if (bgmstate == SoundPlayingState::Pausing)
			{
				// ME 再生開始
				if (!mMEPlaying)
				{
					mME->setLoopEnabled(false);
					mME->play();
					mMEPlaying = true;
				}
				// ME の再生が終了した場合
				else if (mestate != SoundPlayingState::Playing)
				{
					// ME 再生中に BGM がストップしたとかで解放されている場合はなにもしない
					if (mBGM)
					{
						mBGM->fadeVolume(mBGMVolume, mBGMFadeInTime, SoundFadeBehavior::Continue);
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
		SoundPlayingState state = (*itr)->getPlayingState();
		if (state != SoundPlayingState::Playing)
		{
			(*itr)->release();
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
void GameAudioImpl::pushReleaseAtPlayEndList(Sound* sound)
{
	if (sound)
	{
		MutexScopedLock lock(mLock);
		mReleaseAtPlayEndList.push_back(sound);
		LN_SAFE_ADDREF(sound);
	}
}

//------------------------------------------------------------------------------
SoundPtr GameAudioImpl::createSound(const StringRef& filePath)
{
	auto ptr = SoundPtr::makeRef();
	ptr->initialize(filePath);
	return ptr;
}

//------------------------------------------------------------------------------
SoundPtr GameAudioImpl::createSound(detail::AudioStream* audioStream)
{
	auto ptr = SoundPtr::makeRef();
	ptr->initialize(audioStream);
	return ptr;
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
