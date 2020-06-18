
#include "Internal.hpp"
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Animation/AnimationTrack.hpp>
#include <LuminoEngine/Animation/AnimationClip.hpp>
#include <LuminoEngine/Animation/AnimationContext.hpp>
#include "../Asset/AssetManager.hpp"
#include "../Scene/SceneManager.hpp"
#include "VmdLoader.hpp"
#include "AnimationManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// VMDBezierTransformAnimationTrack

// ボーン単位
class VMDBezierTransformAnimationTrack
	: public AnimationTrack
{
public:
	VMDBezierTransformAnimationTrack();
	virtual ~VMDBezierTransformAnimationTrack();
	void init(const detail::MotionFrameTrack& track);

	virtual void evaluate(float time, AnimationValue* outResult) override;

private:
	const detail::MotionFrameTrack* m_vmdTrack;
};


VMDBezierTransformAnimationTrack::VMDBezierTransformAnimationTrack()
	: AnimationTrack(AnimationValueType::Transform)
{
}

VMDBezierTransformAnimationTrack::~VMDBezierTransformAnimationTrack()
{
}

void VMDBezierTransformAnimationTrack::init(const detail::MotionFrameTrack& track)
{
	AnimationTrack::init();
	m_vmdTrack = &track;
	setTargetName(m_vmdTrack->boneName);
}

void VMDBezierTransformAnimationTrack::evaluate(float time, AnimationValue* outResult)
{
	AttitudeTransform result;

	if (!m_vmdTrack->frames.isEmpty())
	{
		int keyIndex = 0;
		for (; keyIndex < m_vmdTrack->frames.size() && m_vmdTrack->frames[keyIndex].frameTime < time; keyIndex++);
		keyIndex = (keyIndex > 0) ? keyIndex - 1 : keyIndex;

		if (keyIndex + 1 < m_vmdTrack->frames.size())
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
			result.rotation = m_vmdTrack->frames.back().quaternion;
			result.translation = m_vmdTrack->frames.back().position;
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
// AssetManager

AnimationManager::AnimationManager()
    : m_assetManager(nullptr)
    , m_sceneManager(nullptr)
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "AnimationManager Initialization started.";

    m_assetManager = settings.assetManager;
    m_animationClipCache.init(64, 0);

    LN_LOG_DEBUG << "AnimationManager Initialization ended.";
}

void AnimationManager::dispose()
{
}

void AnimationManager::addClockToAffiliation(AnimationClock* clock, AnimationClockAffiliation affiliation)
{
    if (LN_REQUIRE(clock)) return;

    switch (affiliation)
    {
    case AnimationClockAffiliation::Standalone:
        break;
    case AnimationClockAffiliation::ActiveWorld:
        m_sceneManager->activeWorld()->animationContext()->addAnimationClock(clock);
        break;
    default:
        LN_UNREACHABLE();
        break;
    }
}

Ref<AnimationClip> AnimationManager::acquireAnimationClip(const AssetPath& assetPath)
{
    uint64_t key = assetPath.calculateHash();
    auto obj = m_animationClipCache.findObject(key);
    if (obj) {
        return obj;
    }
    else {
        obj = makeObject<AnimationClip>();
        loadAnimationClip(obj, assetPath);
        m_animationClipCache.registerObject(key, obj, 0);
        return obj;
    }
}

void AnimationManager::loadAnimationClip(AnimationClip* clip, const AssetPath& assetPath)
{
    auto stream = m_assetManager->openStreamFromAssetPath(assetPath);

    detail::VmdFile vmdFile;
    auto vmdData = vmdFile.load(stream);
    if (vmdData)
    {
        for (auto& track : vmdData->MotionData)
        {
            clip->m_tracks.add(makeObject<VMDBezierTransformAnimationTrack>(track));
        }

		clip->m_lastFrameTime = vmdData->lastFrameTime;

        // VmdData が持っているトラックの情報を後で使いたいので、参照を持っておく
		clip->m_srcData = vmdData;
    }
}

void AnimationManager::updateFrame(float elapsedSeconds)
{
}

} // namespace detail
} // namespace ln

