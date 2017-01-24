
#ifndef LUMINO_MATH_MATH_H
#define LUMINO_MATH_MATH_H

#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	一般的な数学関数と定数をまとめたモジュールです。
*/
class LUMINO_EXPORT Math
{
public:

	static const float PI;			///< 円周率	(3.14159265...)
	static const float PI2;			///< 円周率 * 2	(3.14159265...)
	static const float PIDiv2;		///< 円周率 / 2

public:

	/// 度をラジアンに変換します。
	static inline float ToRadians(float d) { return d * 0.017453292519943295769236907684886f; }	// 0.017… は180 の逆数 * PI

	/// ラジアンを度に変換します。
	static inline float ToDegrees(float r) { return r * 57.295779513082320876798154814105f; }	// 57.29… はPI の逆数 * 180

	/// 値を指定された範囲内に制限します。
	static inline float Clamp(float value, float minValue, float maxValue) { return (value > maxValue) ? maxValue : (value < minValue) ? minValue : value; }

	/// 値を0.0～1.0の範囲内に制限します。
	static inline float Clamp01(float value) { return Clamp(value, 0.0f, 1.0f); }

	/// 指定値以上の最小の 2 のべき乗数を返します。
	static unsigned int NextPow2(unsigned int value);

	static void SinCos(float value, float* outSin, float* outCos);

	/// 誤差を考慮し、浮動小数点の比較を行います。
	static bool EqualsFloat(float value1, float value2);

	/// 値が NaN(非数) かを判定します。
	static bool IsNaN(float value);

	/// 値が NaN(非数) かを判定します。
	static bool IsNaN(double value);

	/// 値が無限大かを判定します。
	static bool IsInf(float value);

	/// 値が無限大かを判定します。
	static bool IsInf(double value);

	/// 値が NaN(非数) または 無限大 かを判定します。
	static bool IsNaNOrInf(float value) { return (IsNaN(value) || IsInf(value)); }

	/// 値が NaN(非数) または 無限大 かを判定します。
	static bool IsNaNOrInf(double value) { return (IsNaN(value) || IsInf(value)); }

	/**
		@brief		等加速度運動の式で値を計算します。
		@param[in]	p	: 開始値
		@param[in]	v	: 初速度
		@param[in]	a	: 加速度
		@param[in]	t	: 時間
		@return		計算結果
	*/
	static float QuadAccel(float p, float v, float a, float t);

	/**
		@brief		2つの値の間を線形補完します。
		@param[in]	v1	: 開始値
		@param[in]	v2	: 終了値
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static float Lerp(float v1, float v2, float t);

	/**
		@brief		エルミートスプライン補間を実行します。
		@param[in]	v1	: 開始値
		@param[in]	a1	: 開始値の接線の傾き(速度)
		@param[in]	v2	: 終了値
		@param[in]	a2	: 終了値の接線の傾き(速度)
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static float Hermite(float v1, float a1, float v2, float a2, float t);

	/**
		@brief		Catmull-Rom 補間を実行します。
		@param[in]	v1	: 1番目の位置
		@param[in]	v2	: 2番目の位置 (t = 0.0 のときの値)
		@param[in]	v3	: 3番目の位置 (t = 1.0 のときの値)
		@param[in]	v4	: 4番目の位置
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static float CatmullRom(float v1, float v2, float v3, float v4, float t);

};

LN_NAMESPACE_END

#endif // LUMINO_MATH_MATH_H
