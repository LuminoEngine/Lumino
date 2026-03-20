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
    static inline constexpr float degreesToRadians(float d) noexcept { return d * 0.017453292519943295769236907684886f; }

    /** ラジアンを度に変換します。 */
    static inline constexpr float radiansToDegrees(float r) noexcept { return r * 57.295779513082320876798154814105f; }

    /** 値を指定された範囲内に制限します。 */
    static inline constexpr float clamp(float value, float minValue, float maxValue) noexcept { return (value > maxValue) ? maxValue : (value < minValue) ? minValue : value; }

    /** 値を0.0～1.0の範囲内に制限します。 */
    static inline constexpr float clamp01(float value) noexcept { return clamp(value, 0.0f, 1.0f); }

    /** 指定値以上の最小の 2 のべき乗数を返します。 */
    static unsigned int nextPow2(unsigned int value);

    /** sin と cos を計算します。 */
    static void sinCos(float value, float* outSin, float* outCos);

    /** 誤差を考慮し、浮動小数点の比較を行います。 */
    static bool nearEqual(float value1, float value2) noexcept;

    /** 誤差を考慮し、浮動小数点の比較を行います。 */
    static bool nearEqual(double value1, double value2) noexcept;

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
    static constexpr float quadAccel(float p, float v, float a, float t) noexcept;

    /**
	 * 2つの値の間を線形補間します。
	 * @param[in]	v1	: 開始値
	 * @param[in]	v2	: 終了値
	 * @param[in]	t	: 加重係数
	 * @return		補間結果の値
	 * t は通常、0.0～1.0 を指定します。
	 */
    static constexpr float lerp(float v1, float v2, float t) noexcept;

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
    static constexpr float hermite(float v1, float a1, float v2, float a2, float t) noexcept;

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
    static constexpr float catmullRom(float v1, float v2, float v3, float v4, float t) noexcept;

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
    static constexpr float cubicBezier(float v1, float v2, float v3, float v4, float t) noexcept;
};


inline bool Math::nearEqual(float value1, float value2) noexcept {
    return std::abs(value1 - value2) < FLT_EPSILON;
}

inline bool Math::nearEqual(double value1, double value2) noexcept {
    return std::abs(value1 - value2) < DBL_EPSILON;
}

constexpr float Math::quadAccel(float p, float v, float a, float t) noexcept
{
	return p + (v * t) + (0.5f * a * t * t);
}

constexpr float Math::lerp(float v1, float v2, float t) noexcept
{
	return v1 + ((v2 - v1) * t);
}

constexpr float Math::hermite(float v1, float a1, float v2, float a2, float t) noexcept
{
	float a = 2.f * (v1 - v2) + (a1 + a2);
	float b = 3.f * (v2 - v1) - (2.f * a1) - a2;
	float r = a;
	r *= t;
	r += b;
	r *= t;
	r += a1;
	r *= t;
	return r + v1;
}

constexpr float Math::catmullRom(float v1, float v2, float v3, float v4, float t) noexcept
{
	float d1 = (v3 - v1) * 0.5f;
	float d2 = (v4 - v2) * 0.5f;
	return (2.0f * v2 - 2.0f * v3 + d1 + d2) * t * t * t + (-3.0f * v2 + 3.0f * v3 - 2.0f * d1 - d2) * t * t + d1 * t + v2;
}

constexpr float Math::cubicBezier(float v1, float v2, float v3, float v4, float t) noexcept
{
	float tp = 1.f - t;
	return t * t * t * v4 + 3.f * t * t * tp * v3 + 3.f * t * tp * tp * v2 + tp * tp * tp * v1;
}

} // namespace ln
