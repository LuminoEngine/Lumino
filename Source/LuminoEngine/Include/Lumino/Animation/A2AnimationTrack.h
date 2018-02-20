
#pragma once
#include <Lumino/Animation/Common.h>

LN_NAMESPACE_BEGIN
namespace a2
{

/** 特定の型のアニメーションを、1つ以上のカーブの集合として表現します。 */
class AbstractAnimationTrack
	: public Object
{
public:

	/** このトラックのアニメーションが適用される要素の名前。（スキンメッシュアニメーションではボーン名、UIElement のアニメーションではプロパティ名など） */
	const String& targetName() const;
};

/** (スキニングでは、モーフィングアニメーションで使用) */
class ScalarAnimationTrack
	: public AbstractAnimationTrack
{
public:
};

/** TODO */
class Vector3AnimationTrack
	: public AbstractAnimationTrack
{
public:
};

/** TODO */
class ColorAnimationTrack
	: public AbstractAnimationTrack
{
public:
};

/** (スキニングでは、ボーンアニメーションで使用) */
class TransformAnimationTrack
	: public AbstractAnimationTrack
{
public:
};

/**
	@brief		オブジェクトやそのプロパティに影響を与えるアニメーションデータです。
	@details	スキンメッシュアニメーションでは、「歩く」「走る」といった 1 モーションの単位と考えることができます。
*/
class AnimationClip
	: public Object
{
public:
	const List<Ref<AbstractAnimationTrack>>& tracks() const { return m_tracks; }

private:
	List<Ref<AbstractAnimationTrack>>	m_tracks;
};

// AnimationState
// AnimationLayer

} // namespace a2
LN_NAMESPACE_END
