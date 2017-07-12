
#pragma once
LN_NAMESPACE_BEGIN

/**
	@brief		ゲーム中の時間情報を表します。
*/
class GameTime final
{
public:
	
	/**
		@brief		前回のフレームから経過したゲーム時間 (秒) を取得します。
	*/
	static double getElapsedGameTime();

	/**
		@brief		前回のフレームから経過した実時間 (秒) を取得します。
	*/
	static double getElapsedRealTime();

	/**
		@brief		開始からの総ゲーム時間 (秒) を取得します。
	*/
	static double getTotalGameTime();

	/**
		@brief		開始からの総実時間 (秒) を取得します。
	*/
	static double getTotalRealTime();
};

LN_NAMESPACE_END
