
#pragma once
#include "Common.hpp"
#include "AnimationTrack.hpp"
#include "AnimationClip.hpp"

namespace ln {
namespace detail {
class AnimationTargetElementBlendLink;
}
class AnimationValue;
class AnimationTrack;
class AnimationLayer;
class AnimationMixerCore;
class AnimationController;

namespace detail {

// [curve,track,clip] <=> [bone,property] の橋渡しを行うクラス。
// アニメーションの値は、実際に各プロパティに送る前に一度ここに集められる
class AnimationTargetElementBlendLink : public RefObject {
public:
    AnimationTargetElementBlendLink(AnimationValueType type) {
        rootValue.resetType(type);
    }

    // Bone animation 用
    AnimationTrackTargetKey key; // AnimationTargetElement の名前 (ボーン名など)
    int targetIndex = -1;

    // Property animation 用
    PropertyRef propertyRef;

    // 共通
    AnimationValue rootValue;     // アニメーション・ブレンドされた値。これを AnimationTargetElement へ送る。
    bool affectAnimation = false; // 更新処理の中で、実際に値がセットされたかどうか。セットされていないボーンにはデフォルト値をセットしたりする。
};

class IAnimationMixerCoreHolder {
public:
    // AnimationClip を add したとき、登録済みの Binding が無い場合に呼び出される。
    // 派生側で new して返すが、対応する名前のボーンやプロパティを持っていない場合は null を返す。
    // インスタンスは IAnimationMixerCoreHolder の実装側で管理する。
    virtual detail::AnimationTargetElementBlendLink* onRequireBinidng(const AnimationTrackTargetKey& key) = 0;

    virtual void onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* binding) = 0;
};

} // namespace detail

/** ブレンドされた値を設定するオブジェクトのインターフェイスです。 */
// deprecated
class IAnimationTargetObject {
public:
    virtual int getAnimationTargetElementCount() const = 0;
    virtual const String& getAnimationTargetElementName(int index) const = 0;
    virtual AnimationValueType getAnimationTargetElementValueType(int index) const = 0;
    virtual void setAnimationTargetElementValue(int index, const AnimationValue& value) = 0;
};

/** AnimationClip の再生状態を表すクラスです。 */
LN_CLASS()
class AnimationState : public Object {
    LN_OBJECT;
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
    void init();
    void init(AnimationLayer* owner, AnimationClip* clip);

private:
    struct AnimationTrackInstance {
        AnimationTrack* track;
        detail::AnimationTargetElementBlendLink* blendLink;
    };

    void attachToTarget(AnimationMixerCore* animatorController);
    float getBlendWeight() const { return m_blendWeight; }
    void setBlendWeight(float weight) { m_blendWeight = weight; }
    void updateTargetElements();

    AnimationLayer* m_owner;
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
class AnimationLayer : public Object {
public:
    AnimationMixerCore* owner() const { return m_owner; }
    const List<Ref<AnimationState>>& animationStatus() const { return m_animationStatus; }

LN_CONSTRUCT_ACCESS:
    AnimationLayer();
    virtual ~AnimationLayer();
    void init(AnimationMixerCore* owner);

public: // TODO: internal
    AnimationState* addClipAndCreateState(AnimationClip* animationClip);
    void removeClipAndDeleteState(AnimationClip* animationClip);
    AnimationState* findAnimationState(const StringView& name);
    void advanceTime(float elapsedTime);
    void updateStateWeights();
    void updateTargetElements();
    void transitionTo(AnimationState* state, float duration);

private:
    struct Transition {
        AnimationState* stateFrom;
        AnimationState* stateTo;
        float duration;
        float time; // 0.0 ~ duration
        float startingOffsetTime;
    };

