#pragma once
#include <mutex>
#include <list>
#include <LuminoEngine/Audio/Common.hpp>

namespace ln {
class Sound;
namespace detail {
class AudioManager;


// Note:
// 内部で使用する Sound たちは外部に公開禁止 & 外部から受け取ることも禁止。
// playSE() とかでは内部的に Sound を生成するが、これの解放責任を UserProgram にやらせるのは使い勝手良くない。
// さらに Audio モジュールは他のモジュールと異なっていて、Engine::update() は無くても動作させたい。
// そうするとフェードアウトや再生停止後の自動開放は、AudioThread でやりたい。
// ここで問題になってくるのは、Binding に公開してしまったときの対処。特に AudioThread で Object を dispose してしまったとき。
// C++ 側だけならまだしも、別言語の Wrapper 側にも排他処理が必要になってしまう。
// そもそもネイティブマルチスレッドを扱えない言語も多くある。
// なので全面的に公開禁止。
// もし既存の Sound を使いたい場合は、playSE(sound) とかしたときに Sound のクローンを作ること。
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

// AudioManager と同じくコマンドを AudioThread に送ることで各処理を行う。
// こうしておかないと、例えば playBGM() したときに MEがフェード中か？ といった判定をするときに、
// AudioThread で更新されるパラメータを監視する必要があり、複雑度が上がってしまう。
class GameAudioImpl2
    : public RefObject
{
public:
    GameAudioImpl2(AudioManager* mamager);
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
    void setMEFadeTimes(float bgmFadeoutTime, float bgmFadeinTime);

    void update(float elapsedSeconds);  // call by AudioThread

private:
    enum class CommandType
    {
        PlayBGM,
        StopBGM,
        PlayBGS,
        StopBGS,
        PlayME,
        StopME,
        PlaySE,
        PlaySE3D,
        StopSE,
    };

    struct Command
    {
        CommandType type;
        detail::SoundCore* sound;
        float volume;
        float pitch;
        float fadeTime;
        Vector3 position;
        float distance;
        // FIXME: これ以上増えるなら union にしたほうがいいかも
    };

    Ref<SoundCore> createSoundCore(const StringRef& filePath);

    AudioManager* m_manager;
    std::mutex m_mutex;
    std::vector<Command> m_commands;

    Ref<SoundCore> m_bgm;
    Ref<SoundCore> m_bgs;
    List<Ref<SoundCore>> m_seList;


    String			            mBGMName;
    String				        mBGSName;
    float						mBGMVolume;
    float						mBGMPitch;
    float						mBGSVolume;
    float						mBGSPitch;
    float						m_bgmFadeOutTimeForME;
    float						m_bgmFadeInTimeForME;
    bool                        mMEPlaying;
    bool                        mBGMRestart;
    bool                        mBGSRestart;

    std::mutex						mLock;
    //ReleaseAtPlayEndList        mReleaseAtPlayEndList;  ///< 再生終了時に解放する音声リスト

    Ref<SoundCore>		                mBGS;
    Ref<SoundCore>						mME;
};


} // namespace detail
} // namespace ln

