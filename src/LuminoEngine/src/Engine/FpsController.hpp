#pragma once

namespace ln {
namespace detail {
class EngineManager;
class DllLoader;

class FpsController
{
public:
    FpsController();

    void setFrameRate(int frameRate);
    void setMeasurementEnabled(bool enabled) { m_measureTimes = enabled; }

    void process();
    void processForMeasure();    // ウェイトは取らず、測定のみ行う (ツール用)
    

    // 前回のフレームから経過したゲーム時間 (秒) を取得します。
    float getElapsedGameTime() const { return m_elapsedGameTimeCache; }

    // 開始からの総時間 (秒) を取得します。
    double getTotalGameTime() const { return m_totalGameTimeCache; }



    // フレームレート分のフレームが経過した瞬間を判定します。
    // フレームレートが 60 であれば、60 フレームに 1 度の間 true を返します。
    bool isRoundFrame() const { return (m_frameCount == 0); }

    // 遅延が発生しているかを確認します。
    // この関数が true を返した場合、前回のフレームは フレームレート以内の時間で完了しなかったことを表します。
    bool isFrameDelay() const { return (m_term <= 0); }

    // 遅延をリセットします。
    // リソースのロード等で時間がかかり長い時間更新処理が行われなかった場合、
    // 本来あるべき時間に追いつこうとしてしばらくの間ノーウェイトでフレーム更新が行われます。
    // その間はアプリケーションが非常に高速に動作しているように見えてしまします。
    // これを回避するため、時間のかかる処理の直後でこの関数を呼ぶことで、FPS 制御に遅延が発生していないことを伝えます。
    void refreshSystemDelay();






    // measurement results.
    // 現在の FPS 値を取得します。
    float totalFps() const { return m_totalFps; }
    // 最大 FPS 値を取得します。
    // 処理にどれだけ余裕があるかを示す値を取得します。
    // この値がフレームレートの値よりも大きいほど、処理に余裕があります。
    // この関数で値を取得できるようにするには、setEnableFpsTest() に true を設定してください。
    float externalFps() const { return m_externalFps; }
    float minFrameMillisecondsPerSeconds() const { return static_cast<float>(m_minFrameMillisecondsPerSeconds); }
    float maxFrameMillisecondsPerSeconds() const { return static_cast<float>(m_maxFrameMillisecondsPerSeconds); }

private:
    void measureTimes(uint64_t externalElapsedTime, uint64_t frameElapsedTime);

    ElapsedTimer m_timer;
    int m_frameRate;
    uint64_t m_frameTime;

    //ElapsedTimer m_timer;
	//uint64_t m_currentGameTime;
	uint64_t m_startTick;
    int m_frameCount;
    float m_elapsedGameTimeCache;
    double m_totalGameTimeCache;

    bool m_measureTimes;
    std::vector<uint64_t> m_frameTimes;
    std::vector<uint64_t> m_externalTimes;
    float m_totalFps;
    float m_externalFps;
    int64_t m_term;
    uint64_t m_averageTime;
    uint64_t m_externalAverageTime;
    uint64_t m_minTime;
    uint64_t m_maxTime;
    uint64_t m_minFrameMillisecondsPerSeconds;
    uint64_t m_maxFrameMillisecondsPerSeconds;

    Ref<detail::DllLoader> m_winmm;
};

} // namespace detail
} // namespace ln

