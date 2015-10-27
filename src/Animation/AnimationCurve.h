
#pragma once
#include <Lumino/Animation/Common.h>

LN_NAMESPACE_BEGIN
namespace Animation
{

/// 値を補完するクラスのベースクラス
class AnimationCurve
	: public RefObject
{
public:
	AnimationCurve();
	virtual ~AnimationCurve();

public:
	/// 値の種類
	virtual ValueType GetValueType() const = 0;

	/// 時間の設定 (mTime をそのまま、mCurrentFramePos をループ考慮で設定する)
	virtual void UpdateValue(double time) = 0;

	/// 終端フレーム位置の取得
	virtual double GetLastFrameTime() const = 0;

	void SetWrapMode(WrapMode mode) { m_wrapMode = mode; }
	WrapMode GetWrapMode() const { return m_wrapMode; }

	void SetTime(double time);

private:
	WrapMode	m_wrapMode;
};

/// FloatAnimationCurve のキーフレーム
struct FloatKeyFrame
{
public:
	double				FrameTime;		///< フレーム位置
	float				Value;			///< 値
	float				Velocity;		///< 速度
	float				Accel;			///< 加速度
	float				LeftSlope;		///< 左辺 (終了) 速度
	float				RightSlope;		///< 右辺 (開始) 速度
	InterpolationMode	Mode;			///< 次のキーとの補間方法 (初期値は線形補間)

public:
	FloatKeyFrame()
		: FrameTime(0.0f)
		, Value(0.0f)
		, Velocity(0.0f)
		, Accel(0.0f)
		, LeftSlope(0.0f)
		, RightSlope(0.0f)
		, Mode(InterpolationMode_Linear)
	{}
};

/// float 用
class FloatAnimationCurve
	: public AnimationCurve
{
public:
	FloatAnimationCurve();
	virtual ~FloatAnimationCurve();

public:

	/// キーフレーム追加 (終端より前に追加した場合はソートを行う)
	void AddKeyFrame(const FloatKeyFrame& keyFrame);

	/// キーフレーム追加 (線形補間のキーフレーム)
	void AddKeyFrame(double frame_pos, float value);

	/// 補間結果の取得 (SetTime() で更新される)
	float GetValue() const { return m_value; }

public:
	// override AnimationCurve
	virtual ValueType GetValueType() const { return ValueType_Float; }
	virtual void UpdateValue(double time);
	virtual double GetLastFrameTime() const;

private:
	typedef Array<FloatKeyFrame>	KeyFrameList;
	KeyFrameList	m_keyFrameList;
	float			m_defaultValue;
	float			m_value;
};

/// VMD用Bezier
class VMDBezierTable
{
public:
	/// 初期化
	void Initialize(float pt_x1, float pt_y1, float pt_x2, float pt_y2);

	/// t = 0.0～1.0
	float GetInterValue(float t);

private:
	float GetYVal(float x, float x1, float y1, float x2, float y2);

private:
	static const int	YVAL_DIV_NUM = 16;
	float	            m_yValue[YVAL_DIV_NUM + 1];
	bool	            m_isLinear;
};


/// VMD用
class VMDBezierSQTTransformAnimation
	: public AnimationCurve
{
public:
	struct KeyFrame
	{
		double			Time;				// フレーム位置
		Vector3			Position;			// 位置
		Quaternion		Rotation;			// 回転(クォータニオン)

		VMDBezierTable	PosXInterBezier;	// X軸移動補間
		VMDBezierTable	PosYInterBezier;	// Y軸移動補間
		VMDBezierTable	PosZInterBezier;	// Z軸移動補間
		VMDBezierTable	RotInterBezier;	    // 回転補間
	};

public:
	VMDBezierSQTTransformAnimation();
	virtual ~VMDBezierSQTTransformAnimation();

public:

	/// キーフレーム追加
	void AddKeyFrame(
		double framePos, const Vector3& pos, const Quaternion& rot,
		char* interpolation_x,
		char* interpolation_y,
		char* interpolation_z,
		char* interpolation_rot);

	/// キーフレームのソート
	void SortKeyFrame();

	/// 補間結果の取得 (SetTime() で更新される)
	const SQTTransform& GetValue() const { return m_transform; }

public:
	virtual ValueType GetValueType() const { return ValueType_SQTTransform; }
	virtual void UpdateValue(double time);
	virtual double GetLastFrameTime() const;

private:
	typedef Array<KeyFrame>	KeyFrameList;
	KeyFrameList	m_keyFrameList;
	SQTTransform	m_transform;
};

} // namespace Animation
LN_NAMESPACE_END
