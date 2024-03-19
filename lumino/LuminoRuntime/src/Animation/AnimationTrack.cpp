
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationCurve.hpp>
#include <LuminoEngine/Animation/AnimationTrack.hpp>

namespace ln {

//==============================================================================
// AnimationValue

AnimationValue::AnimationValue()
	: m_type(AnimationValueType::Null)
{
}

AnimationValue::AnimationValue(AnimationValueType type)
    : AnimationValue()
{
	resetType(type);
}

void AnimationValue::resetType(AnimationValueType type)
{
	m_type = type;
	clearValue();
}

void AnimationValue::clearValue()
{
	m_totalBlendWeights = 0.0f;
	switch (m_type)
	{
	case AnimationValueType::Float:
		v_Float = 0.0f;
		break;
	case AnimationValueType::Vector3:
		v_Vector3 = Vector3();
		break;
	case AnimationValueType::Quaternion:
		v_Quaternion = Quaternion();
		break;
	case AnimationValueType::Transform:
		v_Transform.scale = Vector3();	// 補間した値にウェイトかけてどんどん足していくので 0 で初期化しておく
		v_Transform.rotation = Quaternion();
		v_Transform.translation = Vector3();
		break;
	default:
		break;
	}
}

//==============================================================================
// AnimationTrack

AnimationTrack::AnimationTrack(AnimationValueType type)
    : m_owner(nullptr)
	, m_targetKey()
    , m_type(type)
	, m_translationClass(TranslationClass::Absolute)
{
}

AnimationTrack::~AnimationTrack()
{
}

Result<> AnimationTrack::init() {
    return Object::init();
}

bool AnimationTrack::init(TranslationClass translationClass)
{
	if (!init()) return false;
	m_translationClass = translationClass;
	return true;
}

//==============================================================================
// ScalarAnimationTrack

Ref<ScalarAnimationTrack> ScalarAnimationTrack::create()
{
    return makeObject_deprecated<ScalarAnimationTrack>();
}

ScalarAnimationTrack::ScalarAnimationTrack()
    : AnimationTrack(AnimationValueType::Float)
{
}

ScalarAnimationTrack::~ScalarAnimationTrack()
{
}

void ScalarAnimationTrack::init()
{
}

void ScalarAnimationTrack::setCurve(AnimationCurve* curve)
{
    m_curve = curve;
}

float ScalarAnimationTrack::lastFrameTime() const
{
    return m_curve->lastFrameTime();
}

float ScalarAnimationTrack::evaluate(float time)
{
    return m_curve->evaluate(time);
}

void ScalarAnimationTrack::evaluate(float time, AnimationValue* outResult)
{
    outResult->setFloat(m_curve->evaluate(time));
}

//==============================================================================
// TransformAnimationTrack

TransformAnimationTrack::TransformAnimationTrack()
	: AnimationTrack(AnimationValueType::Transform)
	, m_lastTime(0.0f)
	, m_translationInterpolation(Interpolation::Linear)
	, m_scaleInterpolation(Interpolation::Linear)
{
}

bool TransformAnimationTrack::init()
{
	if (!AnimationTrack::init()) return false;
	return true;
}

bool TransformAnimationTrack::init(TranslationClass translationClass)
{
	if (!AnimationTrack::init(translationClass)) return false;
	return true;
}

void TransformAnimationTrack::setupTranslations(int frames, const float* times, const Vector3* values, Interpolation interpolation)
{
	m_translationKeys.resize(frames);
	for (int i = 0; i < frames; i++) {
		m_translationKeys[i].time = times[i];
		m_translationKeys[i].value = values[i];
		m_lastTime = std::max(m_lastTime, times[i]);
	}
	m_translationInterpolation = interpolation;
}

void TransformAnimationTrack::setupRotations(int frames, const float* times, const Quaternion* values)
{
	m_rotationKeys.resize(frames);
	for (int i = 0; i < frames; i++) {
		m_rotationKeys[i].time = times[i];
		m_rotationKeys[i].value = values[i];
		m_lastTime = std::max(m_lastTime, times[i]);
	}
}

void TransformAnimationTrack::setupScales(int frames, const float* times, const Vector3* values, Interpolation interpolation)
{
	m_scaleKeys.resize(frames);
	for (int i = 0; i < frames; i++) {
		m_scaleKeys[i].time = times[i];
		m_scaleKeys[i].value = values[i];
		m_lastTime = std::max(m_lastTime, times[i]);
	}
	m_scaleInterpolation = interpolation;
}

void TransformAnimationTrack::resizeFramesTQ(int frames)
{
	m_translationKeys.resize(frames);
	m_rotationKeys2.resize(frames);
}

void TransformAnimationTrack::setDataTQ(int frame, float time, const Vector3& pos, const Vector3& rot)
{
	m_translationKeys[frame] = { time, pos };
	m_rotationKeys2[frame] = { time, rot };
	m_lastTime = std::max(m_lastTime, time);
}

template<class TKey>
static int findKey0(const std::vector<TKey>& keys, float t)
{
	assert(keys.size() >= 2);

	int i1 = 0;
	int right = keys.size();
	while (i1 < right) {
		int mid = (i1 + right) >> 1;
		if (t < keys[mid].time) {
			right = mid;
		}
		else {
			i1 = mid + 1;
		}
	}

	return i1 - 1;
}

template<class TKey>
static Vector3 interpolateVector3(const std::vector<TKey>& keys, float time, const Vector3& defaultValue, TransformAnimationTrack::Interpolation interpolation)
{
	if (keys.size() == 0) {
		return defaultValue;
	}
	else if (keys.size() == 1) {
		return keys[0].value;
	}
	else if (time <= keys.front().time) {
		return keys.front().value;
	}
	else if (keys.back().time <= time) {
		return keys.back().value;
	}
	else {
		int i0 = findKey0(keys, time);
		auto v0 = keys[i0].value;
		auto v1 = keys[i0 + 1].value;
		auto t0 = keys[i0].time;
		auto t1 = keys[i0 + 1].time;
		float t = (time - t0) / (t1 - t0);
		switch (interpolation)
		{
		case TransformAnimationTrack::Interpolation::Step:
			return keys[i0].value;
		case TransformAnimationTrack::Interpolation::Linear:
			return v0 + (v1 - v0) * t;
		case TransformAnimationTrack::Interpolation::CubicSpline:
			LN_NOTIMPLEMENTED();
			return defaultValue;
		default:
			LN_NOTIMPLEMENTED();
			return defaultValue;
		}
	}

	LN_UNREACHABLE();
	return defaultValue;
}

template<class TKey>
static Quaternion interpolateQuaternion(const std::vector<TKey>& keys, float time, const Quaternion& defaultValue)
{
	if (keys.size() == 0) {
		return defaultValue;
	}
	else if (keys.size() == 1) {
		return keys[0].value;
	}
	else if (time <= keys.front().time) {
		return keys.front().value;
	}
	else if (keys.back().time <= time) {
		return keys.back().value;
	}
	else {
		int i0 = findKey0(keys, time);
		auto v0 = keys[i0].value;
		auto v1 = keys[i0 + 1].value;
		auto t0 = keys[i0].time;
		auto t1 = keys[i0 + 1].time;
		float t = (time - t0) / (t1 - t0);
		return Quaternion::slerp(v0, v1, t);
	}

	LN_UNREACHABLE();
	return defaultValue;
}


void TransformAnimationTrack::evaluate(float time, AnimationValue* outResult)
{
	AttitudeTransform transform;

	transform.translation = interpolateVector3(m_translationKeys, time, Vector3::Zero, m_translationInterpolation);

	if (!m_rotationKeys2.empty()) {
		auto rot = interpolateVector3(m_rotationKeys2, time, Vector3::Zero, m_translationInterpolation);

		// TODO: BVH ファイルに書かれている CH 番号の順で計算する必要がある
		transform.rotation = Quaternion::makeFromEulerAngles(rot,
			//RotationOrder::ZXY);    // https://research.cs.wisc.edu/graphics/Courses/cs-838-1999/Jeff/BVH.html
			RotationOrder::XYZ);
			//RotationOrder::YZX);

		// bvh_player と同じ実装
		Matrix a;
		a.rotateAxis(Vector3(0, 0, 1), rot.z);
		a.rotateAxis(Vector3(0, 1, 0), rot.y);
		a.rotateAxis(Vector3(1, 0, 0), rot.x);
		transform.rotation = Quaternion::makeFromRotationMatrix(a);
	}
	else
		transform.rotation = interpolateQuaternion(m_rotationKeys, time, Quaternion::Identity);
	transform.scale = interpolateVector3(m_scaleKeys, time, Vector3::Ones, m_scaleInterpolation);

	outResult->setTransform(transform);

	//if (targetKey().bone == HumanoidBones::LeftUpperArm) {
	//	printf("");
	//}
	//if (targetKey().bone == HumanoidBones::RightUpperArm) {
	//	printf("");
	//}
}

} // namespace ln
