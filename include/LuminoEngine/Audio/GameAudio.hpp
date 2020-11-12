#pragma once

namespace ln {

// deprecated
class GameAudio final
{
public:
    /// BGM を演奏する
    static void playBGM(const StringRef& filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

    /// BGM の演奏を停止する
    static void stopBGM(double fadeTime = 0.0);

    /// BGS (環境音) を演奏する
    static void playBGS(const StringRef& filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

    /// BGS の演奏を停止する
    static void stopBGS(double fadeTime = 0.0);

    // ME (効果音楽) を演奏する
    static void playME(const StringRef& filePath, float volume = 1.0f, float pitch = 1.0f);

    /// ME の演奏を停止する
    static void stopME();

    /// SE を演奏する
    static void playSE(const StringRef& filePath, float volume = 1.0f, float pitch = 1.0f);

    /// SE を 3D 空間上で演奏する
    static void playSE3D(const StringRef& filePath, const Vector3& position, float distance, float volume = 1.0f, float pitch = 1.0f);

    /// 全ての SE の演奏を停止する
    static void stopSE();

    /// ME 演奏時の BGM のフェード時間を設定する
    static void setMEFadeState(double begin, double end);

    /// 再生中のBGMの音量を設定する (フェードアウト中は無効)
    static void setBGMVolume(float volume, double fadeTime = 0.0);

    /// 再生中のBGSの音量を設定する (フェードアウト中は無効)
    static void setBGSVolume(float volume, double fadeTime = 0.0);

    /// 同名 BGM の演奏再開フラグの設定
    static void setEnableBGMRestart(bool enabled);

    /// 同名 BGS の演奏再開フラグの設定
    static void setEnableBGSRestart(bool enabled);

};

} // namespace ln

