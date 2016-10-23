
#include "../Internal.h"
#include <Lumino/Reflection/ReflectionEventArgs.h>
#include <Lumino/Animation/AnimationClock.h>
#include <Lumino/Animation/AnimatableObject.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// AnimatableObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(AnimatableObject, Object);

//------------------------------------------------------------------------------
AnimatableObject::AnimatableObject()
{
}

//------------------------------------------------------------------------------
AnimatableObject::~AnimatableObject()
{
}

//------------------------------------------------------------------------------
void AnimatableObject::OnPropertyChanged(tr::PropertyChangedEventArgs* e)
{
	//Object::OnPropertyChanged(e);

	// Animation による設定以外の場合は一度アニメーションを停止する
	if (e->cause != tr::PropertySetSource::ByAnimation)
	{
		DeactivatePropertyAnimation(e->changedProperty);
	}
}

//------------------------------------------------------------------------------
void AnimatableObject::DeactivatePropertyAnimation(const tr::PropertyInfo* targetProperty)
{
	// 再生中のアニメの中に同じターゲットの同じプロパティをアニメーションしているものがあれば停止する
	for (auto& clock : m_playingAnimationClockList)
	{
		for (auto& playingCurveLineInst : clock->m_instanceList)
		{
			if (playingCurveLineInst->GetTargetObject() == this &&
				playingCurveLineInst->GetTargetPropertyInfo() == targetProperty)
			{
				playingCurveLineInst->isActive = false;

				
			}
		}
	}
}

LN_NAMESPACE_END

