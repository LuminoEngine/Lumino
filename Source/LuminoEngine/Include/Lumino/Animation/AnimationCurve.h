
#pragma once
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Reflection/Property.h>
#include <Lumino/Animation/Common.h>
#include <Lumino/Animation/AnimationUtilities.h>

LN_NAMESPACE_BEGIN

class AnimationCurveInstanceBase
	: public RefObject
{
public:
	Ref<AnimationCurve>	owner;
	//Object*			targetObject;		// TODO: アニメ実行中に Target がデストラクトされた時の対応
	//const tr::PropertyInfo*	targetProperty;
	//Variant		StartValue;
	bool			isActive;			// true の場合、実際に再生する (古い再生を停止するときに false にする。本来はリストから delete しても良いのだが、メモリ効率的に。)

public:
	virtual bool applyPropertyAnimation(double time) = 0;
	//virtual AnimatableObject* GetTargetObject() = 0;
	virtual const tr::PropertyInfo* getTargetPropertyInfo() = 0;
};

template<typename TValue> class ValueEasingCurve;

/// 値を補完するクラスのベースクラス
class AnimationCurveLG
	: public RefObject
{
public:
	AnimationCurveLG();
	virtual ~AnimationCurveLG();

public:
	/// 値の種類
	virtual ValueType getValueType() const = 0;

	/// 時間の設定 (mTime をそのまま、mCurrentFramePos をループ考慮で設定する)
	virtual void updateValue(double time) = 0;

	/// 終端フレーム位置の取得
	virtual double getLastFrameTime() const = 0;

	void setWrapMode(WrapMode mode) { m_wrapMode = mode; }
	WrapMode getWrapMode() const { return m_wrapMode; }

	void getTime(double time);

private:
	WrapMode	m_wrapMode;
};





namespace detail {

template<typename TValue>
class ValueEasingCurveInstance
	: public AnimationCurveInstanceBase
{
public:
	ValueEasingCurve<TValue>*	m_owner;
	TValue		m_startValue;
	TValue		m_targetValue;
	tr::PropertyRef<TValue> m_targetProperty;

	ValueEasingCurveInstance(ValueEasingCurve<TValue>* owner_, const tr::PropertyRef<TValue>& targetProperty, const TValue& startValue, const TValue& targetValue)
		: m_owner(owner_)
		, m_startValue(startValue)
		, m_targetValue(targetValue)
		, m_targetProperty(targetProperty)
	{
		owner = owner_;
		isActive = true;
	}

	virtual bool applyPropertyAnimation(double time) override
	{
		return applyPropertyAnimationInternal(time);
	}

	//virtual AnimatableObject* GetTargetObject() override
	//{
	//	return static_cast<AnimatableObject*>(m_targetProperty.getOwenr().Get());
	//}

	virtual const tr::PropertyInfo* getTargetPropertyInfo() override
	{
		return m_targetProperty.getPropertyInfo();
	}

private:
	bool applyPropertyAnimationInternal(double time);
};

} // namespace detail


