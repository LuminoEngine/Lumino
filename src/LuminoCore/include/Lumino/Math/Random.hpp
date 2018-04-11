﻿
#ifndef LUMINO_MATH_RANDOM_H
#define LUMINO_MATH_RANDOM_H

#include "Common.hpp"

namespace ln {

/**
	@brief		疑似乱数を生成するのクラスです。
	@details	アルゴリズムは Xorshift 法です。
*/
class LN_API Randomizer
{
public:

	/**
		@brief		現在の時間に応じて決定されるシード値を使用してインスタンスを初期化します。
	*/
	Randomizer();

	/**
		@brief		指定したシード値を使用してインスタンスを初期化します。
		@param[in]	seed	: シード値
	*/
	Randomizer(int seed);

public:

	/**
		@brief		シード値を変更します。疑似乱数生成器が初期状態にリセットされます。
		@param[in]	seed	: シード値
	*/
	void setSeed(int seed);

	/**
		@brief		整数ランダム値を返します。
		@return		0 以上の整数値
	*/
	int getInt();

	/**
		@brief		指定した最大値より小さい整数ランダム値を返します。(0 <= 値 < maxValue)
		@param[in]	maxValue	: 上限値
		@return		0 以上で maxValue より小さい整数値
	*/
	int getInt(int maxValue);

	/**
		@brief		指定した範囲内の整数ランダム値を返します。(minValue <= 値 < maxValue)
		@param[in]	minValue	: 下限値
		@param[in]	maxValue	: 上限値
		@return		minValue 以上で maxValue より小さい整数値
	*/
	int getIntRange(int minValue, int maxValue);

	/**
		@brief		指定した振れ幅の整数ランダム値を返します。(median-width <= 値 < median+width)
		@param[in]	median	: 中央値
		@param[in]	width	: ±幅
		@return		median-width 以上で median+width より小さい整数値
	*/
	int getIntWidth(int median, int width);

	/**
		@brief		実数ランダム値を返します。
		@return		0.0 以上 1.0 未満の実数値
	*/
	float getFloat();

	/**
		@brief		指定した最大値より小さい実数ランダム値を返します。(0 <= 値 < maxValue)
		@param[in]	maxValue	: 上限値
		@return		0 以上で maxValue より小さい実数値
	*/
	float getFloat(float maxValue);

	/**
		@brief		指定した範囲内の実数ランダム値を返します。(minValue <= 値 < maxValue)
		@param[in]	minValue	: 下限値
		@param[in]	maxValue	: 上限値
		@return		minValue 以上で maxValue より小さい実数値
	*/
	float getFloatRange(float minValue, float maxValue);

	/**
		@brief		指定した振れ幅の実数ランダム値を返します。(median-width <= 値 < median+width)
		@param[in]	median	: 中央値
		@param[in]	width	: ±幅
		@return		median-width 以上で median+width より小さい実数値
	*/
	float getFloatWidth(float median, float width);

private:
	unsigned int x, y, z, w;
};

/**
	@brief	Randomizer のグローバルインスタンス
*/
extern Randomizer Random;

} // namespace ln

#endif // LUMINO_MATH_RANDOM_H
