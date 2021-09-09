#pragma once

namespace ln {

/** 時間情報を取得します。 */
	// TODO : Engine に統合、でいいかも。いまのところクラス分けるほどたくさん関数作る予定無し
LN_CLASS(Static)
class Time
{
public:
	/** 最後のフレームからの秒単位の完了時間 (秒) を取得します。 */
	LN_METHOD()
	static float deltaTime();

	/**
	 * アプリケーション起動からの経過時間 (秒) を取得します。 */
	LN_METHOD()
	static double totalTime();
};

} // namespace ln

