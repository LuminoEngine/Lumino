#pragma once
#include <mutex>
#include "Common.hpp"
#include <LuminoEngine/Animation/EasingFunctions.hpp>

namespace ln {
class AudioContext;
class AudioSourceNode;
class AudioGainNode;
namespace detail {
class GameAudioImpl;
class SoundCore;
}

/** 音声の再生状態を表します。*/
enum class SoundPlayingState {
    Stopped, /**< 停止中 */
    Playing, /**< 再生中 */
    Pausing, /**< 一時停止中 */
};

/** 音量フェード完了時の動作を表します。*/
LN_ENUM()
enum class SoundFadeBehavior {
    /** 再生を継続する */
    Continue,

    /** 停止する */
    Stop,

    /** 停止して、次の再生に備えてサウンドの音量を元の値に戻す */
    StopReset,

    /** 一時停止する */
    Pause,

    /** 一時停止して、次の再生に備えてサウンドの音量を元の値に戻す */
    PauseReset,
};

/*
	[2020/7/7] Note: Sound のライフサイクル問題
	UserProgram から直接 Sound を作る場合…
		考え方的には Unity の AudioSource が近い。Sound 自体が座標なども持っている。
		この使い方の時は、解放は Ref に任せてよい。明示的に削除したいときは dispose。
		つまりこのケースでは、内部的には Sound を弱参照のリストで管理する、ということになる。
		ただし Sound::dispose 時点で AudioThread から更新中、ということもあるので、ただ弱参照にするだけだと null アクセスしてしまう。
	playSE() などで内部管理する場合…
		こちらの場合は Sound のインスタンス管理を内部 (AudioManager あたり) が行う。
		Audio モジュールは updateFrame() 無しでも動くようにしたいので、dispose は AudioThread から行う必要がある。
		しかし MainThread 以外からの dispose は、Binding モジュール側への影響が非常に大きいため、このパターンの Sound は外部に公開したくないところ。
	↓
	総じてやっぱり ARI を分離した動機と同じ原因の問題となる。

	同じように内部的なインスタンスを分離しようか。
	UserProgram から直接 Sound を作る場合…
		Sound::init で data をコマンドに乗せて AudioThread へ add。
		Sound::dispose で remove… でもいいんだけど、↓の兼もあるので lifesycleState みたいなフラグ監視がいいかも。
	playSE() などで内部管理する場合…
		playSE() の戻り値は Sound を返したい。Effect とかと同じく。
		でも多くの場合は不要で、すぐインスタンスは消える。
		でもその場合でも、音の最後までは再生したい。
		そのため Sound::dispose に合わせていきなり remove されるのはちょっと困る。



*/

/**
 * Sound
 */
LN_CLASS()
class Sound : public Object {
    LN_OBJECT;
public:
    ///** Sound クラスのインスタンスを作成します。 */
    //static SoundPtr create(const StringView& filePath);

    ///** Sound クラスのインスタンスを作成します。 */
    //static SoundPtr create(Stream* stream, SoundLoadingMode loadingMode);

    /**
	 * この Sound の音量を設定します。
	 * @param[in]	volume	: 音量 (0.0～1.0。初期値は 1.0)
	 */
    LN_METHOD(Property)
    void setVolume(float value);

    /**
	 * この Sound の音量を取得します。
	 */
    LN_METHOD(Property)
    float getVolume() const;

    /**
	 * この Sound のピッチ (音高) を設定します。
	 * @param[in]	volume	: ピッチ (0.5～2.0。初期値は 1.0)
	 */
    LN_METHOD(Property)
    void setPitch(float value);

    /**
	 * この Sound のピッチ (音高) を取得します。
	 */
    LN_METHOD(Property)
    float getPitch() const;

    /**
	 * ループ再生の有無を設定します。
	 * @param[in]	enabled		: true の場合、ループ再生する
	 */
    LN_METHOD(Property)
    void setLoopEnabled(bool enabled);

    /**
	 * ループ再生が有効かを確認します。
     */
    LN_METHOD(Property)
    bool isLoopEnabled() const;

    /**
	 * ループ範囲を設定します。
	 * @param[in]	begin		: ループ範囲の開始サンプル
	 * @param[in]	length		: ループ範囲のサンプル数
	 * @details	MIDI の場合、ループ範囲はミュージックタイム単位 (四分音符ひとつ分を 768 で表す) で指定します。 
	 */
    LN_METHOD()
    void SetLoopRange(uint32_t begin, uint32_t length);

    /**
	 * この Sound の再生を開始します。
	 */
    LN_METHOD()
    void play();

    /**
	 * この Sound の再生を停止します。
	 */
    LN_METHOD()
    void stop();

    /**
	 * この Sound の再生を一時停止します。
	 */
    LN_METHOD()
    void pause();

    /**
	 * 一時停止中の再生を再開します。
	 */
    LN_METHOD()
    void resume();

    /**
	 * サウンドを 3D 音源として再生するかを設定します。(規定値:false)
	 * 
	 * 設定は play() の前に行う必要があります。
	 */
    void set3DEnabled(bool enabled);

