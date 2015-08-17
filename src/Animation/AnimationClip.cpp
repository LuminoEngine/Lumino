
#include "../Internal.h"
#include "AnimationClip.h"

namespace Lumino
{
namespace Animation
{

//=============================================================================
// AnimationClip
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationClip::AnimationClip()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationClip::~AnimationClip()
{
	LN_FOREACH(AnimationCurveEntry& e, m_animationCurveEntryList) {
		e.Curve->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClip::AddAnimationCurve(const String& relativePath, AnimationCurve* curve)
{
	AnimationCurveEntry e;
	e.RelativePath = relativePath;
	e.Curve = curve;
	LN_SAFE_ADDREF(e.Curve);
	m_animationCurveEntryList.Add(e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void AnimationClip::SetTime(double oldTime, double newTime)
//{
//}

} // namespace Animation
} // namespace Lumino
