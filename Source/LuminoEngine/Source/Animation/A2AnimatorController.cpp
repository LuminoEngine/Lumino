
#include "../Internal.h"
#include <Lumino/Animation/A2AnimationTrack.h>
#include <Lumino/Animation/A2AnimatorController.h>

LN_NAMESPACE_BEGIN
namespace a2
{
namespace detail
{

class AnimationTargetElementBlendLink
	: public RefObject
{
public:
	String name;		// AnimationTargetElement の名前 (ボーン名など)
	int targetIndex;

	AnimationValue rootValue;	// アニメーション・ブレンドされた値。これを AnimationTargetElement へ送る。
	bool affectAnimation = false;		// 更新処理の中で、実際に値がセットされたかどうか。セットされていないボーンにはデフォルト値をセットしたりする。
};

} // namespace detail



//==============================================================================
// AnimationState
//==============================================================================

AnimationState::AnimationState()
	: m_clip(nullptr)
	, m_trackInstances()
	, m_blendWeight(1.0f)
{
}

AnimationState::~AnimationState()
{
}

void AnimationState::initialize(AnimationClip* clip)
{
	Object::initialize();
	m_clip = clip;
}

void AnimationState::attachToTarget(AnimationController* animatorController)
{
	m_trackInstances.clear();

	// Curve の適用先を element から探し、見つかれば t に持っておく
	for (auto& track : m_clip->tracks())
	{
		auto link = animatorController->findAnimationTargetElementBlendLink(track->targetName());
		if (link)
		{
			AnimationTrackInstance t;
			t.track = track;
			t.blendLink = link;
			m_trackInstances.add(t);
		}
	}
}

void AnimationState::updateTargetElements(float time)
{
	for (auto& trackInstance : m_trackInstances)
	{
		AnimationValue value(trackInstance.track->type());
		AnimationValue& rootValue = trackInstance.blendLink->rootValue;

		trackInstance.track->evaluate(time, &value);

		switch (value.type())
		{
			case AnimationValueType::Float:
				LN_NOTIMPLEMENTED();
				trackInstance.blendLink->affectAnimation = true;
				break;
			case AnimationValueType::Vector3:
				LN_NOTIMPLEMENTED();
				trackInstance.blendLink->affectAnimation = true;
				break;
			case AnimationValueType::Quaternion:
				LN_NOTIMPLEMENTED();
				trackInstance.blendLink->affectAnimation = true;
				break;
			case AnimationValueType::Transform:
			{
				const AttitudeTransform& s = value.getTransform();
				AttitudeTransform& t = rootValue.v_Transform;
				t.scale += s.scale * m_blendWeight;
				t.rotation *= Quaternion::slerp(Quaternion::Identity, s.rotation, m_blendWeight);
				t.translation += s.translation * m_blendWeight;
				trackInstance.blendLink->affectAnimation = true;
				break;
			}
			default:
				LN_UNREACHABLE();
				break;
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

void AnimationLayer::initialize(AnimationController* owner)
{
	Object::initialize();
	m_owner = owner;
}

void AnimationLayer::addClipAndCreateState(AnimationClip* animationClip)
{
	if (LN_REQUIRE(animationClip != nullptr)) return;

	auto state = newObject<AnimationState>(animationClip);
	m_animationStatus.add(state);
	state->attachToTarget(m_owner);
}

void AnimationLayer::removeClipAndDeleteState(AnimationClip* animationClip)
{
	if (LN_REQUIRE(animationClip != nullptr)) return;
	m_animationStatus.removeIf([animationClip](const Ref<AnimationState>& state) { return state->animationClip() == animationClip; });
}

void AnimationLayer::updateTargetElements(float time)
{
	for (auto& state : m_animationStatus)
	{
		state->updateTargetElements(time);
	}
}

//==============================================================================
// AnimationController
//==============================================================================

AnimationController::AnimationController()
	: m_targetObject(nullptr)
	, m_layers()
	, m_currentTime(0)
{
}

AnimationController::~AnimationController()
{
}

void AnimationController::initialize(IAnimationTargetObject* targetObject)
{
	Object::initialize();
	m_targetObject = targetObject;
	m_layers.add(newObject<AnimationLayer>(this));

	int count = m_targetObject->getAnimationTargetElementCount();
	for (int i = 0; i < count; i++)
	{
		auto data = Ref<detail::AnimationTargetElementBlendLink>::makeRef();
		data->name = m_targetObject->getAnimationTargetElementName(i);
		data->targetIndex = i;
		data->rootValue.resetType(m_targetObject->getAnimationTargetElementValueType(i));
		m_targetElementBlendLinks.add(data);
	}
}

void AnimationController::addClip(AnimationClip* animationClip)
{
	m_layers[0]->addClipAndCreateState(animationClip);
}

void AnimationController::removeClip(AnimationClip* animationClip)
{
	m_layers[0]->removeClipAndDeleteState(animationClip);
}

void AnimationController::advanceTime(float elapsedTime)
{
	m_currentTime += elapsedTime;
}

void AnimationController::updateTargetElements()
{
	// reset
	for (auto& link : m_targetElementBlendLinks)
	{
		link->rootValue.clearValue();
		link->affectAnimation = false;
	}

	// update
	for (auto& layer : m_layers)
	{
		layer->updateTargetElements(m_currentTime);
	}

	// set
	for (auto& link : m_targetElementBlendLinks)
	{
		if (!link->affectAnimation)
		{
			if (link->rootValue.type() == AnimationValueType::Transform)
			{
				link->rootValue.setTransform(AttitudeTransform());
			}
		}

		m_targetObject->setAnimationTargetElementValue(link->targetIndex, link->rootValue);
	}
}

detail::AnimationTargetElementBlendLink* AnimationController::findAnimationTargetElementBlendLink(const StringRef& name)
{
	auto data = m_targetElementBlendLinks.find([name](const Ref<detail::AnimationTargetElementBlendLink>& data) { return data->name == name; });
	if (data)
		return (*data);
	else
		return nullptr;
}


} // namespace a2
LN_NAMESPACE_END
