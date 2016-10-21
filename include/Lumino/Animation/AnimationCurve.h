
#pragma once
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Reflection/Property.h>
#include <Lumino/Animation/Common.h>
#include <Lumino/Animation/AnimationUtilities.h>

LN_NAMESPACE_BEGIN

class AnimationCurveInstance
	: public RefObject
{
public:
	RefPtr<AnimationCurve>	owner;
	Object*			targetObject;		// TODO: アニメ実行中に Target がデストラクトされた時の対応
	const tr::PropertyInfo*	targetProperty;
	//Variant		StartValue;
	bool			isActive;			// true の場合、実際に再生する (古い再生を停止するときに false にする。本来はリストから delete しても良いのだが、メモリ効率的に。)
};

template<typename TValue>
class TypedAnimationCurveInstance
	: public AnimationCurveInstance
{
public:
	TValue	startValue;

	TypedAnimationCurveInstance(AnimationCurve* owner_, Object* targetObject_, const tr::PropertyInfo* targetProperty_, const TValue& startValue_)
		: startValue(startValue_)
	{
		owner = owner_;
		targetObject = targetObject_;
		targetProperty = targetProperty_;
		isActive = true;
	}
};


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

	//virtual AnimationCurveInstance* CreateAnimationCurveInstance(Object* targetObject, tr::Property* targetProperty) = 0;
	virtual bool ApplyPropertyAnimation(AnimationCurveInstance* instance, double time) { return false; }

private:
	WrapMode	m_wrapMode;
};


template<typename TValue>
class ValueEasingCurve
	: public AnimationCurve
{
public:

	//static FloatEasing* Create(const String& targetName, const String& targetProperty, float targetValue, float duration, Animation::EasingMode easingMode);
	virtual ValueType GetValueType() const { return ValueType_Float; }
	virtual void UpdateValue(double time) {}
	virtual double GetLastFrameTime() const { return 0; }

public:
	static RefPtr<ValueEasingCurve<TValue>> Create(TValue targetValue, double duration, EasingMode easingMode)
	{
		return RefPtr<ValueEasingCurve<TValue>>::MakeRef(targetValue, duration, easingMode);
	}

	ValueEasingCurve(TValue targetValue, double duration, EasingMode easingMode)
		: m_targetValue(targetValue)
		, m_duration(duration)
		, m_easingMode(EasingMode::Linear)
		, m_easingFunction()
	{
		SetEasingMode(easingMode);
	}

	virtual ~ValueEasingCurve() {}

	//void SetTargetName(const String& name) { m_targetName = name; }
	//void SetTargetProperty(const Property* prop) { m_targetProperty = prop; }
	void SetTargetValue(TValue value) { m_targetValue = value; }
	void SetEasingMode(EasingMode easingMode)
	{
		m_easingMode = easingMode;
		m_easingFunction = AnimationUtilities::SelectEasingFunction<TValue, double>(m_easingMode);
	}

	void SetDuration(double duration) { m_duration = duration; }

	AnimationCurveInstance* CreateAnimationCurveInstance(Object* targetObject, const tr::PropertyInfo* targetProperty, const TValue& startValue)
	{
		return LN_NEW TypedAnimationCurveInstance<TValue>(this, targetObject, targetProperty, startValue);
	}

	//virtual bool ApplyPropertyAnimation(AnimationCurveInstance* instance_,/*Object* targetObject, tr::Property* targetProp, const TValue& startValue, */double time)
	//{
 //       return (time < m_duration);
	//}
 //   
 //   
	virtual bool ApplyPropertyAnimation(AnimationCurveInstance* instance_,/*Object* targetObject, tr::Property* targetProp, const TValue& startValue, */double time) override
    {
        TypedAnimationCurveInstance<TValue>* instance = static_cast<TypedAnimationCurveInstance<TValue>*>(instance_);
        
        // 経過時間 0 の場合はそのままセットで良い。0除算対策の意味も込めて。
        // また、時間が既に終端を超えていたり、比較関数が無い場合も直値セット。
        if (m_duration == 0 || m_duration <= time || m_easingFunction == nullptr)
        {
            tr::PropertyInfo::SetPropertyValueDirect(instance->targetObject, instance->targetProperty, m_targetValue, tr::PropertySetSource::ByAnimation);
        }
        // 時間が 0 以前の場合は初期値
        else if (time <= 0)
        {
			tr::PropertyInfo::SetPropertyValueDirect<TValue>(instance->targetObject, instance->targetProperty, instance->startValue, tr::PropertySetSource::ByAnimation);
        }
        // 補間で求める
        else
        {
			tr::PropertyInfo::SetPropertyValueDirect<TValue>(instance->targetObject, instance->targetProperty, m_easingFunction(time, instance->startValue, m_targetValue - instance->startValue, m_duration), tr::PropertySetSource::ByAnimation);
        }
        
        return (time < m_duration);
    }

private:
	TValue		m_targetValue;
	EasingMode	m_easingMode;
	double		m_duration;
	std::function< TValue(double, TValue, TValue, double) >	m_easingFunction;
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
	void AddKeyFrame(double frame_pos, float value, InterpolationMode mode = InterpolationMode_Linear);

	/// 補間結果の取得 (SetTime() で更新される)
	float GetValue() const { return m_value; }

public:
	// override AnimationCurve
	virtual ValueType GetValueType() const { return ValueType_Float; }
	virtual void UpdateValue(double time);
	virtual double GetLastFrameTime() const;

#if 0
	//template<typename TValue>
	/*virtual*/ AnimationCurveInstance* CreateAnimationCurveInstance(Object* targetObject, tr::Property* targetProperty, float startValue)
	{
		return LN_NEW TypedAnimationCurveInstance<float>(this, targetObject, targetProperty, startValue);
	}

	bool ApplyPorpertyAnimation(Object* targetObject, tr::Property* targetProp, float startValue, float time)
	{
		UpdateValue(time);
		targetObject->SetTypedPropertyValue<float>(targetProp, GetValue());
		return (time < m_duration);
	}
#endif

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

class VMDBezierSQTTransformAnimation2
	: public AnimationCurve
{
public:
	struct BezierCurve
	{
		float Epsilon = 1.0e-3f;

		Vector2 v1;
		Vector2 v2;

		float Evaluate(float Progress)
		{
			//ニュートン法による近似
			float t = Math::Clamp(Progress, 0, 1);
			float dt;
			do
			{
				dt = -(fx(t) - Progress) / dfx(t);
				if (Math::IsNaN(dt))
					break;
				t += Math::Clamp(dt, -1.0f, 1.0f);//大幅に移動して別の解に到達するのを防止する用
			} while (abs(dt) > Epsilon);
			return Math::Clamp(fy(t), 0.0f, 1.0f);//念のため、0-1の間に収まるようにした
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
	void AddFrame(const BoneFrameData& frame) { m_keyFrameList.Add(frame); }
	void SortKeyFrame();

public:
	virtual ValueType GetValueType() const { return ValueType_SQTTransform; }
	virtual void UpdateValue(double time);
	virtual double GetLastFrameTime() const;

private:
	typedef Array<BoneFrameData>	KeyFrameList;
	KeyFrameList	m_keyFrameList;
	SQTTransform	m_transform;
};

LN_NAMESPACE_END