template<typename TValue>
class ValueEasingCurve
	: public AnimationCurveLG
{
public:

	//static FloatEasing* create(const String& targetName, const String& targetProperty, float targetValue, float duration, Animation::EasingMode easingMode);
	virtual ValueType getValueType() const { return ValueType_Float; }
	virtual void updateValue(double time) {}
	virtual double getLastFrameTime() const { return 0; }

public:
	static Ref<ValueEasingCurve<TValue>> create(TValue targetValue, double duration, EasingMode easingMode)
	{
		return Ref<ValueEasingCurve<TValue>>::makeRef(targetValue, duration, easingMode);
	}

	ValueEasingCurve(TValue targetValue, double duration, EasingMode easingMode)
		: m_targetValue(targetValue)
		, m_duration(duration)
		, m_easingMode(EasingMode::Linear)
		, m_easingFunction()
	{
		setEasingMode(easingMode);
	}

	virtual ~ValueEasingCurve() {}

	//void SetTargetName(const String& name) { m_targetName = name; }
	//void SetTargetProperty(const Property* prop) { m_targetProperty = prop; }
	void setTargetValue(TValue value) { m_targetValue = value; }
	void setEasingMode(EasingMode easingMode)
	{
		m_easingMode = easingMode;
		m_easingFunction = AnimationUtilities::SelectEasingFunction<TValue, double>(m_easingMode);
	}

	void setDuration(double duration) { m_duration = duration; }
	double getDuration() const { return m_duration; }



	AnimationCurveInstanceBase* createAnimationCurveInstance(const tr::PropertyRef<TValue>& targetProperty, const TValue& startValue)
	{
		return LN_NEW detail::ValueEasingCurveInstance<TValue>(this, targetProperty, startValue, m_targetValue);
	}
	
	const std::function< TValue(double, TValue, TValue, double) >& getEasingFunction() const { return m_easingFunction; }

private:
	TValue		m_targetValue;
	EasingMode	m_easingMode;
	double		m_duration;
	std::function< TValue(double, TValue, TValue, double) >	m_easingFunction;
};





namespace detail {


//------------------------------------------------------------------------------
template<typename TValue>
bool ValueEasingCurveInstance<TValue>::applyPropertyAnimationInternal(double time)
{
	double duration = m_owner->getDuration();
	auto easingFunction = m_owner->getEasingFunction();

	// 経過時間 0 の場合はそのままセットで良い。0除算対策の意味も込めて。
	// また、時間が既に終端を超えていたり、比較関数が無い場合も直値セット。
	if (duration == 0 || duration <= time || easingFunction == nullptr)
	{
		tr::PropertyInfo::setPropertyValueDirect(m_targetProperty, m_targetValue, tr::PropertySetSource::ByAnimation);
	}
	// 時間が 0 以前の場合は初期値
	else if (time <= 0)
	{
		tr::PropertyInfo::setPropertyValueDirect<TValue>(m_targetProperty, m_startValue, tr::PropertySetSource::ByAnimation);
	}
	// 補間で求める
	else
	{
		tr::PropertyInfo::setPropertyValueDirect<TValue>(m_targetProperty, easingFunction(time, m_startValue, m_targetValue - m_startValue, duration), tr::PropertySetSource::ByAnimation);
	}

	return (time < duration);
}


} // namespace detail


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
	: public AnimationCurveLG
{
public:
	FloatAnimationCurve();
	virtual ~FloatAnimationCurve();

public:

	/// キーフレーム追加 (終端より前に追加した場合はソートを行う)
	void addKeyFrame(const FloatKeyFrame& keyFrame);

	/// キーフレーム追加 (線形補間のキーフレーム)
	void addKeyFrame(double frame_pos, float value, InterpolationMode mode = InterpolationMode_Linear);

	/// 補間結果の取得 (getTime() で更新される)
	float getValue() const { return m_value; }

public:
	// override AnimationCurve
	virtual ValueType getValueType() const { return ValueType_Float; }
	virtual void updateValue(double time);
	virtual double getLastFrameTime() const;

private:
	typedef List<FloatKeyFrame>	KeyFrameList;
	KeyFrameList	m_keyFrameList;
	float			m_defaultValue;
	float			m_value;
};

/// VMD用Bezier
class VMDBezierTable
{
public:
	/// 初期化
	void initialize(float pt_x1, float pt_y1, float pt_x2, float pt_y2);

	/// t = 0.0～1.0
	float getInterValue(float t);

private:
	float getYVal(float x, float x1, float y1, float x2, float y2);

private:
	static const int	YVAL_DIV_NUM = 16;
	float	            m_yValue[YVAL_DIV_NUM + 1];
	bool	            m_isLinear;
};


