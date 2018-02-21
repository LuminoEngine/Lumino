
#pragma once
#include <Lumino/Animation/Common.h>

LN_NAMESPACE_BEGIN
namespace a2
{
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

LN_INTERNAL_ACCESS:
	AnimationValue();
	AnimationValue(AnimationValueType type);
	void resetType(AnimationValueType type);
	void setFloat(float value) { v_Float = value; }
	void setVector3(const Vector3& value) { v_Vector3 = value; }
	void setQuaternion(const Quaternion& value) { v_Quaternion = value; }
	void setTransform(const AttitudeTransform& value) { v_Transform = value; }

	AnimationValueType	m_type;
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
	const String& targetName() const { return m_targetName; }

	/** このトラックのアニメーション値の型を取得します。 */
	AnimationValueType type() const { return m_type; }

protected:
	AnimationTrack(AnimationValueType type);
	virtual ~AnimationTrack();
	virtual void evaluate(float time, AnimationValue* outResult) = 0;
	void setTargetName(const String& name) { m_targetName = name; }

private:
	String m_targetName;
	AnimationValueType m_type;

	friend class AnimationState;
};

/** (スキニングでは、モーフィングアニメーションで使用) */
class ScalarAnimationTrack
	: public AnimationTrack
{
public:
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
class TransformAnimationTrack
	: public AnimationTrack
{
public:
};

/**
	@brief		オブジェクトやそのプロパティに影響を与えるアニメーションデータです。
	@details	スキンメッシュアニメーションでは、「歩く」「走る」といった 1 モーションの単位です。
*/
class AnimationClip
	: public Object
{
public:
	/* モーションファイルから AnimationClip を作成します。(.vmd) */
	static Ref<AnimationClip> create(const Path& filePath);

	const List<Ref<AnimationTrack>>& tracks() const { return m_tracks; }

LN_CONSTRUCT_ACCESS:
	AnimationClip();
	virtual ~AnimationClip();
	void initialize(const Path& filePath);

private:
	List<Ref<AnimationTrack>>	m_tracks;
	Ref<RefObject> m_srcData;
};

} // namespace a2
LN_NAMESPACE_END
