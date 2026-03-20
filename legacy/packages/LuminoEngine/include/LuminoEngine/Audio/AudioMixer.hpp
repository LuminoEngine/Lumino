#pragma once

namespace ln {
namespace a2 {

/**
 * 様々な音源を合成したりエフェクトをかるためのオブジェクトです。
 *
 * [UnityのAudioMixer](https://docs.unity3d.com/ja/2018.4/Manual/AudioMixer.html) や
 * [godotのAudioBus](https://docs.godotengine.org/ja/4.x/tutorials/audio/audio_buses.html) と
 * 似た役割を持ちます。
 *
 * なお名前については、ゲームエンジン以外も調べてみると "AudioBus" というのは
 * AudioMixer に内包される概念のひとつのように使われることが多いように見えたのと、
 * 主に実装の参考にしている chromium と godot では違う名前が使われていたため、
 * 混乱を避けるために AudioMixer としています。
 */
class AudioMixer : public Object {
public:
    AudioMixer();
};

} // namespace a2
} // namespace ln

