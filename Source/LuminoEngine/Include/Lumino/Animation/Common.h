
#pragma once
#include <Lumino/Reflection/ReflectionObject.h>

LN_NAMESPACE_BEGIN
class AnimationCurve;

/// AnimationCurve の値の種類
enum ValueType
{
	ValueType_Float = 0,
	ValueType_Vector3,
	ValueType_Quaternion,
	ValueType_SQTTransform,
	//ValueType_Event,
};

/// アニメーションの補間方法
enum InterpolationMode
{
	InterpolationMode_None = 0,		///< 補間なし
	InterpolationMode_Linear,		///< 線形補間
	InterpolationMode_QuadAccel,	///< 等加速度
	InterpolationMode_Hermite,		///< エルミートスプライン補間
	InterpolationMode_CatmullRom,	///< Catmull-Rom 補間
};

/// アニメーションの繰り返し方法
enum WrapMode
{
	WrapMode_Once = 0,		///< 最後まで再生された後、停止する
	WrapMode_Loop,			///< 最後まで再生された後、先頭に戻ってループする
	WrapMode_RoundTrip,		///< 最後まで再生された後、逆方向に戻ってループする
	//WrapMode_PingPong,
	//WrapMode_Default,
	//WrapMode_ClampForever,
};

/** イージング関数の種類を表します。*/
enum class EasingMode
{
	Linear = 0,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,
};

namespace a2
{

/** アニメーションの繰り返し方法 */
enum class AnimationWrapMode
{
	/** 繰り返しを行わず、1度だけ再生します。 */
	Once,

	/** 最後まで再生された後、先頭に戻ってループします。 */
	Loop,

	/** 最後まで再生された後、逆方向に戻ってループします。 */
	//RoundTrip,
};

} // namespace a2
LN_NAMESPACE_END
