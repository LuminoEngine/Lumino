
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/Sound.hpp>
#include "Decoder/AudioDecoder.hpp"
#include "ARIs/ARISourceNode.hpp"
#include "GameAudioImpl.hpp"
#include "AudioManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GameAudioImpl

enum GameAudioFlags
{
    GameAudioFlags_SE = 0x01,
};

GameAudioImpl::GameAudioImpl(AudioManager* mamager)
    : mManager(mamager)
    , mBGM(nullptr)
    , mBGS(nullptr)
    , mME(nullptr)
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

GameAudioImpl::~GameAudioImpl()
{
}

void GameAudioImpl::dispose()
{
    stopBGM(0);
    stopBGS(0);
    stopME();
    stopSE();

    // 再生終了後の解放リストに入っているサウンドを解放
    mReleaseAtPlayEndList.clear();

    mBGM = nullptr;
    mBGS = nullptr;
    mME = nullptr;
}

void GameAudioImpl::playBGM(const StringView& filePath, float volume, float pitch, double fadeTime)
{
    auto sound = createSound(filePath);
    playBGMFromSound(sound, volume, pitch, fadeTime);
    mBGMName = filePath;
}

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
            mBGM = nullptr;
        }

        mBGMName = _T("");
    }
}

void GameAudioImpl::playBGS(const StringView& filePath, float volume, float pitch, double fadeTime)
{
    auto sound = createSound(filePath);

    playBGSFromSound(sound, volume, pitch, fadeTime);

    // ファイル名記憶
    mBGSName = filePath;
}

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

void GameAudioImpl::playME(const StringView& filePath, float volume, float pitch)
{
    auto sound = createSound(filePath);
    playMEFromSound(sound, volume, pitch);
}

void GameAudioImpl::stopME()
{
    if (mME)
    {
        mME->stop();
        mME = nullptr;

        // BGM があって、一時停止中の場合は再開
        if (mBGM)
        {
            SoundPlayingState state = mBGM->playingState();

            if (state == SoundPlayingState::Playing)
            {
                mBGM->fadeVolume(mBGMVolume, mBGMFadeInTime, SoundFadeBehavior::Continue);
                mBGM->resume();
            }
        }

        mMEPlaying = false;
    }
}

void GameAudioImpl::playSE(const StringView& filePath, float volume, float pitch)
{
    auto sound = createSound(filePath);

    // ボリューム・ピッチ設定
    sound->setVolume(volume);
    sound->setPitch(pitch);

    // 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
    sound->core()->setGameAudioFlags(GameAudioFlags_SE);
    pushReleaseAtPlayEndList(sound);

    // 再生
    sound->setLoopEnabled(false);
    sound->play();
}

void GameAudioImpl::playSE3D(const StringView& filePath, const Vector3& position, float distance, float volume, float pitch)
{
    // サウンド作成
    auto sound = createSound(filePath);
    sound->set3DEnabled(true);

    // 位置・ピッチ設定
    sound->setEmitterPosition(position);
    sound->setEmitterMaxDistance(distance);
    sound->setVolume(volume);
    sound->setPitch(pitch);

    // 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
    sound->core()->setGameAudioFlags(GameAudioFlags_SE);
    pushReleaseAtPlayEndList(sound);

    // 再生
    sound->setLoopEnabled(false);
    sound->play();
}

void GameAudioImpl::stopSE()
{
    ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
    ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
    for (; itr != end; ++itr)
    {
        if ((*itr)->core()->gameAudioFlags() & GameAudioFlags_SE)
        {
            (*itr)->stop();
        }
    }
}

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

void GameAudioImpl::setBGMVolume(float volume, double fadeTime)
{
    std::lock_guard<std::mutex> lock(mLock);

    if (mBGM)// && !mBGM->isVolumeFading())
    {
        mBGMVolume = volume;
        mBGM->fadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
    }
}

void GameAudioImpl::setBGSVolume(float volume, double fadeTime)
{
    std::lock_guard<std::mutex> lock(mLock);

    // GameAudioImpl 内では SOUNDFADE_STOP_RESET == フェードアウト中
    if (mBGS)// && !mBGM->isVolumeFading())
    {
        mBGSVolume = volume;
        mBGS->fadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
    }
}

void GameAudioImpl::setEnableBGMRestart(bool enabled)
{
    mBGMRestart = enabled;
}

void GameAudioImpl::setEnableBGSRestart(bool enabled)
{
    mBGSRestart = enabled;
}

