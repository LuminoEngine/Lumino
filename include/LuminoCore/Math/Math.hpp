// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

#include "Common.hpp"

namespace ln {

/** 一般的な数学関数と定数をまとめたモジュールです。 */
class LN_API Math
{
public:
    /** NaN (Not-a-Number: 非数) */
    static const float NaN;

    /** positive infinity */
    static const float Inf;

    /** 円周率 (3.14159265...) */
    static const float PI;

    /** 円周率 * 2 (3.14159265...) */
    static const float PI2;

    /** 円周率 / 2 */
    static const float PIDiv2;

public:
    /** 度をラジアンに変換します。 */
    static inline float degreesToRadians(float d) { return d * 0.017453292519943295769236907684886f; }

    /** ラジアンを度に変換します。 */
    static inline float radiansToDegrees(float r) { return r * 57.295779513082320876798154814105f; }

    /** 値を指定された範囲内に制限します。 */
    static inline float clamp(float value, float minValue, float maxValue) { return (value > maxValue) ? maxValue : (value < minValue) ? minValue : value; }

    /** 値を0.0～1.0の範囲内に制限します。 */
    static inline float clamp01(float value) { return clamp(value, 0.0f, 1.0f); }

    /** 指定値以上の最小の 2 のべき乗数を返します。 */
    static unsigned int nextPow2(unsigned int value);

    /** sin と cos を計算します。 */
    static void sinCos(float value, float* outSin, float* outCos);

    /** 誤差を考慮し、浮動小数点の比較を行います。 */
    static bool nearEqual(float value1, float value2);

    /** 値が NaN(非数) かを判定します。 */
    static bool isNaN(float value);

    /** 値が NaN(非数) かを判定します。 */
    static bool isNaN(double value);

    /** 値が無限大かを判定します。 */
    static bool isInf(float value);

    /** 値が無限大かを判定します。 */
    static bool isInf(double value);

    /** 値が NaN(非数) または 無限大 かを判定します。 */
    static bool isNaNOrInf(float value) { return (isNaN(value) || isInf(value)); }

    /** 値が NaN(非数) または 無限大 かを判定します。 */
    static bool isNaNOrInf(double value) { return (isNaN(value) || isInf(value)); }

    /**
	 * 等加速度運動の式で補間した値を計算します。
	 * @param[in]    p    : 開始値
	 * @param[in]    v    : 初速度
	 * @param[in]    a    : 加速度
	 * @param[in]    t    : 時間
	 * @return        計算結果
	 */
    static float quadAccel(float p, float v, float a, float t);

    /**
	 * 2つの値の間を線形補間します。
	 * @param[in]	v1	: 開始値
	 * @param[in]	v2	: 終了値
	 * @param[in]	t	: 加重係数
	 * @return		補間結果の値
	 * t は通常、0.0～1.0 を指定します。
	 */
    static float lerp(float v1, float v2, float t);

    /**
	 * エルミートスプライン補間を実行します。
	 * @param[in]    v1    : 開始値
	 * @param[in]    a1    : 開始値の接線の傾き(速度)
	 * @param[in]    v2    : 終了値
	 * @param[in]    a2    : 終了値の接線の傾き(速度)
	 * @param[in]    t    : 加重係数
	 * @return        補間結果の値
	 * t は通常、0.0～1.0 を指定します。
	 */
    static float hermite(float v1, float a1, float v2, float a2, float t);

    /**
	 * Catmull-Rom 補間を実行します。
	 * @param[in]    v1    : 1番目の位置
	 * @param[in]    v2    : 2番目の位置 (t = 0.0 のときの値)
	 * @param[in]    v3    : 3番目の位置 (t = 1.0 のときの値)
	 * @param[in]    v4    : 4番目の位置
	 * @param[in]    t    : 加重係数
	 * @return        補間結果の値
	 * t は通常、0.0～1.0 を指定します。 
	 */
    static float catmullRom(float v1, float v2, float v3, float v4, float t);

    /**
	 * 3次ベジェ 補間を実行します。
	 * @param[in]    v1    : 始点
	 * @param[in]    v2    : 1番目の制御点
	 * @param[in]    v3    : 2番目の制御点
	 * @param[in]    v4    : 終点
	 * @param[in]    t    : 加重係数
	 * @return        補間結果の値
	 * t は通常、0.0～1.0 を指定します。
	 */
    static float cubicBezier(float v1, float v2, float v3, float v4, float t);
};

} // namespace ln
