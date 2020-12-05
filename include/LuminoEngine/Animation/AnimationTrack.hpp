
#pragma once
#include "Common.hpp"

namespace ln {
class AnimationCurve;
class AnimationState;

/** 補間されたアニメーション値の型 */
enum class AnimationValueType
{
	Null,
	Float,
	Vector3,
	Quaternion,
	Transform,
};

/** 補間されたアニメーション値 */
class AnimationValue
{
public:

	/** 値の型を取得します。 */
	AnimationValueType type() const { return m_type; }

	/** 値をクリアします。（型は変更しません） */
	void clearValue();

	float getFloat() const { return v_Float; }
	const Vector3& getVector3() const { return v_Vector3; }
	const Quaternion& getQuaternion() const { return v_Quaternion; }
	const AttitudeTransform& getTransform() const { return v_Transform; }


public:
	AnimationValue();
	AnimationValue(AnimationValueType type);
	void resetType(AnimationValueType type);
	void setFloat(float value) { v_Float = value; }
	void setVector3(const Vector3& value) { v_Vector3 = value; }
	void setQuaternion(const Quaternion& value) { v_Quaternion = value; }
	void setTransform(const AttitudeTransform& value) { v_Transform = value; }

	AnimationValueType	m_type;
	float m_totalBlendWeights = 0.0f;
	union
	{
		float v_Float;
		Vector3 v_Vector3;
		Quaternion v_Quaternion;
		AttitudeTransform v_Transform;
	};
};

/** 特定の型のアニメーションを、1つ以上のカーブの集合として表現します。 */
class AnimationTrack
	: public Object
{
public:
	/** このトラックのアニメーションが適用される要素の名前。（スキンメッシュアニメーションではボーン名、UIElement のアニメーションではプロパティ名など） */
	//const String& targetName() const { return m_targetName; }
	const AnimationTrackTargetKey& targetKey() const { return m_targetKey; }

	/** このトラックのアニメーション値の型を取得します。 */
	AnimationValueType type() const { return m_type; }

    void setTargetName(const StringRef& value) { m_targetKey.name = value; }
	void setTargetHumanoidBone(HumanoidBones value) { m_targetKey.bone = value; }

    virtual float lastFrameTime() const { return 0; }

	TranslationClass translationClass() const { return m_translationClass; }

	AnimationClip* animationClip() const { return m_owner; }

	bool m_root = true;

protected:
	AnimationTrack(AnimationValueType type);
	virtual ~AnimationTrack();
	bool init();
	bool init(TranslationClass translationClass);
	virtual void evaluate(float time, AnimationValue* outResult) = 0;
	//void setTargetName(const String& name) { m_targetName = name; }

private:
	AnimationClip* m_owner;
	AnimationTrackTargetKey m_targetKey;
	AnimationValueType m_type;
	TranslationClass m_translationClass;

	friend class AnimationClip;
	friend class AnimationState;
};

/** (スキニングでは、モーフィングアニメーションで使用) */
class ScalarAnimationTrack	// TODO: FloatAnimationTrack の方がいいかも。float?double? な感じだし、インテリセンスから直感的ん検索できないことがあった。他は Vector3 とかなのに。
	: public AnimationTrack
{
public:
    static Ref<ScalarAnimationTrack> create();

    void setCurve(AnimationCurve* curve);


    /** アニメーションの終端の時間を取得します。 */
    virtual float lastFrameTime() const override;

    float evaluate(float time);

protected:
    virtual void evaluate(float time, AnimationValue* outResult) override;

LN_CONSTRUCT_ACCESS:
    ScalarAnimationTrack();
    virtual ~ScalarAnimationTrack();
    void init();

private:
    Ref<AnimationCurve> m_curve;
};

/** TODO */
class Vector3AnimationTrack
	: public AnimationTrack
{
public:
};

/** TODO */
class ColorAnimationTrack
	: public AnimationTrack
{
public:
};

/** (スキニングでは、ボーンアニメーションで使用) */
// translation, rotation, scale をまとめて扱う。
// 理由：
// - 個々の要素を Curve にすると、10 個も Curve ができる。さらに Key(AnimationKeyFrame) というやや大きめのデータもたくさんできる。
//   AnimationKeyFrame は UI アニメやシネマなど、Editor から編集するようなものに対してメインに使う。
//   対して skin mesh animation は基本的に外部のモデラーで変種したデータを扱う。↑に比べて大量のキーフレームができる傾向にある。
// - Quaternion は slerp での補間を行う必要があるが、そのため個々の要素を Curve にするのは現実的ではない。
//   また Edit 可能にしても、人間がいじれるものではない。（やるとしたらオイラー角度での編集か）
// - 外部ファイルからアニメーションの方法を指定してくることもある (glTF は STEP, LINEAR, CUBICSPLINE がある)
//   データの持ち方の考え方的に AnimationCurve を使った従来の方法とは異なり、↑のメモリ効率など インピーダンスミスマッチ がひどくなる。
class TransformAnimationTrack
	: public AnimationTrack
{
public:
	enum class Interpolation
	{
		Step,
		Linear,
		CubicSpline,
	};

	struct Vector3Key
	{
		float time;
		Vector3 value;
	};

	struct QuaternionKey
	{
		float time;
		Quaternion value;
	};

	// for glTF
	void setupTranslations(int frames, const float* times, const Vector3* values, Interpolation interpolation);
	void setupRotations(int frames, const float* times, const Quaternion* values);
	void setupScales(int frames, const float* times, const Vector3* values, Interpolation interpolation);

	// for BVH
	void resizeFramesTQ(int frames);
	//void setDataTQ(int frame, float time, const Vector3& pos, const Quaternion& rot);
	void setDataTQ(int frame, float time, const Vector3& pos, const Vector3& rot);


LN_CONSTRUCT_ACCESS:
	TransformAnimationTrack();

	bool init();
	bool init(TranslationClass translationClass);

	

protected:
	void evaluate(float time, AnimationValue* outResult) override;
	float lastFrameTime() const override { return m_lastTime; }

public:	// TODO: internal
	std::vector<Vector3Key> m_translationKeys;
	std::vector<QuaternionKey> m_rotationKeys;
	std::vector<Vector3Key> m_scaleKeys;
	Interpolation m_translationInterpolation;
	Interpolation m_scaleInterpolation;
	float m_lastTime;

	// exp
	std::vector<Vector3Key> m_rotationKeys2;
};

} // namespace ln
