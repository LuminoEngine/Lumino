
#pragma once
#include "../Animation/AnimationMixer.hpp"

namespace ln {
class SkinnedMeshModel;

/** スキンメッシュアニメーションにおいてキャラクターの挙動を操作するためのクラスです。 */
LN_CLASS()
class AnimationController
	: public Object
	, public detail::IAnimationMixerCoreHolder
{
public:
	/** アニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	LN_METHOD()
	AnimationState* addClip(AnimationClip* animationClip) { return m_core->addClip(animationClip); }

	/** ステート名を指定してアニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(const StringView& stateName, AnimationClip* animationClip) { m_core->addClip(stateName, animationClip); }

	/** アニメーションクリップを除外します。 (レイヤー0 から除外されます) */
	void removeClip(AnimationClip* animationClip) { m_core->removeClip(animationClip); }

	/// 再生中であるかを確認する
	//bool isPlaying() const;

	/// 再生
	void play(const StringView& stateName, float duration = 0.3f/*, PlayMode mode = PlayMode_StopSameLayer*/) { m_core->play(stateName, duration); }

	/** play */
	LN_METHOD()
	void play(AnimationState* state, float duration = 0.3f/*, PlayMode mode = PlayMode_StopSameLayer*/) { m_core->play(state, duration); }

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


	const Ref<AnimationMixerCore>& core() const { return m_core; }

public:
	void advanceTime(float elapsedTime);


	///// AnimationTargetEntity の検索 (見つからなければ NULL)
	//detail::AnimationTargetAttributeEntity* findAnimationTargetAttributeEntity(const String& name);


LN_CONSTRUCT_ACCESS:
	AnimationController();
	bool init(SkinnedMeshModel* model);

protected:
	detail::AnimationTargetElementBlendLink* onRequireBinidng(const AnimationTrackTargetKey& key) override;
	void onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* binding) override;

private:
	SkinnedMeshModel* m_model;
	Ref<AnimationMixerCore> m_core;
	List<Ref<detail::AnimationTargetElementBlendLink>> m_bindings;
};

} // namespace ln

