
#pragma once
#include <Lumino/Base/NonCopyable.h>

LN_NAMESPACE_BEGIN

/**
	@brief		FPS (Frame Par Second) の制御クラス
*/
class FpsController
    : public NonCopyable
{
public:
	FpsController();
	~FpsController();

public:
	
	/**
		@brief		現在の FPS 値を取得します。
	*/
	float GetFps() const { return (m_averageTime > 0) ? (1.0f / m_averageTime) : 0; }

	/**
		@brief		前回のフレームから経過したゲーム時間 (秒) を取得します。
	*/
	float GetElapsedGameTime() const { return m_elapsedGameTime; }

	/**
		@brief		前回のフレームから経過した実時間 (秒) を取得します。
	*/
	float GetElapsedRealTime() const { return m_elapsedRealTime; }

	/**
		@brief		開始からの総ゲーム時間 (秒) を取得します。
	*/
	float GetTotalGameTime() const { return 0.001f * m_totalGameTime; }

	/**
		@brief		開始からの総実時間 (秒) を取得します。
	*/
	float GetTotalRealTime() const { return 0.001f * m_totalRealTime; }

	/**
		@brief		最大 FPS 値を取得します。
		@details	処理にどれだけ余裕があるかを示す値を取得します。
					この値がフレームレートの値よりも大きいほど、処理に余裕があります。<br>
					この関数で値を取得できるようにするには、SetEnableFpsTest() に true を設定してください。
	*/
	float GetCapacityFps() const { return m_capacityFps; }

	/**
		@brief		フレームレートを設定します。初期値は 60 です。
		@param[in]	frameRate	: フレームレート
	*/
	void SetFrameRate(int frameRate);

	/**
		@brief		フレームレートを取得します。
	*/
    int GetFrameRate() const { return m_frameRate; }

	/**
		@brief		Fps 余裕度を測定するかを設定します。初期値は false です。
	*/
	void SetEnableFpsTest(bool enabled) { m_enableFpsTest = enabled; }

	/**
		@brief		フレームレート分のフレームが経過した瞬間を判定します。
		@details	フレームレートが 60 であれば、60 フレームに 1 度の間 true を返します。
	*/
	bool IsRoundFrame() const { return (m_frameCount == 0); }

	/**
		@brief		遅延が発生しているかを確認します。
		@details	この関数が true を返した場合、前回のフレームは フレームレート以内の時間で
					完了しなかったことを表します。
	*/
	bool IsFrameDelay() const { return (m_term <= 0); }

	/**
		@brief		遅延をリセットします。
		@details	リソースのロード等で時間がかかり長い時間更新処理が行われなかった場合、
					本来あるべき時間に追いつこうとしてしばらくの間ノーウェイトでフレーム更新が行われます。
					その間はアプリケーションが非常に高速に動作しているように見えてしまします。
					これを回避するため、時間のかかる処理の直後でこの関数を呼ぶことで、FPS 制御に遅延が発生していないことを伝えます。
	*/
	void RefreshSystemDelay();

private:	// internal
	friend class EngineManager;
	void Process();
    void ProcessForMeasure();	// ウェイトは取らず、測定のみ行う (ツール用)

private:
	int			m_frameRate;		///< フレームレート
	float		m_frameRateRec;		///< フレームレートの逆数
	float		m_fps;				///< 現在の FPS 値
    
	float		m_currentTime;		///< 現在の時間 ( システム起動からの時間 )
	float		m_averageTime;		///< 1 フレームの平均時間

	int		    m_frameCount;		///< fps 計測用の経過フレーム数 ( 60fpsにする場合は0～60でループ )
	float		m_term;				///< wait() 内で使う待ち時間
	float		m_lastTime;			///< 1 周前の時間を記憶する変数
	float		m_baseTime;			///< 1 秒にに一度つくられる基準時間
	float*		m_frameTimes;		///< 各フレームの時間を格納する配列 ( 平均の計算に使う )

    float		m_elapsedGameTime;	///< ひとつ前のフレームからの経過ゲーム時間
    float		m_lastRealTime;		///< ひとつ前のフレームの実時間
    float		m_elapsedRealTime;	///< ひとつ前のフレームからの経過実時間

    uint64_t	m_startTime;		///< コンストラクタで設定したスタート時間
    uint64_t	m_totalGameTime;	///< 経過ゲーム時間の合計
    uint64_t	m_totalRealTime;	///< 経過実時間の合計

    float		m_capacityFps;
    float		m_capaFpsLastTime;
    float*		m_capaFrameTimes;	///< 各フレームの時間を格納する配列 ( 平均の計算に使う )
    float		m_capaAverageTime;

    bool		m_enableFpsTest;		///< true の場合、FPS テストを行う
};

LN_NAMESPACE_END
