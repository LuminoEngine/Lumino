
#include "../Internal.h"
#include <Lumino/Animation/A2AnimationTrack.h>
#include <Lumino/Animation/A2AnimatorController.h>

LN_NAMESPACE_BEGIN
namespace a2
{

//==============================================================================
// AnimationState
//==============================================================================

void AnimationState::attachToTarget(AnimatorController* animatorController)
{
	m_trackInstances.clear();

	// Curve の適用先を element から探し、見つかれば t に持っておく
	for (auto& track : m_clip->tracks())
	{
		int index = animatorController->findAnimationTargetElementIndex(track->targetName());
		if (index >= 0)
		{
			AnimationTrackInstance t;
			t.track = track;
			t.targetElementIndex = index;
			m_trackInstances.add(t);
		}
	}
}

//==============================================================================
// AnimationLayer
//==============================================================================

AnimationLayer::AnimationLayer()
	: m_owner(nullptr)
	, m_animationStatus()
{
}

AnimationLayer::~AnimationLayer()
{
}

void AnimationLayer::initialize(AnimatorController* owner)
{
	Object::initialize();
	m_owner = owner;
}

void AnimationLayer::addClipAndCreateState(AnimationClip* animationClip)
{
	if (LN_REQUIRE(animationClip != nullptr)) return;

	auto state = Ref<AnimationState>::makeRef(animationClip);
	m_animationStatus.add(state);
	state->attachToTarget(m_owner);
}

void AnimationLayer::removeClipAndDeleteState(AnimationClip* animationClip)
{
	if (LN_REQUIRE(animationClip != nullptr)) return;
	m_animationStatus.removeIf([animationClip](const Ref<AnimationState>& state) { state->animationClip() == animationClip; });
}

//==============================================================================
// AnimatorController
//==============================================================================

AnimatorController::AnimatorController()
	: m_targetObject(nullptr)
	, m_layers()
{
}

AnimatorController::~AnimatorController()
{
}

void AnimatorController::initialize(IAnimationTargetObject* targetObject)
{
	Object::initialize();
	m_layers.add(newObject<AnimationLayer>(this));
}

void AnimatorController::advanceTime(float elapsedTime)
{
	LN_NOTIMPLEMENTED();
}

int AnimatorController::findAnimationTargetElementIndex(const StringRef& name)
{
	int count = m_targetObject->getAnimationTargetElementCount();
	for (int i = 0; i < count; i++)
	{
		if (m_targetObject->getAnimationTargetElementName(i) == name)
		{
			return i;
		}
	}
	return -1;
}

} // namespace a2
LN_NAMESPACE_END
