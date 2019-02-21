
#pragma once
#include "Common.hpp"

namespace ln {
class AnimationKeyFrame;
class AnimationTrack;

/**
 * オブジェクトやそのプロパティに影響を与えるアニメーションデータです。
 *
 * スキンメッシュアニメーションでは、「歩く」「走る」といった 1 モーションの単位です。
 */
class AnimationClip
	: public Object
{
public:
	static Ref<AnimationClip> create(const StringRef& name, const StringRef& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes);


	/** アニメーションの繰り返しの動作を取得します。 */
	AnimationWrapMode wrapMode() const { return m_wrapMode; }

	/** アニメーションの繰り返しの動作を取得します。(default: Loop) */
	void setWrapMode(AnimationWrapMode mode) { m_wrapMode = mode; }


    void addTrack(AnimationTrack* track);
	const List<Ref<AnimationTrack>>& tracks() const { return m_tracks; }

	float lastFrameTime() const { return m_lastFrameTime; }

LN_CONSTRUCT_ACCESS:
	AnimationClip();
	virtual ~AnimationClip();
	void init();
	void init(const StringRef& name, const StringRef& targetPath, const std::initializer_list<AnimationKeyFrame>& keyframes);

protected:
	String m_name;
	List<Ref<AnimationTrack>> m_tracks;
	Ref<RefObject> m_srcData;
	float m_lastFrameTime;
	AnimationWrapMode m_wrapMode;
};

/**
	@brief		Vocaloid Motion Data (.vmd) ファイル
*/
// TODO: 継承ではなく、Importer として、AnimationClip を構築
class VmdAnimationClip
	: public AnimationClip
{
public:
	static Ref<VmdAnimationClip> create(const Path& filePath);

LN_CONSTRUCT_ACCESS:
	VmdAnimationClip();
	virtual ~VmdAnimationClip();
	void init(const Path& filePath);
};

} // namespace ln
