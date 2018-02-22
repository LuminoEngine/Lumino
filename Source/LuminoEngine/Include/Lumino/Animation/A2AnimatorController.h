
#pragma once
#include <Lumino/Animation/Common.h>
#include "A2AnimationTrack.h"

LN_NAMESPACE_BEGIN
class Variant;
namespace a2
{
namespace detail { class AnimationTargetElementBlendLink; }
class AnimationValue;
class AnimationTrack;
class AnimationLayer;
class AnimationController;

/** ブレンドされた値を設定するオブジェクトのインターフェイスです。 */
class IAnimationTargetObject
{
public:
	virtual int getAnimationTargetElementCount() const = 0;
	virtual const String& getAnimationTargetElementName(int index) const = 0;
	virtual AnimationValueType getAnimationTargetElementValueType(int index) const = 0;
	virtual void setAnimationTargetElementValue(int index, const AnimationValue& value) = 0;
};



/** AnimationClip の再生状態を表すクラスです。 */
class AnimationState
	: public Object
{
public:

	/** この AnimationState に関連付けられている AnimationClip を取得します。 */
	AnimationClip* animationClip() const { return m_clip; }

	/**
		@brief		この AnimationState のアクティブ状態を設定します。
		@details	アクティブではない AnimationState は、インスタンスとしては存在していても、アニメーション更新時にブレンドされた値をターゲットに設定しません。
					このフラグは AnimationController を使わずに細かい制御を行う場合に使用します。
					AnimationController を使う場合、このフラグはアニメーションの再生管理によって頻繁に変更されます。
	*/
	void setActive(bool value);

LN_CONSTRUCT_ACCESS:
	AnimationState();
	virtual ~AnimationState();
	void initialize(AnimationClip* clip);

private:
	struct AnimationTrackInstance
	{
		AnimationTrack* track;
		detail::AnimationTargetElementBlendLink* blendLink;
	};

	void attachToTarget(AnimationController* animatorController);
	void updateTargetElements(float time);

	Ref<AnimationClip> m_clip;
	List<AnimationTrackInstance> m_trackInstances;
	float m_blendWeight;
	bool m_active;

	friend class AnimationLayer;
};

/** AnimationState をグループ化しアニメーションの適用範囲や方法を制御します。(体と表情のアニメーションを別々に扱う場合などに使用します) */
class AnimationLayer
	: public Object
{
public:

LN_CONSTRUCT_ACCESS:
	AnimationLayer();
	virtual ~AnimationLayer();
	void initialize(AnimationController* owner);

LN_INTERNAL_ACCESS:
	void addClipAndCreateState(AnimationClip* animationClip);
	void removeClipAndDeleteState(AnimationClip* animationClip);
	void updateTargetElements(float time);

	AnimationController* m_owner;
	List<Ref<AnimationState>> m_animationStatus;


	////typedef SortedArray<String, Ref<AnimationState>>	AnimationStateList;

	//Animator*			m_owner;
	//AnimationStateList	m_animationStateList;

	//AnimationLayer(Animator* owner);
	//void createStateAndAttachClip(AnimationClip* animationClip);
	//void removeStateByClip(AnimationClip* animationClip);
	//void transitionState(const StringRef& name, float duration);
	//void advanceTime(float elapsedTime);
	//AnimationState* findAnimationState(const StringRef& clipName);
};

/** スキンメッシュアニメーションにおいてキャラクターの挙動を操作するためのクラスです。 */
class AnimationController
	: public Object
{
public:

	/** アニメーションクリップの追加 (レイヤー0 へ追加されます) */
	void addClip(AnimationClip* animationClip);

	/** アニメーションクリップの除外 (レイヤー0 から除外されます) */
	void removeClip(AnimationClip* animationClip);

	/// 再生中であるかを確認する
	bool isPlaying() const;

	/// 再生
	void play(const StringRef& clipName, float duration = 0.3f/*, PlayMode mode = PlayMode_StopSameLayer*/);

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


public:


	///// AnimationTargetEntity の検索 (見つからなければ NULL)
	//detail::AnimationTargetAttributeEntity* findAnimationTargetAttributeEntity(const String& name);

LN_CONSTRUCT_ACCESS:
	AnimationController();
	~AnimationController();
	void initialize(IAnimationTargetObject* targetObject);

LN_INTERNAL_ACCESS:
	void advanceTime(float elapsedTime);
	void updateTargetElements();
	detail::AnimationTargetElementBlendLink* findAnimationTargetElementBlendLink(const StringRef& name);

private:
	IAnimationTargetObject* m_targetObject;
	List<Ref<AnimationLayer>> m_layers;
	List<Ref<detail::AnimationTargetElementBlendLink>> m_targetElementBlendLinks;
	float m_currentTime;
};

// AnimationState
// AnimationLayer

} // namespace a2
LN_NAMESPACE_END
