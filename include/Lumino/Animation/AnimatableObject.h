
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class AnimationClock;

/**
	@brief	プロパティアニメーションを適用できるクラスのベースクラス。
*/
class AnimatableObject
	: public Object
	//, public tr::IPropertyChangedListener
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	AnimatableObject();
	virtual ~AnimatableObject();

protected:
	//virtual void OnPropertyChanged(tr::PropertyChangedEventArgs* e) override;

private:
	void DeactivatePropertyAnimation(const tr::PropertyInfo* targetProperty);

	friend class detail::AnimationManager;
	friend class AnimationClock;
	Array<RefPtr<AnimationClock>>	m_playingAnimationClockList;
};

LN_NAMESPACE_END

