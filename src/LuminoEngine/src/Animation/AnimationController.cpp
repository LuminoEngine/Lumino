
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationTrack.hpp>
#include <LuminoEngine/Animation/AnimationController.hpp>

namespace ln {



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

void AnimationState::init(AnimationClip* clip)
{
	Object::init();
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

void AnimationState::attachToTarget(AnimationControllerCore* animatorController)
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
		float localTime = 0.0f;

        switch (m_clip->wrapMode())
        {
        case AnimationWrapMode::Once:
            localTime = m_localTime;
            break;
        case AnimationWrapMode::Loop:
            localTime = std::fmod(m_localTime, m_clip->lastFrameTime());
            break;
        case AnimationWrapMode::RoundTrip:
        {
            float freq = m_clip->lastFrameTime() * 2;
            float t = std::fmod(m_localTime, freq);
            float phase = t / freq;
            if (phase <= 0.5) {
                localTime = t;
            }
            else {
                localTime = freq - t;
            }
            break;
        }
        default:
            LN_UNREACHABLE();
            break;
        }
		//if (m_clip->wrapMode() == AnimationWrapMode::Loop)
		//	time = std::fmod(m_localTime, m_clip->lastFrameTime());
		//else
		//	time = m_localTime;

		for (auto& trackInstance : m_trackInstances)
		{
			AnimationValue value(trackInstance.track->type());
			AnimationValue& rootValue = trackInstance.blendLink->rootValue;

			trackInstance.track->evaluate(localTime, &value);

			switch (value.type())
			{
			case AnimationValueType::Float:
            {
                rootValue.v_Float = value.getFloat();
                trackInstance.blendLink->affectAnimation = true;
                break;
            }
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
	m_transition.stateFrom = nullptr;
	m_transition.stateTo = nullptr;
	m_transition.duration = 0;
	m_transition.time = 0;
	m_transition.startingOffsetTime = 0;
}

AnimationLayer::~AnimationLayer()
{
}

void AnimationLayer::init(AnimationControllerCore* owner)
{
	Object::init();
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
	auto state = m_animationStatus.findIf([name](const Ref<AnimationState>& state) { return state->name() == name; });
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
			if (m_transition.stateFrom) m_transition.stateFrom->setActive(false);
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
	float count = 0.0f;
	for (auto& state : m_animationStatus)
	{
		// TODO: 加算モードであれば +1 しない
		if (state->isActive())
		{
			divisor += state->getBlendWeight();
		}
	}

	// normalize
	float t = 1.0f / divisor;
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

		// stop current animation immediately
		if (m_currentState)
		{
			m_currentState->setActive(false);
		}
	}
	else
	{
		m_transition.stateFrom = m_currentState;
		m_transition.stateTo = state;
		m_transition.duration = duration;
		m_transition.time = 0.0f;
		m_transition.startingOffsetTime = 0.0f;	// TODO: 再生中の他のアニメと同期したい場合はここを変更

		if (m_currentState)
		{
			m_transition.startingOffsetTime = m_currentState->localTime();
		}
	}

	m_currentState = state;
	if (m_currentState)
	{
		m_currentState->setActive(true);
		m_currentState->setLocalTime(0.0f);
	}
}

//==============================================================================
// AnimationControllerCore

AnimationControllerCore::AnimationControllerCore()
	: m_layers()
{
}

AnimationControllerCore::~AnimationControllerCore()
{
}

void AnimationControllerCore::init(detail::IAnimationControllerHolder* owner)
{
	Object::init();
	m_owner = owner;
}

AnimationState* AnimationControllerCore::addClip(AnimationClip* animationClip)
{
	return m_layers[0]->addClipAndCreateState(animationClip);
}

AnimationState* AnimationControllerCore::addClip(const StringRef& stateName, AnimationClip* animationClip)
{
	AnimationState* state = addClip(animationClip);
	state->setName(stateName);
	return state;
}

void AnimationControllerCore::removeClip(AnimationClip* animationClip)
{
	m_layers[0]->removeClipAndDeleteState(animationClip);
}

void AnimationControllerCore::play(const StringRef& clipName, float duration)
{
	AnimationState* state = m_layers[0]->findAnimationState(clipName);
	m_layers[0]->transitionTo(state, duration);
}

void AnimationControllerCore::advanceTime(float elapsedTime)
{
	for (auto& layer : m_layers)
	{
		layer->advanceTime(elapsedTime);
	}

	updateTargetElements();
}

void AnimationControllerCore::updateTargetElements()
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

		m_owner->onUpdateTargetElement(link);
	}
}

detail::AnimationTargetElementBlendLink* AnimationControllerCore::findAnimationTargetElementBlendLink(const StringRef& name)
{
	auto data = m_targetElementBlendLinks.findIf([name](const Ref<detail::AnimationTargetElementBlendLink>& data) { return data->name == name; });
	if (data)
		return (*data);
	else
		return nullptr;
}





AnimationController::AnimationController()
{
}

void AnimationController::init(IAnimationTargetObject* targetObject)
{
	Object::init();

	m_targetObject = targetObject;

	m_core = newObject<AnimationControllerCore>(this);
	m_core->addLayer(newObject<AnimationLayer>(m_core));

	int count = m_targetObject->getAnimationTargetElementCount();
	for (int i = 0; i < count; i++)
	{
		auto data = makeRef<detail::AnimationTargetElementBlendLink>();
		data->name = m_targetObject->getAnimationTargetElementName(i);
		data->targetIndex = i;
		data->rootValue.resetType(m_targetObject->getAnimationTargetElementValueType(i));
		m_core->addElementBlendLink(data);
	}
}

void AnimationController::advanceTime(float elapsedTime)
{
	m_core->advanceTime(elapsedTime);
}

void AnimationController::onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* link)
{
	if (link->targetIndex > 0) {
		m_targetObject->setAnimationTargetElementValue(link->targetIndex, link->rootValue);
	}
}


} // namespace ln
