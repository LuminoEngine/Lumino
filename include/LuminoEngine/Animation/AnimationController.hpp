﻿
#pragma once
#include "Common.hpp"
#include "AnimationTrack.hpp"
#include "AnimationClip.hpp"

namespace ln {
class Variant;
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

	/** 名前を取得します。 */
	const String& name() const { return m_name; }

	/** 名前を設定します。 */
	void setName(const String& name) { m_name = name; }

	/**
		@brief		この AnimationState のアクティブ状態を設定します。
		@details	アクティブではない AnimationState は、インスタンスとしては存在していても、アニメーション更新時にブレンドされた値をターゲットに設定しません。
					この機能は AnimationController を使わずに細かい制御を行う場合に使用します。
					AnimationController を使う場合、この値はアニメーションの再生管理によって頻繁に変更されます。
	*/
	void setActive(bool value);

	/** この AnimationState のアクティブ状態を取得します。 */
	bool isActive() const { return m_active; }

	/**
		@brief		この AnimationState のアニメーション時間を設定します。
		@details	この機能は AnimationController を使わずに細かい制御を行う場合に使用します。
					AnimationController を使う場合、この値はアニメーションの再生管理によって頻繁に変更されます。
	*/
	void setLocalTime(float time);

	/** この AnimationState のアニメーション時間を取得します。 */
	float localTime() const { return m_localTime; }

LN_CONSTRUCT_ACCESS:
	AnimationState();
	virtual ~AnimationState();
	void init(AnimationClip* clip);

private:
	struct AnimationTrackInstance
	{
		AnimationTrack* track;
		detail::AnimationTargetElementBlendLink* blendLink;
	};

	void attachToTarget(AnimationController* animatorController);
	float getBlendWeight() const { return m_blendWeight; }
	void setBlendWeight(float weight) { m_blendWeight = weight; }
	void updateTargetElements();

	Ref<AnimationClip> m_clip;
	String m_name;
	List<AnimationTrackInstance> m_trackInstances;
	float m_localTime;
	float m_blendWeight;
	bool m_active;

	friend class AnimationLayer;
	friend class AnimationController;
};

/** AnimationState をグループ化しアニメーションの適用範囲や方法を制御します。(体と表情のアニメーションを別々に扱う場合などに使用します) */
class AnimationLayer
	: public Object
{
public:

LN_CONSTRUCT_ACCESS:
	AnimationLayer();
	virtual ~AnimationLayer();
	void init(AnimationController* owner);

LN_INTERNAL_ACCESS:
	AnimationState* addClipAndCreateState(AnimationClip* animationClip);
	void removeClipAndDeleteState(AnimationClip* animationClip);
	AnimationState* findAnimationState(const StringRef& name);
	void advanceTime(float elapsedTime);
	void updateStateWeights();
	void updateTargetElements();
	void transitionTo(AnimationState* state, float duration);

private:
	struct Transition
	{
		AnimationState* stateFrom;
		AnimationState* stateTo;
		float duration;
		float time;		// 0.0 ~ duration
		float startingOffsetTime;
	};

	AnimationController* m_owner;
	List<Ref<AnimationState>> m_animationStatus;
	AnimationState* m_currentState;
	Transition m_transition;
};

/** スキンメッシュアニメーションにおいてキャラクターの挙動を操作するためのクラスです。 */
class AnimationController
	: public Object
{
public:

	/** アニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(AnimationClip* animationClip);

	/** ステート名を指定してアニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(const StringRef& stateName, AnimationClip* animationClip);

	/** アニメーションクリップを除外します。 (レイヤー0 から除外されます) */
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
	void init(IAnimationTargetObject* targetObject);

LN_INTERNAL_ACCESS:
	void advanceTime(float elapsedTime);
	void updateTargetElements();
	detail::AnimationTargetElementBlendLink* findAnimationTargetElementBlendLink(const StringRef& name);

private:
	IAnimationTargetObject* m_targetObject;
	List<Ref<AnimationLayer>> m_layers;
	List<Ref<detail::AnimationTargetElementBlendLink>> m_targetElementBlendLinks;
};

} // namespace ln