
#pragma once
#include <Lumino/Animation/Common.h>
#include <Lumino/Animation/AnimationCurve.h>

LN_NAMESPACE_BEGIN
namespace Animation
{

/// アニメーションの値を設定するオブジェクトのインターフェイス
class IAnimationTargetAttribute
{
public:
	virtual ~IAnimationTargetAttribute() {}

	virtual const String& GetAnimationTargetName() const = 0;
	virtual void SetAnimationTargetValue(ValueType type, const void* value) = 0;

};

/// IAnimationTargetAttribute に一時バッファを持たせたくなかったので、ラップする構造体を用意する
struct AnimationTargetAttributeEntity
{
	IAnimationTargetAttribute*	Target;
	ValueType					Type;			///< Buffer の値の型
	byte_t						Buffer[64];		///< IAnimationTargetAttribute に設定する前の計算の一時バッファ
	bool						Modified;		///< 1 つ以上値が設定されたか
};

/// データモデルである AnimationClip に対するインスタンス
class AnimationState
	: public RefObject
{
public:
	AnimationState(AnimationClip* clip);
	~AnimationState();

public:
	/// 名前の取得
	const String& GetName() const;

	/// AnimationTarget の再割り当てを行う
	void Refresh(Animator* animator);

	/// 再生状態の設定
	void SetPlayState(PlayState state);

	/// 再生状態の取得
	PlayState GetPlayState() const { return m_state; }

	/// この AnimationState の同レイヤー内のブレンド率
	void SetAddingBlendWeight(float weight) { m_addingBlendWeight = weight; }

	/// 時間を進める
	void AdvanceTime(double elapsedTime);

private:
	void ClearTargetList();
	void SetLocalTime(double time);

private:
	struct AnimationTarget
	{
		AnimationCurve*					Curve;		// このアニメーションは
		AnimationTargetAttributeEntity*	Target;		// TargetState.Value に値をセットする
	};

	typedef Array<AnimationTarget>	AnimationTargetList;

	AnimationClip*			m_clip;
	AnimationTargetList		m_animationTargetList;
	double					m_currentLocalTime;
	PlayState				m_state;
	float					m_addingBlendWeight;
};

} // namespace Animation
LN_NAMESPACE_END