/// VMD用
class VMDBezierAttitudeTransformAnimation
	: public AnimationCurveLG
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
	VMDBezierAttitudeTransformAnimation();
	virtual ~VMDBezierAttitudeTransformAnimation();

public:

	/// キーフレーム追加
	void addKeyFrame(
		double framePos, const Vector3& pos, const Quaternion& rot,
		char* interpolation_x,
		char* interpolation_y,
		char* interpolation_z,
		char* interpolation_rot);

	/// キーフレームのソート
	void sortKeyFrame();

	/// 補間結果の取得 (getTime() で更新される)
	const AttitudeTransform& getValue() const { return m_transform; }

public:
	virtual ValueType getValueType() const { return ValueType_SQTTransform; }
	virtual void updateValue(double time);
	virtual double getLastFrameTime() const;

private:
	typedef List<KeyFrame>	KeyFrameList;
	KeyFrameList	m_keyFrameList;
	AttitudeTransform	m_transform;
};

class VMDBezierSQTTransformAnimation2
	: public AnimationCurveLG
{
public:
	struct BezierCurve
	{
		float Epsilon = 1.0e-3f;

		Vector2 v1;
		Vector2 v2;

		float evaluate(float Progress)
		{
			//ニュートン法による近似
			float t = Math::clamp(Progress, 0, 1);
			float dt;
			do
			{
				dt = -(fx(t) - Progress) / dfx(t);
				if (Math::isNaN(dt))
					break;
				t += Math::clamp(dt, -1.0f, 1.0f);//大幅に移動して別の解に到達するのを防止する用
			} while (std::abs(dt) > Epsilon);
			return Math::clamp(fy(t), 0.0f, 1.0f);//念のため、0-1の間に収まるようにした
		}
	private:
		//fy(t)を計算する関数
		float fy(float t)
		{
			//fy(t)=(1-t)^3*0+3*(1-t)^2*t*v1.y+3*(1-t)*t^2*v2.y+t^3*1
			return 3 * (1 - t) * (1 - t) * t * v1.y + 3 * (1 - t) * t * t * v2.y + t * t * t;
		}
		//fx(t)を計算する関数
		float fx(float t)
		{
			//fx(t)=(1-t)^3*0+3*(1-t)^2*t*v1.x+3*(1-t)*t^2*v2.x+t^3*1
			return 3 * (1 - t) * (1 - t) * t * v1.x + 3 * (1 - t) * t * t * v2.x + t * t * t;
		}
		//dfx/dtを計算する関数
		float dfx(float t)
		{
			//dfx(t)/dt=-6(1-t)*t*v1.x+3(1-t)^2*v1.x-3t^2*v2.x+6(1-t)*t*v2.x+3t^2
			return -6 * (1 - t) * t * v1.x + 3 * (1 - t) * (1 - t) * v1.x
				- 3 * t * t * v2.x + 6 * (1 - t) * t * v2.x + 3 * t * t;
		}
	};


	struct BoneFrameData
	{
		double			Time;					// フレーム位置
		Vector3			Position;				// 位置
		Quaternion		Rotation;	// 回転(クォータニオン)
		std::array<BezierCurve, 4>		Curves;

		//BoneFrameData& operator=(const BoneFrameData& data)
		//{
		//	Time = data.Time;
		//	Position = data.Position;
		//	Rotation = data.Rotation;
		//	Curves[0] = data.Curves[0];
		//}
	};

public:
	VMDBezierSQTTransformAnimation2();
	virtual ~VMDBezierSQTTransformAnimation2();

public:
	void addFrame(const BoneFrameData& frame) { m_keyFrameList.add(frame); }
	void sortKeyFrame();

public:
	virtual ValueType getValueType() const { return ValueType_SQTTransform; }
	virtual void updateValue(double time);
	virtual double getLastFrameTime() const;

private:
	typedef List<BoneFrameData>	KeyFrameList;
	KeyFrameList		m_keyFrameList;
	AttitudeTransform	m_transform;
};

LN_NAMESPACE_END
