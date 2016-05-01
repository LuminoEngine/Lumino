
#pragma once
#include <list>

LN_NAMESPACE_BEGIN
class Vector3;

LN_NAMESPACE_AUDIO_BEGIN
class AudioManagerImpl;

/*
	@brief	
*/
class GameAudioImpl
{
public:

	/// BGM を演奏する
	void PlayBGM(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// 指定された Sound を BGM として演奏する
	void PlayBGMFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// BGM の演奏を停止する
	void StopBGM(double fadeTime = 0.0);

	/// BGS ( 環境音 ) を演奏する
	void PlayBGS(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// 指定された Sound を BGS として演奏する
	void PlayBGSFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// BGS の演奏を停止する
	void StopBGS(double fadeTime = 0.0);

	// ME ( 効果音楽 ) を演奏する
	void PlayME(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f);

	/// 指定された Sound を ME として演奏する
	void PlayMEFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

	/// ME の演奏を停止する
	void StopME();

	/// SE を演奏する
	void PlaySE(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f);

	/// SE を 3D 空間上で演奏する
	void PlaySE3D(const TCHAR* filePath, const Vector3& position, float distance, float volume = 1.0f, float pitch = 1.0f);

	/// 指定された Sound を SE として演奏する
	void PlaySEFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

	/// 全ての SE の演奏を停止する
	void StopSE();

	/// ME 演奏時の BGM のフェード時間を設定する
	void SetMEFadeState(double begin, double end);

	/// 再生中のBGMの音量を設定する (フェードアウト中は無効)
	void SetBGMVolume(float volume, double fadeTime = 0.0);

	/// 再生中のBGSの音量を設定する (フェードアウト中は無効)
	void SetBGSVolume(float volume, double fadeTime = 0.0);

	/// 同名 BGM の演奏再開フラグの設定
	void SetEnableBGMRestart(bool flag) { mBGMRestart = flag; }

	/// 同名 BGS の演奏再開フラグの設定
	void SetEnableBGSRestart(bool flag) { mBGSRestart = flag; }

private:
	friend class AudioManagerImpl;
	GameAudioImpl(AudioManagerImpl* mamager);
	~GameAudioImpl();

private:
	void Polling();
	void PushReleaseAtPlayEndList(Sound* sound);

private:
	typedef std::list<Sound*>	ReleaseAtPlayEndList;
	typedef std::list<Sound*>	SoundList;

	AudioManagerImpl*			mManager;
	Threading::Mutex			mLock;
	ReleaseAtPlayEndList        mReleaseAtPlayEndList;  ///< 再生終了時に解放する音声リスト
	Sound*		                mBGM;
	Sound*		                mBGS;
	Sound*						mME;
	String			            mBGMName;
	String				        mBGSName;
	float						mBGMVolume;
	float						mBGMPitch;
	float						mBGSVolume;
	float						mBGSPitch;
	double						mBGMFadeOutTime;
	double						mBGMFadeInTime;
	bool                        mMEPlaying;
	bool                        mBGMRestart;
	bool                        mBGSRestart;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
