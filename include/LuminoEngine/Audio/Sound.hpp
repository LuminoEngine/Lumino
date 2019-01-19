#pragma once
#include <mutex>
#include "../Animation/EasingFunctions.hpp"

namespace ln {
class AudioContext;
class AudioSourceNode;
class AudioGainNode;
namespace detail { class GameAudioImpl; }

/** 音声の再生状態を表します。*/
enum class SoundPlayingState
{
    Stopped,		/**< 停止中 */
    Playing,			/**< 再生中 */
    Pausing,			/**< 一時停止中 */
};

/** 音量フェード完了時の動作を表します。*/
enum class SoundFadeBehavior
{
    Continue,		/**< 再生を継続する */
    stop,				/**< 停止する */
    StopReset,			/**< 停止して、次の再生に備えてサウンドの音量を元の値に戻す */
    pause,				/**< 一時停止する */
    PauseReset,			/**< 一時停止して、次の再生に備えてサウンドの音量を元の値に戻す */
};

class Sound
	: public Object
{
public:
    ///** Sound クラスのインスタンスを作成します。 */
    //static SoundPtr create(const StringRef& filePath);

    ///** Sound クラスのインスタンスを作成します。 */
    //static SoundPtr create(Stream* stream, SoundLoadingMode loadingMode);

    
	/**
	 * この Sound の音量を設定します。
	 * @param[in]	volume	: 音量 (0.0～1.0。初期値は 1.0)
	 */
	LN_METHOD(Property)
	void setVolume(float volume);

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
	void setPitch(float pitch);

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
	void fadeVolume(float targetVolume, double time, SoundFadeBehavior behavior);

LN_CONSTRUCT_ACCESS:
    Sound();
    virtual ~Sound();
    void init(const StringRef& filePath);
    virtual void dispose() override;

private:
    void setGameAudioFlags(uint32_t flags) { m_gameAudioFlags = flags; }
    uint32_t gameAudioFlags() const { return m_gameAudioFlags; }
    void process(float elapsedSeconds);
    void playInternal();
    void stopInternal();
    void pauseInternal();
    void setVolumeInternal(float volume);

    Ref<AudioSourceNode> m_sourceNode;
    Ref<AudioGainNode> m_gainNode;
    uint32_t m_gameAudioFlags;

    std::mutex m_playerStateLock;    // TODO: 性質上、スピンロックの方が効率がいいかもしれない
    EasingValue<float> m_fadeValue;
    SoundFadeBehavior m_fadeBehavior;
    bool m_fading;

    friend class detail::GameAudioImpl;
    friend class AudioContext;
};

} // namespace ln

