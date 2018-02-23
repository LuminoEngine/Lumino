
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
	, m_localTime(0.0f)
	, m_blendWeight(1.0f)
	, m_active(false)
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

void AnimationState::setActive(bool value)
{
	m_active = value;
}

void AnimationState::setLocalTime(float time)
{
	m_localTime = time;
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

void AnimationState::updateTargetElements()
{
	if (m_active)
	{
		for (auto& trackInstance : m_trackInstances)
		{
			AnimationValue value(trackInstance.track->type());
			AnimationValue& rootValue = trackInstance.blendLink->rootValue;

			trackInstance.track->evaluate(m_localTime, &value);

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
}

//==============================================================================
// AnimationLayer
//==============================================================================

AnimationLayer::AnimationLayer()
	: m_owner(nullptr)
	, m_animationStatus()
	, m_currentState(nullptr)
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

AnimationState* AnimationLayer::addClipAndCreateState(AnimationClip* animationClip)
{
	if (LN_REQUIRE(animationClip != nullptr)) return nullptr;

	auto state = newObject<AnimationState>(animationClip);
	m_animationStatus.add(state);
	state->attachToTarget(m_owner);
	return state;
}

void AnimationLayer::removeClipAndDeleteState(AnimationClip* animationClip)
{
	if (LN_REQUIRE(animationClip != nullptr)) return;
	m_animationStatus.removeIf([animationClip](const Ref<AnimationState>& state) { return state->animationClip() == animationClip; });
}

AnimationState* AnimationLayer::findAnimationState(const StringRef& name)
{
	auto state = m_animationStatus.find([name](const Ref<AnimationState>& state) { return state->name() == name; });
	if (state)
		return *state;
	else
		return nullptr;
}

void AnimationLayer::advanceTime(float elapsedTime)
{
	if (m_transition.stateFrom != nullptr || m_transition.stateTo != nullptr)
	{
		m_transition.time += elapsedTime;
		if (m_transition.duration <= m_transition.time)
		{
			// finish
			m_transition.stateFrom = nullptr;
			m_transition.stateTo = nullptr;
			m_transition.duration = 0.0f;
		}
	}

	for (auto& state : m_animationStatus)
	{
		if (state->isActive())
		{
			state->setLocalTime(state->localTime() + elapsedTime);
		}
	}
}

void AnimationLayer::updateStateWeights()
{
	// reset
	for (auto& state : m_animationStatus)
	{
		state->setBlendWeight(1.0f);
	}

	// apply transition
	if (m_transition.duration > 0.0f)
	{
		float blendWeight = m_transition.time / m_transition.duration;
		if (m_transition.stateFrom) m_transition.stateFrom->setBlendWeight(1.0f - blendWeight);
		if (m_transition.stateTo) m_transition.stateTo->setBlendWeight(blendWeight);
	}

	// calc divisor for normalize
	float totalWeight = 0.0f;
	float weightCount = 0.0f;
	float divisor = 0.0f;
	for (auto& state : m_animationStatus)
	{
		// TODO: 加算モードであれば +1 しない
		if (state->isActive())
		{
			divisor += state->getBlendWeight() * state->getBlendWeight();
		}
	}
	float t = 1.0f / std::sqrt(divisor);

	// normalize
	for (auto& state : m_animationStatus)
	{
		if (state->isActive())
		{
			state->setBlendWeight(t * state->getBlendWeight());
		}
	}
}

void AnimationLayer::updateTargetElements()
{
	for (auto& state : m_animationStatus)
	{
		state->updateTargetElements();
	}
}

void AnimationLayer::transitionTo(AnimationState* state, float duration)
{
	if (Math::nearEqual(duration, 0.0f))
	{
		m_transition.stateFrom = nullptr;
		m_transition.stateTo = nullptr;
		m_transition.duration = 0.0f;
		m_transition.time = 0.0f;
		m_transition.startingOffsetTime = 0.0f;
	}
	else
	{
		m_transition.stateFrom = m_currentState;
		m_transition.stateTo = state;
		m_transition.duration = duration;
		m_transition.time = 0.0f;
		m_transition.startingOffsetTime = 0.0f;	// TODO: 再生中の他のアニメと同期したい場合はここを変更
	}

	m_currentState = state;
	if (m_currentState)
	{
		m_currentState->setActive(true);
		m_currentState->setLocalTime(0.0f);
	}
}

//==============================================================================
// AnimationController
//==============================================================================

AnimationController::AnimationController()
	: m_targetObject(nullptr)
	, m_layers()
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

AnimationState* AnimationController::addClip(AnimationClip* animationClip)
{
	return m_layers[0]->addClipAndCreateState(animationClip);
}

AnimationState* AnimationController::addClip(const StringRef& stateName, AnimationClip* animationClip)
{
	AnimationState* state = addClip(animationClip);
	state->setName(stateName);
	return state;
}

void AnimationController::removeClip(AnimationClip* animationClip)
{
	m_layers[0]->removeClipAndDeleteState(animationClip);
}

void AnimationController::play(const StringRef& clipName, float duration)
{
	AnimationState* state = m_layers[0]->findAnimationState(clipName);
	m_layers[0]->transitionTo(state, duration);
}

void AnimationController::advanceTime(float elapsedTime)
{
	for (auto& layer : m_layers)
	{
		layer->advanceTime(elapsedTime);
	}
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
		layer->updateStateWeights();
		layer->updateTargetElements();
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
