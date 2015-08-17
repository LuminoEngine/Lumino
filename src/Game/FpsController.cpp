
#include "../Internal.h"
#include <Lumino/Threading/Thread.h>
#include <Lumino/Game/FpsController.h>

namespace Lumino
{

/*
待機だけを行いたい場合は初期化後、process() を呼び出すだけで
//              コンストラクタに渡されたフレームレートになるように待機します。
//
//  @note
//              自分のノート PC で、なんか動きが妙にぎこちないと思って
//              1フレームの経過時間を調べたら、不定期で1フレーム70msかかる
//              ものがあった。(ほんとに不定期。1秒(60回に0回だったり4回だったり。)
//              他のフレームではほぼ0.016キープ)
//              詳しく調べてみると、winAPI の Sleep がたまにすごく時間かかってた。
//              Sleep( 16 ) で待機してるのに、さっきと同じく 70ms かかってたとか。
//              Sleep() は確実に指定した時間だけ待ってくれる関数じゃない
//              ことはわかってたけど、環境によってここまで変わるとは
//              (デスクトップの方では問題なかった)正直思わなかったので、
//              もし今後似たようなことがあった場合はこの辺を参考にしてみる。
*/
//==============================================================================
// FpsController
//==============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FpsController::FpsController()
	: m_frameRate(0)
	, m_frameRateRec(0.0)
	, m_fps(0.0)
	, m_currentTime(0.0)
	, m_averageTime(0.0)
	, m_frameCount(0)
	, m_term(0.0)
	, m_lastTime(0.0)
	, m_baseTime(0.0)
	, m_frameTimes(NULL)
	, m_elapsedGameTime(0.0)
	, m_lastRealTime(0.0)
	, m_elapsedRealTime(0.0)
	, m_startTime(Environment::GetTickCount())
	, m_totalGameTime(0)
	, m_totalRealTime(0)
	, m_capacityFps(0.0)
	, m_capaFpsLastTime(0.0)
	, m_capaFrameTimes(NULL)
	, m_capaAverageTime(0.0)
	, m_enableFpsTest(false)
{
	SetFrameRate(60);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FpsController::~FpsController()
{
	LN_SAFE_DELETE_ARRAY(m_frameTimes);
	LN_SAFE_DELETE_ARRAY(m_capaFrameTimes);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FpsController::SetFrameRate(int frameRate)
{
	if (LN_VERIFY_ASSERT(frameRate > 0)) { return; }

	m_frameRate = frameRate;
	m_frameRateRec = 1.0f / m_frameRate;
	m_frameCount = 0;

	LN_SAFE_DELETE_ARRAY(m_frameTimes);
	m_frameTimes = LN_NEW float[m_frameRate];

	LN_SAFE_DELETE_ARRAY(m_capaFrameTimes);
	m_capaFrameTimes = LN_NEW float[m_frameRate];

	m_term = 0.0f;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FpsController::RefreshSystemDelay()
{
	uint64_t currentTime = Environment::GetTickCount();
	m_lastTime = m_baseTime = m_capaFpsLastTime = (0.001f * (currentTime - m_startTime));// + m_frameRateRec;
	m_baseTime -= 1.0f - m_frameRateRec;

	// 現在の m_frameCount までのフレーム数から平均時間を計算する
	m_averageTime = 0;
	for (int i = 0; i < m_frameCount; ++i)
	{
		m_averageTime += m_frameTimes[i];
	}
	m_averageTime /= m_frameCount;

	m_frameCount = 0;
	m_term = 0.0f;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FpsController::Process()
{
	//if (!mEnableFrameWait) {
	//	processForMeasure();
	//	return;
	//}

	m_currentTime = 0.001f * (Environment::GetTickCount() - m_startTime);

	m_elapsedGameTime = m_currentTime - m_lastTime;
	m_elapsedRealTime = m_currentTime - m_lastRealTime;

	m_totalGameTime += static_cast< uint32_t >(1000.0f * m_elapsedGameTime);
	m_totalRealTime += static_cast< uint32_t >(1000.0f * m_elapsedRealTime);

	// (frame_rate_)フレームの1回目なら
	if (m_frameCount == 0)
	{
		// 完全に最初
		if (m_lastTime == 0.0)
		{
			m_term = m_frameRateRec;
		}
		// 前回記録した時間を元に計算
		else
		{
			m_term = m_baseTime + 1.0f - m_currentTime;
		}

		//printf( "f: a:%f bt:%f rc:%f ct:%f lt:%f\n", m_term, m_baseTime, m_frameRateRec * m_frameCount, m_currentTime, m_currentTime - m_lastRealTime );
	}
	else
	{
		// 今回待つべき時間 = 現在あるべき時刻 - 現在の時刻
		m_term = (m_baseTime + m_frameRateRec * m_frameCount) - m_currentTime;

		//if ( m_term < 0 )
		//{
		//    printf( "c:%f\n", m_term );
		//}
		//printf( "s: a:%f bt:%f rc:%f ct:%f lt:%f\n", m_term, m_baseTime, m_frameRateRec * m_frameCount, m_currentTime, m_currentTime - m_lastRealTime );
	}

	//printf( "frame:%d term:%f etime:%f\n", m_frameCount, m_term, m_elapsedRealTime );
	//printf( "etime:%f term:%f \n", m_elapsedRealTime, m_term );
	//static int ii = 0;
	//++ii;

	// 待つべき時間だけ待つ
	if (m_term > 0.0)
	{
		//printf( "t:%lf c:%d\n", m_term, m_frameCount );


		//if ( ii > 120 && m_term < 0.016 )
		{
			/*
			ii = 120;

			double bb = 0;
			for ( int i = 0; i < 60; ++i )
			{
			_p( m_frameTimes[ i ] );
			bb += m_frameTimes[ i ];
			}
			_p((double)ii / 60.0);

			printf( "a\n" );
			*/
		}

		//::Sleep( 16 );

		Threading::Thread::Sleep(static_cast< uint32_t >(m_term * 1000));
	}

	// 現在の時間
	float nt = m_currentTime;

	// m_frameRate フレームに1度基準を作る
	if (m_frameCount == 0)
	{
		// + m_frameRateRec で1フレームずらさないと、
		// 60 フレーム中の 1 フレーム目で待ち時間の計算をするときに
		// m_currentTime の時間が現在あるべき時刻を超えてしまって、
		// 待ち時間が -0.0003 とかになってしまう。
		// 結果、一瞬だけｶｸｯという動作をすることがあった。
		m_baseTime = nt + m_frameRateRec;
	}

	// 今回の分の1周した時間を記録
	m_frameTimes[m_frameCount] = nt - m_lastTime;

	// 現在の時間を、ひとつ前の時間として記憶
	m_lastRealTime = nt;
	m_lastTime = nt;

	// m_frameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
	if (m_frameCount == m_frameRate - 1)
	{
		m_averageTime = 0;
		for (int i = 0; i < m_frameRate; ++i)
		{
			m_averageTime += m_frameTimes[i];
		}
		m_averageTime /= m_frameRate;
	}

	// FPS 余裕度を測定する
	if (m_enableFpsTest)
	{
		// 今回の分の1周した時間を記録
		m_capaFrameTimes[m_frameCount] = m_currentTime - m_capaFpsLastTime;

		// m_frameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
		if (m_frameCount == m_frameRate - 1)
		{
			m_capaAverageTime = 0;
			for (int i = 0; i < m_frameRate; ++i)
			{
				m_capaAverageTime += m_capaFrameTimes[i];
			}

			if (m_capaAverageTime == 0.0f)
			{
				m_capaAverageTime = 0.01f;
			}
			m_capaAverageTime /= m_frameRate;

			m_capacityFps = 1.0f / m_capaAverageTime;
			if (m_capacityFps > 100 * m_frameRate)
			{
				m_capacityFps = 100.0f * m_frameRate;
			}
		}

		m_capaFpsLastTime = 0.001f * (Environment::GetTickCount() - m_startTime);
	}

	// m_frameCount を frame で一周するようにする
	m_frameCount = (++m_frameCount) % m_frameRate;

	m_fps = (m_averageTime > 0) ? (1.0f / m_averageTime) : 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FpsController::ProcessForMeasure()
{
	m_currentTime = 0.001f * (Environment::GetTickCount() - m_startTime);

	m_elapsedGameTime = m_currentTime - m_lastTime;
	m_elapsedRealTime = m_currentTime - m_lastRealTime;

	m_totalGameTime += static_cast< uint32_t >(1000.0f * m_elapsedGameTime);
	m_totalRealTime += static_cast< uint32_t >(1000.0f * m_elapsedRealTime);

	// (frame_rate_)フレームの1回目なら
	if (m_frameCount == 0)
	{
		// 完全に最初
		if (m_lastTime == 0.0)
		{
			m_term = m_frameRateRec;
		}
		// 前回記録した時間を元に計算
		else
		{
			m_term = m_baseTime + 1.0f - m_currentTime;
		}
	}
	else
	{
		// 今回待つべき時間 = 現在あるべき時刻 - 現在の時刻
		m_term = (m_baseTime + m_frameRateRec * m_frameCount) - m_currentTime;
	}

	static int ii = 0;

	++ii;

	// 待つべき時間だけ待つ
	if (m_term > 0.0)
	{
		//Threading::sleep( static_cast< uint32_t >( m_term * 1000 ) );
	}

	// 現在の時間
	float nt = m_currentTime;

	// m_frameRate フレームに1度基準を作る
	if (m_frameCount == 0)
	{
		// + m_frameRateRec で1フレームずらさないと、
		// 60 フレーム中の 1 フレーム目で待ち時間の計算をするときに
		// m_currentTime の時間が現在あるべき時刻を超えてしまって、
		// 待ち時間が -0.0003 とかになってしまう。
		// 結果、一瞬だけｶｸｯという動作をすることがあった。
		m_baseTime = nt + m_frameRateRec;
	}

	// 今回の分の1周した時間を記録
	m_frameTimes[m_frameCount] = nt - m_lastTime;

	// 現在の時間を、ひとつ前の時間として記憶
	m_lastRealTime = nt;
	m_lastTime = nt;


	// m_frameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
	if (m_frameCount == m_frameRate - 1)
	{
		m_averageTime = 0;
		for (int i = 0; i < m_frameRate; ++i)
		{
			m_averageTime += m_frameTimes[i];
		}
		m_averageTime /= m_frameRate;
	}

	if (m_enableFpsTest)
	{
		// 今回の分の1周した時間を記録
		m_capaFrameTimes[m_frameCount] = m_currentTime - m_capaFpsLastTime;

		// m_frameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
		if (m_frameCount == m_frameRate - 1)
		{
			m_capaAverageTime = 0;
			for (int i = 0; i < m_frameRate; ++i)
			{
				m_capaAverageTime += m_capaFrameTimes[i];
			}

			if (m_capaAverageTime == 0.0f)
			{
				m_capaAverageTime = 0.01f;
			}
			m_capaAverageTime /= m_frameRate;

			m_capacityFps = 1.0f / m_capaAverageTime;
			if (m_capacityFps > 100 * m_frameRate)
			{
				m_capacityFps = 100.0f * m_frameRate;
			}
		}

		m_capaFpsLastTime = m_currentTime;
	}

	// m_frameCount を frame で一周するようにする
	m_frameCount = (++m_frameCount) % m_frameRate;

	m_fps = (m_averageTime > 0) ? (1.0f / m_averageTime) : 0;
}

} // namespace Lumino
