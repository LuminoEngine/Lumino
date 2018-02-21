
#include "../Internal.h"
#include <Lumino/Animation/A2AnimationTrack.h>
#include "VmdLoader.h"

LN_NAMESPACE_BEGIN
namespace a2
{

// ボーン単位
class VMDBezierTransformAnimationTrack
	: public AnimationTrack
{
public:
	VMDBezierTransformAnimationTrack();
	virtual ~VMDBezierTransformAnimationTrack();
	void initialize(const detail::MotionFrameTrack& track);

	virtual void evaluate(float time, AnimationValue* outResult) override;

private:
	const detail::MotionFrameTrack* m_vmdTrack;
};

//==============================================================================
// AnimationValue
//==============================================================================

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
//==============================================================================
AnimationTrack::AnimationTrack(AnimationValueType type)
    : m_targetName()
    , m_type(type)
{
}

AnimationTrack::~AnimationTrack()
{
}

//==============================================================================
// VMDBezierTransformAnimationTrack
//==============================================================================
VMDBezierTransformAnimationTrack::VMDBezierTransformAnimationTrack()
	: AnimationTrack(AnimationValueType::Transform)
{
}

VMDBezierTransformAnimationTrack::~VMDBezierTransformAnimationTrack()
{
}

void VMDBezierTransformAnimationTrack::initialize(const detail::MotionFrameTrack& track)
{
	AnimationTrack::initialize();
	m_vmdTrack = &track;
	setTargetName(m_vmdTrack->boneName);
}

void VMDBezierTransformAnimationTrack::evaluate(float time, AnimationValue* outResult)
{
	AttitudeTransform result;

	if (!m_vmdTrack->frames.isEmpty())
	{
		int keyIndex = 0;
		for (; keyIndex < m_vmdTrack->frames.getCount() && m_vmdTrack->frames[keyIndex].frameTime < time; keyIndex++);
		keyIndex = (keyIndex > 0) ? keyIndex - 1 : keyIndex;

		if (keyIndex + 1 < m_vmdTrack->frames.getCount())
		{
			auto& lk = m_vmdTrack->frames[keyIndex];
			auto& rk = m_vmdTrack->frames[keyIndex + 1];

			float diffTime = rk.frameTime - lk.frameTime;
			float t = (time - lk.frameTime) / (float)diffTime;

			Quaternion rot;
			Vector3 pos;
			const Vector4 motionPositions = Vector4::Zero;
			float dummy, gt;

			rot = detail::VmdData::interpolateQuaternionStrict(lk, rk, t, motionPositions.w, diffTime, &dummy);

			gt = detail::VmdData::interpolateStrict(rk.interpolXA, rk.interpolXB, t, motionPositions.x, diffTime, &dummy);
			pos.x = lk.position.x + (rk.position.x - lk.position.x) * gt;
			gt = detail::VmdData::interpolateStrict(rk.interpolYA, rk.interpolYB, t, motionPositions.y, diffTime, &dummy);
			pos.y = lk.position.y + (rk.position.y - lk.position.y) * gt;
			gt = detail::VmdData::interpolateStrict(rk.interpolZA, rk.interpolZB, t, motionPositions.z, diffTime, &dummy);
			pos.z = lk.position.z + (rk.position.z - lk.position.z) * gt;

			result.rotation = rot;
			result.translation = pos;
		}
		else
		{
			result.rotation = m_vmdTrack->frames.getLast().quaternion;
			result.translation = m_vmdTrack->frames.getLast().position;
		}
	}
	else
	{
		// no key frame
		result.rotation = Quaternion::Identity;
		result.translation = Vector3::Zero;
	}

	outResult->setTransform(result);
}


//==============================================================================
// AnimationClip
//==============================================================================

Ref<AnimationClip> AnimationClip::create(const Path& filePath)
{
	return newObject<AnimationClip>(filePath);
}

AnimationClip::AnimationClip()
	: m_tracks()
{
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::initialize(const Path& filePath)
{
	Object::initialize();

	detail::VmdFile vmdFile;
	auto vmdData = vmdFile.load(FileStream::create(filePath.c_str(), FileOpenMode::read));
	if (vmdData)
	{
		for (auto& track : vmdData->MotionData)
		{
			m_tracks.add(newObject<VMDBezierTransformAnimationTrack>(track));
		}

		// 参照を持っておく
		m_srcData = vmdData;
	}
}

} // namespace a2
LN_NAMESPACE_END