void GameAudioImpl::polling()
{
    std::lock_guard<std::mutex> lock(mLock);

    // 演奏する ME がある場合
    if (mME)
    {
        SoundPlayingState mestate = mME->playingState();

        // BGM がある場合
        if (mBGM)
        {
            SoundPlayingState bgmstate = mBGM->playingState();

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
                        mBGM->resume();
                    }
                    mME = nullptr;
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
                mME = nullptr;
                mMEPlaying = false;
            }
        }
    }

    /// 再生終了している音声はリストから外して解放する

    ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
    ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
    for (; itr != end; )
    {
        SoundPlayingState state = (*itr)->playingState();
        if (state != SoundPlayingState::Playing)
        {
            itr = mReleaseAtPlayEndList.erase(itr);
            end = mReleaseAtPlayEndList.end();
        }
        else
        {
            ++itr;
        }
    }
}
void GameAudioImpl::playBGMFromSound(Sound* sound, float volume, float pitch, double fadeTime)
{
    if (LN_REQUIRE(sound != nullptr)) return;

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
        std::lock_guard<std::mutex> lock(mLock);

        mBGM = sound;

        mBGM->setPitch(pitch);
        mBGM->setLoopEnabled(true);

        mBGMVolume = volume;
        mBGMPitch = pitch;

        // ME が再生中ではないかを調べる
        bool me_not_play = true;
        if (mME)
        {
            SoundPlayingState state = mME->playingState();
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
        std::lock_guard<std::mutex> lock(mLock);

        mBGS = sound;

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

void GameAudioImpl::playMEFromSound(Sound* sound, float volume, float pitch)
{
    stopME();

    mME = sound;

    mME->setVolume(volume);
    mME->setPitch(pitch);
    mME->setLoopEnabled(false);

    bool flag = false;  // BGM があり、再生されているかを示すフラグ

                        // BGM がある場合
    if (mBGM)
    {
        SoundPlayingState state = mBGM->playingState();

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
            mBGM->fadeVolume(0, mBGMFadeOutTime, SoundFadeBehavior::Pause);
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

void GameAudioImpl::pushReleaseAtPlayEndList(Sound* sound)
{
    if (sound)
    {
        std::lock_guard<std::mutex> lock(mLock);
        mReleaseAtPlayEndList.push_back(sound);
    }
}

Ref<Sound> GameAudioImpl::createSound(const StringView& filePath)
{
    return makeObject<Sound>(filePath);
}

//==============================================================================
// GameAudioImpl

GameAudioImpl2::GameAudioImpl2(AudioManager* mamager)
    : m_manager(mamager)
{
}

void GameAudioImpl2::dispose()
{
    // Flush commands
    update(1.0f);

    if (m_bgm) {
        m_bgm->lifecycleState = SoundCoreLifecycleState::Disposed;
        m_bgm = nullptr;
    }
}

void GameAudioImpl2::playBGM(const StringView& filePath, float volume, float pitch, double fadeTime)
{
    Command cmd;
    cmd.type = CommandType::PlayBGM;
    cmd.sound = createSoundCore(filePath);
    cmd.volume = volume;
    cmd.pitch = pitch;
    cmd.fadeTime = fadeTime;
    if (cmd.sound) {
        m_commands.push_back(std::move(cmd));
    }
}

void GameAudioImpl2::stopBGM(double fadeTime)
{
    Command cmd;
    cmd.type = CommandType::StopBGM;
    cmd.fadeTime = fadeTime;
    m_commands.push_back(std::move(cmd));
}

void GameAudioImpl2::playBGS(const StringView& filePath, float volume, float pitch, double fadeTime)
{
    Command cmd;
    cmd.type = CommandType::PlayBGS;
    cmd.sound = createSoundCore(filePath);
    cmd.volume = volume;
    cmd.pitch = pitch;
    cmd.fadeTime = fadeTime;
    if (cmd.sound) {
        m_commands.push_back(std::move(cmd));
    }
}

void GameAudioImpl2::stopBGS(double fadeTime)
{
    Command cmd;
    cmd.type = CommandType::StopBGS;
    cmd.fadeTime = fadeTime;
    m_commands.push_back(std::move(cmd));
}

void GameAudioImpl2::playME(const StringView& filePath, float volume, float pitch)
{
    Command cmd;
    cmd.type = CommandType::PlayME;
    cmd.sound = createSoundCore(filePath);
    cmd.volume = volume;
    cmd.pitch = pitch;
    if (cmd.sound) {
        m_commands.push_back(std::move(cmd));
    }
}

void GameAudioImpl2::stopME()
{
    Command cmd;
    cmd.type = CommandType::StopME;
    m_commands.push_back(std::move(cmd));
}

void GameAudioImpl2::playSE(const StringView& filePath, float volume, float pitch)
{
    Command cmd;
    cmd.type = CommandType::PlaySE;
    cmd.sound = createSoundCore(filePath);
    cmd.volume = volume;
    cmd.pitch = pitch;
    if (cmd.sound) {
        m_commands.push_back(std::move(cmd));
    }
}

void GameAudioImpl2::playSE3D(const StringView& filePath, const Vector3& position, float distance, float volume, float pitch)
{
    Command cmd;
    cmd.type = CommandType::PlaySE3D;
    cmd.sound = createSoundCore(filePath);
    cmd.volume = volume;
    cmd.pitch = pitch;
    cmd.position = position;
    cmd.distance = distance;
    if (cmd.sound) {
        m_commands.push_back(std::move(cmd));
    }
}

void GameAudioImpl2::stopSE()
{
    Command cmd;
    cmd.type = CommandType::StopSE;
    m_commands.push_back(std::move(cmd));
}

void GameAudioImpl2::setMEFadeTimes(float bgmFadeoutTime, float bgmFadeinTime)
{
    if (LN_REQUIRE(bgmFadeoutTime >= 0)) return;
    if (LN_REQUIRE(bgmFadeinTime >= 0)) return;
    m_bgmFadeOutTimeForME = bgmFadeoutTime;
    m_bgmFadeInTimeForME = bgmFadeinTime;
}

void GameAudioImpl2::update(float elapsedSeconds)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_commands.empty()) {
        for (const auto& cmd : m_commands) {
            switch (cmd.type) {
                case CommandType::PlayBGM:
                    // TODO: cross-fade
                    m_bgm = cmd.sound;
                    m_bgm->setLoopEnabled(true);
                    m_bgm->setVolume(cmd.volume, cmd.fadeTime, SoundFadeBehavior::Stop);
                    m_bgm->setPitch(cmd.pitch);
                    m_bgm->play();
                    break;
                case CommandType::StopBGM:
                    m_bgm->stop(cmd.fadeTime);
                    break;
                case CommandType::PlayBGS:
                    m_bgs = cmd.sound;
                    m_bgs->setLoopEnabled(true);
                    m_bgs->setVolume(cmd.volume, cmd.fadeTime, SoundFadeBehavior::Stop);
                    m_bgs->setPitch(cmd.pitch);
                    m_bgs->play();
                    break;
                case CommandType::StopBGS:
                    m_bgs->stop(cmd.fadeTime);
                    break;
                case CommandType::PlayME:
                    LN_NOTIMPLEMENTED();
                    break;
                case CommandType::StopME:
                    LN_NOTIMPLEMENTED();
                    break;
                case CommandType::PlaySE:
                    cmd.sound->setVolume(cmd.volume, 0.0f, SoundFadeBehavior::Continue);
                    cmd.sound->setPitch(cmd.pitch);
                    cmd.sound->setLoopEnabled(false);
                    cmd.sound->play();
                    m_seList.add(cmd.sound);
                    break;
                case CommandType::PlaySE3D:
                    LN_NOTIMPLEMENTED();
                    break;
                case CommandType::StopSE:
                    for (const auto& se : m_seList) {
                        se->stop(0.0f);
                    }
                    break;
                default:
                    LN_UNREACHABLE();
                    break;
            }
        }
        m_commands.clear();
    }

    for (int i = m_seList.size() - 1; i >= 0; i--) {
        if (m_seList[i]->m_sourceNode->playingState() == ARISourceNode::State::Stopped) {
            m_seList.removeAt(i);
            // ここではリストから外すだけ。
            // dispose はメインの管理リスト (AudioManager::m_soundCoreList) の polling から行われる。
        }
    }
}

Ref<SoundCore> GameAudioImpl2::createSoundCore(const StringView& filePath)
{
    Ref<detail::AudioDecoder> decoder = m_manager->createAudioDecoder(filePath);

    AudioContext* context = m_manager->primaryContext();
    auto soundCore = makeRef<detail::SoundCore>();
    if (!soundCore->init(m_manager, context, decoder)) {
        return nullptr;
    }
    m_manager->postAddSoundCore(soundCore);

    return soundCore;
}

} // namespace detail
} // namespace ln

