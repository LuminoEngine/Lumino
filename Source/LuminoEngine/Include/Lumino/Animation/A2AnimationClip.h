
#pragma once
#include <Lumino/Animation/Common.h>

LN_NAMESPACE_BEGIN
class AnimationTrack;

/**
	@brief		オブジェクトやそのプロパティに影響を与えるアニメーションデータです。
	@details	スキンメッシュアニメーションでは、「歩く」「走る」といった 1 モーションの単位です。
*/
class AnimationClip
	: public Object
{
public:

	/** アニメーションの繰り返しの動作を取得します。 */
	AnimationWrapMode wrapMode() const { return m_wrapMode; }

	/** アニメーションの繰り返しの動作を取得します。(default: Once) */
	void setWrapMode(AnimationWrapMode mode) { m_wrapMode = mode; }

	const List<Ref<AnimationTrack>>& tracks() const { return m_tracks; }

	float lastFrameTime() const { return m_lastFrameTime; }

LN_CONSTRUCT_ACCESS:
	AnimationClip();
	virtual ~AnimationClip();
	void initialize();

protected:
	List<Ref<AnimationTrack>> m_tracks;
	Ref<RefObject> m_srcData;
	float m_lastFrameTime;
	AnimationWrapMode m_wrapMode;
};

/**
	@brief		Vocaloid Motion Data (.vmd) ファイル
*/
class VmdAnimationClip
	: public AnimationClip
{
public:
	static Ref<VmdAnimationClip> create(const Path& filePath);

LN_CONSTRUCT_ACCESS:
	VmdAnimationClip();
	virtual ~VmdAnimationClip();
	void initialize(const Path& filePath);
};

LN_NAMESPACE_END
