
#pragma once
#include "Common.hpp"
#include "AnimationTrack.hpp"
#include "AnimationClip.hpp"
#include "../Engine/Property.hpp"

namespace ln {
namespace detail { class AnimationTargetElementBlendLink; }
class AnimationValue;
class AnimationTrack;
class AnimationLayer;
class AnimationControllerCore;
class AnimationController;


namespace detail {

// [curve,track,clip] <=> [bone,property] の橋渡しを行うクラス。
// アニメーションの値は、実際に各プロパティに送る前に一度ここに集められる
class AnimationTargetElementBlendLink
	: public RefObject
{
public:
	// Bone animation 用
	String name;		// AnimationTargetElement の名前 (ボーン名など)
	int targetIndex = -1;

	// Property animation 用
	PropertyRef propertyRef;

	// 共通
	AnimationValue rootValue;	// アニメーション・ブレンドされた値。これを AnimationTargetElement へ送る。
	bool affectAnimation = false;		// 更新処理の中で、実際に値がセットされたかどうか。セットされていないボーンにはデフォルト値をセットしたりする。
};

class IAnimationControllerHolder
{
public:
	virtual void onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* link) = 0;
};

} // namespace detail


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

	void attachToTarget(AnimationControllerCore* animatorController);
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
	//friend class AnimationController;
};

/** AnimationState をグループ化しアニメーションの適用範囲や方法を制御します。(体と表情のアニメーションを別々に扱う場合などに使用します) */
class AnimationLayer
	: public Object
{
public:

LN_CONSTRUCT_ACCESS:
	AnimationLayer();
	virtual ~AnimationLayer();
	void init(AnimationControllerCore* owner);

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

	AnimationControllerCore* m_owner;
	List<Ref<AnimationState>> m_animationStatus;
	AnimationState* m_currentState;
	Transition m_transition;
};

class AnimationControllerCore
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
	void play(const StringRef& clipName, float duration/* = 0.3f*//*, PlayMode mode = PlayMode_StopSameLayer*/);

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

	void addLayer(const Ref<AnimationLayer>& layer) { m_layers.add(layer); }
	void addElementBlendLink(const Ref<detail::AnimationTargetElementBlendLink>& link) { m_targetElementBlendLinks.add(link); }

	///// AnimationTargetEntity の検索 (見つからなければ NULL)
	//detail::AnimationTargetAttributeEntity* findAnimationTargetAttributeEntity(const String& name);

protected:

LN_CONSTRUCT_ACCESS:
	AnimationControllerCore();
	~AnimationControllerCore();
	void init(detail::IAnimationControllerHolder* owner);

LN_INTERNAL_ACCESS:
	void advanceTime(float elapsedTime);
	void updateTargetElements();
	detail::AnimationTargetElementBlendLink* findAnimationTargetElementBlendLink(const StringRef& name);

private:
	detail::IAnimationControllerHolder* m_owner;
	List<Ref<AnimationLayer>> m_layers;
	List<Ref<detail::AnimationTargetElementBlendLink>> m_targetElementBlendLinks;
};





/** スキンメッシュアニメーションにおいてキャラクターの挙動を操作するためのクラスです。 */
class AnimationController
	: public Object
	, public detail::IAnimationControllerHolder
{
public:

	/** アニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(AnimationClip* animationClip) { m_core->addClip(animationClip); }

	/** ステート名を指定してアニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(const StringRef& stateName, AnimationClip* animationClip) { m_core->addClip(stateName, animationClip); }

	/** アニメーションクリップを除外します。 (レイヤー0 から除外されます) */
	void removeClip(AnimationClip* animationClip) { m_core->removeClip(animationClip); }

	/// 再生中であるかを確認する
	//bool isPlaying() const;

	/// 再生
	void play(const StringRef& clipName, float duration = 0.3f/*, PlayMode mode = PlayMode_StopSameLayer*/) { m_core->play(clipName, duration); }

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
	void init(IAnimationTargetObject* targetObject);

LN_INTERNAL_ACCESS:
	void advanceTime(float elapsedTime);
	virtual void onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* link) override;

private:
	IAnimationTargetObject* m_targetObject;
	Ref<AnimationControllerCore> m_core;
};

} // namespace ln
