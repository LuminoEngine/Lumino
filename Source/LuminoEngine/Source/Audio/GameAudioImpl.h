
#pragma once
#include <list>

LN_NAMESPACE_BEGIN
class Vector3;

LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{
class AudioManager;

/*
	@brief	
*/
class GameAudioImpl
{
public:

	/// BGM を演奏する
	void playBGM(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// 指定された Sound を BGM として演奏する
	void playBGMFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// BGM の演奏を停止する
	void stopBGM(double fadeTime = 0.0);

	/// BGS ( 環境音 ) を演奏する
	void playBGS(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// 指定された Sound を BGS として演奏する
	void playBGSFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// BGS の演奏を停止する
	void stopBGS(double fadeTime = 0.0);

	// ME ( 効果音楽 ) を演奏する
	void playME(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f);

	/// 指定された Sound を ME として演奏する
	void playMEFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

	/// ME の演奏を停止する
	void stopME();

	/// SE を演奏する
	void playSE(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f);

	/// SE を 3D 空間上で演奏する
	void playSE3D(const TCHAR* filePath, const Vector3& position, float distance, float volume = 1.0f, float pitch = 1.0f);

	/// 指定された Sound を SE として演奏する
	void playSEFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

	/// 全ての SE の演奏を停止する
	void stopSE();

	/// ME 演奏時の BGM のフェード時間を設定する
	void setMEFadeState(double begin, double end);

	/// 再生中のBGMの音量を設定する (フェードアウト中は無効)
	void setBGMVolume(float volume, double fadeTime = 0.0);

	/// 再生中のBGSの音量を設定する (フェードアウト中は無効)
	void setBGSVolume(float volume, double fadeTime = 0.0);

	/// 同名 BGM の演奏再開フラグの設定
	void setEnableBGMRestart(bool flag) { mBGMRestart = flag; }

	/// 同名 BGS の演奏再開フラグの設定
	void setEnableBGSRestart(bool flag) { mBGSRestart = flag; }

private:
	friend class AudioManager;
	GameAudioImpl(AudioManager* mamager);
	~GameAudioImpl();

	void polling();
	void pushReleaseAtPlayEndList(Sound* sound);
	SoundPtr createSound(const StringRef& filePath);
	SoundPtr createSound(detail::AudioStream* audioStream);

private:
	typedef std::list<Sound*>	ReleaseAtPlayEndList;
	typedef std::list<Sound*>	SoundList;

	AudioManager*				mManager;
	Mutex						mLock;
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

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
