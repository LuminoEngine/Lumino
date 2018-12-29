#pragma once
#include <mutex>

namespace ln {
class Sound;
namespace detail {
class AudioManager;

class GameAudioImpl
	: public RefObject
{
public:
    GameAudioImpl(AudioManager* mamager);
	virtual ~GameAudioImpl();
    void dispose();

    void playBGM(const StringRef& filePath, float volume, float pitch, double fadeTime);
    void stopBGM(double fadeTime);
    void playBGS(const StringRef& filePath, float volume, float pitch, double fadeTime);
    void stopBGS(double fadeTime);
    void playME(const StringRef& filePath, float volume, float pitch);
    void stopME();
    void playSE(const StringRef& filePath, float volume, float pitch);
    void playSE3D(const StringRef& filePath, const Vector3& position, float distance, float volume, float pitch);
    void stopSE();
    void setMEFadeState(double begin, double end);
    void setBGMVolume(float volume, double fadeTime);
    void setBGSVolume(float volume, double fadeTime);
    void setEnableBGMRestart(bool enabled);
    void setEnableBGSRestart(bool enabled);

    void polling();

private:
    void playBGMFromSound(Sound* sound, float volume, float pitch, double fadeTime);
    void playBGSFromSound(Sound* sound, float volume, float pitch, double fadeTime);
    void playMEFromSound(Sound* sound, float volume, float pitch);
    void pushReleaseAtPlayEndList(Sound* sound);
    Ref<Sound> createSound(const StringRef& filePath);

    typedef std::list<Ref<Sound>>	ReleaseAtPlayEndList;

    AudioManager*				mManager;
    std::mutex						mLock;
    ReleaseAtPlayEndList        mReleaseAtPlayEndList;  ///< 再生終了時に解放する音声リスト
    Ref<Sound>		                mBGM;
    Ref<Sound>		                mBGS;
    Ref<Sound>						mME;
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
} // namespace ln

