
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationCurve.hpp>
#include <LuminoEngine/Animation/AnimationTrack.hpp>
#include <LuminoEngine/Animation/AnimationClip.hpp>
#include "AnimationManager.hpp"
#include "VmdLoader.hpp"

namespace ln {

//==============================================================================
// AnimationClip

Ref<AnimationClip> AnimationClip::create(/*const StringRef& name, */const StringRef& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes)
{
	return makeObject<AnimationClip>(/*name, */targetPath, keyframes);
}

Ref<AnimationClip> AnimationClip::load(const StringRef& filePath)
{
	const Char* candidateExts[] = { u".vmd" };
	if (const auto assetPath = detail::AssetPath::resolveAssetPath(filePath, candidateExts, LN_ARRAY_SIZE_OF(candidateExts)))
		return detail::EngineDomain::animationManager()->acquireAnimationClip(assetPath);
	else
		return nullptr;
}

AnimationClip::AnimationClip()
    : m_lastFrameTime(0)
	, m_wrapMode(AnimationWrapMode::Loop)
{
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::init()
{
	Object::init();
}

void AnimationClip::init(/*const StringRef& name, */const StringRef& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes)
{
	//if (LN_REQUIRE(!name.isEmpty())) return;

	init();

	//m_name = name;

	auto curve = makeObject<KeyFrameAnimationCurve>();
	for (auto& key : keyframes) {
		curve->addKeyFrame(key);
	}

	auto track = ScalarAnimationTrack::create();
	track->setTargetName(targetPath);
	track->setCurve(curve);
	addTrack(track);
}

void AnimationClip::addTrack(AnimationTrack* track)
{
    m_tracks.add(track);

    // TODO: とりあえず
    m_lastFrameTime = track->lastFrameTime();
}




////==============================================================================
//// VmdAnimationClip
//
//Ref<VmdAnimationClip> VmdAnimationClip::create(const Path& filePath)
//{
//	return makeObject<VmdAnimationClip>(filePath);
//}
//
//VmdAnimationClip::VmdAnimationClip()
//{
//}
//
//VmdAnimationClip::~VmdAnimationClip()
//{
//}
//
//void VmdAnimationClip::init(const Path& filePath)
//{
//	AnimationClip::init();
//
//	detail::VmdFile vmdFile;
//	auto vmdData = vmdFile.load(FileStream::create(filePath.c_str(), FileOpenMode::Read));
//	if (vmdData)
//	{
//		for (auto& track : vmdData->MotionData)
//		{
//			m_tracks.add(makeObject<VMDBezierTransformAnimationTrack>(track));
//		}
//
//		m_lastFrameTime = vmdData->lastFrameTime;
//
//		// VmdData が持っているトラックの情報を後で使いたいので、参照を持っておく
//		m_srcData = vmdData;
//	}
//}
//
} // namespace ln