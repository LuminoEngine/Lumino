
#include "../Internal.h"
#include "VmdLoader.h"
#include "AnimationClip.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// AnimationClip
//==============================================================================

//------------------------------------------------------------------------------
AnimationClipPtr AnimationClip::create(const StringRef& filePath)
{
	auto stream = FileStream::create(filePath.getBegin(), FileOpenMode::read);

	VmdLoader loader;
	loader.load(stream);

	auto ptr = AnimationClipPtr::makeRef();
	for (VmdLoader::BoneAnimation& anim : loader.GetBoneAnimationList())
	{
		ptr->AddAnimationCurve(anim.TargetBoneName, anim.AnimationCurve);
	}
	for (VmdLoader::FaceAnimation& anim : loader.GetFaceAnimationList())
	{
		ptr->AddAnimationCurve(anim.TargetFaceName, anim.AnimationCurve);
	}
	ptr->m_lastFrameTime = loader.GetLastFramePosition();
	return ptr;
}

//------------------------------------------------------------------------------
AnimationClip::AnimationClip()
	: m_name()
	, m_lastFrameTime(0)
	, m_defaultrapMode(WrapMode_Once)
	, m_animationCurveEntryList()
{
}

//------------------------------------------------------------------------------
AnimationClip::~AnimationClip()
{
	for (AnimationCurveEntry& e : m_animationCurveEntryList) {
		e.Curve->release();
	}
}

//------------------------------------------------------------------------------
void AnimationClip::AddAnimationCurve(const String& relativePath, AnimationCurve* curve)
{
	AnimationCurveEntry e;
	e.RelativePath = relativePath;
	e.Curve = curve;
	LN_SAFE_ADDREF(e.Curve);
	m_animationCurveEntryList.add(e);
}

//------------------------------------------------------------------------------
//void AnimationClip::SetTime(double oldTime, double newTime)
//{
//}

LN_NAMESPACE_END

