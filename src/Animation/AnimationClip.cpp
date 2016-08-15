
#include "../Internal.h"
#include "VmdLoader.h"
#include "AnimationClip.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// AnimationClip
//==============================================================================

//------------------------------------------------------------------------------
AnimationClipPtr AnimationClip::Create(const StringRef& filePath)
{
	auto stream = FileStream::Create(filePath.GetBegin(), FileOpenMode::Read);

	VmdLoader loader;
	loader.Load(stream);

	auto ptr = AnimationClipPtr::MakeRef();
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
	LN_FOREACH(AnimationCurveEntry& e, m_animationCurveEntryList) {
		e.Curve->Release();
	}
}

//------------------------------------------------------------------------------
void AnimationClip::AddAnimationCurve(const String& relativePath, AnimationCurve* curve)
{
	AnimationCurveEntry e;
	e.RelativePath = relativePath;
	e.Curve = curve;
	LN_SAFE_ADDREF(e.Curve);
	m_animationCurveEntryList.Add(e);
}

//------------------------------------------------------------------------------
//void AnimationClip::SetTime(double oldTime, double newTime)
//{
//}

LN_NAMESPACE_END

