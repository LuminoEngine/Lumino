
#pragma once
#include "Common.h"
#include "AnimationCurve.h"

namespace Lumino
{
namespace Animation
{

/// アニメーションしたいオブジェクトを保持するオブジェクトのインターフェイス
class IAnimationTargetElement
{
public:
	virtual ~IAnimationTargetElement() {}

	virtual int GetAnimationTargetAttributeCount() const = 0;
	virtual IAnimationTargetAttribute* GetAnimationTargetAttribute(int index) = 0;

	/// (見つからなければ NULL を返す)
	//virtual IAnimationTargetAttribute* FindAnimationTargetAttributes(const String& relativePath) const = 0;
};

/// Animator
class Animator
	: public RefObject
{
public:

public:
	Animator();
	~Animator();

public:

	/// 作成
	void Create(IAnimationTargetElement* element);

	/// 再生中であるかを確認する
	bool IsPlaying() const;

	/// 再生
	void Play(const TCHAR* name/*, PlayMode mode = PlayMode_StopSameLayer*/);

	///// ブレンド (アニメーションの再生には影響しない。停止中のアニメーションがこの関数によって再生開始されることはない)
	//void Blend(const lnKeyChar* animName, lnFloat targetWeight, lnFloat fadeLength);

	///// クロスフェード
	//void CrossFade(const lnKeyChar* animName, lnFloat fadeLength, PlayMode mode = StopSameLayer);

	///// 前のアニメーションが終了した後、再生を開始する
	//void PlayQueued(const lnKeyChar* animName, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer);

	///// 前のアニメーションが終了するとき、クロスフェードで再生を開始する
	//void CrossFadeQueued(const lnKeyChar* animName, lnFloat fadeLength, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer);

	///// 同レイヤー内のアニメーション再生速度の同期
	//void SyncLayer(int layer);

	/// アニメーションを進める
	void AdvanceTime(double elapsedTime);

public:

	/// アニメーションクリップの追加
	void AddAnimationClip(AnimationClip* animationClip, int layer = 0);

	/// アニメーションクリップの削除
	void RemoveAnimationClip(AnimationClip* animationClip);

	/// AnimationTargetEntity の検索 (見つからなければ NULL)
	AnimationTargetAttributeEntity* FindAnimationTargetAttributeEntity(const String& name) const;

	/// AnimationState の検索
	AnimationState* FindAnimationState(const TCHAR* clipName);

	/// AnimationApplyTarget の追加 (できるだけ AnimationController 作成直後に設定すること。重複禁止)
	//void addAnimationApplyTarget(IAnimationApplyTarget* target);

private:
	typedef ArrayList<AnimationTargetAttributeEntity>	AnimationTargetAttributeEntityList;
	typedef SortedArray<String, AnimationState*>		AnimationStateList;

	IAnimationTargetElement*			m_element;
	AnimationTargetAttributeEntityList	m_animationTargetAttributeEntityList;
	AnimationStateList					m_animationStateList;

	///// 追加済み AnimationApplyTarget の検索
	//AnimationApplyTargetState* findAnimationApplyTargetState(const lnKeyChar* naem);

	//typedef std::vector<AnimationState*>			AnimationStateArray;
	//typedef std::vector<AnimationApplyTargetState>	AnimationApplyTargetStateArray;

	//AnimationStateArray				mAnimationStateArray;
	//AnimationApplyTargetStateArray	mAnimationApplyTargetStateArray;
};

} // namespace Animation
} // namespace Lumino