    AnimationMixerCore* m_owner;
    List<Ref<AnimationState>> m_animationStatus;
    AnimationState* m_currentState;
    Transition m_transition;
};

/*
  AnimationController 設計方針 Note:
	- Animator は、Component の派生。これはボーンアニメーションに限らず、アタッチ先の WorldObject 及び子 Components のプロパティにバインドする。
	- AnimationController は、Object の派生。通常 SkinnedMeshModel と 1:1 で作ることになる (Three.js の AnimationMixier のイメージ)
	これらの違いはプロパティのバインド先がプロパティかボーンorモーフかだけとしたい。

	ゲーム作るときほとんどの場合は Animator を使う。SkinnedMeshComponent がボーンをプロパティとして公開し、それにバインドする形になる。

	AnimationController は SkinnedMeshModel を直接コントロールするときに使う。SkeltalAnimationController とか名前変えてもいいかな。
	Animator と比べて余計なプロパティ検索しないので軽量。ただし Editor からいじることはできない。
	Component は使わず SkinnedMeshModel だけを使って描画したいときや、Component の中だけで完結させたい (パーティクルに SkinnedMesh を使うとか？) 時に使う。


	使い方Note:
		SkinnedMesh の場合
		- アタッチしたとき、Bone の分だけ Link を作って addElementBlendLink() しておく。dataId には BoneIndex を入れておく。
		- 後は addClip() されたときに内部で、この登録された Link を名前検索して Trask と紐づけられ、advanceTime() で値が更新される。
		- 更新後、Link を for して dataId に対応する Bone 、Link.value をセットする。
		プロパティアニメの場合
		- Bone と違ってどのようなプロパティが WorldObject に現れるかわからないし、Dynamic に変わったりする。
		  実際のところ Transform のアニメがほとんどで、他のプロパティはアニメの必要が無いものも多い。全部 Link 作るとメモリ効率悪い。
		- addClip() されたときに、Clipが必要としているプロパティを検索して、ヒットすれば Link を作る方がいいかもしれない。
		SkinnedMesh 考え直し
		- プロパティアニメと同じ方法にしておこうか。やり方増やすとメンテ大変だし、実行速度としてもそれほど変わるものではない。
*/
class AnimationMixerCore
    : public Object {
public:
    /** アニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
    AnimationState* addClip(AnimationClip* animationClip);

    /** ステート名を指定してアニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
    AnimationState* addClip(const StringView& stateName, AnimationClip* animationClip);

    /** アニメーションクリップを除外します。 (レイヤー0 から除外されます) */
    void removeClip(AnimationClip* animationClip);

    /// 再生中であるかを確認する
    bool isPlaying() const;

    /// 再生
    void play(const StringView& clipName, float duration /* = 0.3f*/ /*, PlayMode mode = PlayMode_StopSameLayer*/);
    void play(AnimationState* state, float duration /* = 0.3f*/ /*, PlayMode mode = PlayMode_StopSameLayer*/);

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

    float m_animationTranslationBasis = 1.0f;

public:
    void addLayer(const Ref<AnimationLayer>& layer) { m_layers.add(layer); }
    const List<Ref<AnimationLayer>>& layers() const { return m_layers; }

    void addElementBlendLink(const Ref<detail::AnimationTargetElementBlendLink>& link) { m_targetElementBlendLinks.add(link); }

    ///// AnimationTargetEntity の検索 (見つからなければ NULL)
    //detail::AnimationTargetAttributeEntity* findAnimationTargetAttributeEntity(const String& name);

    void advanceTime(float elapsedTime);
    void updateTargetElements();
    detail::AnimationTargetElementBlendLink* requireAnimationTargetElementBlendLink(const AnimationTrackTargetKey& key);


LN_CONSTRUCT_ACCESS:
    AnimationMixerCore();
    ~AnimationMixerCore();
    void init(detail::IAnimationMixerCoreHolder* owner);

    LN_INTERNAL_ACCESS : private : detail::IAnimationMixerCoreHolder* m_owner;
    List<Ref<AnimationLayer>> m_layers;
    List<Ref<detail::AnimationTargetElementBlendLink>> m_targetElementBlendLinks;
};

#if 0

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
	Ref<AnimationMixerCore> m_core;
};

#endif

} // namespace ln
