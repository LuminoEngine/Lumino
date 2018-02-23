
#pragma once
#include <Lumino/Reflection/ReflectionObject.h>

LN_NAMESPACE_BEGIN

namespace detail
{
class AnimationManager;
}

class AnimationCurve;
class AnimationClip;
class AnimationState;
class Animator;


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

/// 
enum PlayMode
{
	StopSameLayer = 0,	///< 同レイヤーのアニメーションをすべて停止する
	StopAll,			///< 全てのアニメーションを停止する
};

/// 
enum PlayState
{
	PlayState_Stopped = 0,	///< 停止中
	PlayState_Playing,		///< 再生中
	PlayState_Pausing,		///< 一時停止中
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
//LN_ENUM_REFLECTION(EasingMode,
//	Linear,
//	EaseInQuad,
//	EaseOutQuad,
//	EaseInOutQuad,
//	EaseInCubic,
//	EaseOutCubic,
//	EaseInOutCubic,
//	EaseInQuart,
//	EaseOutQuart,
//	EaseInOutQuart,
//	EaseInQuint,
//	EaseOutQuint,
//	EaseInOutQuint,
//	EaseInSine,
//	EaseOutSine,
//	EaseInOutSine,
//	EaseInExpo,
//	EaseOutExpo,
//	EaseInOutExpo,
//	EaseInCirc,
//	EaseOutCirc,
//	EaseInOutCirc);
//LN_ENUM_DECLARE(EasingMode);

LN_NAMESPACE_END