    /**
	 *  この Sound が 3D 音声であるかを確認します。		
	 */
    bool is3DEnabled() const;

    /**
	 * 3D 音声としての位置を設定します。
	 */
    void setEmitterPosition(const Vector3& position);

    /**
	 * 3D 音声としての位置を取得します。
	 */
    const Vector3& getEmitterPosition() const;

    /**
	 * 3D 音声としての速度を設定します。	
	 */
    void setEmitterVelocity(const Vector3& velocity);

    /**
	 * 3D 音声としての位置を取得します。	
	 */
    const Vector3& getEmitterVelocity() const;

    /**
	 * 3D 音声としての減衰距離を設定します。(default:100)	
	 */
    void setEmitterMaxDistance(float distance);

    /**
	 * 音声データの全サンプル数を取得します。
	 * 
	 * MIDI の場合はミュージックタイム単位 (四分音符ひとつ分が 768) で、
	 * 1度でも play() で再生を開始していないと取得できません。
	 */
    int64_t getTotalSamples() const;

    /**
	 * 現在の再生サンプル数を取得します。
	 */
    int64_t getPlayedSamples() const;

    /**
	 * サンプリングレートを取得します。
	 * 
	 * MIDI の場合は 768 を返します。
	 */
    int getSamplingRate() const;

    /**
     * この Sound の現在の再生状態を取得します。
     */
    SoundPlayingState playingState() const;
#if 0

	/**
	 * 音声データの再生方法を設定します。(規定値:Unknown)
	 * 
	 * 設定は play() の前に行う必要があります。
	 */
	void SetPlayingMode(SoundPlayingMode mode);
	
	/**
	 * 音声データの再生方法を取得します。
	 */
	SoundPlayingMode getPlayingMode() const;
#endif

    /**
	 * 音量のフェードを開始します。
	 * @param[in]	targetVolume	: フェード先音量
	 * @param[in]	time			: 変化にかける時間 (秒)
	 * @param[in]	state			: 音量フェード完了時の動作
	 * 現在の音量から targetVolume へ音量の遷移を行います。現在の音量は getVolume() で取得できる値です。
	 * フェード中は音量が変更され、getVolume() で取得できる値が変わります。
	 */
    LN_METHOD()
    void fadeVolume(float targetVolume, float time, SoundFadeBehavior behavior);

LN_CONSTRUCT_ACCESS:
    Sound();
    virtual ~Sound();
    void init();
    void init(const StringView& filePath);
    virtual void onDispose(bool explicitDisposing) override;

private:
    const Ref<detail::SoundCore>& core() const { return m_core; }
    //void setGameAudioFlags(uint32_t flags) { m_gameAudioFlags = flags; }
    //uint32_t gameAudioFlags() const { return m_gameAudioFlags; }
    //void process(float elapsedSeconds);
    //void playInternal();
    //void stopInternal();
    //void pauseInternal();
    void setVolumeInternal(float volume);

    detail::AudioManager* m_manager;
    //Ref<AudioSourceNode> m_sourceNode;
    //Ref<AudioGainNode> m_gainNode;
    Ref<detail::SoundCore> m_core;

    //std::mutex m_playerStateLock;    // TODO: 性質上、スピンロックの方が効率がいいかもしれない
    //EasingValue<float> m_fadeValue;
    //SoundFadeBehavior m_fadeBehavior;
    //bool m_fading;

    friend class detail::AudioManager;
    friend class detail::GameAudioImpl;
    friend class AudioContext;
};

namespace detail {

enum class SoundCoreLifecycleState {
    Valid,            // 再生中。Sound オブジェクトも有効。
    Disposed,         // 対応する Sound オブジェクトは破棄されているため、SoundCore も破棄するべき。
    DisposeAfterStop, // 再生中の音声が終わったら破棄する。
};

class SoundCore
    : public RefObject {
public:
    void setVolume(float value, float fadeTime, SoundFadeBehavior behavior);
    float volume() const;
    void setPitch(float value);
    void setLoopEnabled(bool value);
    void play();
    void stop(float fadeTime);
    void pause();
    void fadeVolume(float targetVolume, float fadeTime, SoundFadeBehavior behavior);
    void update(float elapsedSeconds);

    void setGameAudioFlags(uint32_t flags) { m_gameAudioFlags = flags; }
    uint32_t gameAudioFlags() const { return m_gameAudioFlags; }

    AudioContext* m_context = nullptr;
    std::mutex m_mutex;

    Ref<detail::ARISourceNode> m_sourceNode;
    Ref<detail::ARIGainNode> m_gainNode;

    std::atomic<SoundCoreLifecycleState> lifecycleState;
    EasingValue<float> m_fadeValue;
    SoundFadeBehavior m_fadeBehavior;
    uint32_t m_gameAudioFlags;
    bool m_fading;

    SoundCore();
    bool init(AudioManager* manager, AudioContext* context, AudioDecoder* decoder);
    void dispose(); // call by audio thread.
};

} // namespace detail
} // namespace ln
